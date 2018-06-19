cmake_minimum_required(VERSION 3.1) 
project(dfc-opencl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror")
set(CMAKE_CXX_STANDARD 17)

add_executable(dfc-test 
  src/test-main.cpp
  src/test-pattern-aggregator.cpp
)
target_include_directories(dfc-test PRIVATE third-party)
