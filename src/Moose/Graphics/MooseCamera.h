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
#ifndef __MooseCamera_h__
#define __MooseCamera_h__
/////////////////////////////////////////////////////////////////
#include "MooseVector2.h"
#include "MooseQuaternion.h"
#include "MooseMatrix4x4.h"
#include "MoosePositional.h"
#include "MooseOrientable.h"
#include "MooseCone.h"
#include "MooseSphere.h"
#include "MooseFrustum.h"
#include "MooseRay.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    struct MOOSE_API Viewport_t 
    {
      int x;
      int y;
      int width;
      int height;
#if ! defined(SWIG)
      bool operator==( const Moose::Graphics::Viewport_t & o ) const
      {
        return (x == o.x) && (y == o.y) &&
        (width == o.width) && (height == o.height);
      }
      friend std::ostream & operator<<( std::ostream & os, const Moose::Graphics::Viewport_t & o )
      {
        os << o.x << ", " << o.y << ", " << o.width << ", " << o.height;
        return os;
      }
#endif
    };
    /////////////////////////////////////////////////////////////////
    /// Camera class.
    class MOOSE_API CCamera : public Moose::Spatial::CPositional, public Moose::Spatial::COrientable
    {
    protected:
      /// field of view angle in degrees.
      float		m_fFieldOfView;  
      /// viewport for this camera, starting from left corner { x, y, width, height } 
      Moose::Graphics::Viewport_t		m_aViewport;
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
      Moose::Math::CMatrix4x4<float> m_mProjection;
      /// The inverse projection matrix for this camera
      Moose::Math::CMatrix4x4<float> m_mProjectionInv;
      /// The view matrix for this camera
      Moose::Math::CMatrix4x4<float> m_mView;
      /// The inverse view matrix for this camera
      Moose::Math::CMatrix4x4<float> m_mViewInv;
      /// Is the projection changed
      int		 m_bProjectionChanged;
      /// Bounding sphere for frustum.
      Moose::Volume::CSphere m_FrustumSphere;
      /// Bounding sphere for cone.
      Moose::Volume::CCone m_FrustumCone;
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
      /// Sets viewport for this camera.
      /// \param rViewport Viewport struct.
      void SetViewport( const Moose::Graphics::Viewport_t & rViewport);
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
      /// Returns current viewport settings.
      /// \returns Pointer to int array
      Moose::Graphics::Viewport_t & GetViewport();
      ////////////////////
      /// Returns the array of viewport settings.
      /// \returns Pointer to int array
      const Moose::Graphics::Viewport_t & GetViewport() const;
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
#ifndef SWIG
      ////////////////////
      /// Prints out info about camera into stream.
      /// \param stream output stream
      /// \param obj Camera.
      friend std::ostream &operator<<(std::ostream &stream, const CCamera &obj);
#endif
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
      Moose::Volume::CCone   &FrustumCone();
      ////////////////////
      /// Returns reference to the bounding sphere.
      /// \returns Reference to sphere.
      Moose::Volume::CSphere &FrustumSphere();
      ////////////////////
      /// Returns reference to the bounding cone
      /// \returns Reference to cone.
      const Moose::Volume::CCone   &FrustumCone() const;
      ////////////////////
      /// Returns reference to the bounding sphere.
      /// \returns Reference to sphere.
      const Moose::Volume::CSphere &FrustumSphere() const;
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
      void RotateAroundPoint( const Moose::Math::CVector3<float> & vPoint, const Moose::Math::CQuaternion & q);
      ////////////////////
      /// Returns projection matrix.
      /// \returns Reference to projection matrix.
      inline const Moose::Math::CMatrix4x4<float> & GetProjectionMatrix() const
      {
	return m_mProjection;
      }
      ////////////////////
      /// Returns view matrix.
      /// \returns Refernce to view matrix.
      inline const Moose::Math::CMatrix4x4<float> & GetViewMatrix() const
      {
	return m_mView;
      }
      ////////////////////
      /// Returns projection matrix.
      /// \returns Reference to projection matrix.
      inline Moose::Math::CMatrix4x4<float> & GetProjectionMatrix()
      {
	return m_mProjection;
      }
      ////////////////////
      /// Returns view matrix.
      /// \returns Refernce to view matrix.
      inline Moose::Math::CMatrix4x4<float> & GetViewMatrix()
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
      inline Moose::Math::CMatrix4x4<float> & GetInvView() 
      {
	return m_mViewInv;
      }
      ////////////////////
      /// Returns inverse projection matrix.
      /// \returns Reference to inverse projection matrix.
      inline  Moose::Math::CMatrix4x4<float> & GetInvProjection() 
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
      inline void Move ( const Moose::Math::CVector3<float> & vVector )
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
      Moose::Math::CVector3<float> UnProjectToEye( float fX, float fY, float fZ  );
      ////////////////////
      /// Converts eye coordinates into world coordinates.
      /// \param vPosition The position in eye coordinates which is converted into world coordinates.
      /// \returns CVector3<float> the vPosition in world coordinates.
      Moose::Math::CVector3<float> EyeToWorld( const Moose::Math::CVector3<float> &vPosition );
      ////////////////////
      /// Converts window coordinates (x,y,z) into world coordinates. 
      /// \param fX x coordinate with 0,0 as lower left corner.
      /// \param fY y coordinate with 0,0 as lower left corner.
      /// \param fZ z coordinate, 0.0f as the near clipping plane, 
      ///                         1.0f as the far clipping plane.
      /// \returns CVector3<float> the window coordinates as world coordinates.
      Moose::Math::CVector3<float> UnProject( float fX, float fY, float fZ);
      ////////////////////
      /// Gets rotation of sphere using trackball system. If rays projected from
      /// mouse coordinates do not intersect with sphere, rotation does not occur.
      ///�\param sphere Sphere used for rotation (in world coordinates).
      /// \param vStartPoint The point where rotation begins in screen coordinates.
      /// \param vEndPoint The point where rotation ends in screen coordinates.
      /// \param qResult Rotation quaternion is stored here.
      /// \returns Non-zero on success, zero otherwise.
      int VirtualTrackball( const Moose::Volume::CSphere & sphere,
                             const Moose::Math::CVector2<int> &vStartPoint,
                             const Moose::Math::CVector2<int> &vEndPoint,
                             Moose::Math::CQuaternion & qResult  );

      ////////////////////
      /// Creates an imaginary trackball and rotates camera using two
      /// positions on the surface of the sphere.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      /// \param vEndPoint The point where rotation ends.
      void VirtualTrackball( const Moose::Math::CVector3<float> &vPosition,
                             const Moose::Math::CVector2<int> &vStartPoint,
                             const Moose::Math::CVector2<int> &vEndPoint);
      ////////////////////
      /// Creates an imaginary trackball and calculates rotation quaterion using two
      /// positions on the surface of the sphere.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      /// \param vEndPoint The point where rotation ends.
      /// \param qResult Rotation quaternion is stored here.
      /// \returns Non-zero on success, zero otherwise.
      int VirtualTrackball( const Moose::Math::CVector3<float> & vPosition,
			    const Moose::Math::CVector2<int> & vStartPoint,
			    const Moose::Math::CVector2<int> & vEndPoint,
			    Moose::Math::CQuaternion & qResult );
      ////////////////////
      /// Creates an imaginary trackball and rotates camera using two
      /// positions on the surface of the sphere. Sphere center is m_fTrackballCenterDistance away along
      /// forward vector.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      void VirtualTrackball( const Moose::Math::CVector2<int> &vStartPoint,
			     const Moose::Math::CVector2<int> &vEndPoint);
      ////////////////////
      /// Converts world coordinates into screen coordinates.
      /// \param vPosition Position in world coordinates.
      /// \returns CVector3<float> position in window coordinates.
      Moose::Math::CVector3<float> Project( const Moose::Math::CVector3<float> &vPosition);
      ////////////////////
      /// Converts world coordinates into eye coordinates.
      /// \param vPosition Position in world coordinates.
      /// \returns CVector3<float> position in eye coordinates.
      Moose::Math::CVector3<float> ProjectToEye( const Moose::Math::CVector3<float> &vPosition);
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
      Moose::Math::CVector3<float> GetFocusPoint();
      ////////////////////
      /// Creates ray in world space eminating, from opengl screen coordinates. (lower-left is 0,0)
      /// \param fX x-coordinate.
      /// \param fY y-coordinate.
      /// \param ray Ray where values are stored.
      void CreateRay( float fX, float fY, Moose::Math::CRay & ray);
      ///////////////////
      /// Returns aspect ratio of viewport.
      /// \return Aspect ratio of width / height.
      float GetAspectRatio() const;
    };
  }; // namespace Graphics
}; // Moose
#endif
