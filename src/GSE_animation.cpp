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

#include "GSE_animation.h"
#include <string.h>
#include "GSE_Logger.h"
#include "GSE_debug.h"
#include "GSE_particlesystems.h"

using Core::GSE_Logger;
using namespace Animation;

void CalculateTranslation( GSE_Joint *pJoint, GSE_Matrix4x4f &mTranslation );
void CalculateRotation( GSE_Joint *pJoint, GSE_Matrix4x4f &mRotation);
/// Returns true if updating forced
#define IS_FORCEUPDATE() ( !m_MatrixUpdateStack.empty() )

/// The constructor.
GSE_Joint::GSE_Joint( unsigned int nNumTranslationKeyframes, 
		      unsigned int nNumRotationKeyframes) : m_vLocalTranslation(0.0f,0.0f,0.0f),m_vLocalRotation(0.0f,0.0f,0.0f)
{
  Init();
  m_nNumTranslationKeyframes = nNumTranslationKeyframes;
  m_nNumRotationKeyframes    = nNumRotationKeyframes;
  m_pTranslationKeyframes    = new GSE_JointKeyframe[m_nNumTranslationKeyframes];
  m_pRotationKeyframes       = new GSE_JointKeyframe[m_nNumRotationKeyframes];
  
}
/// The destructor
GSE_Joint::~GSE_Joint()
{
  Destroy();
}
/// The copy constructor.
GSE_Joint::GSE_Joint(const GSE_Joint &ref) : m_vLocalTranslation(ref.m_vLocalTranslation),
					     m_vLocalRotation(ref.m_vLocalRotation)
{
  //strncpy(m_szName, ref.m_szName, JOINT_NAME_LENGTH);
  m_bRoot = 0;
  //m_pParent = ref.m_pParent;
  m_mAbsolute = ref.m_mAbsolute;
  m_mRelative = ref.m_mRelative;
  m_mFinal = ref.m_mFinal;
  
  m_bAnimationEnabled = ref.m_bAnimationEnabled;
  m_iStartTime = ref.m_iStartTime;
  m_iEndTime = ref.m_iEndTime;
  m_iCurrentTime = ref.m_iCurrentTime;
  m_bLoop = ref.m_bLoop;
  m_iPassingTime = ref.m_iPassingTime;

  m_nNumTranslationKeyframes = ref.m_nNumTranslationKeyframes;
  m_nNumRotationKeyframes = ref.m_nNumRotationKeyframes;
  // Create the arrays
  m_pTranslationKeyframes = new GSE_JointKeyframe[m_nNumTranslationKeyframes];
  m_pRotationKeyframes    = new GSE_JointKeyframe[m_nNumRotationKeyframes];
  // Copy the array data 
  memcpy( m_pTranslationKeyframes, ref.m_pTranslationKeyframes, sizeof(GSE_JointKeyframe)*m_nNumTranslationKeyframes);
  memcpy( m_pRotationKeyframes,    ref.m_pRotationKeyframes,    sizeof(GSE_JointKeyframe)*m_nNumRotationKeyframes);  
  
}
// The assignment operator
GSE_Joint &
GSE_Joint::operator=(GSE_Joint obj)
{
  Destroy();
  //strncpy(m_szName, obj.m_szName, JOINT_NAME_LENGTH);

  m_bRoot = obj.m_bRoot;
  m_mAbsolute = obj.m_mAbsolute;
  m_mRelative = obj.m_mRelative;
  m_mFinal = obj.m_mFinal;
  m_vLocalTranslation = obj.m_vLocalTranslation;
  m_vLocalRotation = obj.m_vLocalRotation;
  
  m_bAnimationEnabled = obj.m_bAnimationEnabled;
  m_iStartTime = obj.m_iStartTime;
  m_iEndTime = obj.m_iEndTime;
  m_iCurrentTime = obj.m_iCurrentTime;
  m_bLoop = obj.m_bLoop;
  m_iPassingTime = obj.m_iPassingTime;
  
  m_nNumTranslationKeyframes = obj.m_nNumTranslationKeyframes;
  m_nNumRotationKeyframes = obj.m_nNumRotationKeyframes;
  // Create the arrays
  m_pTranslationKeyframes = new GSE_JointKeyframe[m_nNumTranslationKeyframes];
  m_pRotationKeyframes    = new GSE_JointKeyframe[m_nNumRotationKeyframes];
  // Copy the array data 
  memcpy( m_pTranslationKeyframes, obj.m_pTranslationKeyframes, sizeof(GSE_JointKeyframe)*m_nNumTranslationKeyframes);
  memcpy( m_pRotationKeyframes, obj.m_pRotationKeyframes, sizeof(GSE_JointKeyframe)*m_nNumRotationKeyframes);  

  return *this;
}
/// The initialization function, assigns default values to all members.
void
GSE_Joint::Init()
{
  //m_szName[0] = '\0';
  
  m_mRelative = m_mFinal = m_mAbsolute = GSE_Matrix4x4f::Identity();
  m_nNumTranslationKeyframes = 0;
  m_nNumRotationKeyframes = 0;
  m_pTranslationKeyframes = NULL;
  m_pRotationKeyframes = NULL;
  m_bRoot = 0;
  // By default, we display the first frame of the first animation in the file.
  m_bAnimationEnabled = 1;
  m_iStartTime = 0;
  m_iEndTime = 0;
  m_iCurrentTime = 0;
  m_iPassingTime = 0;
  m_bLoop = 0;
}
/// Frees the memory reserved by this object.
void 
GSE_Joint::Destroy()
{

  m_mRelative = m_mFinal = m_mAbsolute = GSE_Matrix4x4f::Identity();

  if ( m_pTranslationKeyframes != NULL ) {
    delete [] m_pTranslationKeyframes;
    m_pTranslationKeyframes = NULL;
  } 

  if ( m_pRotationKeyframes != NULL ){
    delete [] m_pRotationKeyframes;
    m_pRotationKeyframes = NULL;
  }
  
  m_nNumTranslationKeyframes = 0;
  m_nNumRotationKeyframes = 0;
}

