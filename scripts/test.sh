#! /usr/bin/env sh

if [ "$#" -ne 1 ]; then
  echo Must pass build type as parameter
  exit 1
fi

if [ ! -d build ]; then
  mkdir build
fi

cd build

if [ "$(command -v ninja)" ]; then
  cmake .. -DCMAKE_BUILD_TYPE="$1" -G Ninja
  ninja test-dfc
else
  cmake .. -DCMAKE_BUILD_TYPE="$1" -G "Unix Makefiles"
  make test-dfc
fi

./test-dfc
