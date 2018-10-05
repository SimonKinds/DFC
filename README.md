# DFC
High performance string matching algorithm

## Requirements
- CMake (3.1 or greater)
- C++ compiler (C++17 or greater)

## Building
```sh
mkdir build
cd build
cmake ..
make
```

## Simple usage
Configure the file `match/config.hpp` with your preferred direct filter and compact table layout, and alias it to `CustomExecutionLoop`. Once that is done, recompile the project. You can then use the `match` executable to count the amount of matches in a file, given a new-line separated file of patterns. **NOTE** that the project does not currently support the `snort` syntax for binary patterns, instead it currently only supports ASCII patterns.

```sh
Usage: ./match pattern-file data-file [case-sensitive=1|0]
```

## License
MIT
