## Source directory
@SET_MAKE@
#AM_CFLAGS = $(CFLAGS) $(CXXFLAGS)
AM_CXXFLAGS = -O2
GENERIC_RELEASE=0:0:1
GENERIC_LIBRARY_VERSION=1:0:0
GENERIC_LIBRARY_NAME="Moose"

SUBDIRS = Core Audio Math Graphics Scene AI Spatial Volume Util

h_sources = Moose.h MooseDefaultEntities.h

cc_sources = MooseDefaultEntities.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources) 

INCLUDES = -I$(top_srcdir) -I./Core -I./Volume  -I./Graphics -I./Spatial -I./Math -I./AI -I./Scene -I./Audio -I./Util

lib_LTLIBRARIES= libMoose.la
libMoose_la_SOURCES= $(h_sources) $(cc_sources)
libMoose_la_LIBADD = $(CEGUI_LIBS) $(LIBCONFIG_LIBS) Core/libMooseCore.la  Audio/libMooseAudio.la Math/libMooseMath.la Graphics/libMooseGraphics.la Scene/libMooseScene.la AI/libMooseAI.la Spatial/libMooseSpatial.la Volume/libMooseVolume.la Util/libMooseUtil.la
#libMoose_0_99_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)




