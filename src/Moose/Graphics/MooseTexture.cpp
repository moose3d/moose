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

#include "MooseTexture.h"
/////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglBase::COglBase( GLuint nId ) : m_nOglId(nId) 
{
  
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglBase::~COglBase()
{
  
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture::COglTexture( unsigned int nId, TEXTURE_TYPE tType ) : COglBase(nId), 
										      m_tTextureType(tType), m_nWidth(0), m_nHeight(0)
{

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture::~COglTexture()
{
  glDeleteTextures( 1, &m_nOglId );
}
/////////////////////////////////////////////////////////////////
TEXTURE_TYPE 
Moose::Graphics::COglTexture::GetType() const
{
  return m_tTextureType;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::ResolutionType 
Moose::Graphics::COglTexture::GetWidth() const
{
  return m_nWidth;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::ResolutionType 
Moose::Graphics::COglTexture::GetHeight() const
{
  return m_nHeight;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::COglTexture::SetWidth( Moose::Graphics::ResolutionType width )
{
  m_nWidth = width;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::COglTexture::SetHeight( Moose::Graphics::ResolutionType height )
{
  m_nHeight = height;
}
/////////////////////////////////////////////////////////////////
