#include "PhoenixVertexDescriptor.h"
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor::CVertexDescriptor( ELEMENT_TYPE nType, 
							 unsigned int nNumElements)
{
  m_nType = nType;
  switch ( GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
  case ELEMENT_TYPE_NORMAL_3F:
    m_pData = new float[3*nNumElements];
    break;
  case ELEMENT_TYPE_TEX_2F:
    m_pData = new float[2*nNumElements];
    break;
  case ELEMENT_TYPE_COLOR_4UB:
    m_pData = new unsigned char[4*nNumElements];
    break;
  case ELEMENT_TYPE_COLOR_3F:
    m_pData = new float[3*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_1F:
    m_pData = new float[1*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_2F:
    m_pData = new float[2*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_3F:
    m_pData = new float[3*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_4F:
    m_pData = new float[4*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_1I:
    m_pData = new int[1*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_2I:
    m_pData = new int[2*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_3I:
    m_pData = new int[3*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_4I:
    m_pData = new int[4*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_2X2F:
    m_pData = new float[4*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_3X3F:
    m_pData = new float[9*nNumElements];
    break;
  case ELEMENT_TYPE_UNIFORM_4X4F:
    m_pData = new float[16*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_1F:
    m_pData = new float[nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_2F:
    m_pData = new float[2*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_3F:
    m_pData = new float[3*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_4F:
    m_pData = new float[4*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_1I:
    m_pData = new int[nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_2I:
    m_pData = new int[2*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_3I:
    m_pData = new int[3*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_4I:
    m_pData = new int[4*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_1UB:
    m_pData = new unsigned char[nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_2UB:
    m_pData = new unsigned char[2*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_3UB:
    m_pData = new unsigned char[3*nNumElements];
    break;
  case ELEMENT_TYPE_ATTRIB_4UB:
    m_pData = new unsigned char[4*nNumElements];
    break;
  case ELEMENT_TYPE_NULL:
    m_pData = NULL;
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
  case ELEMENT_TYPE_NORMAL_3F:
    delete reinterpret_cast<float *>(m_pData);
    break;
  case ELEMENT_TYPE_COLOR_4UB:
  case ELEMENT_TYPE_ATTRIB_1UB:
  case ELEMENT_TYPE_ATTRIB_2UB:
  case ELEMENT_TYPE_ATTRIB_3UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
    delete reinterpret_cast<unsigned char *>(m_pData);
    break;
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_ATTRIB_1I:
  case ELEMENT_TYPE_ATTRIB_2I:
  case ELEMENT_TYPE_ATTRIB_3I:
  case ELEMENT_TYPE_ATTRIB_4I:
    delete reinterpret_cast<int *>(m_pData);
    break;
  case ELEMENT_TYPE_NULL:
    break;
  }
  m_pData = NULL;
}
/////////////////////////////////////////////////////////////////
