#! /bin/bash

cd build
cmake ..
make

cd bin
sudo cp ./CrookedEditor /usr/local/bin/


