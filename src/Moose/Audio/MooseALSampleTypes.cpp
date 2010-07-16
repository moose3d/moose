#include "MooseALSampleTypes.h"
#include <iostream>
using namespace std;
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
libname::CALSample::CALSample()
{
  alGenBuffers( 1, &GetALObject());
  //cerr << "Creating buffer : " << GetALObject() << endl;
  ReportAndHaltOnALErrors();
}
/////////////////////////////////////////////////////////////////
libname::CALSample::~CALSample()
{
  alDeleteBuffers(1, &GetALObject());
  alGetError();
}
/////////////////////////////////////////////////////////////////
libname::CALStreamSample::CALStreamSample( size_t nBufSize ) : CSoundSampleBase( nBufSize )
{
  alGenBuffers( 2, &GetALObject());
  ReportAndHaltOnALErrors();
}
/////////////////////////////////////////////////////////////////
libname::CALStreamSample::~CALStreamSample()
{
  alDeleteBuffers(2, &GetALObject());
  alGetError();
}
/////////////////////////////////////////////////////////////////
bool
libname::CALStreamSample::HasEnded()
{
  return m_bHasEnded;
}
/////////////////////////////////////////////////////////////////
