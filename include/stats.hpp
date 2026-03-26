#pragma once
#include <cstdint>
#include <string>
#include <string_view>

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
        void print(std::string_view title) const;
    };
} // namespace cache