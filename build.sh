#! /bin/bash

cd build
cmake ..
make

if (( $? == 0)); then  
    cd bin
    ./CrookedEditor ../../test.txt
fi
