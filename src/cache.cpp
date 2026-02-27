#include "cache.hpp"
#include "config.hpp"
#include "cache_set.hpp"
#include "replacement_policy.hpp"
#include "policies/lru.hpp"
#include "stats.hpp"
#include <vector>
#include <memory>
#include <bit>
#include <cstddef>
#include <cstdint>

namespace cache{
    Cache::Cache(const Config &cfg)
    : cfg_(cfg){
        offset_bits_ = std::countr_zero(cfg_.block_size);
        num_sets_ = cfg_.cache_size / (cfg_.block_size * cfg_.ways);
        index_bits_ = std::countr_zero(num_sets_);
        index_mask_ = (1ULL << index_bits_) - 1ULL;
        sets_.assign(num_sets_, CacheSet{cfg_.ways});
        policy_ = std::make_unique<LRUPolicy>(num_sets_, cfg_.ways);
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

            if(is_write){
                stats_.writes++;
                if(!cfg_.write_allocate) return false; // no write-allocate, cache not modified
            }
            else stats_.reads++;

            std::size_t victim_way = policy_->choose_victim(index);
            auto evicted = sets_[index].replace_at(victim_way, tag, is_write);
            policy_->on_insert(index, victim_way);

            if(evicted && evicted->dirty) stats_.writebacks++; // write back evicted dirty cacheline

            return false;
        }
    }

    void Cache::flush(){
        // traverse all sets and write back dirty lines as needed
        for(std::size_t i = 0; i < num_sets_; i++){
            auto set = sets_[i].lines();
            for(auto &line: set) {
                if(line.dirty) stats_.writebacks++;
            }
        }
    }

    StatsCollector Cache::stats() const{
        return stats_;
    }
}