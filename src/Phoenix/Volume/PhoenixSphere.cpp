#include "PhoenixSphere.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixGlobals.h"
#include "PhoenixMath.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere
Phoenix::Volume::CalculateBoundingSphere( const Phoenix::Graphics::CVertexDescriptor &vd )
{
  if ( (vd.GetType() != ELEMENT_TYPE_VERTEX_3F) &&
       (vd.GetType() != ELEMENT_TYPE_V3F_N3F_T2F) )
  {
    return CSphere(CVector3<float>(0,0,0), 0.0f);
  }

  // Minimum and maximum values for each coordinates.
  CVector3<float> vMaxValues;
  CVector3<float> vMinValues;
  size_t nNumVertices = vd.GetSize();



  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    float *pVertices = vd.GetPointer<float>(v);
    float fTempX = pVertices[0];
    float fTempY = pVertices[1];
    float fTempZ = pVertices[2];

    if ( v == 0 )
    {
      vMaxValues[0] = vMinValues[0] = fTempX;
      vMaxValues[1] = vMinValues[1] = fTempY;
      vMaxValues[2] = vMinValues[2] = fTempZ;
    }
    else
    {
      STORE_MAX_MIN(fTempX, vMaxValues[0], vMinValues[0]);
      STORE_MAX_MIN(fTempY, vMaxValues[1], vMinValues[1]);
      STORE_MAX_MIN(fTempZ, vMaxValues[2], vMinValues[2]);
    }
  }

  CSphere sphere;
  sphere.SetPosition( (vMaxValues + vMinValues ) / 2 );
  CVector3<float> vDist = vMaxValues - vMinValues;

  float fMaxRadius = 0.0;

  if ( fabs(vDist[0]) > fabs(vDist[1])){

    if ( fabs(vDist[0]) > fabs(vDist[2]) ){
      fMaxRadius = vDist[0] * 0.5f;
    } else {
      fMaxRadius = vDist[2] * 0.5f;
    }

  } else {

    if ( fabs(vDist[1]) > fabs(vDist[2]) ){
      fMaxRadius = vDist[1] * 0.5f;
    } else {
      fMaxRadius = vDist[2] * 0.5f;
    }

  }
  sphere.SetRadius( fMaxRadius  );
  return sphere;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere
