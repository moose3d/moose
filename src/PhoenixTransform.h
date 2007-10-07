#ifndef __PhoenixTransform_h__
#define __PhoenixTransform_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMatrix4x4.h"
#include "PhoenixQuaternion.h"
#include "PhoenixVector3.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    /// A class which stores scaling, rotation and translation into
    /// 4x4 matrix and provides methods for easily changing any of 
    /// those components.
    class CTransform 
    {
    protected:
      /// Is this transform changed.
      unsigned int    m_bChanged;
      /// Scale factor.
      float		  m_fScaling;
      /// The transformation matrix.
      Phoenix::Math::CMatrix4x4<float> m_mTransform;
      /// Rotation quaternion.
      Phoenix::Math::CQuaternion  m_qRotation;
      /// Translation values.
      Phoenix::Math::CVector3<float>  m_vTranslation;
      /// A pointer to other matrix which is provides values.
      //CMatrix4x4<float> *m_pTransformHook;

    public:
      ////////////////////
      /// Default constructor.
      CTransform() : m_bChanged(0),
                     m_fScaling(1.0f),
                     m_mTransform(1,0,0,0,
				  0,1,0,0,
				  0,0,1,0,
				  0,0,0,1 ),
                     m_qRotation(0.0f,0.0f,0.0f,1.0f),
                     m_vTranslation(0.0f,0.0f,0.0f) { }
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
      /// \param fX X-axis rotation.
      /// \param fY Y-axis rotation.
      /// \param fZ Z-axis rotation.
      void SetRotation( float fX, float fY, float fZ );
      ////////////////////  
      /// Assigns rotation values.
      /// \param vAxisAngles vector with euler rotation angles.
      void SetRotation( const CVector3<float> &vAxisAngles );
      /// Assigns scaling factor.
      /// \param fScale the factor.
      void SetScaling( float fScale );
      ////////////////////
      /// Returns uniform scaling factor.
      /// \returns Scaling factor.
      float GetScaling() const;
      /// Appends given vector to current transform.
      /// \param vVector Vector to be applied.
      void Move( const CVector3<float> &vVector );
      /// Appends given values to current transform.
      /// \param fX X-axis translation value.
      /// \param fY Y-axis translation value.
      /// \param fZ Z-axis translation value.
      void Move( float fX, float fY, float fZ);
      /// Appends rotation to current transform.
      /// \param q Rotation quaternion.
      void Rotate( const CQuaternion & q);
    };
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
