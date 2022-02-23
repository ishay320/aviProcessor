#!/usr/bin/bash
set -x

# update for the latest
sudo apt update

# install openCV dev && make && g++
sudo apt install -y make libopencv-dev g++