#AM_CXXFLAGS = -O0 
h_sources = PhoenixFlock.h  PhoenixMessageSystem.h  PhoenixPathFinding.h  PhoenixStateMachine.h
cc_sources = PhoenixFlock.cpp  PhoenixMessageSystem.cpp    PhoenixStateMachine.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math
noinst_LTLIBRARIES = libPhoenixAI.la
libPhoenixAI_la_SOURCES = $(h_sources) $(cc_sources)
