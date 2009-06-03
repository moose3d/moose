GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources = PhoenixAPI.h PhoenixCore.h PhoenixGraph.h  PhoenixObjectCounter.h PhoenixWindowsWrapper.h PhoenixAVLTree.h PhoenixExceptions.h PhoenixLogger.h PhoenixObjectUpdater.h PhoenixConvert.h  PhoenixGlobals.h     PhoenixLog.h     PhoenixResourceManager.h PhoenixTagged.h PhoenixRadixSort.h
cc_sources = PhoenixExceptions.cpp  PhoenixGraph.cpp  PhoenixLogger.cpp  PhoenixObjectUpdater.cpp  PhoenixWindowsWrapper.cpp	

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/
noinst_LTLIBRARIES = libPhoenixCore.la
libPhoenixCore_la_SOURCES = $(h_sources) $(cc_sources)
