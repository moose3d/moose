#ifndef __PhoenixVertexDescriptor_h__
#define __PhoenixVertexDescriptor_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "PhoenixMath.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Graphics
  {
    enum ELEMENT_TYPE
    {
      ELEMENT_TYPE_NULL = 0,
      ELEMENT_TYPE_VERTEX_3F = 1,
      ELEMENT_TYPE_COLOR_4UB,
      ELEMENT_TYPE_TEX0_2F
    };
    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class CVertexDescriptor
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
      CVertexDescriptor( ELEMENT_TYPE nType, unsigned int nNumElements);
      ////////////////////
      /// Frees reserverd memory.
      ~CVertexDescriptor();
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
};// end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
