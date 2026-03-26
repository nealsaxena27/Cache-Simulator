#include "simulator.hpp"
#include "config.hpp"
#include "trace_reader.hpp"
#include "stats.hpp"
#include <string>
#include <filesystem>
#include <memory>


namespace cache{
    void Simulator::init(const Config &cfg, const Config &l2_cfg, 
        bool l2_flag, const std::filesystem::path &trace_path){
        // store cfg, open trace and create Cache instance
        cfg_ = cfg;
        if(!trace_.open(trace_path)){
            throw std::runtime_error("Simulator::init: failed to open trace: " + trace_path.string());
        }
        l1_cache_ = std::make_unique<Cache>(cfg);
        if(l2_flag){
            l2_cache_ = std::make_unique<Cache>(l2_cfg);
            l1_cache_->init_hierarchy(l2_cache_.get(), nullptr);
            l2_cache_->init_hierarchy(nullptr, l1_cache_.get());
        }
        else{
            l2_cache_ = nullptr;
        }
    }

    void Simulator::run(){
        // repeatedly read trace records and feed to l1_cache_ until EOF
        AccessRecord instruction;
        while(trace_.next(instruction)) {
            l1_cache_->access(instruction.address, instruction.is_write);
        }
        l1_cache_->flush(); // flush the cache at end of simulation
        if(l2_cache_ != nullptr) l2_cache_->flush();
    }

    void Simulator::report() const{
        l1_cache_->stats().print("L1 CACHE");
        if(l2_cache_ != nullptr) l2_cache_->stats().print("L2 CACHE");
    }
}