GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = MooseOctree.h MooseSphereBound.h MooseBoxBound.h MooseAABB.h MooseAACube.h MooseOBB.h MooseSphere.h MooseCapsule.h MooseCone.h MoosePolytope.h MooseDecalVolume.h
cc_sources = MooseCone.cpp MooseSphereBound.cpp MooseBoxBound.cpp MooseAABB.cpp MooseOBB.cpp MooseSphere.cpp 

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core  -I../Spatial  -I../Graphics -I../Scene -I../Math
noinst_LTLIBRARIES = libMooseVolume.la
libMooseVolume_la_SOURCES = $(h_sources) $(cc_sources)
