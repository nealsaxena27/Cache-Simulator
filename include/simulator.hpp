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
            initializes the Simulator with given Configuration and trace path
        */
        void init(const Config &cfg, const std::filesystem::path &trace_path);

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
        std::unique_ptr<Cache> cache_; // ownership of Cache
    };
} // namespace cache