/////////////////////////////////////////////////////////////////
// file : CGeometryData.h
// desc : Header file for the GeometryData and GeometryDataMgr classes
// author : AG/eNtity
/////////////////////////////////////////////////////////////////
#ifndef __GeometryData_h__
#define __GeometryData_h__
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
    typedef enum 
    {
      ELEMENT_TYPE_NULL = 0,
      ELEMENT_TYPE_VERTEX_3F = 1
    } ElementType;
    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class CVertexBuffer
    {  
    protected:
      /// number of elements.
      unsigned int		m_nSize;
      /// element data
      void *			m_pData;
    public:
      ////////////////////
      /// default constructor.
      /// \param nType Element type
      /// \param nNumElements how many elements this vertexbuffer have.
      CVertexBuffer( ElementType nType, 
		     unsigned int nNumElements) 
      {  
	switch ( nType )
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
      /// Frees the memory
      ~CVertexBuffer()
      {
	delete m_pData;
      }
      
    };
  };  // end namespace Graphics
}// end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
