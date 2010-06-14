#include "PhoenixOBB.h"
#include "PhoenixMath.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Phoenix::Volume;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
void
Phoenix::Volume::COrientedBox::SetOrientation(const CVector3<float> &vUp,
					      const CVector3<float> &vForward,
					      const CVector3<float> &vRight)
{
  m_vUpward  = vUp;
  m_vRight   = vRight;
  m_vForward = vForward;
}
/////////////////////////////////////////////////////////////////
/*void
Phoenix::Volume::COrientedBox::CalculatePlanes()
{
  m_Planes[TOP].Calculate(   -GetUpVector(),       GetPosition()+(GetUpVector()*GetHalfHeight()));
  m_Planes[BOTTOM].Calculate( GetUpVector(),       GetPosition()-(GetUpVector()*GetHalfHeight()));
  m_Planes[LEFT].Calculate(   GetRightVector(),    GetPosition()-(GetRightVector()*GetHalfWidth()));
  m_Planes[RIGHT].Calculate( -GetRightVector(),    GetPosition()+(GetRightVector()*GetHalfWidth()));
  m_Planes[FRONT].Calculate( -GetForwardVector(),  GetPosition()+(GetForwardVector()*GetHalfLength()));
  m_Planes[BACK].Calculate(   GetForwardVector(),  GetPosition()-(GetForwardVector()*GetHalfLength()));
}*/
/////////////////////////////////////////////////////////////////
/*void
Phoenix::Volume::COrientedBox::CalculateCorners()
{
  CVector3<float> vTmp;

  vTmp =  GetPosition();
  vTmp += GetForwardVector()*GetHalfLength();
  vTmp -= GetRightVector()*GetHalfWidth();
  vTmp += GetUpVector()*GetHalfHeight();

  GetCorner( TOP_LEFT_FRONT ) = vTmp;
  vTmp += GetRightVector()*GetWidth();
  GetCorner( TOP_RIGHT_FRONT ) = vTmp;

  vTmp += -GetForwardVector()*GetLength();
  GetCorner( TOP_RIGHT_BACK ) = vTmp;

  vTmp += -GetRightVector()*GetWidth();
  GetCorner( TOP_LEFT_BACK ) = vTmp;

  vTmp += -GetUpVector()*GetHeight();
  GetCorner( BOTTOM_LEFT_BACK ) = vTmp;

  vTmp += GetRightVector()*GetWidth();
  GetCorner( BOTTOM_RIGHT_BACK ) = vTmp;

  vTmp += GetForwardVector()*GetLength();
  GetCorner( BOTTOM_RIGHT_FRONT ) = vTmp;

  vTmp += -GetRightVector()*GetWidth();
  GetCorner( BOTTOM_LEFT_FRONT ) = vTmp;
}*/
/////////////////////////////////////////////////////////////////
CVector3<float>
Phoenix::Volume::COrientedBox::GetCorner( BBOX_CORNER_TYPE tCorner ) const
{
    
    
    switch ( tCorner )
    {
    case TOP_LEFT_FRONT:
        return GetPosition() + GetUpVector() * GetHalfHeight() - GetRightVector() * GetHalfWidth() + GetForwardVector() * GetHalfLength() ;
        break;
    case TOP_RIGHT_FRONT:
        return GetPosition() + GetUpVector() * GetHalfHeight() + GetRightVector() * GetHalfWidth() + GetForwardVector() * GetHalfLength() ;
        break;
    case TOP_LEFT_BACK:
        return GetPosition() + GetUpVector() * GetHalfHeight() - GetRightVector() * GetHalfWidth() - GetForwardVector() * GetHalfLength() ;
        break;
    case TOP_RIGHT_BACK:
        return GetPosition() + GetUpVector() * GetHalfHeight() + GetRightVector() * GetHalfWidth() - GetForwardVector() * GetHalfLength() ;
        break;
    case BOTTOM_LEFT_FRONT:
        return GetPosition() - GetUpVector() * GetHalfHeight() - GetRightVector() * GetHalfWidth() + GetForwardVector() * GetHalfLength() ;
        break;
    case BOTTOM_RIGHT_FRONT:
        return GetPosition() - GetUpVector() * GetHalfHeight() + GetRightVector() * GetHalfWidth() + GetForwardVector() * GetHalfLength() ;
        break;
    case BOTTOM_LEFT_BACK:
        return GetPosition() - GetUpVector() * GetHalfHeight() - GetRightVector() * GetHalfWidth() - GetForwardVector() * GetHalfLength() ;
        break;
    case BOTTOM_RIGHT_BACK:
        return GetPosition() - GetUpVector() * GetHalfHeight() + GetRightVector() * GetHalfWidth() - GetForwardVector() * GetHalfLength() ;
        break;
    default:
        assert( 0 && "OBB getcorner: something is very  wrong");
    }
}
/////////////////////////////////////////////////////////////////
/*const CVector3<float> &
Phoenix::Volume::COrientedBox::GetCorner( BBOX_CORNER_TYPE tCorner ) const
{
  return m_vecCorners[tCorner];
}*/
///////////////////////////////////////////////////////////////////////////////
/*
 obOrientedBox[Phoenix::Volume::FRONT].SetNormal(-R);   				\
 obOrientedBox[Phoenix::Volume::FRONT][3]  = rmax;					\
 obOrientedBox[Phoenix::Volume::BACK].SetNormal(R);       				\
 obOrientedBox[Phoenix::Volume::BACK][3] = -rmin;				\
 obOrientedBox[Phoenix::Volume::RIGHT].SetNormal(-S);			\
 obOrientedBox[Phoenix::Volume::RIGHT][3] = smax;					\
 obOrientedBox[Phoenix::Volume::LEFT].SetNormal(S);    				\
 obOrientedBox[Phoenix::Volume::LEFT][3] = -smin;					\
 obOrientedBox[Phoenix::Volume::TOP].SetNormal(-T);    				\
 obOrientedBox[Phoenix::Volume::TOP][3] = tmax;					\
 obOrientedBox[Phoenix::Volume::BOTTOM].SetNormal(T);					\
 obOrientedBox[Phoenix::Volume::BOTTOM][3] = -tmin;    				\
 
 *//////////////////////////////////////////////////////////////////
