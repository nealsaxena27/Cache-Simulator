#pragma once
#include <cstdint>

namespace cache{
    struct CacheLine{
        uint64_t tag = 0; // tag bits
        bool valid = false; // whether line holds valid data
        bool dirty = false; // whether line is dirty (written-back needed)
    };
} // namespace cache