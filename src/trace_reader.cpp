#include "trace_reader.hpp"
#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <optional>
#include <string>
#include <string_view>
#include <filesystem>
#include <cstdint>

namespace cache{
    TraceReader::~TraceReader(){
        close();
    }

    bool TraceReader::open(const std::filesystem::path &path){
        close();
        path_ = path;
        infile_.open(path_, std::ios::in);
        return infile_.is_open();
    }

    void TraceReader::close(){
        if(infile_.is_open()){
            infile_.close();
        }
        path_.clear();
    }

    bool TraceReader::next(AccessRecord &out){
        if(!infile_.is_open()) return false;

        std::string line;
        while(std::getline(infile_, line)){
            if(line.empty()) continue;

            std::istringstream line_stream(line);
            char type;
            std::string addr_str;

            // expected format: <type> <address>
            // <type> can be 'R' 'r' 'W' 'w'
            // <address> can be hex (0x or 0X) or decimal
            if(!(line_stream >> type >> addr_str)) continue; // malformed line

            std::optional<std::uint64_t> parsed;
            if(addr_str.size() > 2 && (addr_str.starts_with("0x") || addr_str.starts_with("0X"))) {
                parsed = parse_u64(std::string_view(addr_str).substr(2), 16); // hex address
            }
            else {
                parsed = parse_u64(std::string_view(addr_str), 10); // decimal address
            }
            if(!parsed) continue; // malformed address
            out.address = *parsed;
            out.is_write = (type == 'W' || type == 'w');
            return true;
        }

        // EOF reached or error.
        return false;
    }

    void TraceReader::reset(){
        if(!infile_.is_open()) return;
        infile_.clear();
        infile_.seekg(0, std::ios::beg);
    }
}