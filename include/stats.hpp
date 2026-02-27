#pragma once
#include <cstdint>

namespace cache{
    struct StatsCollector{
        uint64_t reads = 0;
        uint64_t writes = 0;
        uint64_t hits = 0;
        uint64_t misses = 0;
        uint64_t writebacks = 0;

        /*
            prints a short summary of the collected statistics
        */
        void print() const;
    };
} // namespace cache