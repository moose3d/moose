GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources = PhoenixSpatial.h
cc_sources = PhoenixSpatial.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Scene -I../Math
noinst_LTLIBRARIES = libPhoenixSpatial.la
libPhoenixSpatial_la_SOURCES = $(h_sources) $(cc_sources)
