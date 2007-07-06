/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#include "GSE_GeometryData.h"
#include "GSE_OglUtils.h"
#include <stdlib.h>
#include <assert.h>
/////////////////////////////////////////////////////////////////
#define DELETE(A) if ( A != NULL ) delete A; A=NULL;
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::SetVertexCount(unsigned int iNumElements)
{
  m_iNumElements = iNumElements;
  assert(iNumElements<=65536 && "Number of vertices is greater than 65536");
}
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::InitVertices()
{
  m_pVertices = new float[m_iNumElements*3];
  memset(m_pVertices, 0, sizeof(float)*m_iNumElements*3);
}
/////////////////////////////////////////////////////////////////
void
GSE_GeometryData::InitNormals()
{

  m_pNormals = new float[m_iNumElements*3];
  memset(m_pNormals, 0, sizeof(float)*m_iNumElements*3);
}
/////////////////////////////////////////////////////////////////
void
GSE_GeometryData::InitColors()
{
  m_pColors = new float[m_iNumElements*4];
  memset(m_pColors, 0, sizeof(float)*m_iNumElements*4);
}
/////////////////////////////////////////////////////////////////
void
GSE_GeometryData::InitTexCoords(unsigned int iWhichTexture)
{

  if ( iWhichTexture >= GSE_MAX_TEXCOORD_ARRAYS )
  {
    GSE_ERR( __FUNCTION__ << "Illegal multitexture number" );
  } else {
    m_pTexCoords[iWhichTexture] = new float[m_iNumElements*2];
    memset(m_pTexCoords[iWhichTexture], 0, sizeof(float)*m_iNumElements*2);
  }
  
}
///////////////////////////////////////////////////////////////// 
GSE_GeometryData::GSE_GeometryData()
{
  Nullify();
}
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::SetCacheMethod( CacheTarget_t nTarget, CacheMethod_t nMethod )
{
  m_CacheMethod[nTarget] = nMethod;
}
/////////////////////////////////////////////////////////////////
GSE_GeometryData::CacheMethod_t 
GSE_GeometryData::GetCacheMethod( CacheTarget_t nTarget)
{
  return m_CacheMethod[nTarget];
}
/////////////////////////////////////////////////////////////////
GSE_GeometryData::GSE_GeometryData(const GSE_GeometryData &ref)
{
  
  Nullify();
  
  m_iNumElements = ref.m_iNumElements;

  if ( ref.m_pVertices != NULL )
  {
    InitVertices(); 
    memcpy(m_pVertices, ref.m_pVertices, sizeof(float)*m_iNumElements*3);
  }

  for(unsigned int nTexCoordId = 0;
      nTexCoordId<GSE_MAX_TEXCOORD_ARRAYS;nTexCoordId++)
  {

    if ( ref.m_pTexCoords[nTexCoordId] != NULL ) 
    {
      InitTexCoords(nTexCoordId);
      memcpy(m_pTexCoords[nTexCoordId], 
	     ref.m_pTexCoords[nTexCoordId], 
	     sizeof(float)*m_iNumElements*2);
    }
  }
  if ( ref.m_pNormals != NULL )
  {    
    InitNormals();  
    memcpy(m_pNormals, ref.m_pNormals, sizeof(float)*m_iNumElements*3);
  }
  
  if ( ref.m_pColors != NULL ) 
  {
    InitColors();
    memcpy(m_pColors, ref.m_pColors, sizeof(float)*m_iNumElements*4);
  }

  for(unsigned int nTmp=0;nTmp<GSE_MAX_CACHE_TARGETS;nTmp++)
  {

    switch( ref.m_CacheMethod[nTmp] )
    {
    // if there isn't any cache, or cache of the reference object is 
    // about to be deleted,there shouldn't be cache in this one either.
    case NO_CACHE:
    case DELETE_VBO_CACHE:
      m_CacheMethod[nTmp] = NO_CACHE;
      break;
    // If the object is cached, or asked to regenerate or is requested but 
    // not received, the copy will request a copy for itself.
    case CACHED_IN_VBO:
    case REGENERATE_VBO_CACHE:
    case REQUEST_VBO_CACHE:
      m_CacheMethod[nTmp] = REQUEST_VBO_CACHE;
      break;
    }
  }

}
/////////////////////////////////////////////////////////////////
GSE_GeometryData::~GSE_GeometryData()
{

  GSE_DEBUG( "Killing GeometryData." );
  Destroy();
}
/////////////////////////////////////////////////////////////////
void
GSE_GeometryData::Destroy()
{
  for(unsigned int nTmp=0;nTmp<GSE_MAX_TEXCOORD_ARRAYS;nTmp++)
  {
    DELETE(m_pTexCoords[nTmp]);
  }
  DELETE(m_pVertices);
  DELETE(m_pNormals);
  DELETE(m_pColors);
  m_iNumElements = 0;

}
/////////////////////////////////////////////////////////////////
void
GSE_GeometryData::Nullify()
{
  m_pVertices = NULL;
  for(unsigned int nTmp=0;nTmp<GSE_MAX_TEXCOORD_ARRAYS;nTmp++)
  {
    m_pTexCoords[nTmp] = NULL;
  }
  m_pNormals = NULL;
  m_pColors = NULL;
  m_iNumElements = 0;

  for(unsigned int nTmp=0;nTmp<GSE_MAX_CACHE_TARGETS;nTmp++)
  {
    m_CacheMethod[nTmp] = NO_CACHE;
    m_ObjectName[nTmp] = 0;
  }
  
  
}
/////////////////////////////////////////////////////////////////
GSE_GeometryData &
GSE_GeometryData::operator=(GSE_GeometryData obj)
{

  Destroy();
  m_iNumElements = obj.m_iNumElements;

  if ( obj.m_pVertices != NULL ) 
  {
    InitVertices();
    memcpy(m_pVertices, obj.m_pVertices, sizeof(float)*m_iNumElements*3);
  }

  for(unsigned int nTexCoordId = 0;
      nTexCoordId<GSE_MAX_TEXCOORD_ARRAYS;nTexCoordId++){

    if ( obj.m_pTexCoords[nTexCoordId] != NULL ) 
    {
      InitTexCoords(nTexCoordId);
      memcpy(m_pTexCoords[nTexCoordId], 
	     obj.m_pTexCoords[nTexCoordId], 
	     sizeof(float)*m_iNumElements*2);
    }
  }
  
  if ( obj.m_pNormals != NULL )
  {
    InitNormals();
    memcpy(m_pNormals, obj.m_pNormals, sizeof(float)*m_iNumElements*3);
  }
  
  if ( obj.m_pColors != NULL ) {
    InitColors();
    memcpy(m_pColors, obj.m_pColors, sizeof(float)*m_iNumElements*4);
  }

  for(unsigned int nTmp=0;nTmp<GSE_MAX_CACHE_TARGETS;nTmp++)
  {
    if ( m_ObjectName[nTmp] != 0 )
    {
      GSE_OglUtils::DeleteVBOs(1,&(m_ObjectName[nTmp]));
    }

    switch( obj.m_CacheMethod[nTmp] )
    {
    // if there isn't any cache, or cache of the reference object is 
    // about to be deleted, there shouldn't be cache in this one either.
    case NO_CACHE:
    case DELETE_VBO_CACHE:
      m_CacheMethod[nTmp] = NO_CACHE;
      break;
    // If the object is cached, or asked to regenerate or is requested but 
    // not received, the copy will request a copy for itself.
    case CACHED_IN_VBO:
    case REGENERATE_VBO_CACHE:
    case REQUEST_VBO_CACHE:
      m_CacheMethod[nTmp] = REQUEST_VBO_CACHE;
      break;
    }
  }
  return *this;

}
/////////////////////////////////////////////////////////////////
GLuint &
GSE_GeometryData::Cache( CacheTarget_t nTarget )
{
  return m_ObjectName[nTarget];
}
/////////////////////////////////////////////////////////////////
// Return the pointer to the coordinate array of iVertIndexth vertex
VertexArray	
GSE_GeometryData::GetVertexCoords( unsigned int iVertIndex )
{
  if ( m_pVertices != NULL )
  {
    return &m_pVertices[ 3 * iVertIndex ];
  }
  else
  {
    return NULL;
  }
}
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::SetTexCoords( unsigned int iMultiTexId, 
			     unsigned int iVertexIndex, 
			     float fX, float fY )
{
  unsigned int iIndex = 2*iVertexIndex;
  if ( iVertexIndex >= m_iNumElements )
  {
    GSE_ERR( "iVertexIndex out of bounds (>= m_iNumElements)!!!");
    return;
  }
  
  if ( HasTexCoords( iMultiTexId ))
  {
      m_pTexCoords[iMultiTexId][iIndex]   = fX;
      m_pTexCoords[iMultiTexId][iIndex+1] = fY;
  } 
  else 
  {
    GSE_ERR( "m_pTexCoords " << iMultiTexId << " is not initialized!");
  }
}
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::SetTexCoords( unsigned int iMultiTexId, 
			     unsigned int iVertexIndex, 
			     GSE_Vector2 vCoords )
{
  unsigned int iIndex = 2*iVertexIndex;
  if ( iVertexIndex >= m_iNumElements )
  {
    GSE_ERR( "iVertexIndex out of bounds (>= m_iNumElements)!!!");
    return;
  }
  
  if ( HasTexCoords( iMultiTexId ))
  {
      m_pTexCoords[iMultiTexId][iIndex]   = vCoords[GSE_Vector2::X];
      m_pTexCoords[iMultiTexId][iIndex+1] = vCoords[GSE_Vector2::Y];
  } 
  else 
  {
    GSE_ERR( "m_pTexCoords " << iMultiTexId << " is not initialized!");
  }
}
/////////////////////////////////////////////////////////////////
TextureCoordArray 
GSE_GeometryData::GetTexCoords( unsigned int iMultiTexId, unsigned int iVertexIndex)
{
  unsigned int iIndex = 2*iVertexIndex;

  if ( iIndex >= (iVertexIndex * m_iNumElements * 2))
  {
    GSE_ERR( "index out of bounds (" 
	       << iIndex << " > " 
	       << (iVertexIndex * m_iNumElements * 2) - 1<< ")" );
    return NULL;

  } 
  else if ( !HasTexCoords( iMultiTexId ) )
  {
    GSE_ERR( "No texture coords set for " << iMultiTexId << " array!" );
    return NULL;
  }
  
  TextureCoordArray pArray = NULL;
  pArray = &(m_pTexCoords[iMultiTexId][iIndex]);
  return pArray;
}
/////////////////////////////////////////////////////////////////
NormalArray 
GSE_GeometryData::GetNormal(unsigned int iVertexIndex)
{
  if ( m_pNormals == NULL ) return NULL;
  return &m_pNormals[ iVertexIndex * 3];
}
/////////////////////////////////////////////////////////////////
void 
GSE_GeometryData::SetColor(unsigned int iVertexIndex, GSE_Color vColor)
{

  unsigned int iIndex = 4 * iVertexIndex;
  if ( m_pColors != NULL )
  {
    m_pColors[iIndex]   = vColor[GSE_Color::R];
    m_pColors[iIndex+1] = vColor[GSE_Color::G];
    m_pColors[iIndex+2] = vColor[GSE_Color::B];
    m_pColors[iIndex+3] = vColor[GSE_Color::A];

  } 
  else
  {
    GSE_ERR( "m_pColors is not initialized!" );
  }
}
/////////////////////////////////////////////////////////////////
ColorArray 
GSE_GeometryData::GetColor(unsigned int iVertexIndex)
{

  if ( m_pColors == NULL ) return NULL;
  return &m_pColors[ iVertexIndex * 4];

}
/////////////////////////////////////////////////////////////////
// Does this GeometryData contain color buffer
char 
GSE_GeometryData::HasColors()
{
  return ( m_pColors != NULL );
}
/////////////////////////////////////////////////////////////////
// Does this GeometryData contain vertex buffer
char 
GSE_GeometryData::HasVertices()
{
  return ( m_pVertices != NULL );
}
/////////////////////////////////////////////////////////////////
// Does this GeometryData contain normal vector buffer
char 
GSE_GeometryData::HasNormals()
{
  return ( m_pNormals != NULL );
}
/////////////////////////////////////////////////////////////////
// Does this GeometryData contain texture coordinate buffer
char 
GSE_GeometryData::HasTexCoords(unsigned int nMultiTextureCoordId)
{
  char bRetval = 0;
  if ( nMultiTextureCoordId >= GSE_MAX_TEXCOORD_ARRAYS )
  {
    GSE_ERR("Invalid MultiTextureCoordId!" );
  } 
  else 
  {
    bRetval = ( m_pTexCoords[nMultiTextureCoordId] != NULL );
  }
  return bRetval;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_GeometryData.cpp,v $
// Revision 1.1  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
/////////////////////////////////////////////////////////////////