Phoenix::Volume::CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vd )
{

  // The returned sphere

  if ( (vd.GetType() != ELEMENT_TYPE_VERTEX_3F) &&
       (vd.GetType() != ELEMENT_TYPE_V3F_N3F_T2F))
  {
    return CSphere( CVector3<float>(0,0,0), 0.0f);
  }
  CSphere sphere;
  size_t nNumVertices = vd.GetSize();

  float fLambda1,fLambda2,fLambda3;

  // Calculate the principal axis R by component analysis
  CMatrix3x3<float> mCovar;
  CovarianceMatrix( vd, mCovar );
  CMatrix3x3<float> mEigenVectorMatrix;
  mEigenVectorMatrix.IdentityMatrix();
  CalculateEigensystem( mCovar,
			fLambda1, fLambda2,fLambda3,
			mEigenVectorMatrix);

  CVector3<float> vR;
  GetColumnVector(mEigenVectorMatrix, 0, vR);
  vR.Normalize();
  // Get min and max values of each point in direction R
  CVector3<float> vTemp;

  float fMinExtent, fMaxExtent, fTempDotR;
  unsigned int nMaxIndex = 0, nMinIndex = 0;

  // Initialize the first values
  vTemp.Set( &(vd.GetPointer<float>()[0]));

  fMinExtent = fMaxExtent = vTemp.Dot(vR);
  CVector3<float> vMin = vTemp;
  CVector3<float> vMax = vTemp;

  // for each vertex
  for ( unsigned int v = 1;v<nNumVertices; v++)
  {

    vTemp.Set( vd.GetPointer<float>(v) );
    fTempDotR = vTemp.Dot( vR );

    if ( fTempDotR > fMaxExtent ){

      fMaxExtent = fTempDotR;
      nMaxIndex = v;

    } else if (fTempDotR < fMinExtent){

      fMinExtent = fTempDotR;
      nMinIndex = v;
    }

  }
  // Assign initial center and radius
  vMin.Set( vd.GetPointer<float>(nMinIndex) );
  vMax.Set( vd.GetPointer<float>(nMaxIndex) );

  //sphere.SetPosition( (vMin + vMax ) * 0.5f );

  CVector3<float> vPos = vMin + ((vMax-vMin) * 0.5f);
  sphere.SetPosition( vPos );
  sphere.SetRadius((vMax - sphere.GetPosition()).Length());

  // Check each vertex that they actually are stored inside the sphere
  float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();

  // Temporary point when determining new center and radius
  CVector3<float> vGPoint;

  // The vector from center of the sphere to the current vertex
  CVector3<float> vCenterToPoint;

  // For each vertex
  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    vTemp.Set( vd.GetPointer<float>(v) );

    float fDist = ((vTemp - sphere.GetPosition()).Length());
    float fDistSquared = fDist * fDist;

    if (  fDistSquared > fRadiusSquared )
    {
      // Point lies outside sphere, adjust center and radius
      vCenterToPoint = (vTemp - sphere.GetPosition());
      vCenterToPoint.Normalize();
      vGPoint = sphere.GetPosition() - (sphere.GetRadius() * vCenterToPoint );
      //sphere.SetPosition( vGPoint + (( vTemp-vGPoint) * 0.5));
      sphere.SetPosition( (vGPoint + vTemp) * 0.5f);
      sphere.SetRadius( (vTemp - sphere.GetPosition() ).Length());

      fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
    }
    else
    {
      // Point within sphere, everything is good in
      // the hood tonite.
    }
  } // for each vertex
  // Return the tighter sphere
  return sphere;
}

/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere
Phoenix::Volume::CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor & vd, const CIndexArray &indices )
{

	 if ( (vd.GetType() != ELEMENT_TYPE_VERTEX_3F) &&
	       (vd.GetType() != ELEMENT_TYPE_V3F_N3F_T2F))
	 {
	    return CSphere( CVector3<float>(0,0,0), 0.0f);
	 }

  // The returned sphere
  CSphere sphere;

  float fLambda1,fLambda2,fLambda3;

  // Calculate the principal axis R by component analysis
  CMatrix3x3<float> mCovar;
  Math::CovarianceMatrix( vd, indices , mCovar );
  CMatrix3x3<float> mEigenVectorMatrix;
  mEigenVectorMatrix.IdentityMatrix();

  CalculateEigensystem( mCovar,
			      fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);

  CVector3<float> vR;
  GetColumnVector(mEigenVectorMatrix, 0, vR);
  vR.Normalize();
  // Get min and max values of each point in direction R
  CVector3<float> vTemp;

  float fMinExtent, fMaxExtent, fTempDotR;
  unsigned int nMaxIndex = 0, nMinIndex = 0;

  // Initialize the first values
  vTemp.Set( (vd.GetPointer<float>(  		indices.GetPointer<unsigned short>()[0]   		)));

  fMinExtent = fMaxExtent = vTemp.Dot(vR);
  CVector3<float> vMin = vTemp;
  CVector3<float> vMax = vTemp;

  // for each vertex
  for ( unsigned int i = 1;i<indices.GetNumIndices(); i++)
  {

    vTemp.Set( (vd.GetPointer<float>(indices.GetPointer<unsigned short>()[i])));
    fTempDotR = vTemp.Dot( vR );

    if ( fTempDotR > fMaxExtent ){

      fMaxExtent = fTempDotR;
      nMaxIndex = indices.GetPointer<unsigned short>()[i];

    } else if (fTempDotR < fMinExtent){

      fMinExtent = fTempDotR;
      nMinIndex = indices.GetPointer<unsigned short>()[i];
    }

  }
  // Assign initial center and radius
  vMin.Set(vd.GetPointer<float>(nMinIndex));
  vMax.Set(vd.GetPointer<float>(nMaxIndex));

  //sphere.SetPosition( (vMin + vMax ) * 0.5f );

  CVector3<float> vPos = vMin + ((vMax-vMin) * 0.5f);
  sphere.SetPosition( vPos );
  sphere.SetRadius((vMax - sphere.GetPosition()).Length());

  // Check each vertex that they actually are stored inside the sphere
  float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();

  //Temporary point when determining new center and radius
  CVector3<float> vGPoint;

  // The vector from center of the sphere to the current vertex
  CVector3<float> vCenterToPoint;

  // For each vertex
  for ( unsigned int i = 0;i<indices.GetNumIndices(); i++)
  {

    vTemp.Set( (vd.GetPointer<float>( indices.GetPointer<unsigned short>()[i] )));

    float fDist = ((vTemp - sphere.GetPosition()).Length());
    float fDistSquared = fDist * fDist;

    if (  fDistSquared > fRadiusSquared )
    {
      // Point lies outside sphere, adjust center and radius
      vCenterToPoint = (vTemp - sphere.GetPosition());
      vCenterToPoint.Normalize();
      vGPoint = sphere.GetPosition() - (sphere.GetRadius() * vCenterToPoint );
      //sphere.SetPosition( vGPoint + (( vTemp-vGPoint) * 0.5));
      sphere.SetPosition( (vGPoint + vTemp) * 0.5f);
      sphere.SetRadius( (vTemp - sphere.GetPosition() ).Length());

      fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
    }
    else
    {
      // Point within sphere, everything is good in
      // the hood tonite.
    }
  } // for each vertex
  // Return the tighter sphere
  return sphere;
}

