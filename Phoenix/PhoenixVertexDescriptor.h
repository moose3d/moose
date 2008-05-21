#ifndef __PhoenixVertexDescriptor_h__
#define __PhoenixVertexDescriptor_h__
/////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <PhoenixCore.h>
#include <PhoenixRenderable.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Graphics
  {
    enum ELEMENT_TYPE
    {
      ELEMENT_TYPE_NULL = 0,
      ELEMENT_TYPE_VERTEX_3F = 1,
      ELEMENT_TYPE_NORMAL_3F,
      ELEMENT_TYPE_COLOR_4UB,
      ELEMENT_TYPE_COLOR_3F,
      ELEMENT_TYPE_COLOR_4F,
      ELEMENT_TYPE_TEX_2F,
      ELEMENT_TYPE_UNIFORM_1F,
      ELEMENT_TYPE_UNIFORM_2F,
      ELEMENT_TYPE_UNIFORM_3F,
      ELEMENT_TYPE_UNIFORM_4F,
      ELEMENT_TYPE_UNIFORM_1I,
      ELEMENT_TYPE_UNIFORM_2I,
      ELEMENT_TYPE_UNIFORM_3I,
      ELEMENT_TYPE_UNIFORM_4I,
      ELEMENT_TYPE_UNIFORM_2X2F,
      ELEMENT_TYPE_UNIFORM_3X3F,
      ELEMENT_TYPE_UNIFORM_4X4F,
      ELEMENT_TYPE_ATTRIB_1F,
      ELEMENT_TYPE_ATTRIB_2F,
      ELEMENT_TYPE_ATTRIB_3F,
      ELEMENT_TYPE_ATTRIB_4F,
      ELEMENT_TYPE_ATTRIB_1I,
      ELEMENT_TYPE_ATTRIB_2I,
      ELEMENT_TYPE_ATTRIB_3I,
      ELEMENT_TYPE_ATTRIB_4I,
      ELEMENT_TYPE_ATTRIB_1UB,
      ELEMENT_TYPE_ATTRIB_2UB,
      ELEMENT_TYPE_ATTRIB_3UB,
      ELEMENT_TYPE_ATTRIB_4UB
    };
    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class CVertexDescriptor : public Phoenix::Core::CCacheable<unsigned int>,
			      public Phoenix::Graphics::CRenderable
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
      /// \param nNumElements how many elements this vertexbuffer have (ie. number of vertices).
      /// \par   Not the number of elements in vertex (3 floats), but #vertices.
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
      inline TYPE * GetPointer() const
      {
	return reinterpret_cast<TYPE *>(m_pData);
      }
      ////////////////////
      /// Returns number of elements in descriptor.
      /// \returns Number of elements.
      inline unsigned int GetSize() const
      {
	return m_nSize;
      }
    };
  };  // end namespace Graphics
};// end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