#define APPLY_CORRECT_VALUES(R,S,T,rmax,rmin,smax, smin,tmax, tmin) {			\
  /* The orientation will be formed so, that the forward vector will point along*/ 	\
  /* principal axis (largest eigenvalue ), right vector along vector with*/        	\
  /* second largest and up points to vector with lowest eigenvalue*/               	\
  obOrientedBox.SetOrientation(  T, R, S );                                  		\
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::COrientedBox
Phoenix::Volume::CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &rVertices )
{
  /////////////////////////////////////////////////////////////////
  float fMaxR = 0.0f;
  float fMinR = 0.0f;
  float fMaxS = 0.0f;
  float fMinS = 0.0f;
  float fMaxT = 0.0f;
  float fMinT = 0.0f;
  float fLambda1,fLambda2,fLambda3;
  float fVertDotR = 0.0f;
  float fVertDotS = 0.0f;
  float fVertDotT = 0.0f;
  CVector3<float>	 vTemp;
  /////////////////////////////////////////////////////////////////
  CMatrix3x3<float> mCovar;
  Math::CovarianceMatrix( rVertices, mCovar );
  CMatrix3x3<float> mEigenVectorMatrix;
  mEigenVectorMatrix.IdentityMatrix();
  Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);


  /////////////////////////////////////////////////////////////////
  CVector3<float> vR; Math::GetColumnVector(mEigenVectorMatrix, 0, vR);
  CVector3<float> vS; Math::GetColumnVector(mEigenVectorMatrix, 1, vS);
  CVector3<float> vT; Math::GetColumnVector(mEigenVectorMatrix, 2, vT);
  /////////////////////////////////////////////////////////////////
  vR.Normalize();
  vS.Normalize();
  vT.Normalize();
  /////////////////////////////////////////////////////////////////
  for ( unsigned int v = 0;v<rVertices.GetSize();v++)
  {

    vTemp.Set( rVertices.GetPointer<float>(v) );

    fVertDotR = vTemp.Dot(vR);
    fVertDotS = vTemp.Dot(vS);
    fVertDotT = vTemp.Dot(vT);

    if ( v==0 )
    {

      fMaxR = fMinR = fVertDotR;
      fMaxS = fMinS = fVertDotS;
      fMaxT = fMinT = fVertDotT;

    } else {

      if ( fVertDotR < fMinR )		fMinR = fVertDotR;
      else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;

      if ( fVertDotS < fMinS )		fMinS = fVertDotS;
      else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;

      if ( fVertDotT < fMinT )		fMinT = fVertDotT;
      else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;

    }
  }

  COrientedBox obOrientedBox;


  float fRlen = fMaxR - fMinR;
  float fSlen = fMaxS - fMinS;
  float fTlen = fMaxT - fMinT;

  float fA = (fMaxR + fMinR) * 0.5f;
  float fB = (fMaxS + fMinS) * 0.5f;
  float fC = (fMaxT + fMinT) * 0.5f;

  /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
  if ( fRlen > fSlen )
  {
    /// fLambda1 > fLambda2 !!!
    if ( fSlen > fTlen  )
    {
      APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
      obOrientedBox.SetLength( fRlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fTlen );
    }
    else
    {
      if ( fRlen > fTlen )
      {
	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
	obOrientedBox.SetLength( fRlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fSlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
	obOrientedBox.SetLength( fTlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fSlen );
      }
    }
  }
  else
  {
    /// fLambda1 < fLambda2 !!!

    if ( fSlen < fTlen )
    {
      APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
      obOrientedBox.SetLength( fTlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fRlen );
    }
    else
    {
      /// fLambda3 < fLambda2

      if ( fRlen < fTlen )
      {
	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fRlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fTlen );

      }
    }
  }

  CVector3<float> vPos = (fA*vR) + (fB*vS) + (fC*vT);
  obOrientedBox.SetPosition( vPos );
  //obOrientedBox.CalculatePlanes();
  //obOrientedBox.CalculateCorners();

  return obOrientedBox;

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::COrientedBox
Phoenix::Volume::CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &rVertices,
					       const Phoenix::Graphics::CIndexArray &indexBuffer )
{
  /////////////////////////////////////////////////////////////////
  // Variables used
  float fMaxR = 0.0f;
  float fMinR = 0.0f;
  float fMaxS = 0.0f;
  float fMinS = 0.0f;
  float fMaxT = 0.0f;
  float fMinT = 0.0f;
  float fLambda1,fLambda2,fLambda3;
  float fVertDotR = 0.0f;
  float fVertDotS = 0.0f;
  float fVertDotT = 0.0f;
  CVector3<float>	 vTemp;
  /////////////////////////////////////////////////////////////////
  CMatrix3x3<float> mCovar;
  Math::CovarianceMatrix( rVertices, indexBuffer, mCovar );
  CMatrix3x3<float> mEigenVectorMatrix;
  mEigenVectorMatrix.IdentityMatrix();
  /////////////////////////////////////////////////////////////////
  Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);
  /////////////////////////////////////////////////////////////////
  CVector3<float> vR; GetColumnVector(mEigenVectorMatrix, 0, vR);
  CVector3<float> vS; GetColumnVector(mEigenVectorMatrix, 1, vS);
  CVector3<float> vT; GetColumnVector(mEigenVectorMatrix, 2, vT);
  /////////////////////////////////////////////////////////////////
  vR.Normalize();
  vS.Normalize();
  vT.Normalize();
  /////////////////////////////////////////////////////////////////
  for ( unsigned int i = 0;i<indexBuffer.GetNumIndices();i++)
  {
    if ( indexBuffer.IsShortIndices())
      vTemp.Set( rVertices.GetPointer<float>(indexBuffer.GetPointer<unsigned short int>()[i]) );
    else
      vTemp.Set( rVertices.GetPointer<float>(indexBuffer.GetPointer<unsigned int>()[i]) );

    fVertDotR = vTemp.Dot(vR);
    fVertDotS = vTemp.Dot(vS);
    fVertDotT = vTemp.Dot(vT);

    if ( i==0){

      fMaxR = fMinR = fVertDotR;
      fMaxS = fMinS = fVertDotS;
      fMaxT = fMinT = fVertDotT;

    }
    else
    {

      if ( fVertDotR < fMinR )		fMinR = fVertDotR;
      else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;

      if ( fVertDotS < fMinS )		fMinS = fVertDotS;
      else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;

      if ( fVertDotT < fMinT )		fMinT = fVertDotT;
      else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;

    }
  }
  /////////////////////////////////////////////////////////////////
  COrientedBox obOrientedBox;
