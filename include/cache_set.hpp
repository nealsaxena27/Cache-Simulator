#pragma once
#include "cache_line.hpp"
#include <vector>
#include <optional>
#include <cstddef>
#include <cstdint>

namespace cache{
    class CacheSet{
    public:
        /*
            initializes CacheSet with "ways" number of CacheLines
        */
        CacheSet(std::size_t ways);

        /*
            searches for given tag in the set.
            returns way index if found, otherwise std::nullopt.
        */
        std::optional<std::size_t> find(uint64_t tag) const;

        /*
            replaces the contents at way, with given tag, and sets dirty flag according to mark_dirty.
            returns the old CacheLine if it was valid, otherwise std::nullopt
        */
        std::optional<CacheLine> replace_at(std::size_t way, uint64_t tag, bool mark_dirty);

        /*
            marks the cacheline at way dirty, if is_write is true. 
        */
        void touch(std::size_t way, bool is_write);

        /*
            invalidates the cache line at way.
        */
        void invalidate(std::size_t way);

        /*
            returns const reference to internal lines vector.
        */
        std::vector<CacheLine>& lines();
    private:
        // vector of lines to store the set of CacheLines in the set
        std::vector<CacheLine> lines_;
    };
} // namespace cache