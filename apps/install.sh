#!/bin/bash

mkdir build
cd build
cmake ..
make -j6
cd ..
ln -sf build/apps apps
