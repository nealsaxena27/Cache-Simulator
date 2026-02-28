#pragma once
#include "replacement_policy.hpp"
#include <cstddef>
#include <vector>
#include <limits>


namespace cache{
    class FIFOPolicy : public ReplacementPolicy{
    public:
        /*
            initializes FIFO internal state
        */
        FIFOPolicy(std::size_t num_sets, std::size_t ways);
        ~FIFOPolicy() override = default;

        /*
            changes FIFO internal state, according to cache access on {set_index, way}.
        */
        void on_access(std::size_t set_index, std::size_t way) override;

        /*
            inserts new entry {set_index, way} in the FIFO internal state.
        */
        void on_insert(std::size_t set_index, std::size_t way) override;

        /*
            returns the way that should be evicted in the given set.
        */
        std::size_t choose_victim(std::size_t set_index) override;
    private:
        /*
            internal structures for tracking FIFO state by maintaining
            queues for each set, using circular arrays with front and rear pointers.
        */
        std::vector<std::size_t> front_;
        std::vector<std::size_t> rear_;
        std::vector<std::size_t> next_free_;
        std::vector<std::vector<std::size_t>> queue_;
        static constexpr std::size_t kInvalid = std::numeric_limits<std::size_t>::max();
        std::size_t num_sets_;
        std::size_t ways_;

        // helper functions

        /*
            returns true if queue for given set is full, false otherwise 
        */
        bool is_full(std::size_t set_index) const;

        /*
            pushes back "way" into the queue for set_index
        */
        void push(std::size_t set_index, std::size_t way);

        /*
            pops from front of the queue for set_index
        */
        void pop(std::size_t set_index);
    };
} // namespace cache