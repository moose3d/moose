add_sources (Moose
  MooseALSampleTypes.cpp
  MooseListener.cpp
  MooseOggStreamSample.cpp
  MooseALSoundTypes.cpp
  MooseSoundBase.cpp
  MooseAudioSystem.cpp
  MooseOggSample.cpp
  MooseSoundSampleBase.cpp
 ) 

# Check for Apple platform
if(APPLE)
  add_sources (Moose MooseMusicClip.cpp)
else(APPLE)
  message(STATUS  "Disabling apple SDK")
endif(APPLE)


