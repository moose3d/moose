GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O0
h_sources = 	MooseALObjectArray.h \
		MooseALSampleTypes.h  \
	    	MooseALSoundTypes.h  \
		MooseAudioSystem.h  \
		MooseOggSample.h  \
		MooseOggStreamSample.h  \
		MooseSoundBase.h  \
		MooseSoundSampleBase.h \
		MooseListener.h 
cc_sources = 	MooseALSampleTypes.cpp \
		MooseALSoundTypes.cpp \
		MooseAudioSystem.cpp \
		MooseOggSample.cpp \
		MooseOggStreamSample.cpp \
		MooseSoundBase.cpp \
		MooseSoundSampleBase.cpp \
		MooseListener.cpp 

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES =  -I@top_srcdir@/  -I../Core -I../Math -I../Spatial  -I../Graphics
noinst_LTLIBRARIES = libMooseAudio.la
libMooseAudio_la_SOURCES = $(h_sources) $(cc_sources)




