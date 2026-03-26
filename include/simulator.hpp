#pragma once
#include "config.hpp"
#include "trace_reader.hpp"
#include "cache.hpp"
#include <string>
#include <filesystem>

namespace cache{
    class Simulator{
    public:
        Simulator() = default;

        /*
            initializes the Simulator with given Configuration, and 
            L2 configuration (if l2_flag is true), and trace path
        */
        void init(const Config &cfg, const Config &l2_cfg,
            bool l2_flag, const std::filesystem::path &trace_path);

        /*
            runs the simulation, reading trace records till EOF, feeding them to Cache
        */
        void run();

        /*
            reports the collected statistics
        */
        void report() const;

    private:
        Config cfg_; // configuration of Cache
        TraceReader trace_; // TraceReader for reading AccessRecords from trace input file
        std::unique_ptr<Cache> l1_cache_; // ownership of L1 Cache
        std::unique_ptr<Cache> l2_cache_; // ownership of L2 Cache
    };
} // namespace cache