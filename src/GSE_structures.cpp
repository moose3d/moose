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

// -----------------------------------------------------------------
// $Id: GSE_structures.cpp,v 1.11 2007/05/14 09:46:57 entity Exp $
// desc : Implementation for various stuff in GSE_structures.h
// author : AG/eNtity
// history :
//
//	$Log: GSE_structures.cpp,v $
//	Revision 1.11  2007/05/14 09:46:57  entity
//	more reasonable operations for Vector2
//
//	Revision 1.10  2007/03/31 16:21:25  entity
//	ObjStruct -> GeometryData rename
//
//	Revision 1.9  2007/03/30 12:56:32  entity
//	SetElements->SetVertexCount rename
//
//	Revision 1.8  2007/03/26 18:25:11  entity
//	Rotate() modified and GetRotated() added
//
//	Revision 1.7  2007/03/11 12:48:18  entity
//	const keyword meddling and unneeded vars removed
//
//	Revision 1.6  2007/03/07 13:34:02  entity
//	Vector3 with inlined functions
//
//	Revision 1.5  2007/01/08 12:24:00  entity
//	added GPL licence
//
//	Revision 1.4  2006/10/16 07:10:48  entity
//	Created new GSE_Vector template and corresponding changes
//
//	Revision 1.3  2006/10/13 13:57:05  entity
//	fixed wrong macro name
//
//	Revision 1.2  2006/10/13 13:54:41  entity
//	Added file header, made code cleanups and added comments
//
//
// -----------------------------------------------------------------
#include "GSE_structures.h"
#include "GSE_math.h"
#include "GSE_Logger.h"
#include <math.h>
#include <string.h>
#include "GSE_OglUtils.h"
// -----------------------------------------------------------------
#define DELETE(A) if ( A != NULL ) delete A; A=NULL;
// -----------------------------------------------------------------
using namespace Math;
using namespace Core;
// --------------------------------------------------------------------
// Initialize static members of vector3 class
GSE_Vector3 GSE_Vector3::m_vWorldX = GSE_Vector3(1,0,0);
GSE_Vector3 GSE_Vector3::m_vWorldY = GSE_Vector3(0,1,0);
GSE_Vector3 GSE_Vector3::m_vWorldZ = GSE_Vector3(0,0,1);
// --------------------------------------------------------------------
/////////////////////////////////////////////////////////////////
// The GSE_Vector3 implementation
/////////////////////////////////////////////////////////////////
/// Macro for making code more readably
#define Q_VECTOR GSE_Quaternion(m_pValues[GSE_Vector3::X], m_pValues[GSE_Vector3::Y], m_pValues[GSE_Vector3::Z],0.0f)
/////////////////////////////////////////////////////////////////
GSE_Vector3
GSE_Vector3::GetRotated( const GSE_Quaternion & qRotation ) const
{
  //  W = Qr * Qv * Qr¯¹
  return (qRotation * Q_VECTOR * qRotation.Inverse()).GetVector();

}
/////////////////////////////////////////////////////////////////
void
GSE_Vector3::Rotate( const GSE_Quaternion &qRotation ) 
{
  //  W = Qr * Qv * Qr¯¹
  *this = (qRotation * Q_VECTOR * qRotation.Inverse()).GetVector();
}
/////////////////////////////////////////////////////////////////
#undef Q_VECTOR  
/////////////////////////////////////////////////////////////////
// Returns the World X vector (1.0, 0.0, 0.0);
GSE_Vector3 &
GSE_Vector3::GetWorldX()
{
  return m_vWorldX;
}
// --------------------------------------------------------------------
// Returns the World Y vector (0.0, 1.0, 0.0);
GSE_Vector3 &
GSE_Vector3::GetWorldY()
{
  return m_vWorldY;
}
// --------------------------------------------------------------------
// Returns the World Z vector (0.0, 0.0, 1.0);
GSE_Vector3 &
GSE_Vector3::GetWorldZ()
{
  return m_vWorldZ;
}
// --------------------------------------------------------------------
//
// The more simplistic stream output for debugging.
//
std::ostream& 
operator<<(std::ostream &stream, GSE_Vector3 vVector)
{
  stream << "(" 
	 << vVector[GSE_Vector3::X] << "," 
	 << vVector[GSE_Vector3::Y] << "," 
	 << vVector[GSE_Vector3::Z] << ")" ;
  return stream;
}
// --------------------------------------------------------------------


