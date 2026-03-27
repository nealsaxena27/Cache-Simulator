# Cache Simulator

A basic cache simulator written in modern **C++20**, using **CMake** for build system.  
The simulator is trace-driven and supports configurable cache geometry and multiple replacement policies (currently only LRU, FIFO are supported). The simulator also provides the option to simulate a two-level inclusive cache memory heirarchy.

---

## Quick Summary

- Language: **C++20**
- Build System: **CMake**
- Replacement Policy: **LRU**, **FIFO**
- Configurable: cache size, block size, associativity, replacement policy, write policy, write-miss behavior, single-level/two-level
- Input: trace file (`R/W <address>`). Addresses accepted in both decimal and hexadecimal.
- Output: formatted statistics table (hits, misses, miss rate, etc.) for each level cache

---

## Build

From the project root directory:

```bash
mkdir build && cd build && cmake .. && cmake --build .
```

---

## Usage

```bash
./build/bin/cache_simulator --trace <trace-file> [options]
```

---

## Parameters

- --trace <path> (required): Path to trace file.
- --size <bytes>: Total cache size in bytes. (Default: 32768)
- --block <bytes>: Block (cache line) size in bytes. (Default: 64)
- --assoc <ways>: Associativity (number of ways per set). (Default: 8)
- --write-allocate / --no-write-allocate: Controls write miss behavior. (Default: write-allocate)
- --write-back / --write-through: Controls write policy. (Default: write-back)
- --policy <name>: Replacement policy. (Currently supported: lru, fifo)
- --multilevel: Enables multilevel cache (L1, L2)
- --l2-size <bytes>: Total L2 cache size in bytes. (Default: 32768)
- --l2-block <bytes>: L2 Block (cache line) size in bytes. (Default: 64)
- --l2-assoc <ways>: L2 Associativity (number of ways per set). (Default: 8)
- --l2-write-allocate / --l2-no-write-allocate: Controls L2 write miss behavior. (Default: write-allocate)
- --l2-write-back / --l2-write-through: Controls L2 write policy. (Default: write-back)
- --l2-policy <name>: L2 Replacement policy. (Currently supported: lru, fifo)
- -h, --help: Display help message.
