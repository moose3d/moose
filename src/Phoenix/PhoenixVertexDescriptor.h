#ifndef __PhoenixVertexDescriptor_h__
#define __PhoenixVertexDescriptor_h__
/////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <PhoenixCore.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Graphics
  {
    enum ELEMENT_TYPE
    {
      ELEMENT_TYPE_VERTEX_3F,
      ELEMENT_TYPE_NORMAL_3F,
      ELEMENT_TYPE_COLOR_4UB,
      ELEMENT_TYPE_COLOR_3F,
      ELEMENT_TYPE_COLOR_4F,
      ELEMENT_TYPE_TEX_2F,
      ELEMENT_TYPE_TEX_3F,
      ELEMENT_TYPE_TEX_4F,
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
      ELEMENT_TYPE_ATTRIB_4UB,
      // following types are for combined data 
      ELEMENT_TYPE_V3F_N3F_T2F
    };
    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class PHOENIX_API CVertexDescriptor : public Phoenix::Core::CCacheable<unsigned int>,
					  public Phoenix::Core::CTypeBase<ELEMENT_TYPE>
    {  
    protected:
      /// number of elements.
      size_t 		        m_nSize;
      /// element data
      void *			m_pData;
      /// Size of one element in bytes.
      size_t			m_nElementByteSize;
    public:
      ////////////////////
      /// default constructor.
      /// \param nType Element type
      /// \param nNumElements how many elements this vertexbuffer have (ie. number of vertices).
      /// \par   Not the number of elements in vertex (3 floats), but #vertices.
      CVertexDescriptor( ELEMENT_TYPE nType, unsigned int nNumElements);
      ////////////////////
      /// Copy constructor.
      CVertexDescriptor( const Phoenix::Graphics::CVertexDescriptor & obj );
      ////////////////////
      /// Frees reserverd memory.
      ~CVertexDescriptor();
      ////////////////////
      /// Returns float pointer.
      /// \returns pointer to float array
      template<typename TYPE> 
      inline TYPE * GetPointer( size_t nElement = 0) const
      {
	return reinterpret_cast<TYPE *>(reinterpret_cast<unsigned char *>(m_pData)+(GetElementByteSize()*nElement) );
      }
      ////////////////////
      /// Returns number of elements in descriptor.
      /// \returns Number of elements.
      inline size_t GetSize() const
      {
	return m_nSize;
      }
      ////////////////////
      /// Element byte size.
      inline size_t GetElementByteSize()  const
      {
	return m_nElementByteSize;
      }
      ////////////////////
      /// Returns byte size for entire buffer.
      inline size_t GetByteSize() const
      {
	return GetElementByteSize()*GetSize();
      }
    };
  };  // end namespace Graphics
};// end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
