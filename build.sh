#!/bin/sh

set -xe

clang++ -std=c++17 -g `pkg-config --cflags raylib` -o main *.cpp `pkg-config --libs raylib` -lm -lpthread -L/opt/homebrew/lib -lglfw -framework CoreFoundation -framework Cocoa -framework IOKit -framework OpenGL -Wall

