GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = MooseGrid.h MooseTransformIndicator.h
cc_sources = MooseGrid.cpp MooseTransformIndicator.cpp
library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../Graphics
noinst_LTLIBRARIES = libMooseUtil.la
libMooseUtil_la_SOURCES = $(h_sources) $(cc_sources)
