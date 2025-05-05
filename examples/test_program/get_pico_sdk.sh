#!/bin/bash
git clone -b master https://github.com/raspberrypi/pico-sdk.git ../../lib/pico-sdk
cd ./../../lib/pico-sdk
git submodule update --init