// -----------------------------------------------------------------

/// The default constructor.
GSE_AnimationRenderer::GSE_AnimationRenderer()
{

  m_iRenderPassType = GSE_AnimationRenderer::RENDER_PASS_UPDATE;
  m_iPassedTime = 0;

}
/// The destructor.
GSE_AnimationRenderer::~GSE_AnimationRenderer()
{
  
}
/// Assigns the next render pass type.
void
GSE_AnimationRenderer::SetRenderPass( GSE_AnimRenderPass_t iRenderPass ) 
{
  m_iRenderPassType = iRenderPass;
}
/// Assigns the passed time.
void 
GSE_AnimationRenderer::SetPassedTime( unsigned int iPassedTimeInMilliSeconds ) 
{
  m_iPassedTime = iPassedTimeInMilliSeconds;
}

/// Processes the nodes when RENDER_PASS_UPDATE is set.
int 
GSE_AnimationRenderer::Handle_JointUpdate( GSE_JointNode *pNode )
{
  int iRetval		= 0;
  GSE_Joint *pJoint	= pNode->GetJoint();

  // update passed time
  pJoint->m_iPassingTime += m_iPassedTime;
  
  if ( pNode->GetJoint()->m_bAnimationEnabled )
  {
    pJoint->m_iCurrentTime = pJoint->m_iPassingTime - pJoint->m_iStartTime;
    
  }
  // Check did we reach the end of the animation;
  // if so, in case looping is enabled we go right back to
  // the start. Otherwise we stick in the last frame of the animation.
  /*pJoint->m_iCurrentTime > pJoint->m_iEndTime - pJoint->m_iStartTime*/
  if ( pJoint->IsAnimationTimePassed() )
  {
    /*pJoint->m_bLoop*/
    if ( pJoint->IsAnimationLooping() )
    {
      pJoint->m_nCurrentTranslationKeyframe = 0;
      pJoint->m_nCurrentRotationKeyframe  = 0;
      pJoint->m_iCurrentTime = 0;
      pJoint->m_iPassingTime = pJoint->m_iStartTime;
    } 
    else 
    {
      pJoint->m_iCurrentTime = pJoint->m_iEndTime-pJoint->m_iStartTime;   
    }
  }
  
  if ( pJoint->m_nNumTranslationKeyframes == 0 && 
       pJoint->m_nNumRotationKeyframes    == 0 ){

    // In case there are no keyframes specified for this joint,
    // we use the absolute matrix.
    pJoint->m_mFinal = m_mCurrentFinalMatrix * pJoint->m_mAbsolute;
      
  } else {
        
    GSE_Matrix4x4f mTransform;
    GSE_Matrix4x4f mRelativeFinal = pJoint->m_mRelative;
    GSE_Matrix4x4f mTranslation;
    
    CalculateTranslation( pJoint, mTranslation );
    CalculateRotation( pJoint, mTransform );
      
    // Calculate combined transform
    mTransform = mTransform + mTranslation - GSE_Matrix4x4f::Identity();
    mTransform(3,3) = 1.0;
    mRelativeFinal = mRelativeFinal * mTransform;
      
    // Set the final matrix 
    if ( pNode->GetJoint()->m_bRoot )
    {
      pJoint->m_mFinal = m_mCurrentFinalMatrix * mRelativeFinal;
      GSE_DEBUG("CurrentFinal for root is \n"  << m_mCurrentFinalMatrix);
    } 
    else 
    {
      // We multiply the parent's matrix with our relative matrix
      // to get our final matrix
      pJoint->m_mFinal = ((GSE_JointNode *)pNode->GetArrivingEdges().front()->GetFromNode())->GetJoint()->m_mFinal * mRelativeFinal;
    }

    // Transform each vertex bound to this joint with the final matrix.
    for(unsigned int i=0;i<pNode->GetVertexCount();i++)
    {
      unsigned int iVertIndex = pNode->GetElements()->m_pIndices[i];
	
      GSE_Vector3 vVertexOrig;
      GSE_Vector3 vVertexWorld;    
	
      vVertexOrig.UseExternalData( &(pNode->GetGeometryDataOrig()->m_pVertices[iVertIndex*3]));
      vVertexWorld.UseExternalData( &(pNode->GetGeometryDataWorld()->m_pVertices[iVertIndex*3]));
      
      // Convert the vertex coordinates
      vVertexWorld = Transform(vVertexOrig, pJoint->m_mFinal);
	
      GSE_Vector3 vNormalOrig;
      GSE_Vector3 vNormalWorld;
	
      // Rotate also the normals
      vNormalOrig.UseExternalData(  &(pNode->GetGeometryDataOrig()->m_pNormals[iVertIndex*3]));
      vNormalWorld.UseExternalData( &(pNode->GetGeometryDataWorld()->m_pNormals[iVertIndex*3]));
	
      vNormalWorld = Rotate(vNormalOrig, pJoint->m_mFinal );

      // Normalize so lighting works ok
      vNormalWorld.ToUnit();
    }
  
  } 
  
  return iRetval;
}

