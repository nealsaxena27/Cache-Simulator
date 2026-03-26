#pragma once
#include "config.hpp"
#include "cache_set.hpp"
#include "replacement_policy.hpp"
#include "stats.hpp"
#include <vector>
#include <memory>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace cache{
    class Cache{
    public:
        /*
            initializes Cache with given Configuration
        */
        explicit Cache(const Config &cfg);

        /*
            initializes memory hierarchy for this Cache, with the given upper and lower level references
        */
        void init_hierarchy(Cache* lower_level, Cache* upper_level);

        /*
            accesses the cache with a 64-bit physical address of either read or write instruction.
            returns true for cache hit, and false for cache miss.
        */
        bool access(uint64_t addr, bool is_write);

        /*
            flushes all the dirty lines (write-backs).
        */
        void flush();

        /*
            returns the collected statistics.
        */
        StatsCollector stats() const;

    private:
        Config cfg_; // configuration of Cache
        std::vector<CacheSet> sets_; // vector of CacheSets
        std::unique_ptr<ReplacementPolicy> policy_; // ownership of ReplacementPolicy
        Cache* lower_level_; // reference to lower level cache
        Cache* upper_level_; // reference to upper level cache
        StatsCollector stats_; // statistics
        std::size_t num_sets_; // number of sets
        std::size_t index_bits_; // number of bits for set index
        std::size_t offset_bits_; // number of bits for block offset
        uint64_t index_mask_; // mask for obtaining index, after shift

        // helper functions

        /*
            factory function to create policy according to provided name.
            returns ownership of the created ReplacementPolicy.
        */
        std::unique_ptr<ReplacementPolicy> make_policy(std::string_view name, 
            std::size_t num_sets, std::size_t ways_);
        
        /*
            marks the cacheline corresponding to given 64-bit physical address dirty 
        */
        void mark_dirty(uint64_t addr);

        /*
            marks the cacheline corresponding to given 64-bit physical address invalid
        */
        void mark_invalid(uint64_t addr);
    };
} // namespace cache