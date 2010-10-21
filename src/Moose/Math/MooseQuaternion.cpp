/////////////////////////////////////////////////////////////////
#include "MooseQuaternion.h"
#include <math.h>
#include <iostream>
using Moose::Math::CQuaternion;
using Moose::Math::CVector3;
using Moose::Math::CVector4;
/////////////////////////////////////////////////////////////////
CQuaternion 
CQuaternion::operator * ( const CQuaternion & qQuat ) const
{
  //   q=[v_{xyz], w]
  // q1q2 = [w1v2 + w2v1 + v1^v2, w1w2 - v1.v2]
  //	
  Moose::Math::CVector3<float>		vTmp1;
  Moose::Math::CVector3<float>		vTmp2;
  Moose::Math::CVector3<float>		vResult;
  
  vTmp1.Set( m_aValues );
  vTmp2.Set( qQuat.m_aValues );
  
  vResult =  vTmp1 * qQuat[3];
  vResult += vTmp2 * At(3);
  vResult += vTmp1.Cross(vTmp2);
  return CQuaternion( vResult[0], vResult[1], vResult[2], 
		      (qQuat[3] * At(3)) - vTmp1.Dot(vTmp2) );
  
}
/////////////////////////////////////////////////////////////////
void 
CQuaternion::CreateFromAxisAngleRad ( float fX, float fY, float fZ, float fRad ) 
{
  
  double thetaDiv2    = fRad * 0.5f;
  double sinThetaDiv2 = sin ( thetaDiv2 );

  m_aValues[0] = fX * sinThetaDiv2 ;
  m_aValues[1] = fY * sinThetaDiv2 ;
  m_aValues[2] = fZ * sinThetaDiv2 ;
  m_aValues[3] = cos( thetaDiv2 );
}
/////////////////////////////////////////////////////////////////
void 
CQuaternion::CreateFromAxisAngles( const CVector3<float> &vValues )
{
  // CQuaternion qX,qY,qZ,qTemp;
  
  // qX.CreateFromAxisAngle( 1,0,0, vValues[0]);
  // qY.CreateFromAxisAngle( 0,1,0, vValues[1]);
  // qZ.CreateFromAxisAngle( 0,0,1, vValues[2]);
  // qTemp = qZ * qY;
  // qZ = qTemp * qX;
  *this = AxisAngles(vValues);
  
}
/////////////////////////////////////////////////////////////////
void
CQuaternion::CreateFromAxisAnglesRad( const CVector3<float> &vValues )
{
  *this = AxisAnglesRad(vValues);
}
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CQuaternion 
Moose::Math::CQuaternion::AxisAngleRad( float fX, float fY, float fZ, float fRad )
{
  double thetaDiv2    = fRad * 0.5f;
  double sinThetaDiv2 = sin ( thetaDiv2 );
  return Moose::Math::CQuaternion( fX * sinThetaDiv2,fY * sinThetaDiv2,fZ * sinThetaDiv2, cos( thetaDiv2 ));
}
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CQuaternion 
Moose::Math::CQuaternion::AxisAnglesRad( float fX, float fY, float fZ )
{
  return AxisAngleRad(0,0,1,fZ) * AxisAngleRad(0,1,0,fY) * AxisAngleRad(1,0,0,fX);
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
std::ostream & 
Moose::Math::operator<<(std::ostream &stream, const CQuaternion & qQuat)
{
  stream << qQuat[0] << "," << qQuat[1] << "," << qQuat[2] << "," << qQuat[3];
  return stream;
}
/////////////////////////////////////////////////////////////////
#endif
