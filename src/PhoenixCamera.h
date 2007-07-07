/////////////////////////////////////////////////////////////////
#ifndef __PhoenixCamera_h__
#define __PhoenixCamera_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMatrix4x4.h"
using namespace Phoenix::Spatial;
namespace Phoenix
{
  namespace Graphics
  {
    class CCamera : public CPositional, public CObject, public COrientable
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
      char		m_bLensFlaresEnabled;
      /// Is the orthogonal view enabled
      char		m_bOrtho;
      /// The coordinates for left, right, bottom and top clipping planes
      /// in orthogonal mode
      float		m_aOrthoPlanes[4];

      /// The view frustum for this camera ( Renderer will fill this out );
      CFrustum	 m_Frustum;
      /// The sphere surrounding the frustum
      CSphere	 m_FrustumSphere;
      /// The cone surrounding the frustum
      CCone	 m_FrustumCone;
  
      /// The projection matrix for this camera
      CMatrix4x4<float> m_mProjection;
      /// The inverse projection matrix for this camera
      CMatrix4x4<float> m_mProjectionInv;
      /// The view matrix for this camera
      CMatrix4x4<float> m_mView;
      /// The inverse view matrix for this camera
      CMatrix4x4<float> m_mViewInv;
      /// Is the projection changed
      int		 m_bProjectionChanged;
  
    public:
  
      // The default constructor
      CCamera();
      // The default deconstructor
      ~CCamera();
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
      friend std::ostream &operator<<(std::ostream &stream, CCamera &obj);
      // Returns the reference to frustum object
      CFrustum &Frustum();
      // Calculates the bounding sphere for the frustum.
      void CalculateBoundingSphere();
      // Calculates the bounding cone for the frustum
      void CalculateBoundingCone();
      // Returns reference to the bounding cone
      CCone   &FrustumCone();
      // Returns reference to the bounding sphere
      CSphere &FrustumSphere();
      // Returns true if orthogonal mode is on
      char IsOrthogonal();
      // Returns the pointer to orthogonal plane coordinates
      float *GetOrthoPlanes();
      /// Rotates the camera around a point in space.
      void RotateAroundPoint( const CVector3<float> & vPoint, const CQuaternion & q);
      /// Returns the projection matrix.
      inline const CMatrix4x4<float> & GetProjectionMatrix() const
      {
	return m_mProjection;
      }
      /// Returns the view matrix.
      inline const CMatrix4x4<float> & GetViewMatrix() const
      {
	return m_mView;
      }

      // Updates the projection matrix
      void UpdateProjection();
      // Updates the view matrix
      void UpdateView();
      inline CMatrix4x4<float> & GetInvView() 
      {
	return m_mViewInv;
      }
      inline  CMatrix4x4<float> & GetView() 
      {
	return m_mView;
      }
      inline  CMatrix4x4<float> & GetInvProjection() 
      {
	return m_mProjectionInv;
      }
      inline  CMatrix4x4<float> & GetProjection() 
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
      /// \returns CVector3<float> the window coordinates as eye coordinates.
      CVector3<float> WindowCoordinatesToEye( float fX, float fY, float fZ  );
      /////////////////////////////////////////////////////////////////
      /// Converts eye coordinates into world coordinates.
      /// \param vPosition The position in eye coordinates which is converted into world coordinates.
      /// \returns CVector3<float> the vPosition in world coordinates.
      CVector3<float> EyeToWorld( const CVector3<float> &vPosition );
      /////////////////////////////////////////////////////////////////
      /// Converts window coordinates (x,y,z) into world coordinates. 
      /// \param fX x coordinate with 0,0 as lower left corner.
      /// \param fY y coordinate with 0,0 as lower left corner.
      /// \param fZ z coordinate, 0.0f as the near clipping plane, 
      ///                         1.0f as the far clipping plane.
      /// \returns CVector3<float> the window coordinates as world coordinates.
      CVector3<float> WindowCoordinatesToWorld( float fX, float fY, float fZ);
      /////////////////////////////////////////////////////////////////
      /// Creates an imaginary trackball and rotates camera using two
      /// positions on the surface of the sphere.
      /// \param vPosition The center of the sphere.
      /// \param vStartPoint The point where rotation begins.
      /// \param vEndPoint The point where rotation ends.
      void VirtualTrackball( const CVector3<float> &vPosition, 
			     const CVector2<float> &vStartPoint,
			     const CVector2<float> &vEndPoint );
				   
      CVector3<float> WorldCoordinatesToScreen( const CVector3<float> &vPosition);
    };
  }; // namespace Graphics
}; // Phoenix
#endif
