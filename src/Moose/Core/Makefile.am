GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources =  	MooseAPI.h \
	     	MooseGraphEdge.h \
	     	MooseGraphNode.h \
	     	MooseGraph.h \
	     	MooseSelectable.h \
	     	MooseAVLTree.h \
		MooseHashTable.h \
	     	MooseSingleton.h \
	     	MooseCacheable.h \
	     	MooseLogger.h \
	     	MooseTagged.h \
	     	MooseConvert.h \
	     	MooseLog.h \
	     	MooseTimer.h \
	     	MooseCore.h \
	    	MooseNamed.h \
	        MooseTimeStamp.h \
		MooseEnableable.h  \
		MooseNullable.h \
		MooseTypeBase.h \
		MooseExceptions.h \
		MooseObjectCounter.h \
		MooseUniqueNameCreator.h \
		MooseFocusable.h \
		MooseUpdateable.h \
		MooseViewable.h \
		MooseFPSCounter.h \
		MooseRadixSort.h \
		MooseWindowsWrapper.h \
		MooseGlobals.h \
		MooseResourceManager.h
cc_sources = MooseExceptions.cpp    MooseLogger.cpp   MooseWindowsWrapper.cpp 	
# templatized
# MooseGraph.cpp
library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources) MooseGraph.tpp

INCLUDES = -I@top_srcdir@/
noinst_LTLIBRARIES = libMooseCore.la
libMooseCore_la_SOURCES = $(h_sources) $(cc_sources)
