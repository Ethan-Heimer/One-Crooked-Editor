#! /bin/bash

cd build
cmake .. -DCMAKE_BUILD_TYPE=empty
make

cd bin
sudo cp ./CrookedEditor /usr/local/bin/


