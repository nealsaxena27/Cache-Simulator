#pragma once
#include <cstdint>
#include <cstddef>
#include <string>

namespace cache{
    struct Config{
        std::size_t cache_size = 32768; // total capacity in bytes
        std::size_t block_size = 64; // block / line size in bytes
        std::size_t ways = 8; // set-associativity
        bool write_back = true; // write-back vs write-through
        bool write_allocate = true; // write-allocate vs no-write-allocate
        std::string replacement = "lru"; // replacement policy name - LRU, FIFO
    };
} // namespace cache