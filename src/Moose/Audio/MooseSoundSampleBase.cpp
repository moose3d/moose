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

#include "MooseSoundSampleBase.h"
/////////////////////////////////////////////////////////////////
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
libname::CSoundSampleBase::CSoundSampleBase( size_t nBufSize ) : m_Format(0), m_Size(nBufSize), m_Freq(0), 
							m_bLoop(false), m_nChannels(0)
{
  if( GetBufferSize() > 0)
    m_pBufferData = new char[GetBufferSize()];
  else 
    m_pBufferData = NULL;
  
}
/////////////////////////////////////////////////////////////////
libname::CSoundSampleBase::~CSoundSampleBase()
{
  if ( m_pBufferData) 
    delete [] m_pBufferData;
}
/////////////////////////////////////////////////////////////////
void		
libname::CSoundSampleBase::SetFormat( ALenum format ) 
{
  m_Format = format; 
}
/////////////////////////////////////////////////////////////////
void		
libname::CSoundSampleBase::SetFreq( ALsizei f ) 
{
  m_Freq = f; 
}
/////////////////////////////////////////////////////////////////
void		
libname::CSoundSampleBase::SetNumChannels( size_t nCount ) 
{ 
  m_nChannels = nCount; 
}
/////////////////////////////////////////////////////////////////
ALenum	
libname::CSoundSampleBase::GetFormat() 
{
  return m_Format; 
}
/////////////////////////////////////////////////////////////////
ALsizei	
libname::CSoundSampleBase::GetFreq() 
{
  return m_Freq; 
}
/////////////////////////////////////////////////////////////////
size_t	
libname::CSoundSampleBase::GetNumChannels()
{
  return m_nChannels;
}
/////////////////////////////////////////////////////////////////
size_t	
libname::CSoundSampleBase::GetBufferSize() const
{
  return m_Size;
}
/////////////////////////////////////////////////////////////////
void		
libname::CSoundSampleBase::SetBufferSize( size_t nSize )
{
  m_Size = nSize;
}
/////////////////////////////////////////////////////////////////
