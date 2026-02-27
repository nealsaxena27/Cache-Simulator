#include "simulator.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <cstdint>

namespace{
    void print_usage(std::string_view prog){
        std::cout
            << "Usage: " << prog << " --trace <trace-file> [options]\n\n"
            << "Required:\n"
            << "  --trace <path>            Path to trace file\n\n"
            << "Options:\n"
            << "  --size <bytes>            Cache size in bytes (default: 32768)\n"
            << "  --block <bytes>           Block size in bytes (default: 64)\n"
            << "  --assoc <ways>            Associativity (default: 8)\n"
            << "  --write-allocate/--no-write-allocate   (default: enabled)\n"
            << "  --write-back/--write-through          (default: write-back)\n"
            << "  --policy <name>           Replacement policy (lru) (default: lru)\n"
            << "  -h, --help                Show help\n";
    }
}

int main(int argc, char **argv){
    const std::string prog = (argc > 0 ? std::filesystem::path(argv[0]).filename().string() : "cache_simulator");

    if(argc == 1){
        print_usage(prog);
        return 1;
    }
    cache::Config cfg;
    std::optional<std::filesystem::path> trace_path;

    for(int i = 1; i < argc; i++){
        std::string_view a = argv[i];

        if(a == "--help" || a == "-h"){
            print_usage(prog);
            return 0;
        }

        if(a == "--write-allocate"){
            cfg.write_allocate = true;
            continue;
        }
        
        if(a == "--no-write-allocate"){
            cfg.write_allocate = false;
            continue;
        }

        if(a == "--write-back"){
            cfg.write_back = true;
            continue;
        }
        if(a == "--write-through"){
            cfg.write_back = false;
            continue;
        }

        if(i == argc) continue;

        if(a == "--trace"){
            trace_path = std::filesystem::path(argv[++i]);
            continue;
        }

        if(a == "--size"){
            auto val = cache::parse_u64(argv[++i], 10);
            if(!val){
                std::cerr << "Invalid --size value\n"; return 2;
            }
            cfg.cache_size = *val;
            continue;
        }

        if(a == "--block"){
            auto val = cache::parse_u64(argv[++i], 10);
            if(!val){
                std::cerr << "Invalid --block value\n"; return 2;
            }
            cfg.block_size = static_cast<std::size_t>(*val);
            continue;
        }

        if(a == "--assoc"){
            auto val = cache::parse_u64(argv[++i], 10);
            if(!val){
                std::cerr << "Invalid --assoc value\n"; return 2;
            }
            cfg.ways = static_cast<std::size_t>(*val);
            continue;
        }

        if(a == "--policy"){
            cfg.replacement = argv[++i];
            continue;
        }

        std::cerr << "Unknown or incomplete option: " << a << '\n';
        print_usage(prog);
        return 2;
    }

    if(!trace_path){
        std::cerr << "Error: --trace is required\n";
        print_usage(prog);
        return 2;
    }

    if((cfg.block_size == 0) || (cfg.cache_size == 0) || (cfg.ways == 0)
        || (cfg.cache_size % (cfg.block_size * cfg.ways) != 0)){
        std::cerr << "Error: Invalid cache configuration\n";
        print_usage(prog);
        return 2;
    }

    // Run simulator
    cache::Simulator sim;
    try {
        sim.init(cfg, *trace_path);
        sim.run();
        sim.report();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 3;
    } catch (...) {
        std::cerr << "Unknown fatal error\n";
        return 3;
    }
    return 0;
}