#pragma once
#include <cstddef>

namespace cache{
    // abstract ReplacementPolicy interface
    class ReplacementPolicy{
    public:
        virtual ~ReplacementPolicy() = default;

        /*
            changes internal state according to policy, for accessing {set_index, way}
        */
        virtual void on_access(std::size_t set_index, std::size_t way) = 0;

        /*
            changes internal state according to policy, for inserting new line at {set_index, way}
        */
        virtual void on_insert(std::size_t set_index, std::size_t way) = 0;

        /*
            returns the victim way in the given set, that can be chosen for eviction by policy.
        */
        virtual std::size_t choose_victim(std::size_t set_index) = 0;
    };
} // namespace cache