/// Processes the nodes when RENDER_PASS_JOINT_SETUP is set.
int 
GSE_AnimationRenderer::Handle_JointSetup( GSE_JointNode *pNode )
{
  
  if ( pNode == NULL ){
    GSE_DEBUG("Can't handle NULLs!");
    return 1;
  }
  
  GSE_Joint *pJoint = pNode->GetJoint();

  if ( pJoint == NULL ) {
    GSE_DEBUG("No Joint assigned!");
    return 1;
  }

#ifdef DEBUG
  GSE_DEBUG(pNode->GetName() << " with ");
  if ( pNode->GetElements() != NULL ){
    GSE_DEBUG( pNode->GetElements()->m_nNumIndices);
  } else {
    GSE_DEBUG(" 0 ");
  }
  GSE_DEBUG(" vertices.");  
#endif
  GSE_DEBUG( pNode->GetName() << ": local rotation = " 
	     <<  pJoint->m_vLocalRotation );
  
  pJoint->m_mRelative  = Math::RotationMatrix( pJoint->m_vLocalRotation );
  pJoint->m_mRelative = pJoint->m_mRelative + 
			Math::TranslationMatrix( pJoint->m_vLocalTranslation) -
			GSE_Matrix4x4f::Identity() ;
  
  pJoint->m_mRelative(3,3) = 1.0f;
  
  if ( pNode->GetJoint()->m_bRoot )
  {
    pJoint->m_mAbsolute = pJoint->m_mRelative;
  } 
  else 
  {  
    pJoint->m_mAbsolute = ((GSE_JointNode *)pNode->GetArrivingEdges().front()->GetFromNode())->GetJoint()->m_mAbsolute;
    pJoint->m_mAbsolute = pJoint->m_mAbsolute * pJoint->m_mRelative;
  }
  GSE_Vector3 vTmp;  
  // if joint has vertices bound, transform vertices
  for(unsigned int i=0;i<pNode->GetVertexCount();i++){


    unsigned int iVertInd = pNode->GetElements()->m_pIndices[i];
    
    if ( pNode->GetGeometryDataOrig() != NULL )
    {
      vTmp.UseExternalData(&(pNode->GetGeometryDataOrig()->m_pVertices[iVertInd*3]));
    } 
    else 
    {
      GSE_DEBUG("Cannot set vertices, objStruct is missing."); 
    }

    // Do inverse translation
    vTmp = Math::TranslateInverse( vTmp, pJoint->m_mAbsolute);
    
    // Do inverse rotation
    vTmp = Math::Rotate( vTmp, pJoint->m_mAbsolute.Transposed());
    
  }
  GSE_Vector3 vNormal;

  // If the joint has vertices, transform vertex normals if they exist
  for(unsigned int i=0;i<pNode->GetVertexCount();i++)
  {
    

    unsigned int iVertInd = pNode->GetElements()->m_pIndices[i];
    vNormal.UseExternalData(&(pNode->GetGeometryDataOrig()->m_pNormals[iVertInd*3]));
    vNormal = Math::Rotate( vNormal, pJoint->m_mAbsolute.Transposed());
    vNormal.ToUnit();
    
  }
  // Set vertices to default positions 
  pJoint->m_nCurrentTranslationKeyframe = pJoint->m_nCurrentRotationKeyframe = 0;
  pJoint->m_mFinal = pJoint->m_mAbsolute;
  return 0;
}

