#!/bin/bash

mkdir -p ./tcl/Moose
rm -f Moose_wrap.cxx
swig -c++ -DPHOENIX_API=""  -I/usr/include/Phoenix -tcl -pkgversion 0.9 Moose.i 

g++ -c -Wall -fPIC `pkg-config --cflags phoenix` -D__STDC_CONSTANT_MACROS  Moose_wrap.cxx 
rm -f tcl/Moose/Moose.so
g++ -shared Moose_wrap.o -o tcl/Moose/Moose.so `pkg-config --libs phoenix`
