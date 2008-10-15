#ifndef __COpenALObject_h__
#define __COpenALObject_h__
/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <al.h>
#else
#include <AL/al.h>
#endif
#include <ostream>
#include <iostream>
#include <assert.h>
#include <string>
#include "Core/PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
#define ReportALErrors()					\
{								\
  ALenum error = alGetError();					\
  if ( error != AL_NO_ERROR)					\
  {								\
    std::cerr << GetOpenALErrorString(error) << std::endl;	\
  }								\
}
/////////////////////////////////////////////////////////////////
#define ReportAndHaltOnALErrors()				\
{								\
  ALenum error =  alGetError();					\
  if ( error != AL_NO_ERROR)					\
  {								\
    std::cerr << GetOpenALErrorString(error) << std::endl;	\
    assert( false );						\
  }								\
}
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    template< size_t OBJECTS >
    class PHOENIX_API CALObjectArray
    {
    private:
      ALuint m_Objects[OBJECTS];
    public:

      CALObjectArray();
      virtual ~CALObjectArray() {}
      ALuint & GetALObject( size_t nObject = 0) { return m_Objects[nObject]; }
      std::string GetOpenALErrorString( ALenum error );
    };
  }
}
/////////////////////////////////////////////////////////////////
template< size_t OBJECTS >
Phoenix::Sound::CALObjectArray<OBJECTS>::CALObjectArray()
{
  for( size_t i=0;i<OBJECTS; i++)
  {
    m_Objects[i] = 0;
  }
}
/////////////////////////////////////////////////////////////////
template< size_t OBJECTS >
std::string
Phoenix::Sound::CALObjectArray<OBJECTS>::GetOpenALErrorString( ALenum error )
{
  switch( error )
  {
  case AL_NO_ERROR:
    return std::string("AL_NO_ERROR");
    break;

  case AL_INVALID_NAME:
    return std::string("AL_INVALID_NAME");
    break;

  case AL_INVALID_ENUM:
    return std::string("AL_INVALID_ENUM");
    break;

  case AL_INVALID_VALUE:
    return std::string("AL_INVALID_VALUE");
    break;

  case AL_INVALID_OPERATION:
    return std::string("AL_INVALID_OPERATION");
    break;
  case AL_OUT_OF_MEMORY:
    return std::string("AL_OUT_OF_MEMORY");
    break;
  default:
    return std::string("Unknown enum???");
    break;
  };
}
///////////////////////////////////////////////////////////////////////////
#endif
