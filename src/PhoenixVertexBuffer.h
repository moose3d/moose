#ifndef __PhoenixVertexBuffer_h__
#define __PhoenixVertexBuffer_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "PhoenixMath.h"
// ---------------------------------------------------------------
//
// The GeometryData is the foundation for geometry data in
// every 3D Object, containing vertex colors, texture coordinates
// normals, and position data.
// --------------------------------------------------------------------
namespace Phoenix 
{
  namespace Graphics
  {
    enum ELEMENT_TYPE
    {
      ELEMENT_TYPE_NULL = 0,
      ELEMENT_TYPE_VERTEX_3F = 1
    };
    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class CVertexBuffer
    {  
    protected:
      /// number of elements.
      unsigned int		m_nSize;
      /// element data
      void *			m_pData;
      /// Element type.
      ELEMENT_TYPE		m_nType;
    public:
      ////////////////////
      /// default constructor.
      /// \param nType Element type
      /// \param nNumElements how many elements this vertexbuffer have.
      CVertexBuffer( ELEMENT_TYPE nType, 
		     unsigned int nNumElements) 
      {  
	m_nType = nType;
	switch ( GetType() )
	{
	case ELEMENT_TYPE_VERTEX_3F:
	  m_pData = new float[3*nNumElements];
	  break;
	case ELEMENT_TYPE_NULL:
	  m_pData = NULL;
	  break;
	}
      }
      ////////////////////
      /// Frees reserverd memory.
      ~CVertexBuffer()
      {
	switch ( GetType() )
	{
	case ELEMENT_TYPE_VERTEX_3F:
	  delete reinterpret_cast<float *>(m_pData);
	  break;
	case ELEMENT_TYPE_NULL:
	  m_pData = NULL;
	  break;
	}
      }
      ////////////////////
      /// Returns element type.
      /// \returns ELEMENT_TYPE
      inline ELEMENT_TYPE GetType() const
      {
	return m_nType;
      }
      ////////////////////
      /// Returns float pointer.
      /// \returns pointer to float array
      template<typename TYPE> 
      inline TYPE * GetPointer()
      {
	return reinterpret_cast<TYPE *>(m_pData);
      }
    };
  };  // end namespace Graphics
}// end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
