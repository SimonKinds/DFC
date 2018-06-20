cmake_minimum_required(VERSION 3.1) 
project(dfc-opencl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third-party/benchmark)

add_executable(test-dfc
  src/test-main.cpp
  src/test-pattern-aggregator.cpp
  src/test-indexer.cpp
)
target_include_directories(test-dfc PRIVATE third-party)

add_executable(bm-dfc
  src/bm-indexer.cpp
)
target_link_libraries(bm-dfc benchmark_main)
target_include_directories(bm-dfc PRIVATE third-party)
