set (audio_sources  MooseALSampleTypes.cpp  MooseListener.cpp   MooseOggStreamSample.cpp MooseALSoundTypes.cpp   MooseSoundBase.cpp MooseAudioSystem.cpp    MooseOggSample.cpp ) 

set (audio_sources ${audio_sources} MooseSoundSampleBase.cpp)

# Check for Apple platform
if(APPLE)
  set (audio_sources ${audio_sources} MooseMusicClip.cpp) 
else(APPLE)
  message(STATUS  "Disabling apple SDK")
endif(APPLE)

add_library( Audio ${audio_sources})
