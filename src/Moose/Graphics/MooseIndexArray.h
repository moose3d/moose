
/////////////////////////////////////////////////////////////////
#ifndef __MooseIndexArray_h__
#define __MooseIndexArray_h__
/////////////////////////////////////////////////////////////////
#include <MooseCacheable.h>
#include "MooseAPI.h"
#include "MooseOGLConsts.h"
#include <cstring>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    enum PRIMITIVE_TYPE
      {
        PRIMITIVE_POINT_LIST,
        PRIMITIVE_TRI_LIST,
        PRIMITIVE_TRI_STRIP,
        PRIMITIVE_LINE_LIST,
        PRIMITIVE_LINE_STRIP
#if !defined(MOOSE_APPLE_IPHONE)
        ,
        PRIMITIVE_QUAD_LIST,
        PRIMITIVE_QUAD_STRIP
#endif
      };
    /////////////////////////////////////////////////////////////////
    /// Contains an array of indices and their count.
    class MOOSE_API CIndexArray : public Moose::Core::CCacheable<GLuint>
    {
    protected:
      /// Number of indices in this array.
      unsigned int m_nNumIndices;
      /// Primitive type for this array.
      PRIMITIVE_TYPE m_nType;
      /// Actual index data.
      void *	   m_pIndexData;
      /// Number of indices to be drawn from beginning.
      unsigned int m_nNumDrawableIndices;

    public:
      ////////////////////
      /// Constructor. Initializes index count and sets drawable indices to max indices.
      CIndexArray( PRIMITIVE_TYPE nType, unsigned int nNumIndices) : m_nNumIndices(nNumIndices),
      m_nType(nType),
      m_nNumDrawableIndices(nNumIndices)
      {
        if ( GetNumIndices() > 65536)  m_pIndexData = new unsigned short int[GetNumIndices()];
        else			       m_pIndexData = new unsigned int[GetNumIndices()];
        GetCache() = 0;
      }
      ////////////////////
      /// Copy constructor.
      CIndexArray( const CIndexArray & obj )
      {
        m_nNumIndices = obj.m_nNumIndices;
        m_nType = obj.m_nType;
        m_nNumDrawableIndices = m_nNumIndices;
        m_pIndexData = new char[ GetByteSize() ];
        memcpy( m_pIndexData, obj.m_pIndexData, GetByteSize());
        GetCache() = obj.GetCache();
      }
      ////////////////////
      /// Destructor.
      ~CIndexArray()
      {
        if ( IsShortIndices())
        {
          delete [] reinterpret_cast<unsigned short int *>(m_pIndexData);
        }
        else
        {
          delete [] reinterpret_cast<unsigned int *>(m_pIndexData);
        }
        if ( IsCached()) glDeleteBuffers(1, &GetCache());
      }
      ////////////////////
      /// Returns number of indices.
      /// \returns m_nNumIndices.
      inline unsigned int GetNumIndices() const
      {
        return m_nNumIndices;
      }
      ////////////////////
      /// Returns pointer to index data .
      /// \returns Pointer to m_pIndexData
      template<typename TYPE>
      inline TYPE *GetPointer() const
      {
        return reinterpret_cast<TYPE *>(m_pIndexData);
      }
      ////////////////////
      /// Returns true if indices are stored with ushort int data type.
      /// \returns boolean
      inline int IsShortIndices() const
      {
        return (GetNumIndices() <= 65536);
      }
      ////////////////////
      /// Returns primitive type.
      /// \returns PRIMITIVE_TYPE
      inline int GetPrimitiveType() const
      {
        return m_nType;
      }
      ////////////////////
      /// Returns the number of drawable indices.
      /// \returns Number of indices that is allowed to be drawn.
      inline unsigned int GetDrawableCount() const
      {
        return m_nNumDrawableIndices;
      }
      ////////////////////
      /// Sets the number of drawable indices.
      /// \param nCount Number of indices that is allowed to be drawn.
      inline void SetDrawableCount( unsigned int nCount )
      {
        // Safety check
        if ( nCount > GetNumIndices())
          m_nNumDrawableIndices = GetNumIndices();
        else
          m_nNumDrawableIndices = nCount;
      }
      ////////////////////
      /// Returns byte size for entire array.
      inline size_t GetByteSize()
      {
        if ( IsShortIndices()) return (sizeof(unsigned short int)*GetNumIndices());
        return (sizeof(unsigned int)*GetNumIndices());
      }
      inline void Copy( const void *pArray )
      {
        memcpy(m_pIndexData, pArray, GetByteSize());
      }
        
      void CreateCache()
      {
        if ( !IsCached() )
        {
          glGenBuffers(1, &GetCache());
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetCache());
          glBufferData( GL_ELEMENT_ARRAY_BUFFER, GetByteSize(), GetPointer<unsigned short int>(), 
                        ( (GetUsage() == Moose::Core::CACHE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ) );
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          SetState( Moose::Core::CACHE_UP2DATE);

        }
      }
      void UpdateCache()
      {
        if ( IsCached() )
        {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetCache());
          glBufferData( GL_ELEMENT_ARRAY_BUFFER, GetByteSize(), GetPointer<unsigned short int>(), 
                        ( (GetUsage() == Moose::Core::CACHE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ) );
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          SetState( Moose::Core::CACHE_UP2DATE);
        }
      }
        
    };
    /////////////////////////////////////////////////////////////////
  }; //namespace Graphics
}; //namespace Moose
#endif
/////////////////////////////////////////////////////////////////
