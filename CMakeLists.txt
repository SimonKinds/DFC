cmake_minimum_required(VERSION 3.1) 
project(dfc-opencl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror -fno-omit-frame-pointer")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third-party/benchmark)

set(DFC_SRCS 
  src/pattern.cpp
  src/pattern-aggregator.cpp
)

add_executable(match
  ${DFC_SRCS}

  match/main.cpp
  match/helpers.cpp
)
target_include_directories(match PRIVATE src)

add_executable(test-dfc
  ${DFC_SRCS}

  src/util-test.cpp

  src/test-main.cpp
  src/test-pattern-aggregator.cpp
  src/test-flat-direct-filter.cpp
  src/test-compact-table.cpp
  src/test-matcher.cpp
  src/test-match-path.cpp
  src/test-segmenter.cpp
  src/test-indexer.cpp
  src/test-layered-direct-filter.cpp
  src/test-execution-loop.cpp
  src/test-pattern-parser.cpp
)
target_include_directories(test-dfc PRIVATE third-party)

add_executable(bm-dfc
  ${DFC_SRCS}

  src/util-test.cpp

  src/bm-indexer.cpp
  src/bm-flat-direct-filter.cpp
  src/bm-layered-direct-filter.cpp
  src/bm-compact-table.cpp
)
target_link_libraries(bm-dfc benchmark_main)
