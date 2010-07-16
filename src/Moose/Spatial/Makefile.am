GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = MooseOrientable.h MooseDimensional1D.h MooseDimensional2D.h MooseDimensional3D.h MooseOneDirectional.h MoosePositional.h MooseTriangle.h MooseVertex.h
cc_sources = MooseOrientable.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Scene -I../Math
noinst_LTLIBRARIES = libMooseSpatial.la
libMooseSpatial_la_SOURCES = $(h_sources) $(cc_sources)
