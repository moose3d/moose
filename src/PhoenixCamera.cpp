/////////////////////////////////////////////////////////////////
#include <iostream>
#include "PhoenixCamera.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
CCamera::CCamera() : COrientable()
{
  SetFieldOfView(45.0);
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
CCamera::~CCamera()
{
  
}
/////////////////////////////////////////////////////////////////
float *
CCamera::GetOrthoPlanes()
{
  return m_aOrthoPlanes;
}
/////////////////////////////////////////////////////////////////
char
CCamera::IsOrthogonal()
{
  return m_bOrtho;
}
/////////////////////////////////////////////////////////////////
void
CCamera::SetFieldOfView(float fDegrees)
{
  m_fFieldOfView = fDegrees;
  m_bOrtho = 0;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void
CCamera::SetViewOrtho( float fLeft, float fRight, 
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
CCamera::SetViewport( int iX, int iY, int iWidth, int iHeight)
{

  m_aViewport[0] = iX;
  m_aViewport[1] = iY;
  m_aViewport[2] = iWidth;
  m_aViewport[3] = iHeight;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
CCamera::SetNearClipping(float fNearClipping)
{

  m_fNearClipping = fNearClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
void 
CCamera::SetFarClipping(float fFarClipping)
{

  m_fFarClipping = fFarClipping;
  SetProjectionChanged(1);
}
/////////////////////////////////////////////////////////////////
float
CCamera::GetFarClipping()
{
  return m_fFarClipping;
}
/////////////////////////////////////////////////////////////////
float
CCamera::GetNearClipping()
{
  return m_fNearClipping;
}
/////////////////////////////////////////////////////////////////
float
CCamera::GetFieldOfView()
{
  return m_fFieldOfView;
}
/////////////////////////////////////////////////////////////////
int *
CCamera::GetViewport()
{
  return m_aViewport;
}
/////////////////////////////////////////////////////////////////
char
CCamera::IsLensFlaresEnabled()
{
  return m_bLensFlaresEnabled;
}
/////////////////////////////////////////////////////////////////
void 
CCamera::SetLensFlaresEnabled(char bFlag)
{
  m_bLensFlaresEnabled = bFlag;
}
/////////////////////////////////////////////////////////////////
CFrustum &
CCamera::Frustum()
{
  return m_Frustum;
}
/////////////////////////////////////////////////////////////////
// As described in 
// http://www.flipcode.com/articles/article_frustumculling.shtml
//
void
CCamera::CalculateBoundingSphere()
{
  float fViewLen = m_fFarClipping - m_fNearClipping;
  float fHeight = fViewLen * tan(DEG2RAD(m_fFieldOfView) * 0.5f);
  float fWidth = fHeight;

  // halfway point between near/far planes starting at the origin and 
  // extending along the z axis
  CVector3 vP(0.0f, 0.0f, m_fNearClipping + fViewLen * 0.5f);

  // the calculate far corner of the frustum
  CVector3 vQ(fWidth, fHeight, fViewLen);
  
  // the vector between P and Q
  CVector3 vDiff = vP - vQ;
  
  // the radius becomes the length of this vector
  m_FrustumSphere.SetRadius( vDiff.Length());
  
  // calculate the center of the sphere
  m_FrustumSphere.SetPosition( GetPosition() + (GetForwardVector() * (fViewLen * 0.5f + m_fNearClipping)));
  
}
/////////////////////////////////////////////////////////////////
void
CCamera::CalculateBoundingCone()
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
CCone &
CCamera::FrustumCone()
{
  return m_FrustumCone;
}
/////////////////////////////////////////////////////////////////
CSphere &
CCamera::FrustumSphere()
{
  return m_FrustumSphere;
}
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
CCamera::RotateAroundPoint( const CVector3 & vPoint, const CQuaternion & q)
{

  AppendToRotation(q);
  SetPosition( Math::RotateAroundPoint(GetPosition(), vPoint, q ));
  
}
/////////////////////////////////////////////////////////////////
CCameraMgr::CCameraMgr() : CContainer<CCamera>()
{
  
}
/////////////////////////////////////////////////////////////////
CCameraMgr::~CCameraMgr()
{
  Core::CLogger::Error() << "CameraMgr:: delete\n";
}
/////////////////////////////////////////////////////////////////
CCamera *
CCameraMgr::CreateCamera()
{
  CCamera *pCamera = new CCamera();
  Add( pCamera );
  Map( (char *)pCamera->GetName(), pCamera);
  
  return pCamera;
}
/////////////////////////////////////////////////////////////////
void 
CCamera::UpdateProjection() 
{
  float fH = tanf( DEG2RAD( GetFieldOfView() * 0.5f )) * GetNearClipping(); 
  float fW = fH * (float)m_aViewport[2]/(float)m_aViewport[3];
  float fFar = m_fFarClipping;
  float fNear = m_fNearClipping;
  //             l   r   b    t   n      f
  // glFrustum( -fW, fW, -fH, fH, fNear, fFar );
  m_mProjectionInv = m_mProjection = CMatrix4x4f::Zero();
  
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
CCamera::UpdateView()
{
  // Transform camera reversely so we get proper effect.
  CMatrix4x4f mT = Math::TranslationMatrix( GetPosition());
  CMatrix4x4f mR = GetRotationQuaternion().ToMatrix();
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
CVector3 
CCamera::WindowCoordinatesToEye( float fX, float fY, float fZ )
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
  
  return CVector3(fH * fAspect * fNormX, fH * fNormY, -fZInEye);
  
}
/////////////////////////////////////////////////////////////////
CVector3 
CCamera::EyeToWorld( const CVector3 &vPosition )
{
  CVector<float,4> vTmp;
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
  return CVector3(vTmp[0],vTmp[1],vTmp[2]);

}
/////////////////////////////////////////////////////////////////
CVector3 
CCamera::WindowCoordinatesToWorld( float fX, float fY, float fZ)
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

  CVector<float,4> vTmp;
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
  return CVector3(vTmp[0],vTmp[1],vTmp[2]);
}
/////////////////////////////////////////////////////////////////
// The factor which gives seemingly good results on the trackball.
#define TRACKBALL_FUDGE_FACTOR 0.5f
/////////////////////////////////////////////////////////////////
void 
CCamera::VirtualTrackball( const CVector3 &vPosition, 
			      const CVector2 &vStartPoint,
			      const CVector2 &vEndPoint )
{
  CVector3 vOrig = WindowCoordinatesToWorld( vStartPoint.m_pValues[CVector2::X], 
						vStartPoint.m_pValues[CVector2::Y],
						0.0f);
  CVector3 vEnd = WindowCoordinatesToWorld( vStartPoint.m_pValues[CVector2::X], 
					       vStartPoint.m_pValues[CVector2::Y],
					       1.0f);
  /////////////////////////////////////////////////////////////////
  CVector3 vIntersection0;
  CVector3 vIntersection1;
  CSphere  sphere(vPosition,((GetPosition()-vPosition).Length()-GetNearClipping())*TRACKBALL_FUDGE_FACTOR);
  /////////////////////////////////////////////////////////////////
  if ( Geometry::RayIntersectsSphere( vOrig, vEnd, &vIntersection0, NULL, sphere) >= 1)
  {
    vOrig = WindowCoordinatesToWorld( vEndPoint.m_pValues[CVector2::X], 
				      vEndPoint.m_pValues[CVector2::Y],
				      0.0f);
    vEnd = WindowCoordinatesToWorld( vEndPoint.m_pValues[CVector2::X], 
				     vEndPoint.m_pValues[CVector2::Y],
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
      CQuaternion qRotation = CQuaternion::RotationArc(vOrig,vEnd).Reverse();
      RotateAroundPoint( sphere.GetPosition(), qRotation  );
    }
  }

}
/////////////////////////////////////////////////////////////////
#undef TRACKBALL_FUDGE_FACTOR
/////////////////////////////////////////////////////////////////
CVector3
CCamera::WorldCoordinatesToScreen( const CVector3 &vPosition)
{

  
  CVector<float,4> vTmp;

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
  
  return CVector3(m_aViewport[0] + vTmp[0] * m_aViewport[2],
		     m_aViewport[1] + vTmp[1] * m_aViewport[3],
		     vTmp[2]);
  
}
/////////////////////////////////////////////////////////////////
