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
// file : GSE_Orientable.h
// desc : An object which can be rotated in 3D space.
// author: AG/eNtity
/////////////////////////////////////////////////////////////////
#ifndef __GSE_Orientable_h__
#define __GSE_Orientable_h__
/////////////////////////////////////////////////////////////////
#include "GSE_math.h"
/////////////////////////////////////////////////////////////////
using namespace Math;
/////////////////////////////////////////////////////////////////
/// \brief The generic orientable object. Contains base vectors
/// which are rotated using the provided methods.
class GSE_Orientable
{
 protected:
  /////////////////////////////////////////////////////////////////
  /// The vector pointing right.
  GSE_Vector3 m_vRight;
  /////////////////////////////////////////////////////////////////
  /// The vector pointing upwards.
  GSE_Vector3 m_vUpward;
  /////////////////////////////////////////////////////////////////
  /// The vector pointing forward.
  GSE_Vector3 m_vForward;
  /////////////////////////////////////////////////////////////////
  /// The quaternion holding rotations. 
  GSE_Quaternion m_qRotation;
  /////////////////////////////////////////////////////////////////
  /// A boolean flag indicating the change of rotation.
  char	m_bRotationChanged;
 public:
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_Orientable();
  /////////////////////////////////////////////////////////////////
  /// The destructor.
  ~GSE_Orientable() {   }
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis (1,0,0)
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundX          ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis (0,1,0)
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundY          ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis (0,0,1)
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundZ          ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object over world x,y,z axis by given degrees.
  /// \param vAngles the vector where angles for x,y,z axis rotations is given.
  void Rotate           ( GSE_Vector3  &vAngles );
  /////////////////////////////////////////////////////////////////
  /// Rotates object over world x,y,z axis by given degrees.
  /// \param fAroundWorldXAngle The degrees by which the object is rotated over world x-axis.
  /// \param fAroundWorldYAngle The degrees by which the object is rotated over world y-axis.
  /// \param fAroundWorldZAngle The degrees by which the object is rotated over world z-axis.
  void Rotate           ( float fAroundWorldXAngle, 
			  float fAroundWorldYAngle, 
			  float fAroundWorldZAngle );
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis "right".
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundRight   ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis "up".
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundUp      ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object given degrees over axis "forward".
  /// \param fDegrees the degrees the object is rotated around axis.
  void RotateAroundForward ( float    fDegrees );
  /////////////////////////////////////////////////////////////////
  /// Rotates object over local axes by given degrees.
  /// \param vAngles the vector where angles for right,up and forward axis rotations is given.
  void RotateLocal         ( GSE_Vector3  &vAngles );
  /////////////////////////////////////////////////////////////////
  /// Rotates object over local right,up,forward axis by given degrees.
  /// \param fAroundRightAngle The degrees by which the object is rotated over local right axis.
  /// \param fAroundUpAngle The degrees by which the object is rotated over local up axis.
  /// \param fAroundForwardAngle The degrees by which the object is rotated over local forward axis.
  void RotateLocal          ( float fAroundRightAngle, 
			      float fAroundUpAngle, 
			      float fAroundForwardAngle );
  /////////////////////////////////////////////////////////////////
  /// Initializes the object by assigning axis forward and up, the right
  /// axis is calculated by cross product of the two.
  /// Rotation is set to unit.
  void         SetDirectionForwardUp   ( GSE_Vector3 vForward, 
					 GSE_Vector3 vUp );
  /////////////////////////////////////////////////////////////////
  /// Return the right vector.
  /// \return GSE_Vector3 The vector currently used as local right axis.
  const GSE_Vector3 & GetRightVector   () const;
  /////////////////////////////////////////////////////////////////
  /// Return the up vector.
  /// \return GSE_Vector3 The vector currently used as local up axis.
  const GSE_Vector3 & GetUpVector      () const;
  /////////////////////////////////////////////////////////////////
  /// Return the forward vector.
  /// \return GSE_Vector3 The vector currently used as local forward axis.
  const GSE_Vector3 & GetForwardVector () const;
  /////////////////////////////////////////////////////////////////
  /// Returns the current rotation.
  /// \returns GSE_Quaternion the rotation quaternion.
  GSE_Quaternion	GetRotationQuaternion();
  /////////////////////////////////////////////////////////////////
  /// Initializes the orientation by given local axis and rotates them
  /// by given quaternion.
  /// \param vForward The vector for forward vector.
  /// \param vRight The vector for right vector.
  /// \param vUp The vector for up vector.
  /// \param qQuat The rotation quaternion which is applied to vectors.
  void			SetRotationFromInitialState( GSE_Vector3 &vForward,
						     GSE_Vector3 &vRight,
						     GSE_Vector3 &vUp,
						     GSE_Quaternion & qQuat);
  /////////////////////////////////////////////////////////////////
  /// Rotates the orientation by given quaternion.
  /// \param qQuaternion The rotation which will be applied to the current rotation.
  void			AppendToRotation( const GSE_Quaternion &qQuaternion);
  /////////////////////////////////////////////////////////////////
  /// Returns boolean has the rotation changed.
  /// \returns boolean true if any rotations have been applied, false otherwise.
  int			IsRotationChanged() const;
  /////////////////////////////////////////////////////////////////
  /// Sets the flag which indicates rotation change.
  /// \param bFlag boolean flag.
  void			SetRotationChanged( int bFlag );
  /////////////////////////////////////////////////////////////////
  /// Resets the orientation to original state.
  /// Forward = (0,0,-1)
  /// Up = ( 0,1,0)
  /// Right = ( 1,0,0)
  void			Reset();
  /////////////////////////////////////////////////////////////////
  /// Resets the orientation and applies rotation matrix mMatrix to vectors.
  /// \param mMatrix 4x4 rotation matrix which is applied to original vectors.
  void			SetRotation( const GSE_Matrix4x4f &mMatrix );
  /////////////////////////////////////////////////////////////////
  /// Resets the orientation and applies rotation qRotation to vectors.
  /// \param qRotation The quaternion which is applied to original vectors.
  void			SetRotation( const GSE_Quaternion &qRotation );
 protected:
  /////////////////////////////////////////////////////////////////
  /// Rotates all orientation vectors by q.
  /// \param q The quaternion which is applied to all vectors.
  inline void RotateAllDirections( const GSE_Quaternion &q )
  {
    m_vRight.Rotate(q);
    m_vForward.Rotate(q);
    m_vUpward.Rotate(q);
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// history:
//		$Log: GSE_Orientable.h,v $
//		Revision 1.11  2007/06/01 08:55:43  entity
//		doxygen comments added
//
//		Revision 1.10  2007/03/26 18:18:01  entity
//		Fully commented class (phew)
//
//		Revision 1.9  2007/03/26 12:54:33  entity
//		const & stuff
//
//		Revision 1.8  2007/03/21 08:21:46  entity
//		Get*Vector() is now const
//
//		Revision 1.7  2007/03/20 20:40:30  entity
//		const & stuff added
//
//		Revision 1.6  2007/03/15 14:04:18  entity
//		SetRotation() added
//
//		Revision 1.5  2007/03/14 10:57:10  entity
//		added IsRotationChanged()
//
//		Revision 1.4  2007/03/12 14:50:51  entity
//		Removed unneeded virtual functions
//
//		Revision 1.3  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.2  2006/10/10 07:52:57  entity
//		some changes
//
/////////////////////////////////////////////////////////////////
