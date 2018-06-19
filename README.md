# DFC
High performance string matching algorithm


## NOTE
You're probably looking for the `v1` branch, found at https://github.com/SimonKinds/dfc-opencl/tree/v1


This branch is a rewrite using C++ for added flexibility, using the lessons learned in the C implementation.
By rewriting I can also relicense the code.


## Requirements
- CMake (3.1 or greater)
- OpenCL (1.2 or greater)
- C++ compiler (C++17 or greater)

## Building
```sh
mkdir build
cd build
cmake ..
make
```

## Reference
Byungkwon Choi, JongWook Chae, Muhammad Jamshed, KyoungSoo Park, and Dongsu Han, _DFC: Accelerating String Pattern Matching for Network Applications_, NSDI 2016 [PDF](http://ina.kaist.ac.kr/~dongsuh/paper/nsdi16-paper-choi.pdf)
