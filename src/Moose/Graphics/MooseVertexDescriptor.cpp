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

#include "MooseVertexDescriptor.h"
#include <cassert>
#include <cstring>
#include <Rocket/Core.h>
using namespace Moose::Core;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor::CVertexDescriptor( ELEMENT_TYPE nType, 
                                                        unsigned int nNumElements) 
{
  SetType(nType);
  m_nSize = nNumElements;
  switch ( GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
  case ELEMENT_TYPE_NORMAL_3F:
  case ELEMENT_TYPE_COLOR_3F:
  case ELEMENT_TYPE_UNIFORM_3F:
  case ELEMENT_TYPE_TEX_3F:
  case ELEMENT_TYPE_ATTRIB_3F:
    m_pData = new float[3*nNumElements];
    m_nElementByteSize = sizeof(float)*3;
    break;
  case ELEMENT_TYPE_TEX_2F:
  case ELEMENT_TYPE_ATTRIB_2F:
  case ELEMENT_TYPE_UNIFORM_2F:
    m_pData = new float[2*nNumElements];
    m_nElementByteSize = sizeof(float)*2;
    break;
  case ELEMENT_TYPE_COLOR_4UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
    m_pData = new unsigned char[4*nNumElements];
    m_nElementByteSize = sizeof(unsigned char)*4;
    break;
  case ELEMENT_TYPE_COLOR_4F:
  case ELEMENT_TYPE_UNIFORM_4F:
  case ELEMENT_TYPE_ATTRIB_4F:
  case ELEMENT_TYPE_TEX_4F:
  case ELEMENT_TYPE_UNIFORM_2X2F:
    m_pData = new float[4*nNumElements];
    m_nElementByteSize = sizeof(float)*4;
    break;
  case ELEMENT_TYPE_UNIFORM_1F:
  case ELEMENT_TYPE_ATTRIB_1F:
    m_pData = new float[nNumElements];
    m_nElementByteSize = sizeof(float);
    break;
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_ATTRIB_1I:
    m_pData = new int[nNumElements];
    m_nElementByteSize = sizeof(int);
    break;
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_ATTRIB_2I:
    m_pData = new int[2*nNumElements];
    m_nElementByteSize = sizeof(int)*2;
    break;
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_ATTRIB_3I:
    m_pData = new int[3*nNumElements];
    m_nElementByteSize = sizeof(int)*3;
    break;
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_ATTRIB_4I:
    m_pData = new int[4*nNumElements];
    m_nElementByteSize = sizeof(int)*4;
    break;
  case ELEMENT_TYPE_UNIFORM_3X3F:
    m_pData = new float[9*nNumElements];
    m_nElementByteSize = sizeof(float)*9;
    break;
  case ELEMENT_TYPE_UNIFORM_4X4F:
    m_pData = new float[16*nNumElements];
    m_nElementByteSize = sizeof(float)*16;
    break;
  case ELEMENT_TYPE_ATTRIB_1UB:
    m_pData = new unsigned char[nNumElements];
    m_nElementByteSize = sizeof(unsigned char);
    break;
  case ELEMENT_TYPE_ATTRIB_2UB:
    m_pData = new unsigned char[2*nNumElements];
    m_nElementByteSize = sizeof(unsigned char)*2;
    break;
  case ELEMENT_TYPE_ATTRIB_3UB:
    m_pData = new unsigned char[3*nNumElements];
    m_nElementByteSize = sizeof(unsigned char)*3;
    break;
  case ELEMENT_TYPE_V3F_N3F_T2F:
    m_pData = new float[8*nNumElements];
    m_nElementByteSize = sizeof(float)*8;
    break;
  case ELEMENT_TYPE_ROCKET_DATA:
    m_pData = new unsigned char[sizeof(Rocket::Core::Vertex)*nNumElements];
    m_nElementByteSize = sizeof(Rocket::Core::Vertex);
    break;
  default:
    assert( NULL && "Wrong type" );
    break;
  }
    GetCache() = 0;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor::~CVertexDescriptor()
{
  switch ( GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
  case ELEMENT_TYPE_TEX_2F:
  case ELEMENT_TYPE_UNIFORM_4X4F:
  case ELEMENT_TYPE_UNIFORM_3X3F:
  case ELEMENT_TYPE_UNIFORM_2X2F:
  case ELEMENT_TYPE_UNIFORM_4F:
  case ELEMENT_TYPE_UNIFORM_3F:
  case ELEMENT_TYPE_UNIFORM_2F:
  case ELEMENT_TYPE_UNIFORM_1F:
  case ELEMENT_TYPE_ATTRIB_1F:
  case ELEMENT_TYPE_ATTRIB_2F:
  case ELEMENT_TYPE_ATTRIB_3F:
  case ELEMENT_TYPE_ATTRIB_4F:
  case ELEMENT_TYPE_COLOR_3F:
  case ELEMENT_TYPE_COLOR_4F:
  case ELEMENT_TYPE_NORMAL_3F:
  case ELEMENT_TYPE_TEX_3F:
  case ELEMENT_TYPE_TEX_4F:
  case ELEMENT_TYPE_V3F_N3F_T2F:
    delete [] reinterpret_cast<float *>(m_pData);
    break;
  case ELEMENT_TYPE_COLOR_4UB:
  case ELEMENT_TYPE_ATTRIB_1UB:
  case ELEMENT_TYPE_ATTRIB_2UB:
  case ELEMENT_TYPE_ATTRIB_3UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
    delete [] reinterpret_cast<unsigned char *>(m_pData);
    break;
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_ATTRIB_1I:
  case ELEMENT_TYPE_ATTRIB_2I:
  case ELEMENT_TYPE_ATTRIB_3I:
  case ELEMENT_TYPE_ATTRIB_4I:
    delete [] reinterpret_cast<int *>(m_pData);
    break;
  case ELEMENT_TYPE_ROCKET_DATA:
    delete [] reinterpret_cast<Rocket::Core::Vertex *>(m_pData);
    break;
  }
  m_pData = NULL;
  if ( IsCached() ) glDeleteBuffers( 1, &GetCache());
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor::CVertexDescriptor( const Moose::Graphics::CVertexDescriptor & obj )
{
  SetType(obj.GetType());
  m_nSize = obj.GetSize();
  m_nElementByteSize = obj.m_nElementByteSize;
  m_pData = new char[ obj.GetByteSize() ];
  memcpy( m_pData, obj.m_pData, GetByteSize() );
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CVertexDescriptor::Copy( size_t nToWhichIndex, size_t nNumElements, const void *pData )
{        
    memcpy( GetPointer<void>(nToWhichIndex), pData, GetElementByteSize()*nNumElements);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CVertexDescriptor::CreateCache(  )
{
  if ( ! IsCached() )
  {
    glGenBuffers(1, &GetCache());
    glBindBuffer(GL_ARRAY_BUFFER, GetCache());
    glBufferData( GL_ARRAY_BUFFER, GetByteSize(), GetPointer<float>(), 
                  ( (GetUsage() == Moose::Core::CACHE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    SetState(Moose::Core::CACHE_UP2DATE);
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CVertexDescriptor::UpdateCache(  )
{
  if ( IsCached() )
  {
    glBindBuffer(GL_ARRAY_BUFFER, GetCache());
    glBufferData( GL_ARRAY_BUFFER, GetByteSize(), GetPointer<float>(), 
                  ( (GetUsage() == Moose::Core::CACHE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    SetState(Moose::Core::CACHE_UP2DATE);
  }
}
////////////////////////////////////////////////////////////////////////////////
