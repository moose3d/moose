#include "PhoenixVertexDescriptor.h"
#include <assert.h>
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor::CVertexDescriptor( ELEMENT_TYPE nType, 
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
  default:
    assert( NULL && "Wrong type" );
    break;
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor::~CVertexDescriptor()
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
  }
  m_pData = NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor::CVertexDescriptor( const Phoenix::Graphics::CVertexDescriptor & obj )
{
  SetType(obj.GetType());
  m_nSize = obj.GetSize();
  m_nElementByteSize = obj.m_nElementByteSize;
  m_pData = new char[ obj.GetByteSize() ];
  memcpy( m_pData, obj.m_pData, GetByteSize() );
}
/////////////////////////////////////////////////////////////////
