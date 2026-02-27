#pragma once
#include <charconv>
#include <cstdint>
#include <optional>
#include <string_view>
#include <system_error>

namespace cache{
    /*
        parses an unsigned 64-bit integer from a string_view.
        returns parsed uint64_t, according to base given, 
        or std::nullopt in case of invalid digits, partial parse, or overflow.
    */
    inline std::optional<std::uint64_t> parse_u64(std::string_view sv, int base = 10){
        if(sv.empty()) return std::nullopt;
        const char* first = sv.data();
        const char* last = sv.data() + sv.size();
        std::uint64_t parsed = 0ULL;
        auto result = std::from_chars(first, last, parsed, base);
        if(result.ec != std::errc()) return std::nullopt;
        if(result.ptr != last) return std::nullopt;
        return parsed;
    }
}