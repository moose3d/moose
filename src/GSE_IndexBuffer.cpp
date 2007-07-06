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
#include "GSE_IndexBuffer.h"
/////////////////////////////////////////////////////////////////
void 
GSE_IndexBuffer::Init( unsigned int nSize )
{
  m_nNumIndices = nSize;
  m_nDrawIndices = nSize;
  m_pIndices = new unsigned short[m_nNumIndices];
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_IndexBuffer::CreateLinearIndexSet()
{
  for(unsigned int i=0;i<m_nNumIndices;i++)
  {
    m_pIndices[i] = i;
  }
}
/////////////////////////////////////////////////////////////////
GSE_IndexBuffer::GSE_IndexBuffer()
{
  m_pIndices = NULL;
  m_nNumIndices = 0;
  m_nDrawIndices = 0;
  m_nVerticesInPrimitive = 3;
  m_bBatchingEnabled = true;
  m_lstRenderBatches.clear();
  SetCacheMethod(NO_CACHE);
}
/////////////////////////////////////////////////////////////////
GSE_IndexBuffer::~GSE_IndexBuffer()
{
  Destroy();  
}
/////////////////////////////////////////////////////////////////
GSE_IndexBuffer::GSE_IndexBuffer(const GSE_IndexBuffer &ref)
{
  m_nNumIndices = ref.m_nNumIndices;
  m_nDrawIndices = ref.m_nDrawIndices;
  m_pIndices = new unsigned short[m_nNumIndices];
  m_bBatchingEnabled = ref.m_bBatchingEnabled;
  for(unsigned int i=0;i<m_nNumIndices;i++)
  {
    m_pIndices[i] = ref.m_pIndices[i];
  }
  // In this case we are able to determine the type of 
  // the cache easily.
  switch ( ref.m_nCacheMethod ){
    // No cache or if delete is issued -> NO_CACHE
  case NO_CACHE:
  case DELETE_VBO_CACHE:
    m_nCacheMethod = NO_CACHE;
    break;
    // if VBO is requested, or it exists, or conversion 
    // to VBO is requested -> REQUEST_VBO
  case REQUEST_VBO_CACHE:
  case CACHED_IN_VBO:
  case REGENERATE_VBO_CACHE:
    m_nCacheMethod = REQUEST_VBO_CACHE;
    break;
  }

  m_lstRenderBatches = ref.m_lstRenderBatches;
  GSE_IndexBufferMgr::GetInstance()->Add( this );
}
/////////////////////////////////////////////////////////////////
GSE_IndexBuffer &
GSE_IndexBuffer::operator=(GSE_IndexBuffer obj)
{

  int bThisHasVBOCache = 0;
  Destroy();
  m_nNumIndices = obj.m_nNumIndices;
  m_nDrawIndices = obj.m_nDrawIndices;
  m_pIndices = new unsigned short[m_nNumIndices];
  m_bBatchingEnabled = obj.m_bBatchingEnabled;
  // Copy index data
  for(unsigned int i=0;i<m_nNumIndices;i++)
  {
    m_pIndices[i] = obj.m_pIndices[i];
  }

  // Determine does this object have cache and what type
  switch ( m_nCacheMethod )
  {
  case NO_CACHE:
  case REQUEST_VBO_CACHE:
    bThisHasVBOCache = 0;
    break;
  case CACHED_IN_VBO:
  case REGENERATE_VBO_CACHE:
  case DELETE_VBO_CACHE:
    bThisHasVBOCache = 1;
    break;
  }
  m_lstRenderBatches = obj.m_lstRenderBatches;
  // if this object has an existing cache
  if ( bThisHasVBOCache )  
  {
   switch ( obj.m_nCacheMethod )
   {
     // The other object has no cache or is about to have its cache 
     // removed...
   case NO_CACHE:
   case DELETE_VBO_CACHE:
     // ...Then this must also delete its own cache.
     m_nCacheMethod = DELETE_VBO_CACHE;
     break;
     // The other object is about to have or has a VBO cache...
   case REQUEST_VBO_CACHE:
   case CACHED_IN_VBO:
   case REGENERATE_VBO_CACHE:
     // ...Then this must have a VBO cache 
     m_nCacheMethod = REGENERATE_VBO_CACHE; 
     break;
   }
  } 
  else 
  {
    switch ( obj.m_nCacheMethod )
    {
      // No cache or if delete is issued -> NO_CACHE
    case NO_CACHE:
    case DELETE_VBO_CACHE:
      m_nCacheMethod = NO_CACHE;
      break;
      // if VBO is requested, or it exists, or conversion 
      // to VBO is requested -> REQUEST_VBO
    case REQUEST_VBO_CACHE:
    case CACHED_IN_VBO:
    case REGENERATE_VBO_CACHE:
      m_nCacheMethod = REQUEST_VBO_CACHE;
      break;
    }
  }
  return *this;
}
/////////////////////////////////////////////////////////////////
void
GSE_IndexBuffer::Destroy()
{
  if ( m_pIndices != NULL )
    delete m_pIndices;

  m_pIndices = NULL;
}
/////////////////////////////////////////////////////////////////
void 
GSE_IndexBuffer::SetCacheMethod( CacheMethod_t nMethod )
{
  m_nCacheMethod = nMethod;
}
/////////////////////////////////////////////////////////////////
GSE_IndexBuffer::CacheMethod_t
GSE_IndexBuffer::GetCacheMethod()
{
  return m_nCacheMethod;
}
/////////////////////////////////////////////////////////////////
GLuint &
GSE_IndexBuffer::Cache()
{
  return m_ObjectName;
}
/////////////////////////////////////////////////////////////////
std::list<GSE_IndexBuffer::RenderBatch_t> &
GSE_IndexBuffer::RenderBatches()
{
  return m_lstRenderBatches;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_IndexBuffer.cpp,v $
// Revision 1.1  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
/////////////////////////////////////////////////////////////////
