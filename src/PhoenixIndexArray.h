
/////////////////////////////////////////////////////////////////
#ifndef __PhoenixIndexArray_h__
#define __PhoenixIndexArray_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCore.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    enum PRIMITIVE_TYPE
    {
      PRIMITIVE_NULL = 0,
      PRIMITIVE_POINT_LIST = 1,
      PRIMITIVE_TRI_LIST,
      PRIMITIVE_TRI_STRIP,
      PRIMITIVE_LINE_LIST,
      PRIMITIVE_LINE_STRIP,
      PRIMITIVE_QUAD_LIST
    };
    /////////////////////////////////////////////////////////////////
    /// Contains an array of indices and their count.
    class CIndexArray : public Phoenix::Core::CCacheable<unsigned int>
    {
    protected:

      unsigned int m_nNumIndices;
      PRIMITIVE_TYPE m_nType;
      void *	   m_pIndexData;
    public:
      ////////////////////
      /// The constructor.
      CIndexArray( PRIMITIVE_TYPE nType, unsigned int nNumIndices) : m_nNumIndices(nNumIndices), m_nType(nType)
      {
	if ( GetNumIndices() > 65536)  m_pIndexData = new unsigned short int[GetNumIndices()];
	else			       m_pIndexData = new unsigned int[GetNumIndices()];
	  
      }
      ////////////////////
      /// The destructor.
      ~CIndexArray()
      {
	if ( IsShortIndices())
	{
	  delete reinterpret_cast<unsigned short int *>(m_pIndexData);
	}
	else 
	{
	  delete reinterpret_cast<unsigned int *>(m_pIndexData);
	}
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
    };
    /////////////////////////////////////////////////////////////////
  }; //namespace Graphics
}; //namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
