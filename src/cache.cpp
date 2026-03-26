#include "cache.hpp"
#include "config.hpp"
#include "cache_set.hpp"
#include "replacement_policy.hpp"
#include "policies/lru.hpp"
#include "policies/fifo.hpp"
#include "stats.hpp"
#include <vector>
#include <memory>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace cache{
    Cache::Cache(const Config &cfg)
    : cfg_(cfg){
        offset_bits_ = std::countr_zero(cfg_.block_size);
        num_sets_ = cfg_.cache_size / (cfg_.block_size * cfg_.ways);
        index_bits_ = std::countr_zero(num_sets_);
        index_mask_ = (1ULL << index_bits_) - 1ULL;
        sets_.assign(num_sets_, CacheSet{cfg_.ways});
        policy_ = make_policy(cfg_.replacement, num_sets_, cfg_.ways);
        lower_level_ = nullptr; upper_level_ = nullptr;
    }

    void Cache::init_hierarchy(Cache* lower_level, Cache* upper_level){
        lower_level_ = lower_level;
        upper_level_ = upper_level;
    }

    bool Cache::access(uint64_t addr, bool is_write){
        // parse address into tag/index/offset
        std::size_t index = (addr >> offset_bits_) & index_mask_;
        uint64_t tag = addr >> (index_bits_ + offset_bits_);

        // check for hit
        if(auto way_opt = sets_[index].find(tag)){ // hit
            std::size_t way = *way_opt;
            policy_->on_access(index, way);
            sets_[index].touch(way, is_write);

            if(is_write) stats_.writes++;
            else stats_.reads++;
            stats_.hits++;

            return true;
        }
        else{ // miss
            stats_.misses++;

            if(lower_level_ != nullptr) lower_level_->access(addr, is_write);

            if(is_write){
                stats_.writes++;
                if(!cfg_.write_allocate) return false; // no write-allocate, cache not modified
            }
            else stats_.reads++;

            std::size_t victim_way = policy_->choose_victim(index);
            auto evicted = sets_[index].replace_at(victim_way, tag, is_write);
            policy_->on_insert(index, victim_way);

            if(evicted && evicted->dirty){
                stats_.writebacks++; // write back evicted dirty cacheline
                // get the evicted block address
                uint64_t evicted_addr = index + (evicted->tag << index_bits_);
                evicted_addr = evicted_addr << offset_bits_;
                if(lower_level_ != nullptr){
                    // mark this evicted address dirty in the lower level cache also
                    lower_level_->mark_dirty(evicted_addr);
                }
                if(upper_level_ != nullptr){
                    // mark this evicted address invalid in the upper level cache also
                    upper_level_->mark_invalid(evicted_addr);
                }
            }
            return false;
        }
    }

    void Cache::mark_dirty(uint64_t addr){
        // parse address into tag/index/offset
        std::size_t index = (addr >> offset_bits_) & index_mask_;
        uint64_t tag = addr >> (index_bits_ + offset_bits_);
        if(auto way_opt = sets_[index].find(tag)){
            std::size_t way = *way_opt;
            sets_[index].touch(way, true); // mark cacheline dirty
        }
        // cascade to lower level
        if(lower_level_ != nullptr) lower_level_->mark_dirty(addr);
    }

    void Cache::mark_invalid(uint64_t addr){
        // parse address into tag/index/offset
        std::size_t index = (addr >> offset_bits_) & index_mask_;
        uint64_t tag = addr >> (index_bits_ + offset_bits_);
        if(auto way_opt = sets_[index].find(tag)){
            std::size_t way = *way_opt;
            sets_[index].invalidate(way); // mark cacheline invalid
        }
        // cascade to upper level
        if(upper_level_ != nullptr) upper_level_->mark_invalid(addr);
    }

    void Cache::flush(){
        // traverse all sets and write back dirty lines as needed
        for(std::size_t i = 0; i < num_sets_; i++){
            for(auto &line: sets_[i].lines()) {
                if(line.dirty && line.valid){
                    stats_.writebacks++;
                    if(lower_level_ != nullptr){
                        // mark this {evicted tag, index} dirty in the lower level cache also
                        uint64_t flushed_addr = i + (line.tag << index_bits_);
                        flushed_addr = flushed_addr << offset_bits_;
                        lower_level_->mark_dirty(flushed_addr);
                    }
                    line.valid = false;
                }
            }
        }
    }

    StatsCollector Cache::stats() const{
        return stats_;
    }

    std::unique_ptr<ReplacementPolicy> Cache::make_policy(std::string_view name, 
        std::size_t num_sets, std::size_t ways){
        if(name == "lru"){
            return std::make_unique<LRUPolicy>(num_sets, ways);
        }
        if(name == "fifo"){
            return std::make_unique<FIFOPolicy>(num_sets, ways);
        }
        // default: lru
        return std::make_unique<LRUPolicy>(num_sets, ways);
    }
}