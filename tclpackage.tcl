#!/usr/bin/tclsh
pkg_mkIndex ./tcl/Moose Moose.so
exec cp -Rf ./tcl/Moose /usr/lib
