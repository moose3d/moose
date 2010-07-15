GENERIC_LIBRARY_NAME="Phoenix"
AM_CXXFLAGS = -O2
h_sources = PhoenixFlock.h  PhoenixMessageSystem.h  PhoenixPathFinding.h  PhoenixStateMachine.h PhoenixAIScript.h PhoenixCollisionEvent.h
cc_sources = PhoenixFlock.cpp  PhoenixMessageSystem.cpp    PhoenixStateMachine.cpp  PhoenixAIScript.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../Scene
noinst_LTLIBRARIES = libPhoenixAI.la
libPhoenixAI_la_SOURCES = $(h_sources) $(cc_sources)
