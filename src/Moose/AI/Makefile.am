GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = MooseFlock.h  MooseMessageSystem.h  MoosePathFinding.h  MooseStateMachine.h MooseAIScript.h MooseCollisionEvent.h
cc_sources = MooseFlock.cpp  MooseMessageSystem.cpp    MooseStateMachine.cpp  MooseAIScript.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../Scene
noinst_LTLIBRARIES = libMooseAI.la
libMooseAI_la_SOURCES = $(h_sources) $(cc_sources)
