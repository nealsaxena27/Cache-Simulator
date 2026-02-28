#include "policies/fifo.hpp"
#include <vector>
#include <cstddef>

namespace cache{
    FIFOPolicy::FIFOPolicy(std::size_t num_sets, std::size_t ways)
    : front_(num_sets, 0),
      rear_(num_sets, 0),
      next_free_(num_sets, 0),
      queue_(num_sets, std::vector<std::size_t>(ways + 1, kInvalid)),
      num_sets_(num_sets),
      ways_(ways){}
    
    bool FIFOPolicy::is_full(std::size_t set_index) const{
        return (rear_[set_index] + 1) % (ways_ + 1) == front_[set_index]; 
    }

    void FIFOPolicy::push(std::size_t set_index, std::size_t way){
        queue_[set_index][rear_[set_index]] = way;
        rear_[set_index]++;
        if(rear_[set_index] == ways_ + 1) rear_[set_index] = 0;
    }

    void FIFOPolicy::pop(std::size_t set_index){
        front_[set_index]++;
        if(front_[set_index] == ways_ + 1) front_[set_index] = 0;
    }

    void FIFOPolicy::on_access([[maybe_unused]] std::size_t set_index, [[maybe_unused]] std::size_t way){}

    void FIFOPolicy::on_insert(std::size_t set_index, std::size_t way){
        if(is_full(set_index)) pop(set_index);
        push(set_index, way);
    }

    std::size_t FIFOPolicy::choose_victim(std::size_t set_index){
        if(is_full(set_index)) return queue_[set_index][front_[set_index]];
        std::size_t free_way = next_free_[set_index]; // free ways available
        next_free_[set_index]++;
        return free_way;
    }
}