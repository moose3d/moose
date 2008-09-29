AM_CXXFLAGS = -O0 
h_sources = 	PhoenixALObjectArray.h \
		PhoenixALSampleTypes.h  \
	    	PhoenixALSoundTypes.h  \
		PhoenixAudioSystem.h  \
		PhoenixOggSample.h  \
		PhoenixOggStreamSample.h  \
		PhoenixSoundBase.h  \
		PhoenixSoundSampleBase.h 

cc_sources = 	PhoenixALSampleTypes.cpp \
		PhoenixALSoundTypes.cpp \
		PhoenixAudioSystem.cpp \
		PhoenixOggSample.cpp \
		PhoenixOggStreamSample.cpp \
		PhoenixSoundBase.cpp \
		PhoenixSoundSampleBase.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/
noinst_LTLIBRARIES = libPhoenixAudio.la
libPhoenixAudio_la_SOURCES = $(h_sources) $(cc_sources)




