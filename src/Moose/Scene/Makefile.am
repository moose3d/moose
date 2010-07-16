GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = 	MooseGameObject.h  MooseTransformGraph.h MooseSpatialGraph.h MooseScene.h MooseApplication.h MooseDirectionalLightObject.h MooseCameraObject.h MooseEventQueue.h

cc_sources = 	MooseGameObject.cpp MooseTransformGraph.cpp MooseSpatialGraph.cpp MooseScene.cpp MooseApplication.cpp MooseDirectionalLightObject.cpp MooseCameraObject.cpp MooseEventQueue.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../AI
noinst_LTLIBRARIES = libMooseScene.la
libMooseScene_la_SOURCES = $(h_sources) $(cc_sources)