// -------------------------------------------------------------------------
GSE_Vector2::GSE_Vector2()
{
  
}

// --------------------------------------------------------------------
GSE_Vector2::GSE_Vector2( float fX, float fY )
{
  m_pValues[0] = fX;
  m_pValues[1] = fY;
}
// --------------------------------------------------------------------
GSE_Vector2::~GSE_Vector2()
{
}
// --------------------------------------------------------------------
GSE_Vector2   
GSE_Vector2::operator*(float fScalar) const
{
  return GSE_Vector2(m_pValues[X] * fScalar,
		     m_pValues[Y] * fScalar);
}
// --------------------------------------------------------------------
// The negation operation
GSE_Vector2   
GSE_Vector2::operator-() const
{
  
  return GSE_Vector2(-m_pValues[GSE_Vector2::X],
		     -m_pValues[GSE_Vector2::Y]);
}
// --------------------------------------------------------------------
// The negation operation
GSE_Vector2   
GSE_Vector2::operator-(const GSE_Vector2 &vVector) const
{
  return  GSE_Vector2(m_pValues[X]-vVector.m_pValues[X],
		      m_pValues[Y]-vVector.m_pValues[Y]);
}
// --------------------------------------------------------------------
// The sum operation
GSE_Vector2 
GSE_Vector2::operator+(const GSE_Vector2 & vVector) const
{
  
  return  GSE_Vector2(m_pValues[X]+vVector.m_pValues[X],
		      m_pValues[Y]+vVector.m_pValues[Y]);
  
}
// --------------------------------------------------------------------
//
// The more simplistic stream output for debugging.
//
std::ostream& operator<<(std::ostream &stream, GSE_Vector2 vVector)
{
  stream << "(" 
	 << vVector[GSE_Vector2::X] << "," 
	 << vVector[GSE_Vector2::Y] << ")" ;
  return stream;
}
// --------------------------------------------------------------------
void 
GSE_Vector2::Set( float fX, float fY )
{
  m_pValues[GSE_Vector2::X] = fX;
  m_pValues[GSE_Vector2::Y] = fY;
}
// --------------------------------------------------------------------
unsigned int GSE_Object::m_nObjectCounter = 0;
GSE_Object::GSE_Object()
{
  m_nId = m_nObjectCounter;
  m_nObjectCounter++;
  SetName(NULL);
}
// --------------------------------------------------------------------
void
GSE_Object::SetName( char *sNewName )
{
  if( sNewName == NULL )
  {
    char   sBuf[GSE_OBJECT_MAX_NAME_SIZE];
    
    snprintf( sBuf, GSE_OBJECT_MAX_NAME_SIZE-1, "object%d", m_nId );

    sBuf[GSE_OBJECT_MAX_NAME_SIZE-1] = '\0';
    strncpy( m_sName, sBuf, GSE_OBJECT_MAX_NAME_SIZE );
    GSE_DEBUG("setting name to " << sBuf );
  }
  else
  {
    strncpy( m_sName, sNewName, GSE_OBJECT_MAX_NAME_SIZE );
    GSE_DEBUG("setting name to " << m_sName );
  }
}
// --------------------------------------------------------------------
const char *
GSE_Object::GetName()
{
  return m_sName;
}
// --------------------------------------------------------------------
unsigned int
GSE_Object::GetID()
{
  return m_nId;
}
// --------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------
