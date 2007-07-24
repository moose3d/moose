#include "PhoenixVertexDescriptor.h"
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor::CVertexDescriptor( ELEMENT_TYPE nType, 
							 unsigned int nNumElements)
{
  m_nType = nType;
  switch ( GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
    m_pData = new float[3*nNumElements];
    break;
  case ELEMENT_TYPE_TEX_2F:
    m_pData = new float[2*nNumElements];
    break;
  case ELEMENT_TYPE_COLOR_4UB:
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
    delete reinterpret_cast<float *>(m_pData);
    break;
  case ELEMENT_TYPE_COLOR_4UB:
    delete reinterpret_cast<unsigned char *>(m_pData);
    break;
  case ELEMENT_TYPE_NULL:
    m_pData = NULL;
    break;
  }
}
/////////////////////////////////////////////////////////////////
