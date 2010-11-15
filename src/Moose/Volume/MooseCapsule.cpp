/*
 *  MooseCapsule.cpp
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "MooseCapsule.h"
#include "MooseDefaultEntities.h"
#include "MooseLogger.h"
#include "MooseVector3.h"
#include "MooseMatrix3x3.h"
#include "MooseMath.h"
#include "MooseVertexDescriptor.h"
using namespace Moose::Math;
using namespace Moose::Volume;
using namespace Moose::Core;
using namespace Moose::Default;
using namespace std;
Moose::Volume::CCapsule
Moose::Volume::CalculateBoundingCapsule( const Moose::Graphics::CVertexDescriptor &rVertices )
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
    
    CCapsule capsule;
    
    float fRlen = fMaxR - fMinR;
    float fSlen = fMaxS - fMinS;
    float fTlen = fMaxT - fMinT;
    
    if ( fRlen > fSlen )
    {
        /// fLambda1 > fLambda2 !!!
        if ( fSlen > fTlen  )
        {
            capsule.Set( fMinR*vR, fMaxR *vR);
            capsule.SetRadius( fSlen );
        }
        else
        {
            if ( fRlen > fTlen )
            {
                capsule.Set( fMinR*vR, fMaxR *vR);
                capsule.SetRadius( fTlen );
            }
            else
            {
                capsule.Set( fMinT*vT, fMaxT *vT);
                capsule.SetRadius( fRlen );
            }
        }
    }
    else
    {
        /// fLambda1 < fLambda2 !!!
        
        if ( fSlen < fTlen )
        {
            capsule.Set( fMinT*vT, fMaxT *vT);
            capsule.SetRadius( fSlen );
        }
        else
        {
            /// fLambda3 < fLambda2
            
            if ( fRlen < fTlen )
            {
                capsule.Set( fMinS*vS, fMaxS *vS);
                capsule.SetRadius( fTlen );
            }
            else
            {
                capsule.Set( fMinS*vS, fMaxS *vS);
                capsule.SetRadius( fRlen );                
            }
        }
    }
    
    
    return capsule;
    
}
/////////////////////////////////////////////////////////////////
Moose::Volume::CCapsule
Moose::Volume::CalculateBoundingCapsule( const Moose::Graphics::CVertexDescriptor &rVertices,
                                              const Moose::Graphics::CIndexArray &indexBuffer )
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
    CCapsule capsule;
#ifdef DEBUG
    g_Error << "minR " << fMinR << endl;
    g_Error << "maxR " << fMaxR << endl;
    g_Error << "minS " << fMinS << endl;
    g_Error << "maxS " << fMaxS << endl;
    g_Error << "minT " << fMinT << endl;
    g_Error << "maxT " << fMaxT << endl;
#endif
    
    float fRlen = fMaxR - fMinR;
    float fSlen = fMaxS - fMinS;
    float fTlen = fMaxT - fMinT;
    
   
    if ( fRlen > fSlen )
    {
        /// fLambda1 > fLambda2 !!!
        if ( fSlen > fTlen  )
        {
            capsule.Set( fMinR*vR, fMaxR *vR);
            capsule.SetRadius( fSlen );
        }
        else
        {
            if ( fRlen > fTlen )
            {
                capsule.Set( fMinR*vR, fMaxR *vR);
                capsule.SetRadius( fTlen );
            }
            else
            {
                capsule.Set( fMinT*vT, fMaxT *vT);
                capsule.SetRadius( fRlen );
            }
        }
    }
    else
    {
        /// fLambda1 < fLambda2 !!!
        
        if ( fSlen < fTlen )
        {
            capsule.Set( fMinT*vT, fMaxT *vT);
            capsule.SetRadius( fSlen );
        }
        else
        {
            /// fLambda3 < fLambda2
            
            if ( fRlen < fTlen )
            {
                capsule.Set( fMinS*vS, fMaxS *vS);
                capsule.SetRadius( fTlen );
            }
            else
            {
                capsule.Set( fMinS*vS, fMaxS *vS);
                capsule.SetRadius( fRlen );                
            }
        }
    }
    
    
    return capsule;
    
}