#ifdef DEBUG
  CLogger::Error() << DEBUG_HEADER << "minR " << fMinR << endl;
  CLogger::Error() << DEBUG_HEADER << "maxR " << fMaxR << endl;
  CLogger::Error() << DEBUG_HEADER << "minS " << fMinS << endl;
  CLogger::Error() << DEBUG_HEADER << "maxS " << fMaxS << endl;
  CLogger::Error() << DEBUG_HEADER << "minT " << fMinT << endl;
  CLogger::Error() << DEBUG_HEADER << "maxT " << fMaxT << endl;
#endif

  float fRlen = fMaxR - fMinR;
  float fSlen = fMaxS - fMinS;
  float fTlen = fMaxT - fMinT;

  float fA = (fMaxR + fMinR) * 0.5f;
  float fB = (fMaxS + fMinS) * 0.5f;
  float fC = (fMaxT + fMinT) * 0.5f;


  /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
  if ( fRlen > fSlen )
  {
    /// fLambda1 > fLambda2 !!!
    if ( fSlen > fTlen  )
    {
      APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
      obOrientedBox.SetLength( fRlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fTlen );
    }
    else
    {
      if ( fRlen > fTlen )
      {
	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
	obOrientedBox.SetLength( fRlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fSlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
	obOrientedBox.SetLength( fTlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fSlen );
      }
    }
  }
  else
  {
    /// fLambda1 < fLambda2 !!!

    if ( fSlen < fTlen )
    {
      APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
      obOrientedBox.SetLength( fTlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fRlen );
    }
    else
    {
      /// fLambda3 < fLambda2

      if ( fRlen < fTlen )
      {
	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fRlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fTlen );

      }
    }
  }

  /////////////////////////////////////////////////////////////////
  // The position
  CVector3<float> vPos = (fA*vR) + (fB*vS) + (fC*vT);
  obOrientedBox.SetPosition( vPos );
  /////////////////////////////////////////////////////////////////
  // The orientation will be formed so, that the forward vector will point along
  // principal axis (largest eigenvalue ), right vector along vector with
  // second largest and up points to vector with lowest eigenvalue
  ///obOrientedBox.SetOrientation(  vT, vR, vS );
  //obOrientedBox.CalculatePlanes();
  //obOrientedBox.CalculateCorners();

  /////////////////////////////////////////////////////////////////
  return obOrientedBox;

}
/////////////////////////////////////////////////////////////////
COrientedBox
Phoenix::Volume::MergeOrientedBoxes( const Phoenix::Volume::COrientedBox &obOne, const Phoenix::Volume::COrientedBox &obTwo )
{
  unsigned int bObOneValid =  (obOne.GetLength() * obOne.GetWidth() * obOne.GetHeight()) > 0.0001f;
  unsigned int bObTwoValid =  (obTwo.GetLength() * obTwo.GetWidth() * obTwo.GetHeight()) > 0.0001f;

  if (  bObOneValid && !bObTwoValid ) return obOne;
  if ( !bObOneValid && bObTwoValid ) return obTwo;
  if ( !bObOneValid && !bObTwoValid) return COrientedBox();

#define OPTIMAL_BOUNDING_BOX_MERGE
#ifdef OPTIMAL_BOUNDING_BOX_MERGE

  COrientedBox obMerge;
  CVector3<float> vDir = obTwo.GetPosition() - obOne.GetPosition();
  vDir.Normalize();

  CVector3<float> vCorner;
  CVector3<float> vCornerOnLine;
  CVector3<float> vMin(0,0,0);
  CVector3<float> vMax(0,0,0);
  CVector3<float> vSecondaryAxis;
  CVector3<float> vSecAxisCandidate;

  float fDot;
  float fLength;

  int i;
  // For each corner in box one
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner = obOne.GetCorner( (BBOX_CORNER_TYPE)i) ;
    fDot = vDir.Dot(vCorner - obOne.GetPosition());

    // calculate possible secondary axis.
    vCornerOnLine = obOne.GetPosition() + vDir * fDot;
    vSecAxisCandidate = vCorner - vCornerOnLine;
    fLength = vSecAxisCandidate.Length();

    ///
    if (fDot > vMax[0])  {     vMax[0] = fDot;  }
    else if (fDot < vMin[0]){  vMin[0] = fDot;  }

    // Since this is a symmetric box, we need to check only for
    // max, since width becomes 2 * max.
    if ( fLength > vMax[1] )
    {
      vMax[1]  = fLength;
      vSecondaryAxis = vSecAxisCandidate;
    }
  }

  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner = obTwo.GetCorner( (BBOX_CORNER_TYPE)i);
    fDot = vDir.Dot(vCorner - obOne.GetPosition());

    // calculate possible secondary axis.
    vCornerOnLine = obOne.GetPosition() + vDir * fDot;
    vSecAxisCandidate = vCorner - vCornerOnLine;
    fLength = vSecAxisCandidate.Length();

    ///
    if (fDot > vMax[0])  {     vMax[0] = fDot;  }
    else if (fDot < vMin[0]){  vMin[0] = fDot;  }

    // Since this is a symmetric box, we need to check only for
    // max, since width becomes 2 * max.
    if ( fLength > vMax[1] )
    {
      vMax[1]  = fLength;
      vSecondaryAxis = vSecAxisCandidate;
    }
  }
  vMin[1] = -vMax[1];
  /////////////////////////////////////////////////////////////////
  // At this point, we have initial and secondary axis with extents.
  CVector3<float> vTertiaryAxis = vSecondaryAxis.Cross(vDir);
  vTertiaryAxis.Normalize();
  vSecondaryAxis.Normalize();
  fDot = 0.0f;
  /////////////////////////////////////////////////////////////////
  // For each corner in box one
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner = obOne.GetCorner( (BBOX_CORNER_TYPE)i );
    fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
    if (fDot > vMax[2])     {  vMax[2] = fDot;  }

  }

  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner = obTwo.GetCorner( (BBOX_CORNER_TYPE)i );
    fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
    if (fDot > vMax[2])     {  vMax[2] = fDot;  }

  }
  vMin[2] = -vMax[2];

  CVector3<float> vExtents(vMax - vMin);

  obMerge.SetPosition( ((obOne.GetPosition() + vDir * vMax[0]) +
			(obOne.GetPosition() + vDir * vMin[0])) *0.5f)  ;

  obMerge.SetOrientation( vTertiaryAxis, vDir,  vSecondaryAxis );
  obMerge.SetLength( vExtents[0] );
  obMerge.SetWidth( vExtents[1] );
  obMerge.SetHeight( vExtents[2] );
  //obMerge.CalculateCorners();
  //obMerge.CalculatePlanes();
  return obMerge;
