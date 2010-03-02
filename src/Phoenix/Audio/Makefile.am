GENERIC_LIBRARY_NAME="Phoenix"
AM_CXXFLAGS = 
h_sources = 	PhoenixALObjectArray.h \
		PhoenixALSampleTypes.h  \
	    	PhoenixALSoundTypes.h  \
		PhoenixAudioSystem.h  \
		PhoenixOggSample.h  \
		PhoenixOggStreamSample.h  \
		PhoenixSoundBase.h  \
		PhoenixSoundSampleBase.h \
		PhoenixListener.h 
cc_sources = 	PhoenixALSampleTypes.cpp \
		PhoenixALSoundTypes.cpp \
		PhoenixAudioSystem.cpp \
		PhoenixOggSample.cpp \
		PhoenixOggStreamSample.cpp \
		PhoenixSoundBase.cpp \
		PhoenixSoundSampleBase.cpp \
		PhoenixListener.cpp 

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES =  -I@top_srcdir@/  -I../Core -I../Math -I../Spatial  -I../Graphics
noinst_LTLIBRARIES = libPhoenixAudio.la
libPhoenixAudio_la_SOURCES = $(h_sources) $(cc_sources)




