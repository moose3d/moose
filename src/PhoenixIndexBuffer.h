
/////////////////////////////////////////////////////////////////
#ifndef __PhoenixIndexBuffer_h__
#define __PhoenixIndexBuffer_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Contains an array of indices and their count.
    class CIndexBuffer
    {
    public:
      ////////////////////
      typedef enum 
      {
	PRIMITIVE_NULL = 0,
	PRIMITIVE_POINT_LIST = 1,
	PRIMITIVE_TRI_LIST,
	PRIMITIVE_TRI_STRIP,
	PRIMITIVE_LINE_LIST,
	PRIMITIVE_LINE_STRIP,
	PRIMITIVE_QUAD_LIST
      } PrimitiveType;
    protected:

      unsigned int m_nNumIndices;
      PrimitiveType m_nType;
      void *	   m_pIndexData;
    public:


      /////////////////////////////////////////////////////////////////
      /// The constructor.
      CIndexBuffer( PrimitiveType nType, unsigned int nNumIndices) : m_nNumIndices(nNumIndices), m_nType(nType)
      {
	if ( GetNumIndices() > 65536)  m_pIndexData = new unsigned short int[GetNumIndices()];
	else			       m_pIndexData = new unsigned int[GetNumIndices()];
	  
      }
      /////////////////////////////////////////////////////////////////
      /// The destructor.
      ~CIndexBuffer()
      {
	delete m_pIndexData;
      }
      ////////////////////
      /// Returns number of indices.
      /// \returns m_nNumIndices.
      inline unsigned int GetNumIndices() const
      {
	return m_nNumIndices;
      }
      ////////////////////
      /// Returns pointer to unsigned int index data .
      /// \returns Pointer to m_pIndexData
      inline unsigned int *GetIntIndices()
      {
	return (unsigned int *)m_pIndexData;
      }
      ////////////////////
      /// Returns pointer to unsigned short int index data .
      /// \returns Pointer to m_pIndexData
      inline unsigned int *GetShortIndices()
      {
	return (unsigned short int *)m_pIndexData;
      }
      ////////////////////
      /// Returns true if indices are stored with ushort int data type.
      /// \returns boolean 
      inline int IsShortIndices() const
      {
	return (GetNumIndices() <= 65536);
      }
      
    }; 
    /////////////////////////////////////////////////////////////////
  }; //namespace Graphics
}; //namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
