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

#ifndef __GSE_Camera_h__
#define __GSE_Camera_h__

#include "GSE_structures.h"
#include "GSE_Orientable.h"
#include "GSE_math.h"
#include "GSE_Frustum.h"
#include "GSE_geometry.h"    

using Geometry::GSE_Cone;
using Geometry::GSE_Sphere;

class GSE_Camera : public GSE_Positional, 
		   public GSE_Object, 
		   public GSE_Orientable
{
 protected:

  // field of view angle in degrees.
  float		m_fFieldOfView;  
  // viewport for this camera, starting from 
  // lower left corner { x, y, width, height } 
  int		m_aViewport[4];
  // The distance of near clipping plane 
  float		m_fNearClipping;
  // The distance of the far clipping plane.
  float		m_fFarClipping;
  // Should this camera show lens flares on light sources.
  char		m_bLensFlaresEnabled;
  // Is the orthogonal view enabled
  char		m_bOrtho;
  // The coordinates for left, right, bottom and top clipping planes
  // in orthogonal mode
  float		m_aOrthoPlanes[4];

  // The view frustum for this camera ( Renderer will fill this out );
  GSE_Frustum	 m_Frustum;
  // The sphere surrounding the frustum
  GSE_Sphere	 m_FrustumSphere;
  // The cone surrounding the frustum
  GSE_Cone	 m_FrustumCone;
  
  // The projection matrix for this camera
  GSE_Matrix4x4f m_mProjection;
  // The inverse projection matrix for this camera
  GSE_Matrix4x4f m_mProjectionInv;
  // The view matrix for this camera
  GSE_Matrix4x4f m_mView;
  // The inverse view matrix for this camera
  GSE_Matrix4x4f m_mViewInv;
  /// Is the projection changed
  int		 m_bProjectionChanged;
  
 public:
  
  // The default constructor
  GSE_Camera();
  // The default deconstructor
  ~GSE_Camera();
  // Sets the Field of view.
  void SetFieldOfView(float fDegrees);
  // Sets the orthogonal view left, right, bottom and top plane locations.
  void SetViewOrtho( float fLeft, float fRight, float fBottom, float fTop );
  // Sets the Viewport for this camera
  void SetViewport( int iX, int iY, int iWidth, int iHeight);
  // Sets the Near Clipping plane.
  void SetNearClipping(float fNearClipping);
  // Sets the Far clipping plane
  void SetFarClipping(float fFarClipping);
  // Returns the Near clipping plane distance.
  float GetNearClipping();
  // Returns the Far clipping plane distance
  float GetFarClipping();
  // Returns the array of Viewport settings
  int * GetViewport();
  // Returns the Field Of View angle
  float GetFieldOfView();
  // Returns true if lens flares are enabled.
  char IsLensFlaresEnabled();
  // Sets lens flares enabled or disabled according the bFlag.
  void SetLensFlaresEnabled(char bFlag);
  // For debugging
  friend std::ostream &operator<<(std::ostream &stream, GSE_Camera &obj);
  // Returns the reference to frustum object
  GSE_Frustum &Frustum();
  // Calculates the bounding sphere for the frustum.
  void CalculateBoundingSphere();
  // Calculates the bounding cone for the frustum
  void CalculateBoundingCone();
  // Returns reference to the bounding cone
  GSE_Cone   &FrustumCone();
  // Returns reference to the bounding sphere
  GSE_Sphere &FrustumSphere();
  // Returns true if orthogonal mode is on
  char IsOrthogonal();
  // Returns the pointer to orthogonal plane coordinates
  float *GetOrthoPlanes();
  /// Rotates the camera around a point in space.
  void RotateAroundPoint( const GSE_Vector3 & vPoint, const GSE_Quaternion & q);
  /// Returns the projection matrix.
  inline const GSE_Matrix4x4f & GetProjectionMatrix() const
  {
    return m_mProjection;
  }
  /// Returns the view matrix.
  inline const GSE_Matrix4x4f & GetViewMatrix() const
  {
    return m_mView;
  }

  // Updates the projection matrix
  void UpdateProjection();
  // Updates the view matrix
  void UpdateView();
  inline GSE_Matrix4x4f & GetInvView() 
  {
    return m_mViewInv;
  }
  inline  GSE_Matrix4x4f & GetView() 
  {
    return m_mView;
  }
  inline  GSE_Matrix4x4f & GetInvProjection() 
  {
    return m_mProjectionInv;
  }
  inline  GSE_Matrix4x4f & GetProjection() 
  {
    return m_mProjection;
  }
  inline int IsProjectionChanged() const
  {
    return m_bProjectionChanged;
  }
  inline int IsViewChanged() const
  {
    return (IsPositionChanged() || IsRotationChanged());
  }
  inline void SetProjectionChanged(int bFlag )
  {
    m_bProjectionChanged = bFlag;
  }
  /////////////////////////////////////////////////////////////////
  /// Converts window coordinates (x,y,z) into eye coordinates. 
  /// \param fX x coordinate with 0,0 as lower left corner.
  /// \param fY y coordinate with 0,0 as lower left corner.
  /// \param fZ z coordinate, 0.0f as the near clipping plane, 
  ///                         1.0f as the far clipping plane.
  /// \returns GSE_Vector3 the window coordinates as eye coordinates.
  GSE_Vector3 WindowCoordinatesToEye( float fX, float fY, float fZ  );
  /////////////////////////////////////////////////////////////////
  /// Converts eye coordinates into world coordinates.
  /// \param vPosition The position in eye coordinates which is converted into world coordinates.
  /// \returns GSE_Vector3 the vPosition in world coordinates.
  GSE_Vector3 EyeToWorld( const GSE_Vector3 &vPosition );
  /////////////////////////////////////////////////////////////////
  /// Converts window coordinates (x,y,z) into world coordinates. 
  /// \param fX x coordinate with 0,0 as lower left corner.
  /// \param fY y coordinate with 0,0 as lower left corner.
  /// \param fZ z coordinate, 0.0f as the near clipping plane, 
  ///                         1.0f as the far clipping plane.
  /// \returns GSE_Vector3 the window coordinates as world coordinates.
  GSE_Vector3 WindowCoordinatesToWorld( float fX, float fY, float fZ);
  /////////////////////////////////////////////////////////////////
  /// Creates an imaginary trackball and rotates camera using two
  /// positions on the surface of the sphere.
  /// \param vPosition The center of the sphere.
  /// \param vStartPoint The point where rotation begins.
  /// \param vEndPoint The point where rotation ends.
  void VirtualTrackball( const GSE_Vector3 &vPosition, 
			 const GSE_Vector2 &vStartPoint,
			 const GSE_Vector2 &vEndPoint );
				   
  GSE_Vector3 WorldCoordinatesToScreen( const GSE_Vector3 &vPosition);
};
//
// The Camera Manager class
//
class GSE_CameraMgr : public GSE_Container<GSE_Camera>,
		      public GSE_Singleton<GSE_CameraMgr>,
		      public GSE_Mapper<char *,GSE_Camera>
{
  
  friend class GSE_Singleton<GSE_CameraMgr>;
 private:
   GSE_CameraMgr();
  ~GSE_CameraMgr();
  
 public:
  GSE_Camera *	CreateCamera();
  
  
};

#endif
