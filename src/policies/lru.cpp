#include "policies/lru.hpp"
#include <vector>
#include <cstddef>

namespace cache{
    LRUPolicy::LRUPolicy(std::size_t num_sets, std::size_t ways) 
    : head_(num_sets, kInvalid),
      tail_(num_sets, kInvalid),
      next_free_(num_sets, 0),
      prev_(num_sets, std::vector<std::size_t>(ways, kInvalid)),
      next_(num_sets, std::vector<std::size_t>(ways, kInvalid)),
      num_sets_(num_sets),
      ways_(ways){}

    bool LRUPolicy::is_in_list(std::size_t set_index, std::size_t way) const{
        return (head_[set_index] == way)
            || (prev_[set_index][way] != kInvalid)
            || (next_[set_index][way] != kInvalid); 
    }

    void LRUPolicy::unlink_node(std::size_t set_index, std::size_t way){
        if(!is_in_list(set_index, way)) return;
        std::size_t p = prev_[set_index][way];
        std::size_t n = next_[set_index][way];
        // set next link of previous node
        if(p != kInvalid) next_[set_index][p] = n;
        else head_[set_index] = n;
        // set previous link of next node
        if(n != kInvalid) prev_[set_index][n] = p;
        else tail_[set_index] = p;
        // reset the previous and next links of the node to unlink
        prev_[set_index][way] = kInvalid;
        next_[set_index][way] = kInvalid;
    }

    void LRUPolicy::insert_at_head(std::size_t set_index, std::size_t way){
        std::size_t h = head_[set_index];
        // set the head, and previous and next links of the node
        head_[set_index] = way;
        prev_[set_index][way] = kInvalid;
        next_[set_index][way] = h;
        // set the previous link of the earlier head node
        if(h != kInvalid) prev_[set_index][h] = way;
        else tail_[set_index] = way;
    }

    void LRUPolicy::on_access(std::size_t set_index, std::size_t way){
        if(head_[set_index] == way) return;
        // move the node to head (most recently used)
        unlink_node(set_index, way);
        insert_at_head(set_index, way);
    }

    void LRUPolicy::on_insert(std::size_t set_index, std::size_t way){
        if(head_[set_index] == way) return;
        // move the node to head (most recently used)
        unlink_node(set_index, way);
        insert_at_head(set_index, way);
    }

    std::size_t LRUPolicy::choose_victim(std::size_t set_index){
        if(next_free_[set_index] < ways_){ // free ways available
            std::size_t free_way = next_free_[set_index];
            next_free_[set_index]++;
            return free_way;
        }
        return tail_[set_index]; // tail node is least recently used
    }
}
