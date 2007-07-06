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
#ifndef __GSE_Dimensional_h__
#define __GSE_Dimensional_h__
/////////////////////////////////////////////////////////////////
/// A class which provides dimensions in 3D. Width, height and length.
class GSE_Dimensional
{
 protected:
  /// The width.
  float m_fWidth;
  /// The height.
  float m_fHeight;
  /// The length.
  float m_fLength;
  /// Length * 0.5
  float m_fHalfLength;
  /// Height * 0.5
  float m_fHalfHeight;
  /// Width * 0.5
  float m_fHalfWidth;
  ////////////////////
  /// The constructor. Initializes paramaters.
  GSE_Dimensional() : 
   m_fWidth(0.0f), 
   m_fHeight(0.0f), 
   m_fLength(0.0f),
   m_fHalfLength(0.0f),
   m_fHalfHeight(0.0f),
   m_fHalfWidth(0.0f)
  {
    
  }
  ////////////////////
  /// The parametrized constructor. 
  /// \param fWidth The width of the object
  /// \param fHeight The height of the object
  /// \param fLength The length of the object
  GSE_Dimensional( float fWidth, float fHeight, float fLength ) 
  {
    SetWidth(fWidth);
    SetHeight(fHeight);
    SetLength(fLength);
  }
 public:
  ////////////////////
  /// Assigns the width.
  /// \param fWidth The value for width.
  inline void SetWidth ( float fWidth  )
  {  
    m_fWidth = fWidth;
    m_fHalfWidth = m_fWidth * 0.5f;
  }
  ////////////////////
  /// Assigns the height.
  /// \param fHeight The value for height.
  inline void SetHeight( float fHeight )    
  {
    m_fHeight = fHeight;
    m_fHalfHeight = m_fHeight * 0.5f;
  }
  ////////////////////
  /// Assigns the length.
  /// \param fLength The value for length.
  inline void SetLength( float fLength )
  {	
    m_fLength = fLength;
    m_fHalfLength = m_fLength * 0.5f;
  }
  ////////////////////
  /// Returns the length.
  /// \returns The length.
  inline float GetLength() const
  {	
    return m_fLength;
  }
  ////////////////////
  /// Returns the height.
  /// \returns The height.
  inline float GetHeight() const
  {	
    return m_fHeight;
  }
  ////////////////////
  /// Returns the width.
  /// \returns The width.
  inline float GetWidth() const
  {	
    return m_fWidth;
  }
  ////////////////////
  /// Returns half of the width
  /// \returns half of the width
  inline float GetHalfWidth() const 
  {
    return m_fHalfWidth;
  }
  ////////////////////
  /// Returns half of the height
  /// \returns half of the height
  inline float GetHalfHeight() const 
  {
    return m_fHalfHeight;
  }
  ////////////////////
  /// Returns half of the length
  /// \returns half of the length
  inline float GetHalfLength() const 
  {
    return m_fHalfLength;
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_Dimensional.h,v $
// Revision 1.8  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.7  2007/05/10 21:02:57  entity
// fixed param constr
//
// Revision 1.6  2007/05/10 19:11:59  entity
// halfdimensinons added
//
// Revision 1.5  2007/03/29 06:15:37  entity
// added some comments.
//
// Revision 1.4  2007/03/29 06:15:05  entity
// Comments and parameterized constructor added.
//
/////////////////////////////////////////////////////////////////