// ------------------------------------------------------------------------
//
// The principal Enter()-method.
//
// ------------------------------------------------------------------------
int
GSE_AnimationRenderer::Enter( GSE_GraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  /////////////////////////////////////////////////////////////////  
  if ( m_iRenderPassType == RENDER_PASS_DEBUG_PRINT)
  {
    Handle_DebugPrint_Enter(pNode);
    return 0;
  }
  /////////////////////////////////////////////////////////////////
  switch(pNode->GetType()){
  case ROOT:
    m_mCurrentFinalMatrix = GSE_Matrix4x4f::Identity();
    break;
  case JOINT:
    if ( m_iRenderPassType == RENDER_PASS_SELECT_ANIMATION_TIME_INTERVAL )
    {
      GSE_Joint *pJoint = ((GSE_JointNode *)pNode)->GetJoint();
      pJoint->m_iStartTime = m_nAnimSelectStartTime;
      pJoint->m_iEndTime = m_nAnimSelectEndTime;
      pJoint->m_bLoop = m_nAnimSelectLoop;
      pJoint->m_iPassingTime = m_nAnimSelectStartTime;
      pJoint->m_nCurrentTranslationKeyframe = 0;
      pJoint->m_nCurrentRotationKeyframe  = 0;
      pJoint->m_iCurrentTime = 0;

    } else if ( m_iRenderPassType == RENDER_PASS_JOINT_SETUP )
    {
      Handle_JointSetup((GSE_JointNode *)pNode);
    } 
    else 
    {
      Handle_JointUpdate((GSE_JointNode *)pNode);
    }
    break;
  case PARTICLESYSTEM:
    if ( m_iRenderPassType == RENDER_PASS_UPDATE )
    {
      Handle_ParticleSystemUpdate( static_cast<GSE_AnimParticleSystemNode *>(pNode) );
    }
    break;
  case UNDEFINED:
    GSE_ERR("Animation node type not defined!");
    break;

  }
  return iRetval;
}
/// Updates the particlesystem.
int
GSE_AnimationRenderer::Handle_ParticleSystemUpdate( GSE_AnimParticleSystemNode *pNode )
{
  int iRetval = 0;
  // Sanity check
  if ( pNode == NULL || pNode->GetParticleSystem() == NULL ) 
  {
    GSE_DEBUG("Node or particle system is NULL!");
    iRetval = 1;
  } 
  else 
  {

    if ( pNode->GetParticleSystem()->IsAlive())
    {

      pNode->GetParticleSystem()->Update(m_iPassedTime);
      GSE_IndexBuffer *pIndexBuffer = pNode->GetElements();
      unsigned int nNumParticles = pNode->GetParticleSystem()->GetMaxParticles();

      pIndexBuffer->m_nDrawIndices = 0;
      unsigned int nNumVertices = 3; // The number of vertices in a particle
      
      if ( nNumParticles*nNumVertices != pIndexBuffer->m_nNumIndices)
      {
	GSE_ERR("IndexBuffer and particlesystem have DIFFERENT vertex count?!");
	iRetval = 1;
      } 
      else 
      {
	pIndexBuffer->m_nDrawIndices = pNode->GetParticleSystem()->GetAliveCount() * nNumVertices;
      }
    }
  }
  return iRetval;
}
// ------------------------------------------------------------------------
//
// The principal Leave()-method.
//
// ------------------------------------------------------------------------
int
GSE_AnimationRenderer::Leave( GSE_GraphNode<NodeType> *pNode )
{
  int iRetval = 0;
  if ( m_iRenderPassType == RENDER_PASS_DEBUG_PRINT)
  {
    Handle_DebugPrint_Leave(pNode);
    return 0;
  }
  switch(pNode->GetType()){
  case ROOT:
    break;
  
  case JOINT:
    if ( m_iRenderPassType == RENDER_PASS_JOINT_SETUP ){

      GSE_DEBUG("Finished joint " 
		<< ((GSE_JointNode *)pNode)->GetName());

    } else {
      
    }
    break;
  case PARTICLESYSTEM:
    
    break;
  case UNDEFINED:
    GSE_ERR("Animation node type not defined!");
    break;
  }

  return iRetval;
}
/// Makes the update pass to update all children nodes despite their state.
void
GSE_AnimationRenderer::ForceUpdate()
{
  if ( !IS_FORCEUPDATE()){
    m_MatrixUpdateStack.push('o');
  }
}

