#!/bin/bash

SRC="automata/*.cpp lexical_analysis/*.cpp parser/*.cpp"

echo "Compiling project..."
g++ -std=c++17 -Wall -O2 $SRC -I. -o compiler

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running program..."
./compiler