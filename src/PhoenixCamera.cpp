/////////////////////////////////////////////////////////////////
#include <iostream>
#include "PhoenixCamera.h"
#include "PhoenixCollision.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
using namespace Phoenix::Collision;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CCamera::CCamera() : COrientable()
{
  SetFieldOfView(45.0);
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CCamera::~CCamera()
{
  
}
/////////////////////////////////////////////////////////////////
float *
Phoenix::Graphics::CCamera::GetOrthoPlanes()
{
  return m_aOrthoPlanes;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CCamera::IsOrthogonal()
{
  return m_bOrtho;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CCamera::SetFieldOfView(float fDegrees)
{
  m_fFieldOfView = fDegrees;
  m_bOrtho = 0;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CCamera::SetViewOrtho( float fLeft, float fRight, 
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
Phoenix::Graphics::CCamera::SetViewport( int iX, int iY, int iWidth, int iHeight)
{

  m_aViewport[0] = iX;
  m_aViewport[1] = iY;
  m_aViewport[2] = iWidth;
  m_aViewport[3] = iHeight;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::SetNearClipping(float fNearClipping)
{

  m_fNearClipping = fNearClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::SetFarClipping(float fFarClipping)
{

  m_fFarClipping = fFarClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Graphics::CCamera::GetFarClipping()
{
  return m_fFarClipping;
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Graphics::CCamera::GetNearClipping()
{
  return m_fNearClipping;
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Graphics::CCamera::GetFieldOfView()
{
  return m_fFieldOfView;
}
/////////////////////////////////////////////////////////////////
int *
Phoenix::Graphics::CCamera::GetViewport()
{
  return m_aViewport;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CCamera::IsLensFlaresEnabled()
{
  return m_bLensFlaresEnabled;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::SetLensFlaresEnabled(int bFlag)
{
  m_bLensFlaresEnabled = bFlag;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrustum &
Phoenix::Graphics::CCamera::Frustum()
{
  return m_Frustum;
}
/////////////////////////////////////////////////////////////////
// As described in 
// http://www.flipcode.com/articles/article_frustumculling.shtml
//
// void
// Phoenix::Graphics::CCamera::CalculateBoundingSphere()
// {
//   float fViewLen = m_fFarClipping - m_fNearClipping;
//   float fHeight = fViewLen * tan(Deg2Rad(m_fFieldOfView) * 0.5f);
//   float fWidth = fHeight;

//   // halfway point between near/far planes starting at the origin and 
//   // extending along the z axis
//   CVector3<float> vP(0.0f, 0.0f, m_fNearClipping + fViewLen * 0.5f);

//   // the calculate far corner of the frustum
//   CVector3<float> vQ(fWidth, fHeight, fViewLen);
  
//   // the vector between P and Q
//   CVector3<float> vDiff = vP - vQ;
  
//   // the radius becomes the length of this vector
//   //m_FrustumSphere.SetRadius( vDiff.Length());
  
//   // calculate the center of the sphere
//   //m_FrustumSphere.SetPosition( GetPosition() + (GetForwardVector() * (fViewLen * 0.5f + m_fNearClipping)));
  
// }
/////////////////////////////////////////////////////////////////
// void
// Phoenix::Graphics::CCamera::CalculateBoundingCone()
// {
  
//   float fHalfHeight = m_aViewport[3] * 0.5f; // Half of the screen height
//   float fHalfWidth = m_aViewport[2] * 0.5f;// Half of the screen width

//   // calculate the length of the fov triangle
//   float fDepth  = fHalfHeight / tanf(Deg2Rad(m_fFieldOfView) * 0.5f);

//   // calculate the corner of the screen
//   float fCorner = sqrt(fHalfWidth * fHalfWidth + fHalfHeight * fHalfHeight);
  
//   // now calculate the new fov
//   float fFov = atan(fCorner / fDepth);
  
//   // apply to the cone
//   //m_FrustumCone.SetDirection(GetForwardVector());
//   //m_FrustumCone.SetPosition(GetPosition());
//   //m_FrustumCone.SetAngle(fFov);

// }
/////////////////////////////////////////////////////////////////
// CCone &
// Phoenix::Graphics::CCamera::FrustumCone()
// {
//   return m_FrustumCone;
// }
// /////////////////////////////////////////////////////////////////
// CSphere &
// Phoenix::Graphics::CCamera::FrustumSphere()
// {
//   return m_FrustumSphere;
// }
/////////////////////////////////////////////////////////////////
// Handy operator override  for debug printing
std::ostream &operator<<(std::ostream &stream, CCamera &obj)
{
  
  stream << "position: " << obj.GetPosition() << endl
	 << "forward: " << obj.GetForwardVector() << endl
	 << "up: "      << obj.GetUpVector() << endl
	 << "right: "   << obj.GetRightVector();
  return stream;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::RotateAroundPoint( const CVector3<float> & vPoint, const CQuaternion & q)
{

  AppendToRotation(q);
  SetPosition( Math::RotateAroundPoint(GetPosition(), vPoint, q ));
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::UpdateProjection() 
{
  float fH = tanf( Deg2Rad( GetFieldOfView() * 0.5f )) * GetNearClipping(); 
  float fW = fH * (float)m_aViewport[2]/(float)m_aViewport[3];
  float fFar = m_fFarClipping;
  float fNear = m_fNearClipping;
  //             l   r   b    t   n      f
  // glFrustum( -fW, fW, -fH, fH, fNear, fFar );
  m_mProjection.ZeroMatrix();
  m_mProjectionInv = m_mProjection;
  
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
Phoenix::Graphics::CCamera::UpdateView()
{
  // Transform camera reversely so we get proper effect.
  CMatrix4x4<float> mT = TranslationMatrix( GetPosition());
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
Phoenix::Graphics::CCamera::WindowCoordinatesToEye( float fX, float fY, float fZ )
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
  float fH = tanf( Deg2Rad( GetFieldOfView() * 0.5f )) * fZInEye; 
  
  return CVector3<float>(fH * fAspect * fNormX, fH * fNormY, -fZInEye);
  
}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Phoenix::Graphics::CCamera::EyeToWorld( const CVector3<float> &vPosition )
{
  CVector4<float> vTmp;
  vTmp[0] = vPosition(0);
  vTmp[1] = vPosition(1);
  vTmp[2] = vPosition(2);
  vTmp[3] = 1.0f;
  
  vTmp = m_mViewInv * vTmp;

  if ( !TOO_CLOSE_TO_ZERO(vTmp(3)) )
  {
    vTmp[0] /= vTmp(3);
    vTmp[1] /= vTmp(3);
    vTmp[2] /= vTmp(3);
  }
  return CVector3<float>(vTmp(0),vTmp(1),vTmp(2));

}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Phoenix::Graphics::CCamera::WindowCoordinatesToWorld( float fX, float fY, float fZ)
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
  float fH = tanf( Deg2Rad( GetFieldOfView() * 0.5f )) * fZInEye; 

  CVector4<float> vTmp;
  vTmp[0] = fH * fAspect * fNormX;
  vTmp[1] = fH * fNormY;
  vTmp[2] = -fZInEye;
  vTmp[3] = 1.0f;
  
  //vTmp = m_mViewInv * vTmp;

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
Phoenix::Graphics::CCamera::VirtualTrackball( const CVector3<float> &vPosition, 
					      const CVector2<int> &vStartPoint,
					      const CVector2<int> &vEndPoint )
{
  CVector3<float> vOrig = WindowCoordinatesToWorld( vStartPoint(0), 
						vStartPoint(1),
						0.0f);
  CVector3<float> vEnd = WindowCoordinatesToWorld( vStartPoint(0), 
					       vStartPoint(1),
					       1.0f);
  /////////////////////////////////////////////////////////////////
  CVector3<float> vIntersection0;
  CVector3<float> vIntersection1;
  CSphere  sphere(vPosition,((GetPosition()-vPosition).Length()-GetNearClipping())*TRACKBALL_FUDGE_FACTOR);
  /////////////////////////////////////////////////////////////////
  if ( Collision::RayIntersectsSphere( vOrig, vEnd, &vIntersection0, NULL, sphere) >= 1)
  {
    vOrig = WindowCoordinatesToWorld( vEndPoint(0), vEndPoint(1), 0.0f);
    vEnd = WindowCoordinatesToWorld( vEndPoint(0), vEndPoint(1),  1.0f);
    /////////////////////////////////////////////////////////////////
    if ( Collision::RayIntersectsSphere( vOrig, vEnd, &vIntersection1, NULL, sphere) >= 1)
    {
      vOrig = vIntersection0 - sphere.GetPosition();
      vEnd  = vIntersection1 - sphere.GetPosition();

      vOrig.Normalize();
      vEnd.Normalize();

      CQuaternion qRotation = Phoenix::Math::RotationArc(vOrig,vEnd);
      qRotation.Reverse();
      RotateAroundPoint( sphere.GetPosition(), qRotation  );
    }
  }

}
/////////////////////////////////////////////////////////////////
#undef TRACKBALL_FUDGE_FACTOR
/////////////////////////////////////////////////////////////////
CVector3<float>
Phoenix::Graphics::CCamera::WorldCoordinatesToScreen( const CVector3<float> &vPosition)
{

  
  CVector4<float> vTmp;

  vTmp[0] = vPosition(0);
  vTmp[1] = vPosition(1);
  vTmp[2] = vPosition(2);
  vTmp[3] = 1.0f;

  vTmp = GetViewMatrix() * vTmp;
  vTmp = GetProjectionMatrix() * vTmp;
    
  if ( !TOO_CLOSE_TO_ZERO(vTmp(3)) ) 
  {
    vTmp[0] /= vTmp[3];
    vTmp[1] /= vTmp[3];
    vTmp[2] /= vTmp[3];
  }

  vTmp[0] = vTmp(0) * 0.5f + 0.5f;
  vTmp[1] = vTmp(1) * 0.5f + 0.5f;
  vTmp[2] = vTmp(2) * 0.5f + 0.5f;
  
  return CVector3<float>(m_aViewport[0] + vTmp(0) * m_aViewport[2],
			 m_aViewport[1] + vTmp(1) * m_aViewport[3],
			 vTmp(2));
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CCamera::CalculateFrustum()
{
  // Left clipping plane
  CMatrix4x4<float> mCombo;
  mCombo = GetProjectionMatrix() * GetViewMatrix();
  
  m_Frustum.GetPlane(LEFT)[0] = mCombo(0,3) + mCombo(0,0);
  m_Frustum.GetPlane(LEFT)[1] = mCombo(1,3) + mCombo(1,0);

  m_Frustum.GetPlane(LEFT)[2] = mCombo(2,3) + mCombo(2,0);
  m_Frustum.GetPlane(LEFT)[3] = mCombo(3,3) + mCombo(3,0);

  // p_planes[0].a = comboMatrix._14 + comboMatrix._11;
//   p_planes[0].b = comboMatrix._24 + comboMatrix._21;
//   p_planes[0].c = comboMatrix._34 + comboMatrix._31;
//   p_planes[0].d = comboMatrix._44 + comboMatrix._41;

  // Right clipping plane
  m_Frustum.GetPlane(RIGHT)[0] = mCombo(0,3) - mCombo(0,0);
  m_Frustum.GetPlane(RIGHT)[1] = mCombo(1,3) - mCombo(1,0);
  m_Frustum.GetPlane(RIGHT)[2] = mCombo(2,3) - mCombo(2,0);
  m_Frustum.GetPlane(RIGHT)[3] = mCombo(3,3) - mCombo(3,0);
  // p_planes[1].a = comboMatrix._14 - comboMatrix._11;
//   p_planes[1].b = comboMatrix._24 - comboMatrix._21;
//   p_planes[1].c = comboMatrix._34 - comboMatrix._31;
//   p_planes[1].d = comboMatrix._44 - comboMatrix._41;
  // Top clipping plane
  m_Frustum.GetPlane(TOP)[0] = mCombo(0,3) - mCombo(0,1);
  m_Frustum.GetPlane(TOP)[1] = mCombo(1,3) - mCombo(1,1);
  m_Frustum.GetPlane(TOP)[2] = mCombo(2,3) - mCombo(2,1);
  m_Frustum.GetPlane(TOP)[3] = mCombo(3,3) - mCombo(3,1);
//   p_planes[2].a = comboMatrix._14 - comboMatrix._12;
//   p_planes[2].b = comboMatrix._24 - comboMatrix._22;
//   p_planes[2].c = comboMatrix._34 - comboMatrix._32;
//   p_planes[2].d = comboMatrix._44 - comboMatrix._42;
  // Bottom clipping plane
  m_Frustum.GetPlane(BOTTOM)[0] = mCombo(0,3) + mCombo(0,1);
  m_Frustum.GetPlane(BOTTOM)[1] = mCombo(1,3) + mCombo(1,1);
  m_Frustum.GetPlane(BOTTOM)[2] = mCombo(2,3) + mCombo(2,1);
  m_Frustum.GetPlane(BOTTOM)[3] = mCombo(3,3) + mCombo(3,1);
  // p_planes[3].a = comboMatrix._14 + comboMatrix._12;
//   p_planes[3].b = comboMatrix._24 + comboMatrix._22;
//   p_planes[3].c = comboMatrix._34 + comboMatrix._32;
//   p_planes[3].d = comboMatrix._44 + comboMatrix._42;

  // Near clipping plane
  m_Frustum.GetPlane(BACK)[0] = mCombo(0,2);
  m_Frustum.GetPlane(BACK)[1] = mCombo(1,2);
  m_Frustum.GetPlane(BACK)[2] = mCombo(2,2);
  m_Frustum.GetPlane(BACK)[3] = mCombo(3,2);

//   p_planes[4].a = comboMatrix._13;
//   p_planes[4].b = comboMatrix._23;
//   p_planes[4].c = comboMatrix._33;
//   p_planes[4].d = comboMatrix._43;
  // Far clipping plane
  m_Frustum.GetPlane(Phoenix::Volume::FRONT)[0] = mCombo(0,3) - mCombo(0,2);
  m_Frustum.GetPlane(Phoenix::Volume::FRONT)[1] = mCombo(1,3) - mCombo(1,2);
  m_Frustum.GetPlane(Phoenix::Volume::FRONT)[2] = mCombo(2,3) - mCombo(2,2);
  m_Frustum.GetPlane(Phoenix::Volume::FRONT)[3] = mCombo(3,3) - mCombo(3,2);
  
  // p_planes[5].a = comboMatrix._14 - comboMatrix._13;
//   p_planes[5].b = comboMatrix._24 - comboMatrix._23;
//   p_planes[5].c = comboMatrix._34 - comboMatrix._33;
//   p_planes[5].d = comboMatrix._44 - comboMatrix._43;
  // Normalize the plane equations, if requested
  // if (normalize == true)
//   {
//     NormalizePlane(p_planes[0]);
//     NormalizePlane(p_planes[1]);
//     NormalizePlane(p_planes[2]);
//     NormalizePlane(p_planes[3]);
//     NormalizePlane(p_planes[4]);
//     NormalizePlane(p_planes[5]);
//   }
  m_Frustum.GetPlane(TOP).Normalize();
  m_Frustum.GetPlane(BOTTOM).Normalize();
  m_Frustum.GetPlane(LEFT).Normalize();
  m_Frustum.GetPlane(RIGHT).Normalize();
  m_Frustum.GetPlane(BACK).Normalize();
  m_Frustum.GetPlane(Phoenix::Volume::FRONT).Normalize();
}
