add_sources (Moose

  MooseALSampleTypes.cpp
  MooseListener.cpp
  MooseOggStreamSample.cpp
  MooseALSoundTypes.cpp
  MooseSoundBase.cpp
  MooseAudioSystem.cpp
  MooseOggSample.cpp
  MooseSoundSampleBase.cpp
  MooseListener.h
  MooseALSampleTypes.h
  MooseOggStreamSample.h
  MooseALSoundTypes.h
  MooseSoundBase.h
  MooseAudioSystem.h
  MooseOggSample.h
  MooseSoundSampleBase.h
 ) 

# Check for Apple platform
if(APPLE)
  add_sources (Moose MooseIPhoneAudioSample.mm)
  add_sources (Moose MooseMusicClip.cpp)
  add_sources (Moose MooseIPhoneAudioSample.h)
  add_sources (Moose MooseMusicClip.h)
else(APPLE)
  message(STATUS  "Disabling apple SDK")
endif(APPLE)