/// Handles RENDER_PASS_DEBUG_PRINT pass.
int 
GSE_AnimationRenderer::Handle_DebugPrint_Enter( GSE_GraphNode<NodeType> *pNode )
{
  GSE_Logger::Error() << m_sPadding <<  "|" << std::endl;

  switch( pNode->GetType()){
  case UNDEFINED:  
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Undefined" << std::endl;
    break;

  case JOINT:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "Joint : " <<  pNode->GetName() << std::endl;

    GSE_DEBUG( "absolute:" << endl << ((GSE_JointNode *)pNode)->GetJoint()->m_mAbsolute);
    GSE_DEBUG( "relative:" << endl << ((GSE_JointNode *)pNode)->GetJoint()->m_mRelative );
    GSE_DEBUG( "final:"    << endl << ((GSE_JointNode *)pNode)->GetJoint()->m_mFinal );
    break;
  case PARTICLESYSTEM:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "ParticleSystem" << std::endl;
    break;
  case ROOT:
    GSE_Logger::Error() << m_sPadding <<  m_sArrow <<  "AnimRoot" << std::endl;
    break;
  }
  
  m_sPadding.append(1,' ');
  m_sPadding.append(2,' ');
  
  return 0;
}
/// Handles RENDER_PASS_DEBUG_PRINT pass.
int 
GSE_AnimationRenderer::Handle_DebugPrint_Leave( GSE_GraphNode<NodeType> *pNode )
{
  m_sPadding.erase(0,3);
  return 0;
} 
// undeclare IS_FORCEUPDATE() since it is only local macro.
#undef IS_FORCEUPDATE

/// The implementation for animation sequence selection.
void 
GSE_AnimationRenderer::SelectAnimationSequence( unsigned int nStartTime, 
						unsigned int nEndTime,
						bool bLoop)
{
  m_nAnimSelectStartTime = nStartTime;
  m_nAnimSelectEndTime = nEndTime;
  m_nAnimSelectLoop = bLoop;
}

