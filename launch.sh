#!/bin/bash

WARNINGS="-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast \
  -Wunused -Wcast-align -Wconversion -Wsign-conversion -Wdouble-promotion \
  -Wimplicit-fallthrough -pedantic"
  
echo "formatting..."
clang-format -i src/*.hpp src/*.cpp

echo "tidying..."
#clang-tidy \
#  src/*.cpp src/*.hpp \
#  -- -I./src -std=c++23 -fconstexpr-steps=80000000

echo "building..."
clang++ -std=c++23 -o bin/main ./src/*.cpp -I./src/ $WARNINGS -O3 -fconstexpr-steps=200000000

# echo "running..."
# ./bin/main
