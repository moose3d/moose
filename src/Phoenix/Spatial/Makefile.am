GENERIC_LIBRARY_NAME="Phoenix"
AM_CXXFLAGS = -O2
h_sources = PhoenixOrientable.h PhoenixDimensional1D.h PhoenixDimensional2D.h PhoenixDimensional3D.h PhoenixOneDirectional.h PhoenixPositional.h PhoenixTriangle.h PhoenixVertex.h
cc_sources = PhoenixOrientable.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Scene -I../Math
noinst_LTLIBRARIES = libPhoenixSpatial.la
libPhoenixSpatial_la_SOURCES = $(h_sources) $(cc_sources)
