GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources =  	PhoenixAPI.h \
	     	PhoenixGraphEdge.h \
	     	PhoenixGraphNode.h \
	     	PhoenixGraph.h \
	     	PhoenixSelectable.h \
	     	PhoenixAVLTree.h \
		PhoenixHashTable.h \
	     	PhoenixSingleton.h \
	     	PhoenixCacheable.h \
	     	PhoenixLogger.h \
	     	PhoenixTagged.h \
	     	PhoenixConvert.h \
	     	PhoenixLog.h \
	     	PhoenixTimer.h \
	     	PhoenixCore.h \
	    	PhoenixNamed.h \
	        PhoenixTimeStamp.h \
		PhoenixEnableable.h  \
		PhoenixNullable.h \
		PhoenixTypeBase.h \
		PhoenixExceptions.h \
		PhoenixObjectCounter.h \
		PhoenixUniqueNameCreator.h \
		PhoenixFocusable.h \
		PhoenixObjectUpdater.h \
		PhoenixViewable.h \
		PhoenixFPSCounter.h \
		PhoenixRadixSort.h \
		PhoenixWindowsWrapper.h \
		PhoenixGlobals.h \
		PhoenixResourceManager.h
cc_sources = PhoenixExceptions.cpp    PhoenixLogger.cpp  PhoenixObjectUpdater.cpp  PhoenixWindowsWrapper.cpp 	
# templatized
# PhoenixGraph.cpp
library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources) PhoenixGraph.tpp

INCLUDES = -I@top_srcdir@/
noinst_LTLIBRARIES = libPhoenixCore.la
libPhoenixCore_la_SOURCES = $(h_sources) $(cc_sources)
