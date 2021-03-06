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
#include <iostream>
#include "MooseMath.h"
#include "MooseCamera.h"
#include "MooseCollision.h"
#include <cassert>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Collision;
using namespace Moose::Volume;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CCamera::CCamera() : COrientable()
{
    SetViewport(0,0,640,480); // a rather safe bet
  SetFieldOfView(45.0);
  SetProjectionChanged(1);
  m_aOrthoPlanes[0] = m_aOrthoPlanes[1] = m_aOrthoPlanes[2] = m_aOrthoPlanes[3] = 0.0f;
  SetTrackballDistance( 3.24f);
  m_fDecalOffset = 1.0f;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CCamera::~CCamera()
{
  
}
/////////////////////////////////////////////////////////////////
float *
Moose::Graphics::CCamera::GetOrthoPlanes()
{
  return m_aOrthoPlanes;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CCamera::IsOrthogonal()
{
  return m_bOrtho;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CCamera::SetFieldOfView(float fDegrees)
{
  m_fFieldOfView = fDegrees;
  m_bOrtho = 0;
  m_aOrthoPlanes[0] = m_aOrthoPlanes[1] = m_aOrthoPlanes[2] = m_aOrthoPlanes[3] = 0.0f;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CCamera::SetViewOrtho( float fLeft, float fRight, 
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
Moose::Graphics::CCamera::SetViewport( int iX, int iY, int iWidth, int iHeight)
{
  m_aViewport.x = iX;
  m_aViewport.y = iY;
  m_aViewport.width = iWidth;
  m_aViewport.height = iHeight;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::SetViewport( const Moose::Graphics::Viewport_t & rViewport )
{
  m_aViewport = rViewport;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::SetNearClipping(float fNearClipping)
{

  m_fNearClipping = fNearClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::SetFarClipping(float fFarClipping)
{

  m_fFarClipping = fFarClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
float
Moose::Graphics::CCamera::GetFarClipping()
{
  return m_fFarClipping;
}
/////////////////////////////////////////////////////////////////
float
Moose::Graphics::CCamera::GetNearClipping()
{
  return m_fNearClipping;
}
/////////////////////////////////////////////////////////////////
float
Moose::Graphics::CCamera::GetFieldOfView() const
{
  return m_fFieldOfView;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::Viewport_t &
Moose::Graphics::CCamera::GetViewport()
{
  return m_aViewport;
}
/////////////////////////////////////////////////////////////////
const Moose::Graphics::Viewport_t &
Moose::Graphics::CCamera::GetViewport() const
{
  return m_aViewport;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CCamera::IsLensFlaresEnabled()
{
  return m_bLensFlaresEnabled;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::SetLensFlaresEnabled(int bFlag)
{
  m_bLensFlaresEnabled = bFlag;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFrustum &
Moose::Graphics::CCamera::Frustum()
{
  return m_Frustum;
}
const Moose::Graphics::CFrustum &
Moose::Graphics::CCamera::Frustum() const
{
  return m_Frustum;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// As described in 
// http://www.flipcode.com/articles/article_frustumculling.shtml
//
void
Moose::Graphics::CCamera::CalculateBoundingSphere()
{
  float fViewLen = GetFarClipping() - GetNearClipping();
  float fHeight = fViewLen * tan(Deg2Rad(GetFieldOfView()) * 0.5f);
  float fWidth = fHeight;
  
  // halfway point between near/far planes starting at the origin and 
  // extending along the z axis
  CVector3<float> vP(0.0f, 0.0f, m_fNearClipping + fViewLen * 0.5f);

  // the calculate far corner of the frustum
  CVector3<float> vQ(fWidth, fHeight, fViewLen);
  
  // the vector between P and Q
  CVector3<float> vDiff = vP - vQ;
  
  // the radius becomes the length of this vector
  m_FrustumSphere.SetRadius( vDiff.Length());
  
  // calculate the center of the sphere
  m_FrustumSphere.SetPosition( GetPosition() + (GetForwardVector() * (fViewLen * 0.5f + m_fNearClipping)));
  
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CCamera::CalculateBoundingCone()
{
  
  float fHalfHeight = m_aViewport.height * 0.5f; // Half of the screen height
  float fHalfWidth = m_aViewport.width * 0.5f;// Half of the screen width

  // calculate the length of the fov triangle
  float fDepth  = fHalfHeight / tan(Deg2Rad(GetFieldOfView()) * 0.5f);

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
CCone &
Moose::Graphics::CCamera::FrustumCone()
{
  return m_FrustumCone;
}
/////////////////////////////////////////////////////////////////
CSphere &
Moose::Graphics::CCamera::FrustumSphere()
{
  return m_FrustumSphere;
}
/////////////////////////////////////////////////////////////////
const CCone &
Moose::Graphics::CCamera::FrustumCone() const
{
  return m_FrustumCone;
}
/////////////////////////////////////////////////////////////////
const CSphere &
Moose::Graphics::CCamera::FrustumSphere() const
{
  return m_FrustumSphere;
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
// Handy operator override  for debug printing
std::ostream &
operator<<(std::ostream &stream, const CCamera &obj)
{
  stream << "position: " << obj.GetPosition() << endl
	 << "forward: " << obj.GetForwardVector() << endl
	 << "up: "      << obj.GetUpVector() << endl
	 << "right: "   << obj.GetRightVector();
  return stream;
}
#endif
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::RotateAroundPoint( const CVector3<float> & vPoint, const CQuaternion & q)
{

  AppendToRotation(q);
  Moose::Math::RotateAroundPoint(GetPosition(), vPoint, q, const_cast<CVector3<float> & >(GetPosition()) );
  //SetPosition(  );
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::UpdateProjection() 
{
  // OpenGL projection matrix is calculated using vertical field of view.
  float fAspect = (float)m_aViewport.height/(float)m_aViewport.width;
  
  ////////////////////
  /// Old code; just for reminder
  //float fE = 1.0f / tanf(Deg2Rad(GetFieldOfView()) * 0.5f);
  //float fFieldOfViewVertical = atanf(fAspect/fE);
  //float fH = tanf( Deg2Rad( (GetFieldOfView()) * 0.5f )) * GetNearClipping(); 
  //float fH = tanf( fFieldOfViewVertical) * GetNearClipping(); 
  //float fH = (fAspect/fE) * GetNearClipping(); 
  ////////////////////
  float fH = (fAspect*tan(Deg2Rad(GetFieldOfView()) * 0.5f)) * GetNearClipping(); 

  float fW = fH * (float)m_aViewport.width/(float)m_aViewport.height;
  float fFar = m_fFarClipping;
  float fNear = m_fNearClipping;

  //             l   r   b    t   n      f
  // glFrustum( -fW, fW, -fH, fH, fNear, fFar );
  m_mProjection.ZeroMatrix();
  m_mProjectionInv = m_mProjection;
  
  if ( IsOrthogonal() )
  {
    float *pOrthoPlanes = GetOrthoPlanes();
    float fLeft = pOrthoPlanes[0];
    float fRight = pOrthoPlanes[1];
    float fBottom = pOrthoPlanes[2];
    float fTop = pOrthoPlanes[3];
    float fRightMinusLeft = fRight-fLeft;
    float fTopMinusBottom = fTop - fBottom;
    float fFarMinusNear = fFar - fNear;
    
    m_mProjection(0,0) = 2.0f / fRightMinusLeft;
    m_mProjection(0,3) = -( (fRight + fLeft) / fRightMinusLeft);
    m_mProjection(1,1) = 2.0f / fTopMinusBottom;
    m_mProjection(1,3) = -((fTop + fBottom) / fTopMinusBottom);
    m_mProjection(2,2) = -2.0f / fFarMinusNear;
    m_mProjection(2,3) = -((fFar + fNear) / fFarMinusNear) ;
    m_mProjection(3,3) = 1.0f;
    
    m_mProjectionInv(0,0) = fRightMinusLeft * 0.5f;
    m_mProjectionInv(0,3) = (fRight + fLeft) * 0.5f;
    m_mProjectionInv(1,1) = fTopMinusBottom * 0.5f;
    m_mProjectionInv(1,3) = (fTop + fBottom)  * 0.5f;
    m_mProjectionInv(2,2) = (fFarMinusNear ) * -0.5f;
    m_mProjectionInv(2,3) = (fNear + fFar) * 0.5f;
    m_mProjectionInv(3,3) = 1.0f;
    
  }
  else 
  {
    m_mProjection(0,0) = (2.0f * fNear) / (fW + fW); 
    m_mProjection(0,2) = 0.0f;
    m_mProjection(1,1) = (2.0f * fNear ) / (fH + fH );
    m_mProjection(1,2) = 0.0f; // (t+b) / (t-b) 
    m_mProjection(2,2) = -((fFar + fNear ) / ( fFar - fNear ));
    m_mProjection(2,3) = (-2.0f * fFar * fNear) / (fFar - fNear );
    m_mProjection(3,2) = -1.0f;
    m_mProjection(3,3) = 0.0f;
     
    m_mProjectionInv(0,0) = (fW + fW) / (2.0f * fNear);
    m_mProjectionInv(0,3) = 0.0f; //((r+l) / 2n)
    m_mProjectionInv(1,1) = (fH + fH ) / (2.0f * fNear);
    m_mProjectionInv(1,3) = 0.0f; // t + b / 2n
    m_mProjectionInv(2,3) = -1.0f;
    m_mProjectionInv(3,2) = (-(fFar - fNear))  / (2.0f * fFar * fNear );
    m_mProjectionInv(3,3) = (fFar + fNear)  / (2.0f * fFar * fNear );
  }
  /// \warn This is a bit bad - inverse projection matrix won't be affected.
  m_mProjection(2,2) *= m_fDecalOffset;
  m_bProjectionChanged = 0;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::UpdateView()
{
  // Transform camera reversely so we get proper effect.
  CMatrix4x4<float> mT;
  TranslationMatrix( GetPosition(), mT);
  CMatrix4x4<float> mR;
  QuaternionToMatrix( GetRotationQuaternion(), mR );
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
CVector3<float> 
Moose::Graphics::CCamera::UnProjectToEye( float fX, float fY, float fZ )
{

  // Convert mouse coordinates to opengl window coordinates 
  // ( as it would fill entire screen - there can be _two_ or _more_ viewports on one screen. )
  float fOglX = fX - (float)m_aViewport.x;
  float fOglY = fY - (float)m_aViewport.y;
  
  float fCenterX = (m_aViewport.width * 0.5f);
  float fCenterY = (m_aViewport.height * 0.5f); 
  
  float fAspect =  (float)m_aViewport.height/(float)m_aViewport.width;

  float fNormX = (fOglX - fCenterX) / fCenterX;
  float fNormY = (fOglY - fCenterY) / fCenterY;
  float fZInEye = m_fNearClipping + (m_fFarClipping - m_fNearClipping)*fZ;
  float fH;
  if ( IsOrthogonal())
  {
    return CVector3<float>(fNormX * (GetOrthoPlanes()[1] - GetOrthoPlanes()[0])*0.5f,
                           fNormY * (GetOrthoPlanes()[3] - GetOrthoPlanes()[2])*0.5f,
                           -fZInEye);
  }   
  else
  {
    fH = tan( Deg2Rad( GetFieldOfView() * 0.5f )) * fZInEye; 
    return CVector3<float>(fH * fNormX,fH * fAspect * fNormY, -fZInEye);
  }

  
}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Moose::Graphics::CCamera::EyeToWorld( const CVector3<float> &vPosition )
{
  CVector4<float> vTmp;
  vTmp[0] = vPosition[0];
  vTmp[1] = vPosition[1];
  vTmp[2] = vPosition[2];
  vTmp[3] = 1.0f;
  
  vTmp = m_mViewInv * vTmp;

  if ( !TOO_CLOSE_TO_ZERO(vTmp[3]) )
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }
  return CVector3<float>(vTmp[0],vTmp[1],vTmp[2]);

}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Moose::Graphics::CCamera::UnProject( float fX, float fY, float fZ)
{

  // Convert mouse coordinates to opengl window coordinates 
  // ( as it would fill entire screen - there can be _two_ or _more_ viewports on one screen. )
  float fOglX = fX - (float)m_aViewport.x;
  float fOglY = fY - (float)m_aViewport.y;
  
  float fCenterX = (m_aViewport.width * 0.5f);
  float fCenterY = (m_aViewport.height * 0.5f); 
  // We use FovY, so aspect is not width / height, but height / width.
  float fAspect =  (float)m_aViewport.height/(float)m_aViewport.width;
  
  float fNormX = (fOglX - fCenterX) / fCenterX;
  float fNormY = (fOglY - fCenterY) / fCenterY;

  float fZInEye = m_fNearClipping + (m_fFarClipping - m_fNearClipping)*fZ;
  /////////////////////////////////////////////////////////////////
  // the distance from center to edge of frustum.
  /////////////////////////////////////////////////////////////////
  //
  //  \    |    /
  //   \   |_h_/    Calculation in perspective mode.
  //    \ z|  /      
  //     \ |f/
  //      \|/
  /////////////////////////////////////////////////////////////////
  //   _______
  //  |   |   |
  //  |_h_|   | Ortho mode is independent of z.
  //  |___|___|
  //
  /////////////////////////////////////////////////////////////////

  CVector4<float> vTmp;
  if ( IsOrthogonal())
  {

      vTmp[0] = fNormX * (GetOrthoPlanes()[1] - GetOrthoPlanes()[0]) * 0.5f;
      vTmp[1] = fNormY * (GetOrthoPlanes()[3] - GetOrthoPlanes()[2]) * 0.5f;
      vTmp[2] = -fZInEye;
      vTmp[3] = 1.0f;
  } 
  else
  {
    float fH = tan( Deg2Rad( GetFieldOfView() * 0.5f )) * fZInEye;
    vTmp[0] = fH * fNormX;
    vTmp[1] = fH * fAspect * fNormY;
    vTmp[2] = -fZInEye;
    vTmp[3] = 1.0f;
  }

  //cerr << "vTmp " << vTmp << endl;
  vTmp = m_mViewInv * vTmp;
  ////////////////////
  // m_mViewInv[3][3] should be 1.
  if ( !TOO_CLOSE_TO_ZERO(vTmp[3]) )
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }

  return CVector3<float>(vTmp[0],vTmp[1],vTmp[2]);
}
/////////////////////////////////////////////////////////////////
// The factor which gives seemingly good results on the trackball.
#define TRACKBALL_FUDGE_FACTOR 0.5f
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::VirtualTrackball( const CVector2<int> &vStartPoint, const CVector2<int> &vEndPoint  )
{
  VirtualTrackball( GetPosition()+GetForwardVector()*GetTrackballDistance(), vStartPoint, vEndPoint);
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::VirtualTrackball( const CVector3<float> &vPosition, const CVector2<int> &vStartPoint, const CVector2<int> &vEndPoint )
{
  CQuaternion qRotation;
  CSphere  sphere(vPosition,((GetPosition()-vPosition).Length()-GetNearClipping())*TRACKBALL_FUDGE_FACTOR);
  if ( VirtualTrackball( vPosition, vStartPoint, vEndPoint, qRotation ) )
  {
    qRotation.Reverse();
    RotateAroundPoint( sphere.GetPosition(), qRotation  );
  }
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CCamera::VirtualTrackball( const CSphere & sphere, const CVector2<int> &vStartPoint, const CVector2<int> &vEndPoint, CQuaternion & qResult )
{
      CVector3<float> vOrig = UnProject( static_cast<float>(vStartPoint[0]),
                         static_cast<float>(vStartPoint[1]),
                                0.0f);
      CVector3<float> vEnd = UnProject( static_cast<float>(vStartPoint[0]),
                        static_cast<float>(vStartPoint[1]),
                        1.0f);
      CRay ray;
      ray.SetPosition( vOrig );
      ray.SetDirection( vEnd-vOrig );
      /////////////////////////////////////////////////////////////////
      CVector3<float> vIntersection0;
      CVector3<float> vIntersection1;
      /////////////////////////////////////////////////////////////////
      if ( Collision::RayIntersectsSphere( ray, &vIntersection0, NULL, sphere) >= 1)
      {
        vOrig = UnProject( static_cast<float>(vEndPoint[0]), static_cast<float>(vEndPoint[1]), 0.0f);
        vEnd = UnProject( static_cast<float>(vEndPoint[0]), static_cast<float>(vEndPoint[1]),  1.0f);
        ray.SetPosition( vOrig );
        ray.SetDirection( vEnd-vOrig);
        /////////////////////////////////////////////////////////////////
        if ( Collision::RayIntersectsSphere( ray, &vIntersection1, NULL, sphere) >= 1)
        {
          vOrig = vIntersection0 - sphere.GetPosition();
          vEnd  = vIntersection1 - sphere.GetPosition();
            //float fLen = (sphere.GetPosition()-GetPosition()).Length();
          vOrig.Normalize();
          vEnd.Normalize();
            //vOrig *= fLen;
            //vEnd *= fLen;
          Moose::Math::RotationArc(vOrig,vEnd, qResult);
          return 1;
        }
      }
      return 0;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CCamera::VirtualTrackball( const CVector3<float> &vPosition, const CVector2<int> &vStartPoint, const CVector2<int> &vEndPoint, CQuaternion & qResult )
{
  CSphere  sphere(vPosition,((GetPosition()-vPosition).Length()-GetNearClipping())*TRACKBALL_FUDGE_FACTOR);
  return VirtualTrackball( sphere, vStartPoint, vEndPoint, qResult);
}
/////////////////////////////////////////////////////////////////
#undef TRACKBALL_FUDGE_FACTOR
/////////////////////////////////////////////////////////////////
CVector3<float>
Moose::Graphics::CCamera::ProjectToEye( const CVector3<float> &vPosition)
{
  CVector4<float> vTmp;

  vTmp[0] = vPosition[0];
  vTmp[1] = vPosition[1];
  vTmp[2] = vPosition[2];
  vTmp[3] = 1.0f;

  vTmp = GetViewMatrix() * vTmp;
  return CVector3<float>( vTmp[0], vTmp[1], vTmp[2] );  
}
/////////////////////////////////////////////////////////////////
CVector3<float>
Moose::Graphics::CCamera::Project( const CVector3<float> &vPosition)
{

  
  CVector4<float> vTmp;

  vTmp[0] = vPosition[0];
  vTmp[1] = vPosition[1];
  vTmp[2] = vPosition[2];
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
  
  return CVector3<float>(m_aViewport.x + vTmp[0] * m_aViewport.width,
			 m_aViewport.y + vTmp[1] * m_aViewport.height,
			 vTmp[2]);
  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::CalculateFrustum()
{
  float fE = 1.0f / tan( Deg2Rad(GetFieldOfView()) * 0.5f );
  // This is as specified in math for 3d game progrm. & comp. graphics. (height/width)
  float fAspect = (float)m_aViewport.height/(float)m_aViewport.width; 
  float f1DivSqrtEPow2plus1 = 1.0f / sqrtf( (fE * fE) + 1.0f); 
  float f1DivSqrtEPow2plusAspectPow2 = 1.0f / sqrtf( (fE * fE) + (fAspect * fAspect));
  
  // Near clipping plane
  m_Frustum.GetPlane(BACK)[0] = 0.0f;
  m_Frustum.GetPlane(BACK)[1] = 0.0f;
  m_Frustum.GetPlane(BACK)[2] = -1.0f;
  m_Frustum.GetPlane(BACK)[3] = -GetNearClipping();
  
  // Far clipping plane
  m_Frustum.GetPlane(Moose::Volume::FRONT)[0] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::FRONT)[1] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::FRONT)[2] = 1.0f;
  m_Frustum.GetPlane(Moose::Volume::FRONT)[3] = GetFarClipping();
  
  // Left clipping plane
  m_Frustum.GetPlane(Moose::Volume::LEFT)[0] = fE * f1DivSqrtEPow2plus1;
  m_Frustum.GetPlane(Moose::Volume::LEFT)[1] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::LEFT)[2] = -f1DivSqrtEPow2plus1;
  m_Frustum.GetPlane(Moose::Volume::LEFT)[3] = 0.0f;
  
  // Right clipping plane
  m_Frustum.GetPlane(Moose::Volume::RIGHT)[0] = -(fE *f1DivSqrtEPow2plus1);
  m_Frustum.GetPlane(Moose::Volume::RIGHT)[1] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::RIGHT)[2] = -f1DivSqrtEPow2plus1; 
  m_Frustum.GetPlane(Moose::Volume::RIGHT)[3] = 0.0f;

  // Bottom clipping plane
  m_Frustum.GetPlane(Moose::Volume::BOTTOM)[0] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::BOTTOM)[1] = fE *f1DivSqrtEPow2plusAspectPow2;
  m_Frustum.GetPlane(Moose::Volume::BOTTOM)[2] = -fAspect *f1DivSqrtEPow2plusAspectPow2;
  m_Frustum.GetPlane(Moose::Volume::BOTTOM)[3] = 0.0f;

  // Top clipping plane
  m_Frustum.GetPlane(Moose::Volume::TOP)[0] = 0.0f;
  m_Frustum.GetPlane(Moose::Volume::TOP)[1] = -fE *f1DivSqrtEPow2plusAspectPow2;
  m_Frustum.GetPlane(Moose::Volume::TOP)[2] = -fAspect *f1DivSqrtEPow2plusAspectPow2;
  m_Frustum.GetPlane(Moose::Volume::TOP)[3] = 0.0f;
  // Planes are transformed like any other vector, but with the exception that 
  // we use inverse transpose of transformation matrix (M^-1)^T.
  // we would transform it by ((inverse view)^-1)^T = view^T.
  CVector4<float> vTmp;
  CMatrix4x4<float> vTranspose = GetViewMatrix().GetTransposition();
#define CONVERT_TO_WORLD_SPACE( PLANE ) {\
  vTmp = vTranspose * static_cast<CVector4<float> >(PLANE);	\
  PLANE[0] = vTmp[0]; 						\
  PLANE[1] = vTmp[1]; 						\
  PLANE[2] = vTmp[2]; 						\
  PLANE[3] = vTmp[3];                                           \
}
 
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::BOTTOM));
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::TOP));
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::LEFT));
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::RIGHT));
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::FRONT));
  CONVERT_TO_WORLD_SPACE(m_Frustum.GetPlane( Moose::Volume::BACK));


//   std::cerr << "Moose LEFT: " << m_Frustum.GetPlane(LEFT) << std::endl;
//   std::cerr << "Moose RIGHT: " << m_Frustum.GetPlane(RIGHT) << std::endl;
//   std::cerr << "Moose TOP: " << m_Frustum.GetPlane(TOP) << std::endl;
//   std::cerr << "Moose BOTTOM: " << m_Frustum.GetPlane(BOTTOM) << std::endl;
//   std::cerr << "Moose BACK: " << m_Frustum.GetPlane(BACK) << std::endl;
//   std::cerr << "Moose FRONT: " << m_Frustum.GetPlane(FRONT) << std::endl;  

  // Calculate corners
  CVector3<float> vCorner;
  int iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
                                       m_Frustum.GetPlane(BACK),
                                       m_Frustum.GetPlane(LEFT),
                                       vCorner );
  assert( iRetval == 0);


  m_Frustum.SetCorner( CFrustum::TOP_NEAR_LEFT, vCorner );
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
                                   m_Frustum.GetPlane(BACK),
                                   m_Frustum.GetPlane(RIGHT),
                                   vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::TOP_NEAR_RIGHT, vCorner );
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
                                       m_Frustum.GetPlane(FRONT),
                                       m_Frustum.GetPlane(LEFT),
                                       vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::TOP_FAR_LEFT, vCorner );
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
                                   m_Frustum.GetPlane(FRONT),
                                   m_Frustum.GetPlane(RIGHT),
                                   vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::TOP_FAR_RIGHT, vCorner );
  
  iRetval =CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
                                  m_Frustum.GetPlane(BACK),
                                  m_Frustum.GetPlane(LEFT),
                                  vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::BOTTOM_NEAR_LEFT, vCorner );
  
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
                                   m_Frustum.GetPlane(BACK),
                                   m_Frustum.GetPlane(RIGHT),
                                   vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::BOTTOM_NEAR_RIGHT, vCorner );
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
                                   m_Frustum.GetPlane(FRONT),
                                   m_Frustum.GetPlane(LEFT),
                                   vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::BOTTOM_FAR_LEFT, vCorner );
  
  iRetval = CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
                                   m_Frustum.GetPlane(FRONT),
                                   m_Frustum.GetPlane(RIGHT),
                                   vCorner );
  assert( iRetval == 0);
  
  m_Frustum.SetCorner( CFrustum::BOTTOM_FAR_RIGHT, vCorner);
  
  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::SetDecalOffset( float fDelta, float fZ )
{
  m_fDecalOffset = 1.0f + (-2.0f * GetFarClipping() * GetNearClipping() * fDelta) / 
                          ((GetFarClipping() + GetNearClipping()) * fZ *( fZ + fDelta ));
  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::ResetDecalOffset()
{
  m_fDecalOffset = 1.0f;
}
/////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float> 
Moose::Graphics::CCamera::GetFocusPoint()
{
  return GetPosition()+GetForwardVector()*GetTrackballDistance();
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CCamera::CreateRay( float fX, float fY, CRay & ray)
{
  ray.SetPosition( UnProject( fX, fY,  0.0f) );
  ray.SetDirection( UnProject( fX, fY, 1.0f) - ray.GetPosition() );
  //std::cout << "Ray: " << ray.GetDirection() << ", " << ray.GetPosition() << endl;
}
/////////////////////////////////////////////////////////////////
float
Moose::Graphics::CCamera::GetAspectRatio() const
{
	if ( m_aViewport.height == 0 ) return 0.0f;
	return static_cast<float>(m_aViewport.width) /
				 static_cast<float>(m_aViewport.height);
}
/////////////////////////////////////////////////////////////////
// Float clip[16];
//   float modl[16], proj[16];
//   glGetFloatv( GL_PROJECTION_MATRIX, proj );
//   glGetFloatv( GL_MODELVIEW_MATRIX,  modl );

//   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
//   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
//   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
//   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
  
//   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
//   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
//   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
//   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
  
//   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
//   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
//   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
//   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
  
//   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
//   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
//   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
//   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
//   cerr << "opengl SHOULD be" << endl;
//   for( unsigned int r=0;r<4;r++)
//   {
//     for(unsigned int c=0;c<4;c++)
//     {
//       cerr << clip[r*4+c] << " ";
//     }
//     cerr << endl;
//   }
//   CMatrix4x4<float> mClip, mModelv, mProj;
//   float *pClipArray = mClip.GetArray();
  
//   glGetFloatv( GL_PROJECTION_MATRIX, mProj.GetArray() );
//   glGetFloatv( GL_MODELVIEW_MATRIX,  mModelv.GetArray() );
//   mClip = mModelv * mProj; 
  
//   // calculate RIGHT plane
//   CPlane tmpPlane;





  

  
//   //std::cerr << "Moose planes: " << std::endl;
  
//   // Left clipping plane
//   m_Frustum.GetPlane(LEFT)[0] = mCombo(3,0) + mCombo(0,0);
//   m_Frustum.GetPlane(LEFT)[1] = mCombo(3,1) + mCombo(0,1);
//   m_Frustum.GetPlane(LEFT)[2] = mCombo(3,2) + mCombo(0,2);
//   m_Frustum.GetPlane(LEFT)[3] = mCombo(3,3) + mCombo(0,3);

//   // Right clipping plane
//   m_Frustum.GetPlane(RIGHT)[0] = mCombo(3,0) - mCombo(0,0);
//   m_Frustum.GetPlane(RIGHT)[1] = mCombo(3,1) - mCombo(0,1);
//   m_Frustum.GetPlane(RIGHT)[2] = mCombo(3,2) - mCombo(0,2);
//   m_Frustum.GetPlane(RIGHT)[3] = mCombo(3,3) - mCombo(0,3);

//   // Top clipping plane
//   m_Frustum.GetPlane(TOP)[0] = mCombo(3,0) - mCombo(1,0);
//   m_Frustum.GetPlane(TOP)[1] = mCombo(3,1) - mCombo(1,1);
//   m_Frustum.GetPlane(TOP)[2] = mCombo(3,2) - mCombo(1,2);
//   m_Frustum.GetPlane(TOP)[3] = mCombo(3,3) - mCombo(1,3);

//   // Bottom clipping plane
//   m_Frustum.GetPlane(BOTTOM)[0] = mCombo(3,0) + mCombo(1,0);
//   m_Frustum.GetPlane(BOTTOM)[1] = mCombo(3,1) + mCombo(1,1);
//   m_Frustum.GetPlane(BOTTOM)[2] = mCombo(3,2) + mCombo(1,2);
//   m_Frustum.GetPlane(BOTTOM)[3] = mCombo(3,3) + mCombo(1,3);

//   // Near clipping plane
//   m_Frustum.GetPlane(BACK)[0] = mCombo(3,0) + mCombo(2,0);
//   m_Frustum.GetPlane(BACK)[1] = mCombo(3,1) + mCombo(2,1);
//   m_Frustum.GetPlane(BACK)[2] = mCombo(3,2) + mCombo(2,2);
//   m_Frustum.GetPlane(BACK)[3] = mCombo(3,3) + mCombo(2,3);

//   // Far clipping plane
//   m_Frustum.GetPlane(Moose::Volume::FRONT)[0] = mCombo(3,0) - mCombo(2,0);
//   m_Frustum.GetPlane(Moose::Volume::FRONT)[1] = mCombo(3,1) - mCombo(2,1);
//   m_Frustum.GetPlane(Moose::Volume::FRONT)[2] = mCombo(3,2) - mCombo(2,2);
//   m_Frustum.GetPlane(Moose::Volume::FRONT)[3] = mCombo(3,3) - mCombo(2,3);


//   // Normalize the plane equations, if requested
//   // if (normalize == true)
// //   {
// //     NormalizePlane(p_planes[0]);
// //     NormalizePlane(p_planes[1]);
// //     NormalizePlane(p_planes[2]);
// //     NormalizePlane(p_planes[3]);
// //     NormalizePlane(p_planes[4]);
// //     NormalizePlane(p_planes[5]);
// //   }

//   std::cerr << "*Moose LEFT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(LEFT)) << std::endl;
//   std::cerr << "*Moose RIGHT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(RIGHT)) << std::endl;
//   std::cerr << "*Moose TOP: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(TOP)) << std::endl;
//   std::cerr << "*Moose BOTTOM: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(BOTTOM)) << std::endl;
//   std::cerr << "*Moose BACK: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(BACK)) << std::endl;
//   std::cerr << "*Moose FRONT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(FRONT)) << std::endl;  

//   m_Frustum.GetPlane(TOP).Normalize();
//   m_Frustum.GetPlane(BOTTOM).Normalize();
//   m_Frustum.GetPlane(LEFT).Normalize();
//   m_Frustum.GetPlane(RIGHT).Normalize();
//   m_Frustum.GetPlane(BACK).Normalize();
//   m_Frustum.GetPlane(Moose::Volume::FRONT).Normalize();

//   std::cerr << "Moose LEFT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(LEFT)) << std::endl;
//   std::cerr << "Moose RIGHT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(RIGHT)) << std::endl;
//   std::cerr << "Moose TOP: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(TOP)) << std::endl;
//   std::cerr << "Moose BOTTOM: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(BOTTOM)) << std::endl;
//   std::cerr << "Moose BACK: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(BACK)) << std::endl;
//   std::cerr << "Moose FRONT: " << static_cast<CVector4<float> >(m_Frustum.GetPlane(FRONT)) << std::endl;  

//   CVector3<float> vCorner;
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
// 				 m_Frustum.GetPlane(BACK),
// 				 m_Frustum.GetPlane(LEFT),
// 				 vCorner ) == 0);


//   m_Frustum.SetCorner( CFrustum::TOP_NEAR_LEFT, vCorner );
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
// 				 m_Frustum.GetPlane(BACK),
// 				 m_Frustum.GetPlane(RIGHT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::TOP_NEAR_RIGHT, vCorner );
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
// 				 m_Frustum.GetPlane(FRONT),
// 				 m_Frustum.GetPlane(LEFT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::TOP_FAR_LEFT, vCorner );

//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(TOP), 
// 				 m_Frustum.GetPlane(FRONT),
// 				 m_Frustum.GetPlane(RIGHT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::TOP_FAR_RIGHT, vCorner );
  
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
// 				 m_Frustum.GetPlane(BACK),
// 				 m_Frustum.GetPlane(LEFT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::BOTTOM_NEAR_LEFT, vCorner );
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
// 				 m_Frustum.GetPlane(BACK),
// 				 m_Frustum.GetPlane(RIGHT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::BOTTOM_NEAR_RIGHT, vCorner );
  
//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
// 				 m_Frustum.GetPlane(FRONT),
// 				 m_Frustum.GetPlane(LEFT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::BOTTOM_FAR_LEFT, vCorner );

//   assert( CollisionPoint3Planes( m_Frustum.GetPlane(BOTTOM), 
// 				 m_Frustum.GetPlane(FRONT),
// 				 m_Frustum.GetPlane(RIGHT),
// 				 vCorner ) == 0);
  
//   m_Frustum.SetCorner( CFrustum::BOTTOM_FAR_RIGHT, vCorner);
  


//   //cerr << "OGL mv " << mModelv << endl;
//   //cerr << "OGL proj " << mProj << endl;
//   //cerr << "my mv " << pCamera->GetView() << endl;
//   //cerr << "my proj " << pCamera->GetProjection() << endl;
//   //exit(1);
  
//   //pCamera->Frustum().SetPlane( GSE_Frustum::RIGHT, tmpPlane );
  
//   // calculate RIGHT plane
//   tmpPlane[0] = pClipArray[3]  - pClipArray[0];
//   tmpPlane[1] = pClipArray[7]  - pClipArray[4];
//   tmpPlane[2] = pClipArray[11] - pClipArray[8];
//   tmpPlane[3] = pClipArray[15] - pClipArray[12];
//   tmpPlane.Normalize();
//   std::cerr << "OpenGL RIGHT: " << static_cast<CVector4<float> >(tmpPlane) << std::endl;

//   // // calculate LEFT plane
//   tmpPlane[0] = pClipArray[3]  + pClipArray[0];
//   tmpPlane[1] = pClipArray[7]  + pClipArray[4];
//   tmpPlane[2] = pClipArray[11] + pClipArray[8];
//   tmpPlane[3] = pClipArray[15] + pClipArray[12];
//   tmpPlane.Normalize();
//   std::cerr << "OpenGL LEFT: " << static_cast<CVector4<float> >(tmpPlane) << std::endl;

//   // calculate BOTTOM plane
//   tmpPlane[0] = pClipArray[ 3] + pClipArray[ 1];
//   tmpPlane[1] = pClipArray[ 7] + pClipArray[ 5];
//   tmpPlane[2] = pClipArray[11] + pClipArray[ 9];
//   tmpPlane[3] = pClipArray[15] + pClipArray[13];
//   tmpPlane.Normalize();
//   std::cerr << "OpenGL BOTTOM: " << static_cast<CVector4<float> >(tmpPlane) << std::endl;

//   // calculate TOP plane
//   tmpPlane[0] = pClipArray[ 3] - pClipArray[ 1];
//   tmpPlane[1] = pClipArray[ 7] - pClipArray[ 5];
//   tmpPlane[2] = pClipArray[11] - pClipArray[ 9];
//   tmpPlane[3] = pClipArray[15] - pClipArray[13];
//   tmpPlane.Normalize();
//   std::cerr << "OpenGL TOP: " << static_cast<CVector4<float> >(tmpPlane) << std::endl;

//   // calculate FAR plane
//   tmpPlane[0] = pClipArray[ 3] - pClipArray[ 2];
//   tmpPlane[1] = pClipArray[ 7] - pClipArray[ 6];
//   tmpPlane[2] = pClipArray[11] - pClipArray[10];
//   tmpPlane[3] = pClipArray[15] - pClipArray[14];
//   //tmpPlane.Normalize();
//   std::cerr << "OpenGL FAR: " << tmpPlane << std::endl;
  
//   // calculate NEAR plane
//   tmpPlane[0] = pClipArray[ 3] + pClipArray[ 2];
//   tmpPlane[1] = pClipArray[ 7] + pClipArray[ 6];
//   tmpPlane[2] = pClipArray[11] + pClipArray[10];
//   tmpPlane[3] = pClipArray[15] + pClipArray[14];
//   //tmpPlane.Normalize();
//   std::cerr << "OpenGL NEAR: " << tmpPlane << std::endl;
