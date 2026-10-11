#! /bin/bash

# goal: create a dir that contains all header needed for plugins

devdir="./devheaders"

cd build
mkdir $devdir
cd ..

# symlink all util headers
for d in ./libs/utillibs/*; do
    headers=$(find $d -maxdepth 1 -type f \( -name '*.h' -o -name '*.hpp' \))

    cd build/$devdir
    for h in $headers; do
        echo $h
        ln -s $h $(basename $h)
    done
    cd ../..
done

# symlink all core headers

for d in ./kernal/*/include/; do
    coreheaders=$(find $d -maxdepth 1 -type f \( -name '*.h' -o -name '*.hpp' \))

    cd build/$devdir
    for h in $coreheaders; do
        echo $h
        ln -s $h $(basename $h)
    done
    cd ../..
done
