#!/bin/bash
set -e
source env.src
rm -rf ./lib/picotool
git clone https://github.com/raspberrypi/picotool.git ./../../lib/picotool
cd ./lib/picotool
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
picotool --help
