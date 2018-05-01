#!/bin/bash

mkdir build
cd build 
cmake ..
make

./CDecompose ../data/block.off