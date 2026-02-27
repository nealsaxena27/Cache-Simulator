#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <filesystem>

namespace cache{
    // representation of a memory access record.
    struct AccessRecord{
        uint64_t address;
        bool is_write;
    };

    class TraceReader{
    public:
        TraceReader() = default;
        ~TraceReader();

        TraceReader(const TraceReader&) = delete;
        TraceReader& operator=(const TraceReader&) = delete;
        TraceReader(TraceReader&&) noexcept = default;
        TraceReader& operator=(TraceReader&&) noexcept = default;

        /*
            opens a trace file.
            returns true on success, false otherwise.
        */
        bool open(const std::filesystem::path &path);

        /*
            closes the file.
        */
        void close();

        /*
            reads next AccessRecord into out.
            returns false on false on EOF or error.
        */
        bool next(AccessRecord &out);

        /*
            resets file pointer to the beginning.
        */
        void reset();

        /*
            returns true if the file is open, false otherwise.
        */
        bool is_open() const noexcept{ return infile_.is_open(); }
    private:
        std::ifstream infile_; // input file stream
        std::filesystem::path path_; // path of the file
    };
} // namespace cache

