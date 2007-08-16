/////////////////////////////////////////////////////////////////
#include "PhoenixQuaternion.h"
#include <math.h>
#include <iostream>
using Phoenix::Math::CQuaternion;
/////////////////////////////////////////////////////////////////
CQuaternion 
CQuaternion::operator * ( const CQuaternion & qQuat ) const
{
  //   q=[v_{xyz], w]
  // q1q2 = [w1v2 + w2v1 + v1^v2, w1w2 - v1.v2]
  //	
  Phoenix::Math::CVector3<float>		vTmp1;
  Phoenix::Math::CVector3<float>		vTmp2;
  Phoenix::Math::CVector3<float>		vResult;
  
  vTmp1.UseExternalData( m_pValues );
  vTmp2.UseExternalData( qQuat.m_pValues );
  
  vResult =  vTmp1 * qQuat(3);
  vResult += vTmp2 * At(3);
  vResult += vTmp1.Cross(vTmp2);
  return CQuaternion( vResult(0), vResult(1), vResult(2), 
		      (qQuat(3) * At(3)) - vTmp1.Dot(vTmp2) );
  
}
/////////////////////////////////////////////////////////////////
void 
CQuaternion::CreateFromAxisAngleRad ( float fX, float fY, float fZ, float fRad ) 
{
  
  float thetaDiv2    = fRad * 0.5f;
  float sinThetaDiv2 = sinf ( thetaDiv2 );

  m_pValues[0] = fX * sinThetaDiv2 ;
  m_pValues[1] = fY * sinThetaDiv2 ;
  m_pValues[2] = fZ * sinThetaDiv2 ;
  m_pValues[3] = cosf( thetaDiv2 );  
}
/////////////////////////////////////////////////////////////////
void 
CQuaternion::CreateFromAxisAngles( const CVector3<float> &vValues )
{
  CQuaternion qX,qY,qZ,qTemp;
  
  qX.CreateFromAxisAngle( 1,0,0, vValues(0));
  qY.CreateFromAxisAngle( 0,1,0, vValues(1));
  qZ.CreateFromAxisAngle( 0,0,1, vValues(2));
  qTemp = qZ * qY;
  qZ = qTemp * qX;
  *this = qZ;
  
}
/////////////////////////////////////////////////////////////////
void
CQuaternion::CreateFromAxisAnglesRad( const CVector3<float> &vValues )
{
  CQuaternion qX,qY,qZ,qCombined;

  qX.CreateFromAxisAngleRad( 1,0,0, vValues(0));
  qY.CreateFromAxisAngleRad( 0,1,0, vValues(1));
  qZ.CreateFromAxisAngleRad( 0,0,1, vValues(2));
  qCombined = qZ * qY * qX;
  
  *this = qCombined;
}
/////////////////////////////////////////////////////////////////
std::ostream & 
Phoenix::Math::operator<<(std::ostream &stream, const CQuaternion & qQuat)
{
  stream << qQuat(0) << "," << qQuat(1) << "," << qQuat(2) << "," << qQuat(3) << std::endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
