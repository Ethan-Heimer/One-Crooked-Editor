#! /bin/bash

cd build
cmake ..
make

if (( $? == 0)); then  
    cd bin
    ./CrookedEditor ../../source/main.cpp
fi
