#include "simulator.hpp"
#include "config.hpp"
#include "trace_reader.hpp"
#include "stats.hpp"
#include <string>
#include <filesystem>
#include <memory>


namespace cache{
    void Simulator::init(const Config &cfg, const std::filesystem::path &trace_path){
        // store cfg, open trace and create Cache instance
        cfg_ = cfg;
        if(!trace_.open(trace_path)){
            throw std::runtime_error("Simulator::init: failed to open trace: " + trace_path.string());
        }
        cache_ = std::make_unique<Cache>(cfg);
    }

    void Simulator::run(){
        // repeatedly read trace records and feed to cache_ until EOF
        AccessRecord instruction;
        while(trace_.next(instruction)) {
            cache_->access(instruction.address, instruction.is_write);
        }
        cache_->flush(); // flush the cache at end of simulation
    }

    void Simulator::report() const{
        cache_->stats().print();
    }
}