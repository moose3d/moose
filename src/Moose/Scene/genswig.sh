#!/bin/bash

swig -I.. -I../AI -I../Volume -I../Graphics -I../Spatial -I../Math -I../Core -DPHOENIX_API="" -c++ -tcl PhoenixGameObject.i

g++ -c -Wall -fPIC `pkg-config --cflags phoenix` PhoenixGameObject_wrap.cxx

g++ -shared PhoenixGameObject.o PhoenixGameObject_wrap.o -o scene.so `pkg-config --libs phoenix` -lconfig++
