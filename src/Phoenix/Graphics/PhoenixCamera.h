/////////////////////////////////////////////////////////////////
#ifndef __PhoenixCamera_h__
#define __PhoenixCamera_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector2.h"
#include "PhoenixQuaternion.h"
#include "PhoenixMatrix4x4.h"
#include "PhoenixPositional.h"
#include "PhoenixOrientable.h"
#include "PhoenixCone.h"
#include "PhoenixSphere.h"
#include "PhoenixFrustum.h"
#include "PhoenixRay.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Camera class.
    class PHOENIX_API CCamera : public Phoenix::Spatial::CPositional, public Phoenix::Spatial::COrientable
    {
    protected:
      /// field of view angle in degrees.
      float		m_fFieldOfView;  
      /// viewport for this camera, starting from 
      /// lower left corner { x, y, width, height } 
      int		m_aViewport[4];
      /// The distance of near clipping plane 
      float		m_fNearClipping;
      /// The distance of the far clipping plane.
      float		m_fFarClipping;
      /// Should this camera show lens flares on light sources.
      int		m_bLensFlaresEnabled;
      /// Is the orthogonal view enabled
      int		m_bOrtho;
      /// The coordinates for left, right, bottom and top clipping planes in orthogonal mode
      float		m_aOrthoPlanes[4];
      /// View frustum for this camera
      CFrustum		m_Frustum;
      /// The projection matrix for this camera
      Phoenix::Math::CMatrix4x4<float> m_mProjection;
      /// The inverse projection matrix for this camera
      Phoenix::Math::CMatrix4x4<float> m_mProjectionInv;
      /// The view matrix for this camera
      Phoenix::Math::CMatrix4x4<float> m_mView;
      /// The inverse view matrix for this camera
      Phoenix::Math::CMatrix4x4<float> m_mViewInv;
      /// Is the projection changed
      int		 m_bProjectionChanged;
      /// Bounding sphere for frustum.
      Phoenix::Volume::CSphere m_FrustumSphere;
      /// Bounding sphere for cone.
      Phoenix::Volume::CCone m_FrustumCone;
      /// Distance to point along forwardvector, which will be used in virtual trackball.
      float			m_fTrackballCenterDistance;
      /// Offset value for drawing decals.
      float			m_fDecalOffset;
    public:
      ////////////////////
      /// Default constructor
      CCamera();
      ////////////////////
      /// Destructor.
      ~CCamera();
      ////////////////////
      /// Sets the Field of view.
      /// \param fDegrees Field of view in degrees.
      void SetFieldOfView(float fDegrees);
      ////////////////////
      /// Sets the orthogonal view left, right, bottom and top plane locations.
      /// \param fLeft Left plane distance.
      /// \param fRight Right plane distance.
      /// \param fBottom Bottom plane distance.
      /// \param fTop Top plane distance.
      void SetViewOrtho( float fLeft, float fRight, float fBottom, float fTop );
      ////////////////////
      /// Sets viewport for this camera.
      /// \param iX X-coordinate of lower-left corner.
      /// \param iY Y-coordinate of lower-left corner.
      /// \param iWidth Width of viewport.
      /// \param iHeight Height of viewport.
      void SetViewport( int iX, int iY, int iWidth, int iHeight);
      ////////////////////
      /// Sets near clipping plane.
      /// \param fNearClipping Near clipping plane distance
      void SetNearClipping(float fNearClipping);
      ////////////////////
      /// Sets the Far clipping plane
      /// \param fFarClipping Far clipping plane distance
      void SetFarClipping(float fFarClipping);
      ////////////////////
      /// Returns near clipping plane distance.
      /// \returns Near clipping plane distance.
      float GetNearClipping();
      ////////////////////
      /// Returns the Far clipping plane distance.
      /// \returns Far clipping plane distance.
      float GetFarClipping();
      ////////////////////
      /// Returns the array of viewport settings.
      /// \returns Pointer to int array
      int * GetViewport();
      ////////////////////
      /// Returns the array of viewport settings.
      /// \returns Pointer to int array
      const int * GetViewport() const;
      ////////////////////
      /// Returns field of view angle.
      /// \returns field of view in degrees.
      float GetFieldOfView() const;
      ////////////////////
      /// Returns true if lens flares are enabled.
      /// \returns Non-zero if true
      /// \returns Zero if false
      int IsLensFlaresEnabled();
      ////////////////////
      /// Sets lens flares enabled or disabled according the bFlag.
      /// \param bFlag Non-zero to enable, zero to disable.
      void SetLensFlaresEnabled(int bFlag);
      ////////////////////
      /// Prints out info about camera into stream.
      /// \param stream output stream
      /// \param obj Camera.
      friend std::ostream &operator<<(std::ostream &stream, const CCamera &obj);
      ////////////////////
      /// Returns reference to frustum object
      /// \returns Reference to frustum
      CFrustum &Frustum();
      ////////////////////
      /// Returns reference to frustum object
      /// \returns Reference to frustum
      const CFrustum &Frustum() const;
      ////////////////////
      /// Calculates frustum.
      void CalculateFrustum();
      ////////////////////
      /// Calculates the bounding sphere for the frustum. Not optimal.
      void CalculateBoundingSphere();
      ////////////////////
      /// Calculates the bounding cone for the frustum
      void CalculateBoundingCone();
      ////////////////////
      /// Returns reference to the bounding cone
      /// \returns Reference to cone.
      Phoenix::Volume::CCone   &FrustumCone();
      ////////////////////
      /// Returns reference to the bounding sphere.
      /// \returns Reference to sphere.
      Phoenix::Volume::CSphere &FrustumSphere();
      ////////////////////
      /// Returns reference to the bounding cone
      /// \returns Reference to cone.
      const Phoenix::Volume::CCone   &FrustumCone() const;
      ////////////////////
      /// Returns reference to the bounding sphere.
      /// \returns Reference to sphere.
      const Phoenix::Volume::CSphere &FrustumSphere() const;
      ////////////////////
      /// Returns true if orthogonal mode is on
      /// \returns Non-zero camera view is orthogonal
      /// \returns Zero if camera view is perspective.
      int IsOrthogonal();
      ////////////////////
      /// Returns the pointer to orthogonal plane coordinates.
      /// \param Pointer to array of ortho plane values.
      float *GetOrthoPlanes();
      ////////////////////
      /// Rotates camera around a point in space.
      /// \param vPoint Point around which rotation occurs.
      /// \param q Rotation quaternion.
      void RotateAroundPoint( const Phoenix::Math::CVector3<float> & vPoint, const Phoenix::Math::CQuaternion & q);
      ////////////////////
      /// Returns projection matrix.
      /// \returns Reference to projection matrix.
      inline const Phoenix::Math::CMatrix4x4<float> & GetProjectionMatrix() const
      {
	return m_mProjection;
      }
      ////////////////////
      /// Returns view matrix.
      /// \returns Refernce to view matrix.
      inline const Phoenix::Math::CMatrix4x4<float> & GetViewMatrix() const
      {
	return m_mView;
      }
      ////////////////////
      /// Returns projection matrix.
      /// \returns Reference to projection matrix.
      inline Phoenix::Math::CMatrix4x4<float> & GetProjectionMatrix()
      {
	return m_mProjection;
      }
      ////////////////////
      /// Returns view matrix.
      /// \returns Refernce to view matrix.
      inline Phoenix::Math::CMatrix4x4<float> & GetViewMatrix()
      {
	return m_mView;
      }
      ////////////////////
      /// Recalculates projection matrix
      void UpdateProjection();
      ////////////////////
      /// Recalculates view matrix
      void UpdateView();
      ////////////////////
      /// Returns inverse view matrix.
      /// \returns Reference to inverse view matrix.
      inline Phoenix::Math::CMatrix4x4<float> & GetInvView() 
      {
	return m_mViewInv;
      }
      ////////////////////
      /// Returns inverse projection matrix.
      /// \returns Reference to inverse projection matrix.
      inline  Phoenix::Math::CMatrix4x4<float> & GetInvProjection() 
      {
	return m_mProjectionInv;
      }
      ////////////////////
      /// Checks does projection need updating.
      /// \returns Non-zero if changed.
      /// \returns Zero if not changed.
      inline int IsProjectionChanged() const
      {
	return m_bProjectionChanged;
      }
      ////////////////////
      /// Checks does view need updating.
      /// \returns Non-zero if changed.
      /// \returns Zero if not changed.
      inline int IsViewChanged() const
      {
	return (IsPositionChanged() || IsRotationChanged());
      }
      ////////////////////
      /// Sets projection change flag.
      /// \param bFlag boolean.
      inline void SetProjectionChanged(int bFlag )
      {
	m_bProjectionChanged = bFlag;
      }
      ////////////////////
      /// Strafes camera (moves sideways).
      /// \param fAmount Amount of movement along right vector.
      inline void Strafe( float fAmount ) 
      {
	CPositional::Move( fAmount * GetRightVector());
      }
      ////////////////////
      /// Lifts / lowers camera.
      /// \param fAmount Amount of movement along up vector.
      inline void Elevate( float fAmount ) 
      {
	CPositional::Move( fAmount * GetUpVector());
      }
      ////////////////////
      ///  Moves camera forward or backward.
      /// \param fAmount Amount of movement along forward vector.
      inline void Move ( float fAmount )
      {
	CPositional::Move( fAmount * GetForwardVector());
      }
      ////////////////////
      /// Moves camera using direction and length of given vector.
      /// \param vVector Movement vector.
      inline void Move ( const Phoenix::Math::CVector3<float> & vVector )
      {
	CPositional::Move( vVector );
      }
      ////////////////////
      /// Converts window coordinates (x,y,z) into eye coordinates. 
      /// \param fX x coordinate with 0,0 as lower left corner.
      /// \param fY y coordinate with 0,0 as lower left corner.
      /// \param fZ z coordinate, 0.0f as the near clipping plane, 
      ///                         1.0f as the far clipping plane.
      /// \returns CVector3<float> the window coordinates as eye coordinates.
      Phoenix::Math::CVector3<float> UnProjectToEye( float fX, float fY, float fZ  );
      ////////////////////
      /// Converts eye coordinates into world coordinates.
      /// \param vPosition The position in eye coordinates which is converted into world coordinates.
      /// \returns CVector3<float> the vPosition in world coordinates.
      Phoenix::Math::CVector3<float> EyeToWorld( const Phoenix::Math::CVector3<float> &vPosition );
      ////////////////////
      /// Converts window coordinates (x,y,z) into world coordinates. 
      /// \param fX x coordinate with 0,0 as lower left corner.
      /// \param fY y coordinate with 0,0 as lower left corner.
      /// \param fZ z coordinate, 0.0f as the near clipping plane, 
      ///                         1.0f as the far clipping plane.
      /// \returns CVector3<float> the window coordinates as world coordinates.
      Phoenix::Math::CVector3<float> UnProject( float fX, float fY, float fZ);
      ////////////////////
      /// Creates an imaginary trackball and rotates camera using two
      /// positions on the surface of the sphere.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      /// \param vEndPoint The point where rotation ends.
      void VirtualTrackball( const Phoenix::Math::CVector3<float> &vPosition,
			     const Phoenix::Math::CVector2<int> &vStartPoint,
			     const Phoenix::Math::CVector2<int> &vEndPoint );
      ////////////////////
      /// Creates an imaginary trackball and calculates rotation quaterion using two
      /// positions on the surface of the sphere.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      /// \param vEndPoint The point where rotation ends.
      /// \param qResult Rotation quaternion is stored here.
      /// \returns Non-zero on success, zero otherwise.
      int VirtualTrackball( const Phoenix::Math::CVector3<float> & vPosition,
			    const Phoenix::Math::CVector2<int> & vStartPoint,
			    const Phoenix::Math::CVector2<int> & vEndPoint,
			    Phoenix::Math::CQuaternion & qResult );
      ////////////////////
      /// Creates an imaginary trackball and rotates camera using two
      /// positions on the surface of the sphere. Sphere center is m_fTrackballCenterDistance away along
      /// forward vector.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      void VirtualTrackball( const Phoenix::Math::CVector2<int> &vStartPoint,
			     const Phoenix::Math::CVector2<int> &vEndPoint);
      ////////////////////
      /// Converts world coordinates into screen coordinates.
      /// \param vPosition Position in world coordinates.
      /// \returns CVector3<float> position in window coordinates.
      Phoenix::Math::CVector3<float> Project( const Phoenix::Math::CVector3<float> &vPosition);
      ////////////////////
      /// Converts world coordinates into eye coordinates.
      /// \param vPosition Position in world coordinates.
      /// \returns CVector3<float> position in eye coordinates.
      Phoenix::Math::CVector3<float> ProjectToEye( const Phoenix::Math::CVector3<float> &vPosition);
      ////////////////////
      /// Returns trackball center distance.
      /// \return point distance along forward vector.
      float GetTrackballDistance() const 
      {
	return m_fTrackballCenterDistance;
      }
      ////////////////////
      /// Sets trackball center distance.
      /// \param fDist  point distance along forward vector.
      void SetTrackballDistance( float fDist )
      {
	m_fTrackballCenterDistance = fDist;
      }
      ////////////////////
      /// Sets decal offset value. Helps rendering decals over already drawn surfaces. 
      /// Remember to call UpdateProjection afterwards.
      /// \param fDelta Offset value
      /// \param fZ Projected z-coordinate of the decal that will be rendered.
      void SetDecalOffset( float fDelta, float fZ );
      ////////////////////
      /// Resets decal offset value - practically same as SetDecalOffset( 0.0f, * ), but without calculations.
      void ResetDecalOffset();
      /////////////////////////////
      /// Returns trackball rotation point.
      /// \returns trackball rotation point.
      Phoenix::Math::CVector3<float> GetFocusPoint();
      ////////////////////
      /// Creates ray in world space eminating, from opengl screen coordinates. (lower-left is 0,0)
      /// \param fX x-coordinate.
      /// \param fY y-coordinate.
      /// \param ray Ray where values are stored.
      void CreateRay( float fX, float fY, Phoenix::Math::CRay & ray);
      ///////////////////
      /// Returns aspect ratio of viewport.
      /// \return Aspect ratio of width / height.
      float GetAspectRatio() const;
    };
  }; // namespace Graphics
}; // Phoenix
#endif
