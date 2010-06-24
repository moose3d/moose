#!/bin/bash

rm -f Moose_wrap.cxx
swig -c++ -DPHOENIX_API=""  -I/usr/include/Phoenix -tcl Moose.i 

g++ -c -Wall -fPIC `pkg-config --cflags phoenix` -D__STDC_CONSTANT_MACROS  Moose_wrap.cxx
rm -f Moose.so

g++ -shared Moose_wrap.o -o Moose.so `pkg-config --libs phoenix`

