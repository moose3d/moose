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

#ifndef __MooseTransform_h__
#define __MooseTransform_h__
/////////////////////////////////////////////////////////////////
#include <MooseMatrix4x4.h>
#include <MooseQuaternion.h>
#include <MooseVector3.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    /// A class which stores scaling, rotation and translation into
    /// 4x4 matrix and provides methods for easily changing any of 
    /// those components.
    class MOOSE_API CTransform
    {
    protected:
      /// Is this transform changed.
      bool    m_bChanged;
      /// Scale factor.
      Moose::Math::CVector3<float>  m_vScaling;
      /// The transformation matrix.
      Moose::Math::CMatrix4x4<float> m_mTransform;
      /// Rotation quaternion.
      Moose::Math::CQuaternion  m_qRotation;
      /// Translation values.
      Moose::Math::CVector3<float>  m_vTranslation;
      /// A pointer to other matrix which is provides values.
      //CMatrix4x4<float> *m_pTransformHook;

    public:
      ////////////////////
      /// Default constructor.
      CTransform() : m_bChanged(0),
      m_vScaling(1.0f,1.0f,1.0),
      m_mTransform(1,0,0,0,
                   0,1,0,0,
                   0,0,1,0,
                   0,0,0,1 ),
      m_qRotation(0.0f,0.0f,0.0f,1.0f),
      m_vTranslation(0.0f,0.0f,0.0f) { }
      virtual ~CTransform()  {}
      ////////////////////
      /// Adds hook to given matrix.
      /// \param pMatrix A pointer to CMatrix4x4f.
      ///void AddHook( CMatrix4x4f *pMatrix);

      ////////////////////
      /// Gets current transform matrix.
      /// \return Reference to matrix object.
      const CMatrix4x4<float> & GetMatrix();
      ////////////////////
      /// Sets translation values.
      /// \param mMatrix Matrix representing a translation.
      void SetTranslation( const CMatrix4x4<float> &mMatrix );
      ////////////////////
      /// Sets translation values.
      /// \param vTranslation A CVector3 representing a translation.
      void SetTranslation( const CVector3<float> &vTranslation );
      ////////////////////
      /// Sets translation values.
      /// \param fX X axis translation.
      /// \param fY Y axis translation.
      /// \param fZ Z axis translation.
      void SetTranslation( float fX, float fY, float fZ );
      ////////////////////
      /// Returns current translation as vector.
      /// \returns Translation vector.
      const CVector3<float> & GetTranslation() const;
      ////////////////////  
      /// Assigns rotation values.
      /// \param mMatrix rotation matrix.
      void SetRotation( const CMatrix4x4<float> &mMatrix );
      ////////////////////  
      /// Assigns rotation values.
      /// \param qRotation a Rotation quaternion.
      void SetRotation( const CQuaternion &qRotation );
      ////////////////////  
      /// Assigns rotation values.
      /// \param fXrad X-axis rotation in Radians.
      /// \param fYrad Y-axis rotation in Radians.
      /// \param fZrad Z-axis rotation in Radians.
      void SetRotation( float fXrad, float fYrad, float fZrad );
      ////////////////////  
      /// Assigns rotation values.
      /// \param vAxisAngles vector with euler rotation angles in Radians.
      void SetRotation( const CVector3<float> &vAxisAngles );
      ////////////////////
      /// Returns rotation quaternion.
      /// \returns Quaternion.
      const CQuaternion & GetRotation() const;
      ////////////////////
      /// Scales all axis according to given values (appends values to existing scale).
      /// \param vScale Scale values to be appended to axes.
      void Scale( const CVector3<float> & vScale );
      ////////////////////
      /// Scales all axis according to given values (appends values to existing scale).
      /// \param fX Scale x axis.
      /// \param fY Scale y axis.
      /// \param fZ Scale z axis.
      void Scale( float fX, float fY, float fZ );
      ////////////////////
      /// Assigns uniform scaling factor.
      /// \param fScale the factor.
      void SetScaling( float fScale );
      ////////////////////
      /// Assigns scaling factors.
      /// \param fX Scale x axis.
      /// \param fY Scale y axis.
      /// \param fZ Scale z axis.
      void SetScaling( float fX, float fY, float fZ);
      ////////////////////
      /// Assigns scaling factors.
      /// \param vScale Vector containing each axis scale values.
      void SetScaling( const CVector3<float> & vScale );
      ////////////////////
      /// Returns uniform scaling factor.
      /// \returns Scaling factor.
      const CVector3<float> & GetScaling() const;
      ////////////////////
      /// Appends given vector to current transform.
      /// \param vVector Vector to be applied.
      void Move( const CVector3<float> &vVector );
      ////////////////////
      /// Appends given values to current transform.
      /// \param fX X-axis translation value.
      /// \param fY Y-axis translation value.
      /// \param fZ Z-axis translation value.
      void Move( float fX, float fY, float fZ);
      ////////////////////
      /// Appends rotation to current transform.
      /// \param q Rotation quaternion.
      void Rotate( const CQuaternion & q);
      ////////////////////
      /// Appends rotation to current transform using SLERP.
      /// \param q Rotation quaternion to be applied.
      /// \param fValue relative amount of applied rotation [0.0 - 1.0].
      void Slerp( const CQuaternion & q, float fValue );
      ////////////////////
      /// Is this transform been changed.
      /// \returns true if changed, false otherwise.
      bool IsChanged() const;
      ////////////////////
      /// Set change flag.
      /// \bFlag true for changed, false for unchanged.
      void SetChanged(bool bFlag );
      ////////////////////
      /// Resets all transforms.
      void SetIdentity();
      ////////////////////
      /// Returns forward vector.
      Moose::Math::CVector3<float> GetForwardVector();
      ////////////////////
      /// Returns right vector.
      Moose::Math::CVector3<float> GetRightVector();
      ////////////////////
      /// Returns up vector.
      Moose::Math::CVector3<float> GetUpVector();
    };
    ////////////////////
    /// Multiplies two CTransforms as matrices and stores result.
    /// \param rTransformRight Right-side transform.
    /// \param rTransformLeft  Left-side transform.
    /// \param rTransformResult Resulting transform.
    MOOSE_API void Multiply( const CTransform & rTransformLeft, 
                             const CTransform & rTransformRight, 
                             CTransform & rTransformResult);


  }; // namespace Math
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
