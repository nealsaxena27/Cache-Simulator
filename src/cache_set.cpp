#include "cache_set.hpp"
#include <vector>
#include <optional>
#include <cstddef>
#include <cstdint>

namespace cache{
    CacheSet::CacheSet(std::size_t ways)
        : lines_(ways){}

    std::optional<std::size_t> CacheSet::find(uint64_t tag) const{
        for(std::size_t i = 0; i < lines_.size(); i++){
            if(lines_[i].valid && lines_[i].tag == tag) return i; // found
        }
        return std::nullopt; // not found
    }

    std::optional<CacheLine> CacheSet::replace_at(std::size_t way, uint64_t tag, bool mark_dirty){
        CacheLine old = lines_[way]; // copy old line
        lines_[way].tag = tag; // place new line
        lines_[way].valid = true;
        lines_[way].dirty = mark_dirty;
        if(old.valid) return old; // old cacheline valid
        return std::nullopt; // old cacheline invalid
    }

    void CacheSet::touch(std::size_t way, bool is_write){
        if(is_write) lines_[way].dirty = true;
    }

    const std::vector<CacheLine>& CacheSet::lines() const{
        return lines_;
    }
}
