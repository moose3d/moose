GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources = 	PhoenixCollision.h     PhoenixMath.h       PhoenixMatrix2x2.h  PhoenixMatrix4x4.h   PhoenixTransform.h  PhoenixVector3.h PhoenixMathGeometry.h  PhoenixMathUtils.h  PhoenixMatrix3x3.h  PhoenixQuaternion.h  PhoenixVector2.h    PhoenixVector4.h

cc_sources = 	PhoenixCollision.cpp  PhoenixMath.cpp  PhoenixMathGeometry.cpp  PhoenixQuaternion.cpp  PhoenixTransform.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial
noinst_LTLIBRARIES = libPhoenixMath.la
libPhoenixMath_la_SOURCES = $(h_sources) $(cc_sources)
