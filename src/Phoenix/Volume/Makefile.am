#AM_CXXFLAGS = -O0 
h_sources = PhoenixOctree.h PhoenixVolume.h
cc_sources = PhoenixVolume.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core  -I../Spatial  -I../Graphics -I../Scene -I../Math
noinst_LTLIBRARIES = libPhoenixVolume.la
libPhoenixVolume_la_SOURCES = $(h_sources) $(cc_sources)
