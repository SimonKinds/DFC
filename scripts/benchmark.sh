build_dir="build-benchmark"
build_target="bm-dfc"

if [ ! -d "$build_dir" ]; then
  mkdir "$build_dir"
fi

cd "$build_dir"

if [ "$(command -v ninja)" ]; then
  cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
  ninja "$build_target"
else
  cmake .. -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
  make "$build_target"
fi

./"$build_target"
