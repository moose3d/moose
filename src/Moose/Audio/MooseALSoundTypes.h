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

#ifndef __CALSoundTypes_h__
#define __CALSoundTypes_h__
/////////////////////////////////////////////////////////////////
#include "MooseSoundBase.h"
#include "MooseALObjectArray.h"
#include "MooseALSampleTypes.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CALSoundOperations : virtual public CALObjectArray<1>,
			       virtual public CSoundBase
    {
    public:

      bool IsPlaying() const;  
      void Play();
  
      void Pause();
      void Stop();
      void Rewind();
      void CommitChanges();
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CALSound : public CALSoundOperations
    {
    public:
      CALSound();
      virtual ~CALSound();
      void UseSample( CALSample & sample );
      bool IsLooping();
      void SetLooping( bool flag);  
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CALStreamSound : public CALSoundOperations
    {
    protected:
      CALStreamSample *m_pStream;
      bool              m_bLooping;
    public:

      CALStreamSound();
      virtual ~CALStreamSound();

      bool IsLooping();
      void SetLooping( bool flag);

      void UseStream( CALStreamSample * pSample );
      bool Update();
      void Clear();
      void Rewind();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
