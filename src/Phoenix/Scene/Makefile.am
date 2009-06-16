GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources = 	PhoenixGameObject.h  PhoenixTransformGraph.h PhoenixSpatialGraph.h 

cc_sources = 	PhoenixGameObject.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math
noinst_LTLIBRARIES = libPhoenixScene.la
libPhoenixScene_la_SOURCES = $(h_sources) $(cc_sources)
