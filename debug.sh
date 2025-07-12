mkdir -p build
cd build
cmake -B . -S ../ -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld" \
    -DCMAKE_BUILD_TYPE=Debug
ninja && ninja Shaders && ./CupEngine
cd ..
