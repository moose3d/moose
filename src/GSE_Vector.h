/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_Vector__
#define __GSE_Vector__
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
class GSE_Vector
{
 protected:
  /// Is the data referenced vie m_pValues member pointer.
  char m_bExternalData;
  // The actual storage array for local values.
  TYPE  m_aValues[SIZE];
  /////////////////////////////////////////////////////////////////
 public:
  /// A pointer to an array of TYPEs (m_aValues).
  /// It is with the programmer to utilize this correctly.
  TYPE *m_pValues; 
  ////////////////////
  /// The default constructor.
  GSE_Vector();
  ////////////////////
  /// The parametrized constructor copies data from an existing array.
  GSE_Vector( TYPE *pArray );
  ////////////////////
  /// The copy constructor.
  GSE_Vector( const GSE_Vector & vVector);

  ////////////////////
  /// Assigns the values from aValues.
  /// \returns reference to this object.
  GSE_Vector &Set( TYPE aValues[SIZE] );
  ////////////////////
  /// Makes the vector to handle data from pArray, it won't be released if object is deleted.
  void UseExternalData( TYPE *pArray );
  ////////////////////
  /// [] operator to help accessing the values, returns reference to value at index iIndex.
  /// \returns TYPE.
  inline TYPE &operator[](unsigned int iIndex);
  ////////////////////
  /// The assignment operator.
  /// \returns reference to this object.
  inline GSE_Vector &operator=(GSE_Vector vVector);
  ////////////////////
  /// The comparison operator.
  /// \returns truth value - non-zero if corresponding each corresponding component in both vectors have the same value, zero otherwise.
  int operator==(GSE_Vector vVector);
  
};
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
GSE_Vector<TYPE,SIZE>::GSE_Vector() : m_bExternalData(0)
{
  // init array
  // memset(&m_aValues[0], 0, sizeof(TYPE)*SIZE);
  // set pointer to beginning of the array
  m_pValues        = &m_aValues[0];

  
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
GSE_Vector<TYPE,SIZE>::GSE_Vector( TYPE *pArray ) : m_bExternalData(0)
{
  m_pValues        = &m_aValues[0];
  for ( unsigned int i=0;i<SIZE;i++)
  {
    m_pValues[i] = pArray[i];
  }
  //memcpy( m_pValues, pArray, SIZE*sizeof(TYPE));
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
GSE_Vector<TYPE,SIZE>::GSE_Vector( const GSE_Vector & vVector)
{
  if ( vVector.m_bExternalData )
  {
    m_pValues = vVector.m_pValues;
    m_bExternalData = 1;
  } 
  else
  {
    m_pValues    = &m_aValues[0];
    memcpy( m_pValues, vVector.m_pValues, SIZE*sizeof(TYPE) );
    m_bExternalData = 0;
  }
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
GSE_Vector<TYPE,SIZE> &
GSE_Vector<TYPE,SIZE>::Set( TYPE aValues[SIZE] )
{
  for ( unsigned int i=0;i<SIZE;i++)
  {
    m_pValues[i] = aValues[i];
  }
  return *this;
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
void
GSE_Vector<TYPE,SIZE>::UseExternalData( TYPE *pArray )
{
  m_pValues = pArray;
  m_bExternalData = 1;
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, size_t SIZE>
inline TYPE &
GSE_Vector<TYPE,SIZE>::operator[](unsigned int iIndex)
{
  return m_pValues[iIndex];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE, size_t SIZE>
inline GSE_Vector<TYPE,SIZE> &
GSE_Vector<TYPE,SIZE>::operator=(GSE_Vector vVector)
{
  
  for(unsigned int nElem=0;nElem<SIZE;nElem++)
  {
    m_pValues[nElem] = vVector.m_pValues[nElem];
  }
  return *this;
}   
/////////////////////////////////////////////////////////////////
template<typename TYPE, size_t SIZE>
inline int 
GSE_Vector<TYPE,SIZE>::operator==(GSE_Vector<TYPE,SIZE> vVector)
{
  int bEqual = 1;
  
  for( unsigned int nElem=0;nElem<SIZE;nElem++)
  {
    bEqual = bEqual && (m_pValues[nElem] == vVector.m_pValues[nElem]);
  }
  return bEqual;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Id: GSE_Vector.h,v 1.10 2007/05/31 13:21:03 entity Exp $
// desc : A vector template which is the base for all vectors.
// author : AG/eNtity
// history : 
//		$Log: GSE_Vector.h,v $
//		Revision 1.10  2007/05/31 13:21:03  entity
//		more comments added.
//
//		Revision 1.9  2007/05/31 13:20:50  entity
//		doxy comments  added.
//
//		Revision 1.7  2007/03/09 10:45:04  entity
//		default constructor set as public
//
//		Revision 1.6  2007/03/07 13:36:59  entity
//		inline function added for operator=
//
//		Revision 1.5  2007/03/06 14:11:15  entity
//		better solution for funroll-loops
//
//		Revision 1.4  2007/02/28 11:52:23  entity
//		fixed data copy bug on Set()
//
//		Revision 1.3  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.2  2006/10/16 14:46:55  entity
//		Removed debug output
//
//		Revision 1.1  2006/10/16 07:10:48  entity
//		Created new GSE_Vector template and corresponding changes
//
//
/////////////////////////////////////////////////////////////////
