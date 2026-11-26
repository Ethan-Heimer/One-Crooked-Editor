#! /bin/bash

cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

if (( $? == 0)); then  
    cd bin
    ./CrookedEditor ./tester.txt
fi
