GENERIC_LIBRARY_NAME="Phoenix"
AM_CXXFLAGS = -O2
h_sources = 	PhoenixGameObject.h  PhoenixTransformGraph.h PhoenixSpatialGraph.h PhoenixScene.h PhoenixApplication.h PhoenixDirectionalLightObject.h PhoenixCameraObject.h

cc_sources = 	PhoenixGameObject.cpp PhoenixTransformGraph.cpp PhoenixSpatialGraph.cpp PhoenixScene.cpp PhoenixApplication.cpp PhoenixDirectionalLightObject.cpp PhoenixCameraObject.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../AI
noinst_LTLIBRARIES = libPhoenixScene.la
libPhoenixScene_la_SOURCES = $(h_sources) $(cc_sources)
