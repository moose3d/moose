#include <math.h>
#include <iostream>
#include "PhoenixMath.h"
#include <assert.h>
#include <iostream>
#ifdef WIN32
// isnan() is C99
#include <float.h>
#define isnan( VAL ) _isnan(VAL)
#endif
using namespace std;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::QuaternionToMatrix( const CQuaternion &qQuat, CMatrix4x4<float> &mMatrix )
{

// The values in Row-Major mode matrix which represents the
// rotation matrix from the Quaternion:
// (From http://skal.planet-d.net/demo/matrixfaq.htm#Q2)
//
//          ¦                                                     ¦
//          ¦ 1 - (2Y²  + 2Z² ) 2XY - 2ZW           2XZ + 2YW     ¦
//          ¦                                                     ¦
//          ¦                                                     ¦
//      M = ¦ 2XY + 2ZW         1 - (2X²  + 2Z² )   2YZ - 2XW     ¦
//          ¦                                                     ¦
//          ¦                                                     ¦
//          ¦ 2XZ - 2YW         2YZ + 2XW         1 - (2X²  + 2Y²)¦
//          ¦                                                     ¦
//

  float xx = qQuat[0] * qQuat[0];
  float yy = qQuat[1] * qQuat[1];
  float zz = qQuat[2] * qQuat[2];

  float xy = qQuat[0] * qQuat[1];
  float xz = qQuat[0] * qQuat[2];
  float yz = qQuat[1] * qQuat[2];

  float wz = qQuat[3] * qQuat[2];
  float wy = qQuat[3] * qQuat[1];
  float wx = qQuat[3] * qQuat[0];

  mMatrix(0,0) = 1.0f - yy - yy - zz - zz;
  mMatrix(0,1) = xy + xy - wz - wz;
  mMatrix(0,2) = xz + xz + wy + wy;
  mMatrix(0,3) = 0.0f;

  mMatrix(1,0) = xy + xy + wz + wz;
  mMatrix(1,1) = 1.0f - xx - xx - zz - zz;
  mMatrix(1,2) = yz + yz - wx - wx;
  mMatrix(1,3) = 0.0f;

  mMatrix(2,0) = xz + xz - wy - wy;
  mMatrix(2,1) = yz + yz + wx + wx;
  mMatrix(2,2) = 1.0f - xx - xx - yy - yy;
  mMatrix(2,3) = 0.0f;

  mMatrix(3,0) = 0.0f;
  mMatrix(3,1) = 0.0f;
  mMatrix(3,2) = 0.0f;
  mMatrix(3,3) = 1.0f;

}
/////////////////////////////////////////////////////////////////
// std::ostream &
// operator<<(std::ostream &stream, const CQuaternion & qQuat)
// {
//   stream << qQuat(0) << "," <<qQuat(1) << "," <<qQuat(2) << "," << qQuat(3);
//   return stream;
// }
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::Slerp( CQuaternion qFrom, CQuaternion qTo, float fInterpolation, CQuaternion &result )
{

  float fCos = qFrom.Dot(qTo);
  float fScaleFrom, fScaleTo;

  if( fCos < 0.0f )
  {
    fCos = -fCos;
    qTo = -qTo;
  }

  if ( 1.0f - fCos > 0.00001f )
  {
    float fOmega = acos(fCos);
    float f1DivSinOmega = (1.0f/sin(fOmega));
    fScaleFrom = sin((1.0f - fInterpolation )*fOmega) * f1DivSinOmega;
    fScaleTo   = sin( fInterpolation * fOmega )  * f1DivSinOmega;

  }
  else
  {
    fScaleFrom = 1.0f - fInterpolation;
    fScaleTo = fInterpolation;
  }

  qFrom *= fScaleFrom;
  qTo *= fScaleTo;
  result = qFrom + qTo;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationMatrix(const CVector3<float> & vAxis, float fRadians, CMatrix4x4<float> &result)
{
  RotationMatrix( vAxis[0], vAxis[1], vAxis[2], fRadians, result );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationMatrix(float fX, float fY, float fZ, float fRadians, CMatrix4x4<float> &mResult)
{
  // does not need to be initialized, values will be set later.
  CMatrix4x4<float> mS;

  CMatrix4x4<float> uuT(0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f);

  // old version;
  // CMatrix4x4<float> mResult;

  // An unit vector is required
  CVector3<float> vAxisUnit(fX,fY,fZ);
  vAxisUnit.Normalize();
  // For readability
  fZ = vAxisUnit[2];
  fX = vAxisUnit[0];
  fY = vAxisUnit[1];
  // Sine and cosine of the fRadians
  float fSinAlpha = sin(fRadians);
  float fCosAlpha = cos(fRadians);

  // To remove couple of overlapping mulitiplications
  float fXX = fX * fX;
  float fXY = fX * fY;
  float fXZ = fX * fZ;
  float fYY = fY * fY;
  float fYZ = fY * fZ;
  float fZZ = fZ * fZ;
  // At first, we construct matrix S:
  //
  //     ( 0 -z -y )
  // S = ( z  0 -x )
  //     (-y  x  0 )
  //
  mS(0,0) = 0.0; mS(0,1) = -fZ; mS(0,2) =  fY; mS(0,3) = 0.0;
  mS(1,0) =  fZ; mS(1,1) = 0.0; mS(1,2) = -fX; mS(1,3) = 0.0;
  mS(2,0) = -fY; mS(2,1) = -fX; mS(2,2) = 0.0; mS(2,3) = 0.0;
  mS(3,0) = 0.0; mS(3,1) = 0.0; mS(3,2) = 0.0; mS(3,3) = 1.0;
  // then sin (fRadians) * S
  mS = mS * fSinAlpha;
  // uuT = uT * u;
  uuT(0,0) = fXX; uuT(0,1)= fXY; uuT(0,2) = fXZ;
  uuT(1,0) = fXY; uuT(1,1)= fYY; uuT(1,2) = fYZ;
  uuT(2,0) = fXZ; uuT(2,1)= fYZ; uuT(2,2) = fZZ;
  CMatrix4x4<float> mTemp;
  mTemp.IdentityMatrix();
  mResult = uuT + (mTemp - uuT)*fCosAlpha + mS;
  mResult(3,3) = 1.0f;

  //return mResult;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationMatrix( const CVector3<float> &vRadians, CMatrix4x4<float> & result)
{
  RotationMatrix( vRadians[0],
		  vRadians[1],
		  vRadians[2], result);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationMatrix(float fX, float fY, float fZ, CMatrix4x4<float> & mResult )
{
  // Create quaternions from angles
  CQuaternion qX,qY,qZ,qR;
  //CMatrix4x4<float> mResult;

  qX.CreateFromAxisAngleRad( 1.0, 0.0, 0.0, fX);
  qY.CreateFromAxisAngleRad( 0.0, 1.0, 0.0, fY);
  qZ.CreateFromAxisAngleRad( 0.0, 0.0, 1.0, fZ);
  qR = qZ * qY * qX;

  Phoenix::Math::QuaternionToMatrix(qR,mResult);
  // combine quaternions and convert to matrix.
  //return mResult;
}
/////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------------
//
// Calculates the tangent, bitangent and normal vectors in tangent space
// for given vertices vPoint0, vPoint1, vPoint2
//
// ---------------------------------------------------------------------------------
void
Phoenix::Math::GetTBNMatrix( CVector3<float> vPoint0, CVector3<float> vPoint1, CVector3<float> vPoint2,
	      CVector2<float> vTexCoord0, CVector2<float> vTexCoord1, CVector2<float> vTexCoord2,
	      CMatrix3x3<float> & mResult)
{
  //CMatrix3x3<float> mResult;

  CVector3<float> vQ1 = vPoint1 - vPoint0;
  CVector3<float> vQ2 = vPoint2 - vPoint0;
  CVector2<float> vQTex1 = vTexCoord1 - vTexCoord0;
  CVector2<float> vQTex2 = vTexCoord2 - vTexCoord0;
  //  ----------------------------------------------------------
  // The formula for retrieving the tangent space matrix
  //
  //   Q1.xyz = V1.xyz - V0.xyz
  //   Q2.xyz = V2.xyz - V0.xyz
  //   Q1.s   = V1.s - V0.s
  //   Q2.s   = V2.s - V0.s
  //
  //
  //  | Tx Ty Tz |              1               |  Q2.t -Q1.t | | Q1.x Q1.y Q1.z |
  //  |          | =    ------------------    * |             |*|                |
  //  | Bx By Bz |     Q1.s*Q2.t - Q2.s*Q1.t    | -Q2.s  Q1.s | | Q2.x Q2.y Q2.z |
  //
  //  ----------------------------------------------------------
  float fDenominator = (vQTex1[0] * vQTex2[1]) - (vQTex2[0] * vQTex1[1]);
  float fCoefficient = 0.0f;

  if ( TOO_CLOSE_TO_ZERO(fCoefficient))
  {
    // Handle error
    mResult.IdentityMatrix();
    //return mResult;
  }
  else
  {

    CVector3<float> vNormal(0.0f, 0.0f, 1.0f);
    fCoefficient = 1.0f / fDenominator;

    CVector3<float> vTangent( vQTex2[1] * vQ1[0] +
			      -vQTex1[1] * vQ2[0],
			      vQTex2[1] * vQ1[1] +
			      -vQTex1[1] * vQ2[1],
			      vQTex2[1] * vQ1[2] +
			      -vQTex1[1] * vQ2[2] );

    CVector3<float> vBitangent( -vQTex2[0] * vQ1[0] +
				vQTex1[0] * vQ2[0],
				-vQTex2[0] * vQ1[1] +
				vQTex1[0] * vQ2[1],
				-vQTex2[0] * vQ1[2] +
				vQTex1[0] * vQ2[2]) ;

    vTangent   *= fCoefficient;
    vBitangent *= fCoefficient;
    // Ensure that the vectors are orthogonal
    vTangent = vTangent - ( vNormal.Dot(vTangent) * vNormal);
    vBitangent = vBitangent - ( vNormal.Dot(vBitangent) * vNormal ) -
    ( vTangent.Dot(vBitangent)*vTangent );
    // construct the matrix
    mResult(0,0) = vTangent[0];
    mResult(0,1) = vTangent[1];
    mResult(0,2) = vTangent[2];
    mResult(1,0) = vBitangent[0];
    mResult(1,1) = vBitangent[1];
    mResult(1,2) = vBitangent[2];
    mResult(2,0) = vNormal[0];
    mResult(2,1) = vNormal[1];
    mResult(2,2) = vNormal[2];

  }

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CalculateTangentArray( const CVertexDescriptor &vertices,
				      const CVertexDescriptor &normals,
				      const CVertexDescriptor &texCoords,
				      const CIndexArray &indices,
				      CVertexDescriptor & tangents)
{

  // Verify that we're dealing with correct items.
  assert( indices.GetPrimitiveType() == PRIMITIVE_TRI_LIST );
  assert( vertices.GetType() == ELEMENT_TYPE_VERTEX_3F );
  assert( texCoords.GetType() == ELEMENT_TYPE_TEX_2F );
  assert( normals.GetType() == ELEMENT_TYPE_NORMAL_3F );
  assert( tangents.GetType() == ELEMENT_TYPE_ATTRIB_4F );
  assert( tangents.GetSize() == vertices.GetSize());

  float *pTangents = new float[vertices.GetSize()*6];
  float *pTangents2 = pTangents + (vertices.GetSize()*3);
  memset( pTangents, 0, sizeof(float)*vertices.GetSize()*6);

  unsigned int nTriCount = indices.GetNumIndices() / 3;

  int bUseShort = indices.IsShortIndices();
  unsigned int i1,i2,i3;
  unsigned int idx;
  CVector3<float> vTmp2;
  CVector3<float> v1,v2,v3;
  CVector2<float> w1,w2,w3;
  CVector3<float> vSdir,vTdir;
  CVector3<float> vTmp;
  CVector4<float> vTangent;
  for (unsigned int a = 0; a < nTriCount; a++)
  {
    idx = a*3;
    if ( bUseShort )
    {
      i1 = indices.GetPointer<unsigned short int>()[idx];
      i2 = indices.GetPointer<unsigned short int>()[idx+1];
      i3 = indices.GetPointer<unsigned short int>()[idx+2];
    }
    else
    {
      i1 = indices.GetPointer<unsigned int>()[idx];
      i2 = indices.GetPointer<unsigned int>()[idx+1];
      i3 = indices.GetPointer<unsigned int>()[idx+2];
    }
    v1.Set(&vertices.GetPointer<float>()[i1*3]);
    v2.Set(&vertices.GetPointer<float>()[i2*3]);
    v3.Set(&vertices.GetPointer<float>()[i3*3]);

    w1.Set(&texCoords.GetPointer<float>()[i1*2]);
    w2.Set(&texCoords.GetPointer<float>()[i2*2]);
    w3.Set(&texCoords.GetPointer<float>()[i3*2]);

    float x1 = v2[0] - v1[0];
    float x2 = v3[0] - v1[0];
    float y1 = v2[1] - v1[1];
    float y2 = v3[1] - v1[1];
    float z1 = v2[2] - v1[2];
    float z2 = v3[2] - v1[2];

    float s1 = w2[0] - w1[0];
    float s2 = w3[0] - w1[0];
    float t1 = w2[1] - w1[1];
    float t2 = w3[1] - w1[1];

    float r = 1.0f / (s1 * t2 - s2 * t1);
    vSdir[0] = (t2 * x1 - t1 * x2) * r;
    vSdir[1] = (t2 * y1 - t1 * y2) * r;
    vSdir[2] = (t2 * z1 - t1 * z2) * r;

    vTdir[0] = (s1 * x2 - s2 * x1) * r;
    vTdir[1] = (s1 * y2 - s2 * y1) * r;
    vTdir[2] = (s1 * z2 - s2 * z1) * r;

    //vTmp.UseExternalData( &pTangents[i1*3] );    vTmp += vSdir;
    //vTmp.UseExternalData( &pTangents[i2*3] );    vTmp += vSdir;
    //vTmp.UseExternalData( &pTangents[i3*3] );    vTmp += vSdir;
    pTangents[i1*3]   += vSdir[0];
    pTangents[i1*3+1] += vSdir[1];
    pTangents[i1*3+2] += vSdir[2];


    pTangents[i2*3]   += vSdir[0];
    pTangents[i2*3+1] += vSdir[1];
    pTangents[i2*3+2] += vSdir[2];


    pTangents[i3*3]   += vSdir[0];
    pTangents[i3*3+1] += vSdir[1];
    pTangents[i3*3+2] += vSdir[2];

    //vTmp.UseExternalData( &pTangents2[i1*3] );    vTmp += vTdir;
    //vTmp.UseExternalData( &pTangents2[i2*3] );    vTmp += vTdir;
    //vTmp.UseExternalData( &pTangents2[i3*3] );    vTmp += vTdir;

    pTangents2[i1*3]   += vTdir[0];
    pTangents2[i1*3+1] += vTdir[1];
    pTangents2[i1*3+2] += vTdir[2];

    pTangents2[i2*3]   += vTdir[0];
    pTangents2[i2*3+1] += vTdir[1];
    pTangents2[i2*3+2] += vTdir[2];

    pTangents2[i3*3]   += vTdir[0];
    pTangents2[i3*3+1] += vTdir[1];
    pTangents2[i3*3+2] += vTdir[2];

  }

  for (unsigned int a = 0; a < vertices.GetSize(); a++)
  {

    v1.Set( &normals.GetPointer<float>()[a*3]);
    v2.Set( &pTangents[a*3] );

    //vTangent.UseExternalData( &tangents.GetPointer<float>()[a*4]);

    // Gram-Schmidt orthogonalize
    vTmp2 = v2 - (v1 * (v1.Dot(v2)));
    vTmp2.Normalize();

    //vTangent[0] = vTmp2[0];
    //vTangent[1] = vTmp2[1];
    //vTangent[2] = vTmp2[2];

    tangents.GetPointer<float>()[a*4]   = vTmp2[0];
    tangents.GetPointer<float>()[a*4+1] = vTmp2[1];
    tangents.GetPointer<float>()[a*4+2] = vTmp2[2];

    // Calculate handedness
    vTmp.Set( &pTangents2[a*3] );
    //vTangent[3] = v1.Cross(v2).Dot( vTmp) < 0.0f ? -1.0f : 1.0f;
    tangents.GetPointer<float>()[a*4+3] = v1.Cross(v2).Dot( vTmp) < 0.0f ? -1.0f : 1.0f;
  }
  delete pTangents;
  pTangents = pTangents2 = NULL;
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Math::Det(const CMatrix4x4<float> & mMatrix)
{
  float fDet = 0.0;
  CMatrix3x3<float> m;
  RemoveRowAndColumn(mMatrix, 1,1, m); fDet += mMatrix.At(0,0) * Det(m);
  RemoveRowAndColumn(mMatrix, 1,2, m); fDet -= mMatrix.At(0,1) * Det(m);
  RemoveRowAndColumn(mMatrix, 1,3, m); fDet += mMatrix.At(0,2) * Det(m);
  RemoveRowAndColumn(mMatrix, 1,4, m); fDet -= mMatrix.At(0,3) * Det(m);
  return fDet;
}
////////////////////
/// Returns the determinant of the 3x3 float matrix.
float
Phoenix::Math::Det(const CMatrix3x3<float> & mMatrix)
{
  float fDet = 0.0;
  CMatrix2x2<float> m;
  RemoveRowAndColumn(mMatrix, 1,1,m); fDet += mMatrix.At(0,0) * Det(m);
  RemoveRowAndColumn(mMatrix, 1,2,m); fDet -= mMatrix.At(0,1) * Det(m);
  RemoveRowAndColumn(mMatrix, 1,3,m); fDet += mMatrix.At(0,2) * Det(m);
  return fDet;
}
////////////////////
/// Returns the determinant of the 2x2 float matrix.
float
Phoenix::Math::Det(const CMatrix2x2<float> & mMatrix)
{
  return mMatrix.At(0,0) * mMatrix.At(1,1) - mMatrix.At(0,1) * mMatrix.At(1,0);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CovarianceMatrix( const CVertexDescriptor &vertexDescriptor, CMatrix3x3<float> & mCovariance)
{


  /// Covariance matrix is valid only for 3d coordinates
  if ( (vertexDescriptor.GetType() != ELEMENT_TYPE_VERTEX_3F) &&
       (vertexDescriptor.GetType() != ELEMENT_TYPE_V3F_N3F_T2F) )
  {
    mCovariance.IdentityMatrix();
    return;
  }

  CVector3<float> vAveragePos(0.0f,0.0f,0.0f);
  mCovariance.Set(0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f );

  ////////////////////
  // Okay, type is correct.
  // Calculate average position
  for(unsigned int iVertComponent = 0;iVertComponent < vertexDescriptor.GetSize(); iVertComponent++)
  {
    vAveragePos[0] += vertexDescriptor.GetPointer<float>(iVertComponent)[0];
    vAveragePos[1] += vertexDescriptor.GetPointer<float>(iVertComponent)[1];
    vAveragePos[2] += vertexDescriptor.GetPointer<float>(iVertComponent)[2];
  }

  vAveragePos /= static_cast<float>(vertexDescriptor.GetSize());

  // Calculate Covariance matrix
  for(unsigned int iVertComponent = 0;iVertComponent < vertexDescriptor.GetSize(); iVertComponent++)
  {
    float fTmpX = vertexDescriptor.GetPointer<float>(iVertComponent)[0] - vAveragePos[0];
    float fTmpY = vertexDescriptor.GetPointer<float>(iVertComponent)[1] - vAveragePos[1];
    float fTmpZ = vertexDescriptor.GetPointer<float>(iVertComponent)[2] - vAveragePos[2];

    mCovariance(0,0) += fTmpX * fTmpX;
    mCovariance(1,1) += fTmpY * fTmpY;
    mCovariance(2,2) += fTmpZ * fTmpZ;
    mCovariance(0,1) = mCovariance(1,0) += fTmpX * fTmpY;
    mCovariance(0,2) = mCovariance(2,0) += fTmpX * fTmpZ;
    mCovariance(1,2) = mCovariance(2,1) += fTmpY * fTmpZ;

  }
  mCovariance /= static_cast<float>(vertexDescriptor.GetSize());
  //return mCovariance;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CovarianceMatrix(  const CVertexDescriptor &vertexDescriptor, const CIndexArray &indices, CMatrix3x3<float> & mCovariance )
{

  /// Covariance matrix is valid only for 3d coordinates
  if ( (vertexDescriptor.GetType()  != ELEMENT_TYPE_VERTEX_3F) &&
       (vertexDescriptor.GetType()  != ELEMENT_TYPE_V3F_N3F_T2F) )
  {
    mCovariance.IdentityMatrix();
    return;
  }

  CVector3<float> vAveragePos(0.0f,0.0f,0.0f);
  unsigned int nVertexIndex;
  unsigned short int *m_pIndexShortArray = NULL;
  unsigned int *m_pIndexIntArray = NULL;
  mCovariance.Set(0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f );

  if ( indices.IsShortIndices())
    m_pIndexShortArray = indices.GetPointer<unsigned short int>();
  else
    m_pIndexIntArray = indices.GetPointer<unsigned int>();

  // Calculate average position
  for(unsigned int nIndex = 0;nIndex < indices.GetNumIndices(); nIndex++)
  {
    // The index points to and triplet of three floats, which are
    // stored continuously into vertexdescriptor.
    if ( indices.IsShortIndices() )
      nVertexIndex = m_pIndexShortArray[nIndex]; //* 3;
    else
      nVertexIndex = m_pIndexIntArray[nIndex];// * 3;

    vAveragePos[0] += vertexDescriptor.GetPointer<float>(nVertexIndex)[0];
    vAveragePos[1] += vertexDescriptor.GetPointer<float>(nVertexIndex)[1];
    vAveragePos[2] += vertexDescriptor.GetPointer<float>(nVertexIndex)[2];
  }

  vAveragePos /= static_cast<float>(indices.GetNumIndices());

  float fTmpX;
  float fTmpY;
  float fTmpZ;

  // Calculate Covariance matrix
  for(unsigned int nIndex = 0;nIndex < indices.GetNumIndices(); nIndex++)
  {

    if ( indices.IsShortIndices() )
      nVertexIndex = m_pIndexShortArray[nIndex];// * 3;
    else
      nVertexIndex = m_pIndexIntArray[nIndex];// * 3;

    fTmpX = vertexDescriptor.GetPointer<float>(nVertexIndex)[0] - vAveragePos[0];
    fTmpY = vertexDescriptor.GetPointer<float>(nVertexIndex)[1] - vAveragePos[1];
    fTmpZ = vertexDescriptor.GetPointer<float>(nVertexIndex)[2] - vAveragePos[2];

    mCovariance(0,0) += fTmpX * fTmpX;
    mCovariance(1,1) += fTmpY * fTmpY;
    mCovariance(2,2) += fTmpZ * fTmpZ;
    mCovariance(0,1) = mCovariance(1,0) += fTmpX * fTmpY;
    mCovariance(0,2) = mCovariance(2,0) += fTmpX * fTmpZ;
    mCovariance(1,2) = mCovariance(2,1) += fTmpY * fTmpZ;

  }
  mCovariance /= static_cast<float>(indices.GetNumIndices());

  //return mCovariance;
}
/////////////////////////////////////////////////////////////////
#define SIGN(VAL) (VAL < 0.0F ? -1.0F : 1.0F)
void
Phoenix::Math::CalculateEigensystem( CMatrix3x3<float> &mMatrix, float &fLambda1, float &fLambda2, float &fLambda3, CMatrix3x3<float> &mRes)
{
  const float fEpsilon = 1.0e-10F;
  const int nMaxSweeps = 32;

  float m11 = mMatrix(0,0);
  float m12 = mMatrix(0,1);
  float m13 = mMatrix(0,2);
  float m22 = mMatrix(1,1);
  float m23 = mMatrix(1,2);
  float m33 = mMatrix(2,2);

  mRes.IdentityMatrix();

  for(int a=0;a<nMaxSweeps;a++)
  {
    //Core::CLogger::Log() << "loop" << a << std::endl;
    // Exit if off-diagonal entries are small enough
    if ( (fabs(m12) < fEpsilon) &&
	 (fabs(m13) < fEpsilon) &&
	 (fabs(m23) < fEpsilon)) break;

    if ( m12 != 0.0F)
    {

      float u = (m22 - m11 ) * 0.5F / m12;
      float u2 = u * u;
      float u2p1 = u2 + 1.0F;
      float t = (u2p1 != u2 ) ? SIGN(u) * (sqrt(u2p1) - fabs(u)) : 0.5F / u;

      float c = 1.0F / sqrt(t * t + 1.0F);
      float s = c * t;

      m11 -= t * m12;
      m22 += t * m12;
      m12 =  0.0 ;

      float temp = (c * m13) - (s * m23);
      m23 = (s * m13) + (c * m23);
      m13 = temp;

      for(int i=0;i<3;i++)
      {
	float tmp = (c * mRes(i,0)) - (s * mRes(i,1));
	mRes(i,1) = (s * mRes(i,0)) + (c * mRes(i,1));
	mRes(i,0) = tmp;
      }

    }

    if ( m13 != 0.0F )
    {

      float u = (m33 - m11 ) * 0.5F / m13;
      float u2 = u * u;
      float u2p1 = u2 + 1.0F;
      float t = (u2p1 != u2 ) ? SIGN(u) * (sqrt(u2p1) - fabs(u)) : 0.5F / u;

      float c = 1.0F / sqrt((t * t) + 1.0F);
      float s = c * t;


      m11 -= t * m13;
      m33 += t * m13;
      m13  = 0.0;

      float temp = (c * m12) - (s * m23);
      m23 = (s * m12) + (c * m23);
      m12 = temp;

      for(int i=0;i<3;i++)
      {
	float tmp = (c * mRes(i,0)) - (s * mRes(i,2));
	mRes(i,2) = (s * mRes(i,0)) + (c * mRes(i,2));
	mRes(i,0) = tmp;
      }
    }

    if ( m23 != 0.0F )
    {
      float u = (m33 - m22 ) * 0.5F / m23;
      float u2 = u * u;
      float u2p1 = u2 + 1.0F;
      float t = ( u2p1 != u2 ) ? SIGN(u) * (sqrt(u2p1) - fabs(u)) : 0.5F / u;

      float c = 1.0F / sqrt(t * t + 1.0F);
      float s = c * t;


      m22 -= t * m23;
      m33 += t * m23;
      m23  = 0.0;

      float temp = (c * m12) - (s * m13);
      m13 = (s * m12) + (c * m13);
      m12 = temp;

      for(int i=0;i<3;i++)
      {
	float tmp = (c * mRes(i,1)) - (s * mRes(i,2));
	mRes(i,2) = (s * mRes(i,1)) + (c * mRes(i,2));
	mRes(i,1) = tmp;
      }
    }
  } //  for(int a=0;a<nMaxSweeps;a++)

  fLambda1 = m11;
  fLambda2 = m22;
  fLambda3 = m33;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::SetRowVector(CMatrix3x3<float> &mMatrix, unsigned int iRow, const CVector3<float> & vVector)
{

  float *pArray = mMatrix.GetArray();
  unsigned int nIndex = (iRow % 3) * 3;

  pArray[nIndex]   = vVector[0];
  pArray[nIndex+1] = vVector[1];
  pArray[nIndex+2] = vVector[2];

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::SetColumnVector( CMatrix3x3<float> &mMatrix, unsigned int iCol, const CVector3<float> & vVector)
{

  float *pArray = mMatrix.GetArray();
  unsigned int nIndex = iCol % 3;
  pArray[nIndex]   = vVector[0];
  pArray[nIndex+3] = vVector[1];
  pArray[nIndex+6] = vVector[2];

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::GetRowVector( const CMatrix3x3<float> & mMatrix, unsigned int iRow, CVector3<float> & result)
{
  result[0] = mMatrix(iRow, 0);
  result[1] = mMatrix(iRow, 1);
  result[2] = mMatrix(iRow, 2);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::GetColumnVector(const CMatrix3x3<float> & mMatrix, unsigned int iCol, CVector3<float> & result )
{
  result[0] = mMatrix(0, iCol);
  result[1] = mMatrix(1, iCol);
  result[2] = mMatrix(2, iCol);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::Translate(const CVector3<float> & v, const CMatrix4x4<float> & m, CVector3<float> & result)
{
  result[0] = v[0]+m(0,3);
  result[1] = v[1]+m(1,3);
  result[2] = v[2]+m(2,3);

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::TranslateInverse(const CVector3<float> & v, const CMatrix4x4<float> & m, CVector3<float> & result)
{
  result[0] = v[0]-m(0,3);
  result[1] = v[1]-m(1,3);
  result[2] = v[2]-m(2,3);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::Rotate( const CVector3<float> & v, const CMatrix4x4<float> &m, CVector3<float> & result)
{
  result[0] = m.At(0,0) * v[0] + m.At(0,1) * v[1] + m.At(0,2) * v[2];
  result[1] = m.At(1,0) * v[0] + m.At(1,1) * v[1] + m.At(1,2) * v[2];
  result[2] = m.At(2,0) * v[0] + m.At(2,1) * v[1] + m.At(2,2) * v[2];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::Transform( const CVector3<float> &v, const CMatrix4x4<float> &m, CVector3<float> & result)
{
#define T_X    v[0]
#define T_Y    v[1]
#define T_Z    v[2]
#define m00  m.At(0,0)
#define m01  m.At(0,1)
#define m02  m.At(0,2)
#define m03  m.At(0,3)
#define m10  m.At(1,0)
#define m11  m.At(1,1)
#define m12  m.At(1,2)
#define m13  m.At(1,3)
#define m20  m.At(2,0)
#define m21  m.At(2,1)
#define m22  m.At(2,2)
#define m23  m.At(2,3)

  result[0] = m00*T_X + m01*T_Y + m02*T_Z + m03;
  result[1] = m10*T_X + m11*T_Y + m12*T_Z + m13;
  result[2] = m20*T_X + m21*T_Y + m22*T_Z + m23;
#undef T_X
#undef T_Y
#undef T_Z
#undef m00
#undef m01
#undef m02
#undef m03
#undef m10
#undef m11
#undef m12
#undef m13
#undef m20
#undef m21
#undef m22
#undef m23
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Math::Transform( const CVector3<float> &v, const CTransform &t, CVector3<float> & result)
{
	result = v;
	RotateVector(t.GetRotation(), result);
	result+=t.GetTranslation();
	result*=t.GetScaling();
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::MultiplyFromRight( const CMatrix3x3<float> & mMatrix, const CVector3<float> &vVector, CVector3<float> &result)
{

  result[0] = mMatrix(0,0) * vVector[0] +  mMatrix(0,1) * vVector[1] + mMatrix(0,2) * vVector[2];
  result[1] = mMatrix(1,0) * vVector[0] +  mMatrix(1,1) * vVector[1] + mMatrix(1,2) * vVector[2];
  result[2] = mMatrix(2,0) * vVector[0] +  mMatrix(2,1) * vVector[1] + mMatrix(2,2) * vVector[2];

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationMatrixToQuaternion( const CMatrix4x4<float> &mMatrix, CQuaternion & qRetval )
{

  //CQuaternion qRetval;
  float fTrace = mMatrix.Trace();
  float fS = 0.0f;
  float f1DivS = 0.0f;
  const CMatrix4x4<float> &m = mMatrix;

  if ( fTrace > 0.000001f )  // Greater than zero
  {

    fS = 2.0f * sqrt(fTrace);
    f1DivS = 1.0f / fS;
    qRetval[3] = 0.25f * fS;

    qRetval[0] = ( m.At(2,1) - m.At(1,2) ) * f1DivS;
    qRetval[1] = ( m.At(0,2) - m.At(2,0) ) * f1DivS;
    qRetval[2] = ( m.At(1,0) - m.At(0,1) ) * f1DivS;

  } else {

    // determine column where diagonal element has the greatest value
    unsigned int nColumnWithGreatestValue = 0;

    if ( m.At(0,0) > m.At(1,1)) {
      if ( m.At(0,0) > m.At(2,2) ) {
	nColumnWithGreatestValue = 0;
      } else {
	nColumnWithGreatestValue = 2;
      }
    } else {

      if ( m.At(1,1) > m.At(2,2) ) {
	nColumnWithGreatestValue = 1;
      } else {
	nColumnWithGreatestValue = 2;
      }
    }

    switch( nColumnWithGreatestValue ){
    case 0:
      fS  = sqrtf( 1.0f + m.At(0,0) - m.At(1,1) - m.At(2,2) ) * 2.0f;
      qRetval[0] = 0.25f * fS;
      qRetval[1] = (m.At(1,0) + m.At(0,1) ) * f1DivS;
      qRetval[2] = (m.At(0,2) + m.At(2,0) ) * f1DivS;
      qRetval[3] = (m.At(2,1) - m.At(1,2) ) * f1DivS;
      break;
    case 1:
      fS  = sqrtf( 1.0f + m.At(1,1) - m.At(0,0) - m.At(2,2) ) * 2.0f;
      qRetval[0] = (m.At(1,0) + m.At(0,1) ) * f1DivS;
      qRetval[1] = 0.25f * fS;
      qRetval[2] = (m.At(2,1) + m.At(1,2) ) * f1DivS;
      qRetval[3] = (m.At(0,2) - m.At(2,0) ) * f1DivS;
      break;
    case 2:
      fS  = sqrtf( 1.0f + m.At(2,2) - m.At(0,0) - m.At(1,1) ) * 2.0f;
      qRetval[0] = (m.At(0,2) + m.At(2,0) ) * f1DivS;
      qRetval[1] = (m.At(2,1) + m.At(1,2) ) * f1DivS;
      qRetval[2] = 0.25f * fS;
      qRetval[3] = (m.At(1,0) + m.At(0,1) ) * f1DivS;
      break;
    default:
      break;
    }

  }
  //return qRetval;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotateVector( const CQuaternion &qRotation, CVector3<float> &vVector)
{
  CQuaternion qVect(vVector[0],vVector[1],vVector[2],0.0f);
  CQuaternion qResult = qRotation * qVect * qRotation.GetInverse();
  vVector[0] = qResult[0];
  vVector[1] = qResult[1];
  vVector[2] = qResult[2];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotateAroundPoint( const CVector3<float> & vVector,
				  const CVector3<float> & vPoint,
				  const CQuaternion & ref,
				  CVector3<float> &result )
{
  // CVector3<float> vTemp = (vVector - vPoint);
//   Phoenix::Math::RotateVector( ref, vTemp );
//   return vTemp + vPoint;
  result = vVector-vPoint;
  Phoenix::Math::RotateVector( ref, result );
  result += vPoint;
}
/////////////////////////////////////////////////////////////////
// As described in MatrixFAQ:
//
//     cos_angle  = qr -> qw;
//     angle      = acos( cos_angle ) * 2 * RADIANS;
//     sin_angle  = sqrt( 1.0 - cos_angle * cos_angle );
//     if ( fabs( sin_angle ) < 0.0005 )
//       sin_angle = 1;
//     axis -> vx = qr -> qx / sin_angle;
//     axis -> vy = qr -> qy / sin_angle;
//     axis -> vz = qr -> qz / sin_angle;
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::QuaternionToRotationAxisAndAngle( const CQuaternion &qQuat,
						 CVector3<float> &vAxis,
						 float &fAngleInDegrees )
{
  float fCosAngle = qQuat[4];
  float fSinAngle = sqrt( 1.0f - fCosAngle * fCosAngle );

  /// If denominator is too close to zero.
  if ( fabsf(fSinAngle < EPSILON)) fSinAngle = 1;
  /// The axis.
  float f1DivSinAngle = 1.0f / fSinAngle;
  vAxis[0] = qQuat[0] * f1DivSinAngle;
  vAxis[1] = qQuat[1] * f1DivSinAngle;
  vAxis[2] = qQuat[2] * f1DivSinAngle;
  /// The angle
  fAngleInDegrees = Rad2Deg(acos( fCosAngle )) * 2.0f;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RemoveRowAndColumn(const CMatrix4x4<float> & mMatrix,
				  unsigned int iRowSkip,
				  unsigned int iColSkip,
				  CMatrix3x3<float> & mResult)
{

    //CMatrix3x3<float> mResult;
    unsigned int iResultRow = 0;
    unsigned int iResultCol = 0;

    for( unsigned int iRow = 0;iRow < 4;iRow++)
    {
        if ( iRow == iRowSkip ){}
        else
        {
            iResultCol = 0;
            for( unsigned int iCol = 0;iCol < 4;iCol++)
            {

                if ( iCol == iColSkip ){}
                else
                {
                    mResult(iResultRow, iResultCol) = mMatrix(iRow,iCol);
                    iResultCol++;
                }

            }
            iResultRow++;
        }
    }
    //return mResult;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RemoveRowAndColumn( const CMatrix3x3<float> & mMatrix,
				   unsigned int iRowSkip,
				   unsigned int iColSkip,
				   CMatrix2x2<float> & mResult)
{

    //CMatrix2x2<float> mResult;
    unsigned int iResultRow = 0;
    unsigned int iResultCol = 0;

    for( unsigned int iRow = 0;iRow < 3;iRow++)
    {
        if ( iRow == iRowSkip ){}
        else
        {
            iResultCol = 0;
            for( unsigned int iCol = 0;iCol < 3;iCol++)
            {

                if ( iCol == iColSkip ){}
                else
                {
                    mResult(iResultRow, iResultCol) = mMatrix(iRow,iCol);
                    iResultCol++;
                }

            }
            iResultRow++;
        }
    }
    //return mResult;
}
/////////////////////////////////////////////////////////////////
#define MATRIX_NOT_INVERTIBLE 1
#define MATRIX_INVERTIBLE     0
/////////////////////////////////////////////////////////////////
int
Phoenix::Math::InverseMatrix( CMatrix2x2<float> mOrig, CMatrix2x2<float> &mInverse)
{
#define MATRIX_NOT_INVERTIBLE 1
#define MATRIX_INVERTIBLE     0
  mInverse.IdentityMatrix();
  unsigned int iRow, iCol, iPivot;
  float fScale, fMultValue;
  iCol = 0;
  iPivot = 0;
  ////////////////////
  /// for each column
  for( iCol=0;iCol<2;iCol++)
  {
    // Find largest absolute value from current column.
    for( iPivot = iCol, iRow = iCol;iRow<2; iRow++ )
    {
      if ( fabs(mOrig(iRow,iCol)) > fabs(mOrig(iPivot, iCol))) { iPivot = iRow; }

    } // For each row in iCol
    ////////////////////
    /// If current value is zero, matrix cannot be inverted.
    if ( TOO_CLOSE_TO_ZERO(mOrig(iPivot, iCol)))
    {
      //std::cerr << "Matrix is not invertible!" << std::endl;
      return MATRIX_NOT_INVERTIBLE;
    }
    ////////////////////
    if ( iPivot != iCol )
    {
      // Swap rows
      mOrig.SwapRows(    iPivot, iCol );
      mInverse.SwapRows( iPivot, iCol );
    }
    ////////////////////
    /// Set element (iCol,iCol) to 1
    if ( mOrig(iCol,iCol) != 1.0f)
    {
      fMultValue = 1.0f/mOrig(iCol,iCol);
      mOrig.MultiplyRowBy( iCol, fMultValue   );
      mInverse.MultiplyRowBy( iCol, fMultValue );
    }

    ////////////////////
    /// subtract this row from others to make rest of column iCol zero
    for(unsigned int iR=0;iR<2;iR++)
    {
      /// skip diagonal element
      if ( (iR != iCol) && !TOO_CLOSE_TO_ZERO(mOrig(iR,iCol)))
      {
	/// Add -M_{rj} x M(iR,*) to M(iCol, *)
	fScale = -mOrig(iR, iCol);
	for( unsigned int iTmpCol=0;iTmpCol<2;iTmpCol++)
	{
	  mOrig(iR,iTmpCol)    += fScale * mOrig(iCol,iTmpCol);
	  mInverse(iR,iTmpCol) += fScale * mInverse(iCol,iTmpCol);
	}
      }
    } // for each row
  } // for each column
  // At this point, matrix has been completely inverted and all is good.
  return MATRIX_INVERTIBLE;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Math::InverseMatrix( CMatrix3x3<float> mOrig, CMatrix3x3<float> &mInverse)
{

  mInverse.IdentityMatrix();
  unsigned int iRow, iCol, iPivot;
  float fScale, fMultValue;
  iCol = 0;
  iPivot = 0;
  ////////////////////
  /// for each column
  for( iCol=0;iCol<3;iCol++)
  {
    // Find largest absolute value from current column.
    for( iPivot = iCol, iRow = iCol;iRow<3; iRow++ )
    {
      if ( fabs(mOrig(iRow,iCol)) > fabs(mOrig(iPivot, iCol))) { iPivot = iRow; }

    } // For each row in iCol
    ////////////////////
    /// If current value is zero, matrix cannot be inverted.
    if ( TOO_CLOSE_TO_ZERO(mOrig(iPivot, iCol)))
    {
      //std::cerr << "Matrix is not invertible!" << std::endl;
      return MATRIX_NOT_INVERTIBLE;
    }
    ////////////////////
    if ( iPivot != iCol )
    {
      // Swap rows
      mOrig.SwapRows(    iPivot, iCol );
      mInverse.SwapRows( iPivot, iCol );
    }


    ////////////////////
    /// Set element (iCol,iCol) to 1
    if ( mOrig(iCol,iCol) != 1.0f)
    {
      fMultValue = 1.0f/mOrig(iCol,iCol);
      mOrig.MultiplyRowBy( iCol, fMultValue   );
      mInverse.MultiplyRowBy( iCol, fMultValue );
    }

    ////////////////////
    /// subtract this row from others to make rest of column iCol zero
    for(unsigned int iR=0;iR<3;iR++)
    {
      /// skip diagonal element
      if ( (iR != iCol) && !TOO_CLOSE_TO_ZERO(mOrig(iR,iCol)))
      {
	/// Add -M_{rj} x M(iR,*) to M(iCol, *)
	fScale = -mOrig(iR, iCol);
	for( unsigned int iTmpCol=0;iTmpCol<3;iTmpCol++)
	{
	  mOrig(iR,iTmpCol)    += fScale * mOrig(iCol,iTmpCol);
	  mInverse(iR,iTmpCol) += fScale * mInverse(iCol,iTmpCol);
	}
      }
    } // for each row
  } // for each column

  // At this point, matrix has been completely inverted and all is good.
  return MATRIX_INVERTIBLE;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Math::InverseMatrix( CMatrix4x4<float> mOrig, CMatrix4x4<float> &mInverse)
{
  mInverse.IdentityMatrix();
  unsigned int iRow, iCol, iPivot;
  float fScale, fMultValue;
  iCol = 0;
  iPivot = 0;
  ////////////////////
  /// for each column
  for( iCol=0;iCol<4;iCol++)
  {
    // Find largest absolute value from current column.
    for( iPivot = iCol, iRow = iCol;iRow<4; iRow++ )
    {
      if ( fabs(mOrig(iRow,iCol)) > fabs(mOrig(iPivot, iCol))) { iPivot = iRow; }

    } // For each row in iCol
    ////////////////////
    /// If current value is zero, matrix cannot be inverted.
    if ( TOO_CLOSE_TO_ZERO(mOrig(iPivot, iCol)))
    {
      //std::cerr << "Matrix is not invertible!" << std::endl;
      return MATRIX_NOT_INVERTIBLE;
    }

    ////////////////////
    if ( iPivot != iCol )
    {
      // Swap rows
      mOrig.SwapRows(    iPivot, iCol );
      mInverse.SwapRows( iPivot, iCol );
    }


    ////////////////////
    /// Set element (iCol,iCol) to 1
    if ( mOrig(iCol,iCol) != 1.0f)
    {
      fMultValue = 1.0f/mOrig(iCol,iCol);
      mOrig.MultiplyRowBy( iCol, fMultValue   );
      mInverse.MultiplyRowBy( iCol, fMultValue );
    }

    ////////////////////
    /// subtract this row from others to make rest of column iCol zero
    for(unsigned int iR=0;iR<4;iR++)
    {
      /// skip diagonal element
      if ( (iR != iCol) && !TOO_CLOSE_TO_ZERO(mOrig(iR,iCol)))
      {
	/// Add -M_{rj} x M(iR,*) to M(iCol, *)
	fScale = -mOrig(iR, iCol);
	for( unsigned int iTmpCol=0;iTmpCol<4;iTmpCol++)
	{
	  mOrig(iR,iTmpCol)    += fScale * mOrig(iCol,iTmpCol);
	  mInverse(iR,iTmpCol) += fScale * mInverse(iCol,iTmpCol);
	}
      }
    } // for each row
  } // for each column
  // At this point, matrix has been completely inverted and all is good.
  return MATRIX_INVERTIBLE;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::RotationArc( CVector3<float> v0, CVector3<float> v1, Phoenix::Math::CQuaternion & result)
{
  v0.Normalize();
  v1.Normalize();
  CVector3<float> vCross = v0.Cross(v1);

  float  fDot = v0.Dot(v1);
  float  fS   = sqrtf((1.0f+fDot)*2.0f);
  float  f1DivS = 1.0f / fS;

  result[0] = vCross[0] * f1DivS;
  result[1] = vCross[1] * f1DivS;
  result[2] = vCross[2] * f1DivS;
  result[3] = fS * 0.5f;

}
/////////////////////////////////////////////////////////////////
float
Phoenix::Math::AngleBetweenVectors( const CVector3<float> &vVect1, const CVector3<float> &vVect2)
{
  float fDot = vVect1.Dot(vVect2);
  float fLengthMult = vVect1.Length() * vVect2.Length();
  float fAngle = 0.0f;

  if ( fLengthMult != 0.0f )
  {
    fAngle = acos( fDot / fLengthMult );
    // sanity check
    if ( isnan(fAngle) ) fAngle = 0.0f;
  }
  return fAngle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CVector4<float>
Phoenix::Math::operator*( const CMatrix4x4<float> &mMatrix, const CVector4<float> &vVector )
{
  return CVector4<float>( mMatrix.At(0,0) * vVector[0] + mMatrix.At(0,1) * vVector[1] + mMatrix.At(0,2) * vVector[2] + mMatrix.At(0,3) * vVector[3],
			  mMatrix.At(1,0) * vVector[0] + mMatrix.At(1,1) * vVector[1] + mMatrix.At(1,2) * vVector[2] + mMatrix.At(1,3) * vVector[3],
			  mMatrix.At(2,0) * vVector[0] + mMatrix.At(2,1) * vVector[1] + mMatrix.At(2,2) * vVector[2] + mMatrix.At(2,3) * vVector[3],
			  mMatrix.At(3,0) * vVector[0] + mMatrix.At(3,1) * vVector[1] + mMatrix.At(3,2) * vVector[2] + mMatrix.At(3,3) * vVector[3] );
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CVector3<float>
Phoenix::Math::operator*( const CMatrix3x3<float> &mMatrix, const CVector3<float> &vVector )
{
  return CVector3<float>( mMatrix.At(0,0) * vVector[0] + mMatrix.At(0,1) * vVector[1] + mMatrix.At(0,2) * vVector[2],
			  mMatrix.At(1,0) * vVector[0] + mMatrix.At(1,1) * vVector[1] + mMatrix.At(1,2) * vVector[2],
			  mMatrix.At(2,0) * vVector[0] + mMatrix.At(2,1) * vVector[1] + mMatrix.At(2,2) * vVector[2] );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::OrientedBoxAxisToRotationMatrix ( const Phoenix::Volume::COrientedBox &ob, Phoenix::Math::CMatrix4x4<float> & matrix  )
{
   /*matrix.Set( ob.GetForwardVector()[0], ob.GetRightVector()[0], ob.GetUpVector()[0], 0.0,
			   ob.GetForwardVector()[1], ob.GetRightVector()[1], ob.GetUpVector()[1], 0.0,
			   ob.GetForwardVector()[2], ob.GetRightVector()[2], ob.GetUpVector()[2], 0.0,
			   0.0,                      0.0,          			 0.0,                 1.0 );*/
   matrix.Set( ob.GetForwardVector()[0], 	ob.GetForwardVector()[1], 	ob.GetForwardVector()[2], 	0.0,
			   ob.GetUpVector()[0], 		ob.GetUpVector()[1], 		ob.GetUpVector()[2], 		0.0,
			   ob.GetRightVector()[0], 		ob.GetRightVector()[1], 	ob.GetRightVector()[2], 	0.0,
				0.0f, 0.0f, 0.0f, 1.0f );
}
/////////////////////////////////////////////////////////////////
