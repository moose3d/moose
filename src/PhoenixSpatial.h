/////////////////////////////////////////////////////////////////
#ifndef __PhoenixSpatial_h__
#define __PhoenixSpatial_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector4.h"
#include "PhoenixVector3.h"
#include "PhoenixVector2.h"
#include "PhoenixQuaternion.h"
#include "PhoenixMatrix4x4.h"
#include "PhoenixMatrix3x3.h"
#include "PhoenixMath.h"
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Spatial
  {
    /////////////////////////////////////////////////////////////////
    /// The base class for Objects with position.
    class CPositional 
    {
    protected:
      /// Vector where position is stored.
      CVector3<float> m_vPosition;
      /// has the position changed.
      int	      m_bPositionChanged;
    public:
      ////////////////////
      /// Default constructor.
      CPositional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
      {
	
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param vPosition vector for location-
      CPositional( const CVector3<float> & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
      {
      }
      ////////////////////
      /// Assigns position.
      /// \param vPosition vector of which values are used.
      inline void SetPosition( const CVector3<float> &vPosition )
      {
	m_vPosition = vPosition;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Assigns the position.
      /// \param fX position on X-axis.
      /// \param fY position on Y-axis.
      /// \param fZ position on Z-axis.
      inline void SetPosition( float fX, float fY, float fZ )
      {
	m_vPosition[0] = fX;
	m_vPosition[1] = fY;
	m_vPosition[2] = fZ;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Returns position.
      /// \param current position.
      inline const CVector3<float> & GetPosition() const
      {
	return m_vPosition;
      }
      ////////////////////
      /// Adds vector to current position.
      /// \param vPosition the movement applied to current position.
      inline void Move( const CVector3<float> & vPosition )
      {
	m_vPosition = m_vPosition + vPosition;
	SetPositionChanged(1);
      }
      ////////////////////
      /// Returns true if position has changed.
      /// \returns boolean
      int IsPositionChanged() const
      {
	return m_bPositionChanged;
      }
      ////////////////////
      /// Sets the position change flag.
      /// \param bFlag true for enable, false for disable.
      inline void SetPositionChanged(int bFlag )
      {
	m_bPositionChanged = bFlag;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// The class for single-directional object.
    class COneDirectional 
    {
    protected:
      /// Direction vector.
      CVector3<float> m_vDirection;
    public:
      ////////////////////
      /// Assigns the direction
      /// \param vDirection direction vector to be assigned.
      inline void SetDirection( const CVector3<float> & vDirection )
      {
	m_vDirection = vDirection;
      }
      ////////////////////
      /// Returns the direction.
      /// \returns Vector with current direction.
      inline const CVector3<float> & GetDirection() const
      {
	return m_vDirection;
      }
    };
    /////////////////////////////////////////////////////////////////



#ifndef _WIN32EXPORT 
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
#ifdef WIN32
    class CMaterialMgr;
    extern template class __declspec(dllimport) CSingleton<CMaterialMgr>;
#endif
#endif
    ////////////////////
    /// Vertex class for some cases where it is easier to handle things as a blob.
    class CVertex
    {
    public:
      /// Vertex position.
      CVector3<float> m_vPosition;
      /// Vertex normal.
      CVector3<float> m_vNormal;
      /// Vertex texture coordinates.
      CVector2<float> m_vTexCoord;
      /// Vertex color.
      CVector3<unsigned char> m_vColor;
      ////////////////////
      /// The equality comparison operator.
      /// \param vert CVertex object which this is compared against.
      bool operator==( CVertex vert)
      {
	return (m_vPosition == vert.m_vPosition &&
		m_vNormal   == vert.m_vNormal   &&
		m_vTexCoord == vert.m_vTexCoord && 
		m_vColor    == vert.m_vColor );
      }
      ////////////////////
      /// The less than comparison operator.
      bool operator< (CVertex vert)
      {
	return m_vPosition[1] < vert.m_vPosition[1];
      }
    };
    /////////////////////////////////////////////////////////////////
    /// \brief The generic orientable object. Contains base vectors
    /// which are rotated using the provided methods.
    class COrientable
    {
    protected:

      /// The vector pointing right.
      CVector3<float> m_vRight;
      /// The vector pointing upwards.
      CVector3<float> m_vUpward;
      /// The vector pointing forward.
      CVector3<float> m_vForward;
      /// The quaternion holding rotations. 
      CQuaternion m_qRotation;
      /////////////////////////////////////////////////////////////////
      /// A boolean flag indicating the change of rotation.
      char	m_bRotationChanged;
    public:
      /////////////////////////////////////////////////////////////////
      /// The constructor.
      COrientable();
      /////////////////////////////////////////////////////////////////
      /// The destructor.
      ~COrientable() {   }
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
      void Rotate           ( CVector3<float>  &vAngles );
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
      void RotateLocal         ( CVector3<float>  &vAngles );
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
      void         SetDirectionForwardUp   ( CVector3<float> vForward, 
					     CVector3<float> vUp );
      /////////////////////////////////////////////////////////////////
      /// Return the right vector.
      /// \return CVector3<float> The vector currently used as local right axis.
      inline const CVector3<float> & GetRightVector   () const
      {
	return m_vRight;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the up vector.
      /// \return CVector3<float> The vector currently used as local up axis.
      inline const CVector3<float> & GetUpVector      () const
      {
	return m_vUpward;
      }
      /////////////////////////////////////////////////////////////////
      /// Return the forward vector.
      /// \return CVector3<float> The vector currently used as local forward axis.
      inline const CVector3<float> & GetForwardVector () const
      {
	return m_vForward;
      }
      /////////////////////////////////////////////////////////////////
      /// Returns the current rotation.
      /// \returns CQuaternion the rotation quaternion.
      inline const CQuaternion	& GetRotationQuaternion() const
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
      void			SetRotationFromInitialState( const CVector3<float> &vForward,
							     const CVector3<float> &vRight,
							     const CVector3<float> &vUp,
							     const CQuaternion & qQuat);
      /////////////////////////////////////////////////////////////////
      /// Rotates the orientation by given quaternion.
      /// \param qQuaternion The rotation which will be applied to the current rotation.
      void			AppendToRotation( const CQuaternion &qQuaternion);
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
      void			SetRotation( const CMatrix4x4<float> &mMatrix );
      /////////////////////////////////////////////////////////////////
      /// Resets the orientation and applies rotation qRotation to vectors.
      /// \param qRotation The quaternion which is applied to original vectors.
      void			SetRotation( const CQuaternion &qRotation );
    protected:
      /////////////////////////////////////////////////////////////////
      /// Rotates all orientation vectors by q.
      /// \param q The quaternion which is applied to all vectors.
      inline void RotateAllDirections( const CQuaternion &q )
      {
	Phoenix::Math::RotateVector(q, m_vRight);
	Phoenix::Math::RotateVector(q, m_vForward);
	Phoenix::Math::RotateVector(q, m_vUpward);
      }
    };

  }; // namespace Spatial
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