void
CalculateTranslation( GSE_Joint *pJoint, GSE_Matrix4x4f & mTranslation)
{
  
  unsigned int nFrame = 0;
  int iTmpTime = 0;            // The actual time in milliseconds, 
			       // between 0 -> animation length

  // Check the next translation keyframe we will be interpolating to
  nFrame = pJoint->m_nCurrentTranslationKeyframe;
  GSE_DEBUG("Translation keyframe was : " << nFrame );
  
  iTmpTime = pJoint->m_iStartTime + pJoint->m_iCurrentTime;
  
  while ( nFrame   < pJoint->m_nNumTranslationKeyframes &&
	  pJoint->m_pTranslationKeyframes[nFrame].m_iTime < iTmpTime )
  {
    nFrame++;
  }
      
  pJoint->m_nCurrentTranslationKeyframe = nFrame;
  GSE_DEBUG("Translation keyframe selected : " << nFrame );

  

  // first keyframe
  if ( nFrame == 0 ) { 
    mTranslation = Math::TranslationMatrix(pJoint->m_pTranslationKeyframes[0].m_vValues);  
  } 
  // last keyframe    
  else if ( nFrame >= pJoint->m_nNumTranslationKeyframes ) 
  {   
    mTranslation = Math::TranslationMatrix(pJoint->m_pTranslationKeyframes[nFrame-1].m_vValues); 
  }
  else 
  {
    // Perform linear interpolation between keyframes
    GSE_JointKeyframe &kfTowards = pJoint->m_pTranslationKeyframes[nFrame];
    GSE_JointKeyframe &kfFrom    = pJoint->m_pTranslationKeyframes[nFrame-1];
    float fInterpolation = (float)( iTmpTime - kfFrom.m_iTime ) / 
    (float)( kfTowards.m_iTime - kfFrom.m_iTime);
    //vTranslate = 
    mTranslation = Math::TranslationMatrix(kfFrom.m_vValues + ( kfTowards.m_vValues - kfFrom.m_vValues ) * fInterpolation);
  }
}

void
CalculateRotation( GSE_Joint *pJoint, GSE_Matrix4x4f &mTransform)
{


  unsigned int nFrame = 0;
  int iTmpTime = 0;
  GSE_Quaternion qFrom, qTowards, qFinal;
  
  iTmpTime = pJoint->m_iStartTime + pJoint->m_iCurrentTime;
  
  // Check the next rotation keyframe we will be interpolating to 
  nFrame = pJoint->m_nCurrentRotationKeyframe;

  while ( nFrame < pJoint->m_nNumRotationKeyframes &&
	  pJoint->m_pRotationKeyframes[nFrame].m_iTime<iTmpTime  )
  {
    nFrame++;
  }
  pJoint->m_nCurrentRotationKeyframe = nFrame;
      
  GSE_DEBUG("Rotation keyframe selected : " << nFrame );

  // The first keyframe      
  if ( nFrame == 0 ) { mTransform = RotationMatrix(pJoint->m_pRotationKeyframes[0].m_vValues); } 
  // The last keyframe
  else if ( nFrame >= pJoint->m_nNumRotationKeyframes )
  {
    mTransform = RotationMatrix(pJoint->m_pRotationKeyframes[nFrame-1].m_vValues);
  } 
  else 
  {
    // Perform SLERP between keyframes
    GSE_JointKeyframe &kfTowards = pJoint->m_pRotationKeyframes[nFrame];
    GSE_JointKeyframe &kfFrom    = pJoint->m_pRotationKeyframes[nFrame-1];
	
    float fInterpolation = (float)( iTmpTime - kfFrom.m_iTime ) / (float)( kfTowards.m_iTime - kfFrom.m_iTime);

    // Create quaternions from Euler angles
    GSE_Quaternion qX, qY, qZ;
	
    qX.CreateFromAxisAngleRad(1.0, 0.0, 0.0, kfFrom.m_vValues[GSE_Vector3::X]);
    qY.CreateFromAxisAngleRad(0.0, 1.0, 0.0, kfFrom.m_vValues[GSE_Vector3::Y]);
    qZ.CreateFromAxisAngleRad(0.0, 0.0, 1.0, kfFrom.m_vValues[GSE_Vector3::Z]);
    qFrom = qZ * qY * qX;
	
    qX.CreateFromAxisAngleRad(1.0, 0.0, 0.0, kfTowards.m_vValues[GSE_Vector3::X]);
    qY.CreateFromAxisAngleRad(0.0, 1.0, 0.0, kfTowards.m_vValues[GSE_Vector3::Y]);
    qZ.CreateFromAxisAngleRad(0.0, 0.0, 1.0, kfTowards.m_vValues[GSE_Vector3::Z]);
    qTowards = qZ * qY * qX;
	
    // Perform spherical linear interpolation between quaternions
    qFinal = GSE_Quaternion::Slerp( qFrom, qTowards, fInterpolation );
	
    // Convert quaternion into a matrix
    mTransform = qFinal.ToMatrix();

  }

}
