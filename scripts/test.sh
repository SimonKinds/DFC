#! /usr/bin/env sh

build_dir="build-test"
build_target="test-dfc"

if [ "$#" -ne 1 ]; then
  echo Must pass build type as parameter
  exit 1
fi


if [ ! -d "$build_dir" ]; then
  mkdir "$build_dir"
fi

cd "$build_dir"

if [ "$(command -v ninja)" ]; then
  cmake .. -DCMAKE_BUILD_TYPE="$1" -G Ninja
  ninja "$build_target"
else
  cmake .. -DCMAKE_BUILD_TYPE="$1" -G "Unix Makefiles"
  make "$build_target"
fi

./"$build_target"
