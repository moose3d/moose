/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