#else
  // construct a box that contains the input boxes
  COrientedBox obMerge;

  // The first guess at the box center.  This value will be updated later
  // after the input box vertices are projected onto axes determined by an
  // average of box axes.
  obMerge.SetPosition( 0.5f*(obOne.GetPosition() + obTwo.GetPosition()));

  // A box's axes, when viewed as the columns of a matrix, form a rotation
  // matrix.  The input box axes are converted to quaternions.  The average
  // quaternion is computed, then normalized to unit length.  The result is
  // the slerp of the two input quaternions with t-value of 1/2.  The result
  // is converted back to a rotation matrix and its columns are selected as
  // the merged box axes.
  CQuaternion qOne, qTwo;

  CMatrix4x4f mOne = OrientedBoxAxisToRotationMatrix( obOne );
  CMatrix4x4f mTwo = OrientedBoxAxisToRotationMatrix( obTwo );

  qOne = RotationMatrixToQuaternion( mOne );
  qTwo = RotationMatrixToQuaternion( mTwo );

  if ( qOne.Dot(qTwo) < 0.0f)
  {
    qTwo = -qTwo;
  }

  CQuaternion  qQuat = qOne + qTwo;
  qQuat.Normalize();

  CMatrix4x4f mFinal = qQuat.ToMatrix();

  CVector3<float> vUp(mFinal(0,2), mFinal(1,2), mFinal(2,2) );
  CVector3<float> vForward(mFinal(0,0), mFinal(1,0), mFinal(2,0) );
  CVector3<float> vRight(mFinal(0,1), mFinal(1,1), mFinal(2,1) );

  obMerge.SetOrientation( vUp, vForward, vRight);

  // Project the input box vertices onto the merged-box axes.  Each axis
  // D[i] containing the current center C has a minimum projected value
  // pmin[i] and a maximum projected value pmax[i].  The corresponding end
  // points on the axes are C+pmin[i]*D[i] and C+pmax[i]*D[i].  The point C
  // is not necessarily the midpoint for any of the intervals.  The actual
  // box center will be adjusted from C to a point C' that is the midpoint
  // of each interval,
  //   C' = C + sum_{i=0}^2 0.5*(pmin[i]+pmax[i])*D[i]
  // The box extents are
  //   e[i] = 0.5*(pmax[i]-pmin[i])

  int i = 0;
  float fDot = 0.0f;

  CVector3<float> vCorner, vDiff;
  CVector3<float> vMin(0,0,0);
  CVector3<float> vMax(0,0,0);

  // For each corner in box one
  for (i = 0; i < 8; i++)
  {
    // Forward axis
    vCorner = obOne.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetForwardVector() );
    if (fDot > vMax[0])  {     vMax[0] = fDot;    }
    else if (fDot < vMin[0]){    vMin[0] = fDot;  }

    // right axis
    vCorner = obOne.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetRightVector() );
    if (fDot > vMax[1])  {     vMax[1] = fDot;    }
    else if (fDot < vMin[1]){    vMin[1] = fDot;  }

    // Up axis
    vCorner = obOne.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetUpVector() );
    if (fDot > vMax[2])  {     vMax[2] = fDot;    }
    else if (fDot < vMin[2]){    vMin[2] = fDot;  }

  }

  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Forward axis
    vCorner =  obTwo.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetForwardVector() );
    if (fDot > vMax[0])  {     vMax[0] = fDot; }
    else if (fDot < vMin[0]){  vMin[0] = fDot; }

    // right axis
    vCorner = obTwo.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetRightVector() );
    if (fDot > vMax[1])  {     vMax[1] = fDot; }
    else if (fDot < vMin[1]){  vMin[1] = fDot; }

    // Up axis
    vCorner = obTwo.GetCorner( (COrientedBox::BoxCorner_t)i);
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetUpVector() );
    if (fDot > vMax[2])  {     vMax[2] = fDot; }
    else if (fDot < vMin[2]){  vMin[2] = fDot; }

  }

  // [kMin,kMax] is the axis-aligned box in the coordinate system of the
  // merged box axes.  Update the current box center to be the center of
  // the new box.  Compute the extent based on the new center.

  CVector3<float> vMean    = (vMax + vMin) * 0.5f;
  CVector3<float> vExtents = (vMax - vMin );

  obMerge.SetPosition( obMerge.GetPosition() + (vMean[0] *obMerge.GetForwardVector()));
  obMerge.SetLength( vExtents[0] );

  obMerge.SetPosition( obMerge.GetPosition() + (vMean[1]*obMerge.GetRightVector()));
  obMerge.SetWidth( vExtents[1] );

  obMerge.SetPosition( obMerge.GetPosition() + (vMean[2]*obMerge.GetUpVector()));
  obMerge.SetHeight( vExtents[2] );

  //obMerge.CalculatePlanes();
  //obMerge.CalculateCorners();
  return obMerge;
#endif
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Volume::COrientedBox::Transform( const Phoenix::Math::CTransform & transf )
{
  // Transform box according to world transformation
  Move( transf.GetTranslation() );
  AppendToRotation( transf.GetRotation() );
    SetLength( GetLength()*transf.GetScaling()[2]);
    SetHeight( GetHeight()*transf.GetScaling()[1]);
    SetWidth( GetWidth()*transf.GetScaling()[0]);
  // Update box values
  //CalculatePlanes();
  //CalculateCorners();
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
void
Phoenix::Volume::operator*=( Phoenix::Volume::COrientedBox & box, const Phoenix::Math::CTransform & transf )
{
    box.Transform(transf);
}
/////////////////////////////////////////////////////////////////
#endif
