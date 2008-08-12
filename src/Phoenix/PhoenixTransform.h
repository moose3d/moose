#ifndef __PhoenixTransform_h__
#define __PhoenixTransform_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixMatrix4x4.h>
#include <PhoenixQuaternion.h>
#include <PhoenixVector3.h>
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
      bool    m_bChanged;
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
      /// Assigns scaling factor.
      /// \param fScale the factor.
      void SetScaling( float fScale );
      ////////////////////
      /// Returns uniform scaling factor.
      /// \returns Scaling factor.
      float GetScaling() const;
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
    };
    ////////////////////
    /// Multiplies two CTransforms as matrices and stores result.
    /// \param rTransformRight Right-side transform.
    /// \param rTransformLeft  Left-side transform.
    /// \param rTransformResult Resulting transform.
    void Multiply( const CTransform & rTransformLeft, 
		   const CTransform & rTransformRight, 
		   CTransform & rTransformResult);

    /////////////////////////////////////////////////////////////////
    /// Transformable base class
    class CTransformable 
    {
    protected:
      Phoenix::Math::CTransform m_LocalTransform;
      Phoenix::Math::CTransform m_WorldTransform;
      bool m_bChanged;
    public:
      CTransformable();
      virtual ~CTransformable() {} 
      ////////////////////
      /// Returns reference to local transform.
      /// \returns Local transform
      Phoenix::Math::CTransform & GetLocalTransform();
      ////////////////////
      /// Returns reference to local transform.
      /// \returns Local transform
      const Phoenix::Math::CTransform & GetLocalTransform() const;
      ////////////////////
      /// Returns reference to world transform.
      /// \returns World transform
      Phoenix::Math::CTransform & GetWorldTransform();
      ////////////////////
      /// Returns reference to world transform.
      /// \returns World transform
      const Phoenix::Math::CTransform & GetWorldTransform() const;
      ////////////////////
      /// Assigns world transform.
      /// \param rTransform transform to be set as world
      void SetWorldTransform( const Phoenix::Math::CTransform & rTransform );
      ////////////////////
      /// Assigns local transform.
      /// \param rTransform transform to be set as local
      void SetLocalTransform( const Phoenix::Math::CTransform & rTransform );
      ////////////////////
      /// Is this transformable been updated.
      /// \returns true if changed, false otherwise.
      bool IsChanged() const;
      ////////////////////
      /// Set change flag.
      /// \bFlag true for changed, false for unchanged.
      void SetChanged(bool bFlag );
    };
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
