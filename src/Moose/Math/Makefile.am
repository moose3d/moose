GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = 	MooseCollision.h     MooseMath.h       MooseMatrix2x2.h  MooseMatrix4x4.h   MooseTransform.h  MooseVector3.h MooseLine.h MooseLineSegment.h MoosePlane.h MooseQuad.h MooseMathUtils.h  MooseMatrix3x3.h  MooseQuaternion.h  MooseVector2.h    MooseVector4.h MooseCollider.h MooseSphereCollider.h MooseOBBCollider.h MooseRay.h MooseCompoundCollider.h MooseCapsuleCollider.h 

cc_sources = 	MooseCollision.cpp  MooseMath.cpp  MooseQuad.cpp  MoosePlane.cpp MooseQuaternion.cpp  MooseTransform.cpp MooseSphereCollider.cpp MooseOBBCollider.cpp MooseCompoundCollider.cpp MooseCapsuleCollider.cpp 

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial
noinst_LTLIBRARIES = libMooseMath.la
libMooseMath_la_SOURCES = $(h_sources) $(cc_sources)
