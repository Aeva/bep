#/usr/bin/env sh
clang++ -O3 -ohail_eris -std=c++14 `pkg-config --static --libs rtmidi` glue/*.cpp *.cpp