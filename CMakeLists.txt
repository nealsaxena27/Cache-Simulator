cmake_minimum_required(VERSION 4.0)

project("L1 Cache Simulator"
    VERSION 0.1
    DESCRIPTION "L1 Cache Simulator"
    LANGUAGES CXX)

# --- Language / compiler ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recommend out-of-source builds
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(WARNING "In-source build detected. Recommended: mkdir build && cd build && cmake ..")
endif()

# Default build type for single-config generators
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Put produced binaries/libs in build/bin, build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options (simple)
option(ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# --- Source files (explicit list) ---
set(SRC_FILES
    src/main.cpp
    src/cache.cpp
    src/cache_set.cpp
    src/simulator.cpp
    src/trace_reader.cpp
    src/stats.cpp
    src/policies/lru.cpp
    src/policies/fifo.cpp
)

# Public headers directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# --- Core library ---
add_library(cachelib STATIC ${SRC_FILES})
target_include_directories(cachelib
    PUBLIC
      ${INCLUDE_DIR}
)

# Compiler warning flags (cross-platform)
if (MSVC)
  target_compile_options(cachelib PRIVATE /W4 /permissive-)
else()
  target_compile_options(cachelib PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (ENABLE_WARNINGS_AS_ERRORS)
  if (MSVC)
    target_compile_options(cachelib PRIVATE /WX)
  else()
    target_compile_options(cachelib PRIVATE -Werror)
  endif()
endif()

# --- Executable ---
add_executable(cache_simulator src/main.cpp)
target_link_libraries(cache_simulator PRIVATE cachelib)

# --- Install rules (optional) ---
install(TARGETS cache_simulator cachelib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
