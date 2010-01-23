## Source directory
@SET_MAKE@
#AM_CFLAGS = $(CFLAGS) $(CXXFLAGS)
#AM_CXXFLAGS = -O2
GENERIC_RELEASE=0:0:1
GENERIC_LIBRARY_VERSION=1:0:0
GENERIC_LIBRARY_NAME="Phoenix"

SUBDIRS = Core Audio Math Graphics Scene AI Spatial Volume

h_sources = Phoenix.h PhoenixDefaultEntities.h

cc_sources = PhoenixDefaultEntities.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources) 

INCLUDES = -I$(top_srcdir) -I./Core -I./Volume  -I./Graphics -I./Spatial -I./Math -I./AI -I./Scene -I./Audio

lib_LTLIBRARIES= libPhoenix.la
libPhoenix_la_SOURCES= $(h_sources) $(cc_sources)
libPhoenix_la_LIBADD = $(CEGUI_LIBS) $(LIBCONFIG_LIBS) Core/libPhoenixCore.la  Audio/libPhoenixAudio.la Math/libPhoenixMath.la Graphics/libPhoenixGraphics.la Scene/libPhoenixScene.la AI/libPhoenixAI.la Spatial/libPhoenixSpatial.la Volume/libPhoenixVolume.la 
#libPhoenix_0_99_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)




