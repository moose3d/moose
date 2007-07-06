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
#include <iostream>
#include "GSE_Camera.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
GSE_Camera::GSE_Camera() : GSE_Orientable()
{
  SetFieldOfView(45.0);
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
GSE_Camera::~GSE_Camera()
{
  
}
/////////////////////////////////////////////////////////////////
float *
GSE_Camera::GetOrthoPlanes()
{
  return m_aOrthoPlanes;
}
/////////////////////////////////////////////////////////////////
char
GSE_Camera::IsOrthogonal()
{
  return m_bOrtho;
}
/////////////////////////////////////////////////////////////////
void
GSE_Camera::SetFieldOfView(float fDegrees)
{
  m_fFieldOfView = fDegrees;
  m_bOrtho = 0;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Camera::SetViewOrtho( float fLeft, float fRight, 
			  float fBottom, float fTop )
{
  m_aOrthoPlanes[0] = fLeft;
  m_aOrthoPlanes[1] = fRight;
  m_aOrthoPlanes[2] = fBottom;
  m_aOrthoPlanes[3] = fTop;
  m_bOrtho = 1;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::SetViewport( int iX, int iY, int iWidth, int iHeight)
{

  m_aViewport[0] = iX;
  m_aViewport[1] = iY;
  m_aViewport[2] = iWidth;
  m_aViewport[3] = iHeight;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::SetNearClipping(float fNearClipping)
{

  m_fNearClipping = fNearClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::SetFarClipping(float fFarClipping)
{

  m_fFarClipping = fFarClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
float
GSE_Camera::GetFarClipping()
{
  return m_fFarClipping;
}
/////////////////////////////////////////////////////////////////
float
GSE_Camera::GetNearClipping()
{
  return m_fNearClipping;
}
/////////////////////////////////////////////////////////////////
float
GSE_Camera::GetFieldOfView()
{
  return m_fFieldOfView;
}
/////////////////////////////////////////////////////////////////
int *
GSE_Camera::GetViewport()
{
  return m_aViewport;
}
/////////////////////////////////////////////////////////////////
char
GSE_Camera::IsLensFlaresEnabled()
{
  return m_bLensFlaresEnabled;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::SetLensFlaresEnabled(char bFlag)
{
  m_bLensFlaresEnabled = bFlag;
}
/////////////////////////////////////////////////////////////////
GSE_Frustum &
GSE_Camera::Frustum()
{
  return m_Frustum;
}
/////////////////////////////////////////////////////////////////
// As described in 
// http://www.flipcode.com/articles/article_frustumculling.shtml
//
void
GSE_Camera::CalculateBoundingSphere()
{
  float fViewLen = m_fFarClipping - m_fNearClipping;
  float fHeight = fViewLen * tan(DEG2RAD(m_fFieldOfView) * 0.5f);
  float fWidth = fHeight;

  // halfway point between near/far planes starting at the origin and 
  // extending along the z axis
  GSE_Vector3 vP(0.0f, 0.0f, m_fNearClipping + fViewLen * 0.5f);

  // the calculate far corner of the frustum
  GSE_Vector3 vQ(fWidth, fHeight, fViewLen);
  
  // the vector between P and Q
  GSE_Vector3 vDiff = vP - vQ;
  
  // the radius becomes the length of this vector
  m_FrustumSphere.SetRadius( vDiff.Length());
  
  // calculate the center of the sphere
  m_FrustumSphere.SetPosition( GetPosition() + (GetForwardVector() * (fViewLen * 0.5f + m_fNearClipping)));
  
}
/////////////////////////////////////////////////////////////////
void
GSE_Camera::CalculateBoundingCone()
{
  
  float fHalfHeight = m_aViewport[3] * 0.5f; // Half of the screen height
  float fHalfWidth = m_aViewport[2] * 0.5f;// Half of the screen width

  // calculate the length of the fov triangle
  float fDepth  = fHalfHeight / tan(DEG2RAD(m_fFieldOfView) * 0.5f);

  // calculate the corner of the screen
  float fCorner = sqrt(fHalfWidth * fHalfWidth + fHalfHeight * fHalfHeight);
  
  // now calculate the new fov
  float fFov = atan(fCorner / fDepth);
  
  // apply to the cone
  m_FrustumCone.SetDirection(GetForwardVector());
  m_FrustumCone.SetPosition(GetPosition());
  m_FrustumCone.SetAngle(fFov);

}
/////////////////////////////////////////////////////////////////
GSE_Cone &
GSE_Camera::FrustumCone()
{
  return m_FrustumCone;
}
/////////////////////////////////////////////////////////////////
GSE_Sphere &
GSE_Camera::FrustumSphere()
{
  return m_FrustumSphere;
}
/////////////////////////////////////////////////////////////////
// Handy operator override  for debug printing
std::ostream &operator<<(std::ostream &stream, GSE_Camera &obj)
{
  
  stream << "position: " << obj.GetPosition() << endl
	 << "forward: " << obj.GetForwardVector() << endl
	 << "up: "      << obj.GetUpVector() << endl
	 << "right: "   << obj.GetRightVector();
  return stream;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::RotateAroundPoint( const GSE_Vector3 & vPoint, const GSE_Quaternion & q)
{

  AppendToRotation(q);
  SetPosition( Math::RotateAroundPoint(GetPosition(), vPoint, q ));
  
}
/////////////////////////////////////////////////////////////////
GSE_CameraMgr::GSE_CameraMgr() : GSE_Container<GSE_Camera>()
{
  
}
/////////////////////////////////////////////////////////////////
GSE_CameraMgr::~GSE_CameraMgr()
{
  Core::GSE_Logger::Error() << "CameraMgr:: delete\n";
}
/////////////////////////////////////////////////////////////////
GSE_Camera *
GSE_CameraMgr::CreateCamera()
{
  GSE_Camera *pCamera = new GSE_Camera();
  Add( pCamera );
  Map( (char *)pCamera->GetName(), pCamera);
  
  return pCamera;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::UpdateProjection() 
{
  float fH = tanf( DEG2RAD( GetFieldOfView() * 0.5f )) * GetNearClipping(); 
  float fW = fH * (float)m_aViewport[2]/(float)m_aViewport[3];
  float fFar = m_fFarClipping;
  float fNear = m_fNearClipping;
  //             l   r   b    t   n      f
  // glFrustum( -fW, fW, -fH, fH, fNear, fFar );
  m_mProjectionInv = m_mProjection = GSE_Matrix4x4f::Zero();
  
  if ( IsOrthogonal() )
  {
    m_mProjection(0,0) = 2.0f / (fW + fW );
    m_mProjection(0,3) = 0.0f;
    m_mProjection(1,1) = 2.0f / (fH + fH );
    m_mProjection(1,3) = 0.0f;
    m_mProjection(2,2) = -2.0f / (fFar - fNear);
    m_mProjection(2,3) = (fFar + fNear) / (fFar - fNear );
    m_mProjection(3,3) = 1.0f;
    
    m_mProjectionInv(0,0) = (fW + fW) / 2.0f;
    m_mProjectionInv(0,3) = 0.0f;
    m_mProjectionInv(1,1) = (fH + fH) / 2.0f;
    m_mProjectionInv(1,3) = 0.0f;
    m_mProjectionInv(2,2) = (fFar - fNear ) / -2.0f;
    m_mProjectionInv(2,3) = (fNear + fFar) / 2.0f;
    m_mProjectionInv(3,3) = 1.0f;
    
  }
  else 
  {
    m_mProjection(0,0) = (2.0f * fNear) / (fW + fW); 
    m_mProjection(0,2) = 0.0f;
    m_mProjection(1,1) = (2.0f * fNear ) / (fH + fH );
    m_mProjection(1,2) = 0.0f; // (t+b) / (t-b) 
    m_mProjection(2,2) = -(fFar + fNear ) / ( fFar - fNear );
    m_mProjection(2,3) = (-2.0f * fFar * fNear) / (fFar - fNear );
    m_mProjection(3,2) = -1.0f;
    m_mProjection(3,3) = 0.0;
     
    m_mProjectionInv(0,0) = (fW + fW) / (2.0 * fNear);
    m_mProjectionInv(0,3) = 0.0f; //((r+l) / 2n)
    m_mProjectionInv(1,1) = (fH + fH ) / (2.0 * fNear);
    m_mProjectionInv(1,3) = 0.0f; // t + b / 2n
    m_mProjectionInv(2,3) = -1.0f;
    m_mProjectionInv(3,2) = (-(fFar - fNear))  / (2.0f * fFar * fNear );
    m_mProjectionInv(3,3) = (fFar + fNear)  / (2.0f * fFar * fNear );
  }
  m_bProjectionChanged = 0;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::UpdateView()
{
  // Transform camera reversely so we get proper effect.
  GSE_Matrix4x4f mT = Math::TranslationMatrix( GetPosition());
  GSE_Matrix4x4f mR = GetRotationQuaternion().ToMatrix();
  m_mViewInv = mT * mR;

  mT(0,3) = -mT(0,3);
  mT(1,3) = -mT(1,3);
  mT(2,3) = -mT(2,3);
  mR.Transpose();
  m_mView    = mR * mT;
  
  //cerr << "view " << m_mView << endl;
  //cerr << "view^-1 " << m_mViewInv << endl;
  //cerr << "view * view^-1 " << m_mView * m_mViewInv;
  SetRotationChanged(0);
  SetPositionChanged(0);
}
/////////////////////////////////////////////////////////////////
// 0,0 lower left corner
GSE_Vector3 
GSE_Camera::WindowCoordinatesToEye( float fX, float fY, float fZ )
{

  // Convert mouse coordinates to opengl window coordinates 
  // ( as it would fill entire screen )
  float fOglX = fX - (float)m_aViewport[0];
  float fOglY = fY - (float)m_aViewport[1];
  
  float fCenterX = (m_aViewport[2] * 0.5f);
  float fCenterY = (m_aViewport[3] * 0.5f); 
  
  float fAspect =  (float)m_aViewport[2]/(float)m_aViewport[3];

  float fNormX = (fOglX - fCenterX) / fCenterX;
  float fNormY = (fOglY - fCenterY) / fCenterY;
  float fZInEye = m_fNearClipping + (m_fFarClipping - m_fNearClipping)*fZ;
  float fH = tanf( DEG2RAD( GetFieldOfView() * 0.5f )) * fZInEye; 
  
  return GSE_Vector3(fH * fAspect * fNormX, fH * fNormY, -fZInEye);
  
}
/////////////////////////////////////////////////////////////////
GSE_Vector3 
GSE_Camera::EyeToWorld( const GSE_Vector3 &vPosition )
{
  GSE_Vector<float,4> vTmp;
  vTmp[0] = vPosition.m_pValues[0];
  vTmp[1] = vPosition.m_pValues[1];
  vTmp[2] = vPosition.m_pValues[2];
  vTmp[3] = 1.0f;
  
  vTmp = m_mViewInv * vTmp;

  if ( !TOO_CLOSE_TO_ZERO(vTmp[3]) )
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }
  return GSE_Vector3(vTmp[0],vTmp[1],vTmp[2]);

}
/////////////////////////////////////////////////////////////////
GSE_Vector3 
GSE_Camera::WindowCoordinatesToWorld( float fX, float fY, float fZ)
{
  // Convert mouse coordinates to opengl window coordinates 
  // ( as it would fill entire screen )
  float fOglX = fX - (float)m_aViewport[0];
  float fOglY = fY - (float)m_aViewport[1];
  
  float fCenterX = (m_aViewport[2] * 0.5f);
  float fCenterY = (m_aViewport[3] * 0.5f); 
  
  float fAspect =  (float)m_aViewport[2]/(float)m_aViewport[3];

  float fNormX = (fOglX - fCenterX) / fCenterX;
  float fNormY = (fOglY - fCenterY) / fCenterY;
  float fZInEye = m_fNearClipping + (m_fFarClipping - m_fNearClipping)*fZ;
  float fH = tanf( DEG2RAD( GetFieldOfView() * 0.5f )) * fZInEye; 

  GSE_Vector<float,4> vTmp;
  vTmp[0] = fH * fAspect * fNormX;
  vTmp[1] = fH * fNormY;
  vTmp[2] = -fZInEye;
  vTmp[3] = 1.0f;
  
  vTmp = m_mViewInv * vTmp;

  if ( !TOO_CLOSE_TO_ZERO(vTmp[3]) )
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }
  return GSE_Vector3(vTmp[0],vTmp[1],vTmp[2]);
}
/////////////////////////////////////////////////////////////////
// The factor which gives seemingly good results on the trackball.
#define TRACKBALL_FUDGE_FACTOR 0.5f
/////////////////////////////////////////////////////////////////
void 
GSE_Camera::VirtualTrackball( const GSE_Vector3 &vPosition, 
			      const GSE_Vector2 &vStartPoint,
			      const GSE_Vector2 &vEndPoint )
{
  GSE_Vector3 vOrig = WindowCoordinatesToWorld( vStartPoint.m_pValues[GSE_Vector2::X], 
						vStartPoint.m_pValues[GSE_Vector2::Y],
						0.0f);
  GSE_Vector3 vEnd = WindowCoordinatesToWorld( vStartPoint.m_pValues[GSE_Vector2::X], 
					       vStartPoint.m_pValues[GSE_Vector2::Y],
					       1.0f);
  /////////////////////////////////////////////////////////////////
  GSE_Vector3 vIntersection0;
  GSE_Vector3 vIntersection1;
  GSE_Sphere  sphere(vPosition,((GetPosition()-vPosition).Length()-GetNearClipping())*TRACKBALL_FUDGE_FACTOR);
  /////////////////////////////////////////////////////////////////
  if ( Geometry::RayIntersectsSphere( vOrig, vEnd, &vIntersection0, NULL, sphere) >= 1)
  {
    vOrig = WindowCoordinatesToWorld( vEndPoint.m_pValues[GSE_Vector2::X], 
				      vEndPoint.m_pValues[GSE_Vector2::Y],
				      0.0f);
    vEnd = WindowCoordinatesToWorld( vEndPoint.m_pValues[GSE_Vector2::X], 
				     vEndPoint.m_pValues[GSE_Vector2::Y],
				     1.0f);
    /////////////////////////////////////////////////////////////////
    if ( Geometry::RayIntersectsSphere( vOrig, vEnd, &vIntersection1, NULL, sphere) >= 1)
    {
      vOrig = vIntersection0 - sphere.GetPosition();
      vEnd  = vIntersection1 - sphere.GetPosition();

      //cerr << "start : " << vIntersection0 << endl;
      //cerr << "end : "   << vIntersection1 << endl;
      vOrig.ToUnit();
      vEnd.ToUnit();
      GSE_Quaternion qRotation = GSE_Quaternion::RotationArc(vOrig,vEnd).Reverse();
      RotateAroundPoint( sphere.GetPosition(), qRotation  );
    }
  }

}
/////////////////////////////////////////////////////////////////
#undef TRACKBALL_FUDGE_FACTOR
/////////////////////////////////////////////////////////////////
GSE_Vector3
GSE_Camera::WorldCoordinatesToScreen( const GSE_Vector3 &vPosition)
{

  
  GSE_Vector<float,4> vTmp;

  vTmp[0] = vPosition.m_pValues[0];
  vTmp[1] = vPosition.m_pValues[1];
  vTmp[2] = vPosition.m_pValues[2];
  vTmp[3] = 1.0f;

  vTmp = GetViewMatrix() * vTmp;
  vTmp = GetProjectionMatrix() * vTmp;
    
  if ( !TOO_CLOSE_TO_ZERO(vTmp[3]) ) 
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }

  vTmp[0] = vTmp[0] * 0.5f + 0.5f;
  vTmp[1] = vTmp[1] * 0.5f + 0.5f;
  vTmp[2] = vTmp[2] * 0.5f + 0.5f;
  
  return GSE_Vector3(m_aViewport[0] + vTmp[0] * m_aViewport[2],
		     m_aViewport[1] + vTmp[1] * m_aViewport[3],
		     vTmp[2]);
  
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_Camera.cpp,v $
// Revision 1.21  2007/05/09 11:51:55  entity
// removed some comments
//
// Revision 1.20  2007/05/09 11:51:25  entity
// improved WindowCoordinatesToWorld, WorldCoordinatesToScreen and EyeToWorld methods
//
// Revision 1.19  2007/03/27 05:58:42  entity
// no more magic number at VirtualTrackball
//
// Revision 1.18  2007/03/26 18:21:28  entity
// comments added
//
// Revision 1.17  2007/03/26 16:41:27  entity
// UpdateView() enhanced and fixed WindowCoordinatesTo* z-coordinate mapping
//
// Revision 1.16  2007/03/26 13:01:30  entity
// const & stuff + VirtualTrackball
//
// Revision 1.15  2007/03/19 15:41:06  entity
// better view inverse calculation
//
// Revision 1.14  2007/03/16 14:43:34  entity
// WindowCoordinatesToWorld added
//
// Revision 1.13  2007/03/16 12:41:06  entity
// checks for inverses done
//
// Revision 1.12  2007/03/16 11:44:27  entity
// WindowCoordinatesToEye now with z-coordinate
//
// Revision 1.11  2007/03/16 11:32:27  entity
// EyeToWorld() added
//
// Revision 1.10  2007/03/16 11:25:55  entity
// WindowCoordinatesToEye added
//
/////////////////////////////////////////////////////////////////