// /////////////////////////////////////////////////////////////////
CSphere
Phoenix::Volume::MergeSpheres( const Phoenix::Volume::CSphere &sphereOne, const Phoenix::Volume::CSphere &sphereTwo)
{

  CVector3<float>  vCenterDiff = sphereTwo.GetPosition() - sphereOne.GetPosition();
  float		fCenterDiffLen = 0.0f;
  float		fRadiusDiff    = sphereTwo.GetRadius() - sphereOne.GetRadius();
  float		fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
  float		fLengthSqr     = vCenterDiff.LengthSqr();

//   int bSphereOneValid = sphereOne.GetRadius() > EPSILON;
//   int bSphereTwoValid = sphereTwo.GetRadius() > EPSILON;
//   if (  bSphereOneValid && !bSphereTwoValid ) return sphereOne;
//   if ( !bSphereOneValid &&  bSphereTwoValid ) return sphereTwo;

  if ( fRadiusDiffSqr >= fLengthSqr )
  {
    if ( fRadiusDiff >= 0.0f ) // Sphere two contains the sphere one
    {
      return sphereTwo;
    }
    else // Sphere one contains the sphere two
    {
      return sphereOne;
    }

  } else {

    fCenterDiffLen = sqrt( fLengthSqr );
    float fT = (fCenterDiffLen + sphereTwo.GetRadius() -
		sphereOne.GetRadius()) /(2.0f * fCenterDiffLen);

    return CSphere( sphereOne.GetPosition() + (fT * vCenterDiff),
		    (fCenterDiffLen + sphereTwo.GetRadius() + sphereOne.GetRadius())*0.5f);
  }
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
void
Phoenix::Volume::operator*=( Phoenix::Volume::CSphere & sphere, const Phoenix::Math::CTransform & transf )
{
  sphere.Transform(transf);
}
/////////////////////////////////////////////////////////////////
#endif
void
Phoenix::Volume::CSphere::Transform( const Phoenix::Math::CTransform & transf )
{
    Move(transf.GetTranslation());
    SetRadius( GetRadius() * transf.GetScaling());
}
/////////////////////////////////////////////////////////////////
