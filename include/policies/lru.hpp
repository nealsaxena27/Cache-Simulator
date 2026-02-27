#pragma once
#include "replacement_policy.hpp"
#include <cstddef>
#include <vector>
#include <limits>

namespace cache{
    class LRUPolicy : public ReplacementPolicy{
    public:
        /*
            initializes LRU internal state
        */
        LRUPolicy(std::size_t num_sets, std::size_t ways);
        ~LRUPolicy() override = default;

        /*
            changes LRU internal state, according to cache access on {set_index, way}.
        */
        void on_access(std::size_t set_index, std::size_t way) override;

        /*
            inserts new entry {set_index, way} in the LRU internal state.
        */ 
        void on_insert(std::size_t set_index, std::size_t way) override;

        /*
            returns the way that should be evicted in the given set.
        */
        std::size_t choose_victim(std::size_t set_index) override;
        
    private:
        /*
            internal structures for tracking LRU state by maintaining
            doubly linked lists for each set, with MRU entries at heads of the lists.
        */
        std::vector<std::size_t> head_;
        std::vector<std::size_t> tail_;
        std::vector<std::size_t> next_free_;
        std::vector<std::vector<std::size_t>> prev_;
        std::vector<std::vector<std::size_t>> next_;
        static constexpr std::size_t kInvalid = std::numeric_limits<std::size_t>::max();
        std::size_t num_sets_;
        std::size_t ways_;
        
        // helper functions

        /*
            returns true if {set_index, way} is in list.
        */
        bool is_in_list(std::size_t set_index, std::size_t way) const;

        /*
            unlinks the given node {set_index, way} from the list.
        */
        void unlink_node(std::size_t set_index, std::size_t way);

        /*
            inserts the given node {set_index, way} at the head of the list.
        */
        void insert_at_head(std::size_t set_index, std::size_t way);
    };
} // namespace cache