#include "stats.hpp"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

namespace cache{
    void StatsCollector::print(std::string_view title) const{
        const uint64_t total_accesses = reads + writes;
        
        // table layout
        constexpr int COL_LEFT = 32;
        constexpr int COL_RIGHT = 16;
        constexpr int TABLE_WIDTH = COL_LEFT + COL_RIGHT;

        // header
        std::cout << std::endl;
        std::cout << std::string(TABLE_WIDTH, '=') << '\n';
        std::cout << std::left << std::setw(COL_LEFT) << "STATISTICS"
        << std::right << std::setw(COL_RIGHT) << title << '\n';
        std::cout << std::string(TABLE_WIDTH, '=') << '\n';
        std::cout << std::left << std::setw(COL_LEFT) << "METRIC"
        << std::right << std::setw(COL_RIGHT) << "VALUE" << '\n';
        std::cout << std::string(TABLE_WIDTH, '-') << '\n';

        // rows
        std::cout << std::left << std::setw(COL_LEFT) << "Total Accesses"
        << std::right << std::setw(COL_RIGHT) << total_accesses << '\n';
        
        std::cout << std::left << std::setw(COL_LEFT) << "Reads"
        << std::right << std::setw(COL_RIGHT) << reads << '\n';
        
        std::cout << std::left << std::setw(COL_LEFT) << "Writes"
        << std::right << std::setw(COL_RIGHT) << writes << '\n';
        
        std::cout << std::left << std::setw(COL_LEFT) << "Hits"
        << std::right << std::setw(COL_RIGHT) << hits << '\n';
        
        std::cout << std::left << std::setw(COL_LEFT) << "Misses"
        << std::right << std::setw(COL_RIGHT) << misses << '\n';
        
        std::cout << std::left << std::setw(COL_LEFT) << "Writebacks"
        << std::right << std::setw(COL_RIGHT) << writebacks << '\n';
        
        // miss rate
        if(total_accesses == 0){
            std::cout << std::left << std::setw(COL_LEFT) << "Miss Rate (%)"
            << std::right << std::setw(COL_RIGHT) << "N/A" << '\n';
        } 
        else{
            const double miss_rate = (static_cast<double>(misses) * 100.0) / static_cast<double>(total_accesses);
            std::cout << std::left << std::setw(COL_LEFT) << "Miss Rate (%)"
            << std::right << std::setw(COL_RIGHT)
            << std::fixed << std::setprecision(4) << miss_rate << '\n';
        }

        // footer
        std::cout << std::string(TABLE_WIDTH, '=') << std::endl;
        std::cout << std::endl;
    }
}
