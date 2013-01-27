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

#ifndef __CALSampleTypes_h__
#define __CALSampleTypes_h__
/////////////////////////////////////////////////////////////////
#include "MooseSoundSampleBase.h"
#include "MooseALObjectArray.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CALSample : public CSoundSampleBase,
		      public CALObjectArray<1>
    {
    public:
      CALSample();
      ~CALSample();
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CALStreamSample : public CSoundSampleBase,
			    public CALObjectArray<2>
    {
    protected:
      bool m_bHasEnded;
    public:
      CALStreamSample( size_t nBufSize = 0 );
      ~CALStreamSample();
      virtual bool Stream( ALuint buffer ) = 0;
      bool HasEnded();
      virtual bool Rewind() = 0;
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
