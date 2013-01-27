/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/////////////////////////////////////////////////////////////////
#ifndef __MooseSpatial_h__
#define __MooseSpatial_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#include "MooseVector3.h"
#include "MooseQuaternion.h"
#include "MooseMatrix4x4.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
    void MOOSE_API RotateVector( const CQuaternion &qRotation, CVector3<float> &vVector);
  }
  namespace Spatial
  {

    // WHOA!! This was probably the most mind-boggling thing to fix. This is
    // related to the differences between UNIX shared object files (.so) and
    // Windows's Dynamically Linked Libraries (.DLL)
    // Using Singleton objects in a DLL and application using the DLL without
    // the following extern declaration makes the compiler to create two instances
    // of the singleton; one inside the DLL and one inside the application binary.
    // And since the point of Singleton is to have a single instance of an object, so
    // this nasty feature wreaks havoc on implementation. Following statements
    // say that the CSingleton<CMaterialMgr> template has already been
    // created in DLL and should not be created again.

    //    class CMaterialMgr;
    //extern template class __declspec(dllimport) CSingleton<CMaterialMgr>;


    /////////////////////////////////////////////////////////////////
    /// Generic orientable object. Contains base vectors
    /// which are rotated using the provided methods.
    class MOOSE_API COrientable
    {
    protected:

      /// The vector pointing right.
      Moose::Math::CVector3<float> m_vRight;
      /// The vector pointing upwards.
      Moose::Math::CVector3<float> m_vUpward;
      /// The vector pointing forward.
      Moose::Math::CVector3<float> m_vForward;
      /// The quaternion holding rotations.
      Moose::Math::CQuaternion m_qRotation;
      /////////////////////////////////////////////////////////////////
      /// A boolean flag indicating a change in rotation.
      char	m_bRotationChanged;
    public:
      /////////////////////////////////////////////////////////////////
      /// The constructor.
      COrientable();
      /////////////////////////////////////////////////////////////////
      /// The destructor.
      virtual ~COrientable() {}
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
      void Rotate           ( const Moose::Math::CVector3<float>  &vAngles );
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
      void RotateLocal         ( Moose::Math::CVector3<float>  &vAngles );
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
      void         SetDirectionForwardUp   ( Moose::Math::CVector3<float> vForward,
					     Moose::Math::CVector3<float> vUp );
      /////////////////////////////////////////////////////////////////
      /// Return the right vector.
      /// \return CVector3<float> The vector currently used as local right axis.
      inline const Moose::Math::CVector3<float> & GetRightVector  () const
      {
	return m_vRight;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the up vector.
      /// \return CVector3<float> The vector currently used as local up axis.
      inline const Moose::Math::CVector3<float> &GetUpVector      () const
      {
	return m_vUpward;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the forward vector.
      /// \return CVector3<float> The vector currently used as local forward axis.
      inline const Moose::Math::CVector3<float> & GetForwardVector () const
      {
	return m_vForward;
      }
      /////////////////////////////////////////////////////////////////
      /// Returns the current rotation.
      /// \returns CQuaternion the rotation quaternion.
      inline const Moose::Math::CQuaternion	& GetRotationQuaternion() const
      {
	return m_qRotation;
      }
      /////////////////////////////////////////////////////////////////
      /// Initializes the orientation by given local axis and rotates them
      /// by given quaternion.
      /// \param vForward The vector for forward vector.
      /// \param vRight The vector for right vector.
      /// \param vUp The vector for up vector.
      /// \param qQuat The rotation quaternion which is applied to vectors.
      void			SetRotationFromInitialState( const Moose::Math::CVector3<float> &vForward,
							     const Moose::Math::CVector3<float> &vRight,
							     const Moose::Math::CVector3<float> &vUp,
							     const Moose::Math::CQuaternion & qQuat);
      /////////////////////////////////////////////////////////////////
      /// Rotates the orientation by given quaternion.
      /// \param qQuaternion The rotation which will be applied to the current rotation.
      void			AppendToRotation( const Moose::Math::CQuaternion &qQuaternion);
      /////////////////////////////////////////////////////////////////
      /// Returns boolean has the rotation changed.
      /// \returns boolean true if any rotations have been applied, false otherwise.
      inline int	       IsRotationChanged() const
      {
	return m_bRotationChanged;
      }
      /////////////////////////////////////////////////////////////////
      /// Sets the flag which indicates rotation change.
      /// \param bFlag boolean flag.
      inline void SetRotationChanged( int bFlag )
      {
	m_bRotationChanged = bFlag;
      }
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation to original state.
      /// Forward = (0,0,-1)
      /// Up = ( 0,1,0)
      /// Right = ( 1,0,0)
      void			Reset();
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation and applies rotation matrix mMatrix to vectors.
      /// \param mMatrix 4x4 rotation matrix which is applied to original vectors.
      void			SetRotation( const Moose::Math::CMatrix4x4<float> &mMatrix );
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation and applies rotation qRotation to vectors.
      /// \param qRotation The quaternion which is applied to original vectors.
      void			SetRotation( const Moose::Math::CQuaternion &qRotation );
    protected:
      /////////////////////////////////////////////////////////////////
      /// Rotates all orientation vectors by q.
      /// \param q The quaternion which is applied to all vectors.
      inline void RotateAllDirections( const Moose::Math::CQuaternion &q )
      {
	Moose::Math::RotateVector(q, m_vRight);
	Moose::Math::RotateVector(q, m_vForward);
	Moose::Math::RotateVector(q, m_vUpward);
          m_vRight.Normalize();
          m_vForward.Normalize();
          m_vUpward.Normalize();
        
      }
    }; // COrientable



  }; // namespace Spatial
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
