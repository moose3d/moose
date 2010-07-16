#!/bin/bash

mkdir -p ./tcl/Moose
rm -f Moose_wrap.cxx
swig -c++ -DMOOSE_API=""  -I./src/Moose -I./src/Moose/AI -I./src/Moose/Audio -I./src/Moose/Core -I./src/Moose/Graphics -I./src/Moose/Scene -I./src/Moose/Spatial  -I./src/Moose/Volume -I./src/Moose/Util -I./src/Moose/Math -tcl -pkgversion 0.9 Moose.i 

g++ -c -Wall -fPIC `pkg-config --cflags moose` -D__STDC_CONSTANT_MACROS  Moose_wrap.cxx 
rm -f tcl/Moose/Moose.so
rm -f tcl/Moose/pkgIndex.tcl
g++ -shared Moose_wrap.o -o tcl/Moose/Moose.so `pkg-config --libs moose`

