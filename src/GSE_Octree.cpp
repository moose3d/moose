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
/////////////////////////////////////////////////////////////////
#include "GSE_Octree.h"
#include "GSE_Logger.h"
#include "GSE_structures.h"
#include "GSE_GraphNodes.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
int IsInside(  GSE_Vector3 &vCenter,
	       float fHalfWidth, float fHalfHeight, float fHalfLength,
	       GSE_Vector3 &vPosition)
{
  int bRetval = 0;

#define EPSILON 0.0001f
  if ( vPosition[GSE_Vector3::X] <= vCenter[GSE_Vector3::X]+(fHalfWidth)+EPSILON &&
       vPosition[GSE_Vector3::X] >= vCenter[GSE_Vector3::X]-(fHalfWidth)-EPSILON){
    
    if ( vPosition[GSE_Vector3::Y] <= vCenter[GSE_Vector3::Y]+(fHalfHeight)+EPSILON &&
	 vPosition[GSE_Vector3::Y] >=  vCenter[GSE_Vector3::Y]-(fHalfHeight)-EPSILON){
      if ( vPosition[GSE_Vector3::Z] <= vCenter[GSE_Vector3::Z]+(fHalfLength)+EPSILON &&
	   vPosition[GSE_Vector3::Z] >= vCenter[GSE_Vector3::Z]-(fHalfLength)-EPSILON){
	bRetval = 1;
      } 
    } 
  }
  
  return bRetval;
}
/////////////////////////////////////////////////////////////////
/// Macro for easier deletion
#define DELETE(A) \
 {\
  if ( A != NULL ){\
    delete A;\
    A = NULL;\
  }\
}
/////////////////////////////////////////////////////////////////
GSE_Octree::GSE_Octree()
{
  // initialize nodes 
  m_pNodes[TOP_LEFT_FRONT] = NULL;
  m_pNodes[TOP_LEFT_BACK] = NULL;
  m_pNodes[TOP_RIGHT_FRONT] = NULL;
  m_pNodes[TOP_RIGHT_BACK] = NULL;
  m_pNodes[BOTTOM_LEFT_FRONT] = NULL;
  m_pNodes[BOTTOM_LEFT_BACK] = NULL;
  m_pNodes[BOTTOM_RIGHT_FRONT] = NULL;
  m_pNodes[BOTTOM_RIGHT_BACK] = NULL;
}
/////////////////////////////////////////////////////////////////
void
GSE_Octree::Init()
{

  DELETE(m_pNodes[TOP_LEFT_FRONT]);
  DELETE(m_pNodes[TOP_LEFT_BACK]);
  DELETE(m_pNodes[TOP_RIGHT_FRONT]);
  DELETE(m_pNodes[TOP_RIGHT_BACK]);
  DELETE(m_pNodes[BOTTOM_LEFT_FRONT]);
  DELETE(m_pNodes[BOTTOM_LEFT_BACK]);
  DELETE(m_pNodes[BOTTOM_RIGHT_FRONT]);
  DELETE(m_pNodes[BOTTOM_RIGHT_BACK]);
  
}
/////////////////////////////////////////////////////////////////
GSE_Octree::~GSE_Octree()
{
  Init();
}
/////////////////////////////////////////////////////////////////
GSE_Octree *
GSE_Octree::operator[]( Octree_Section iSection )
{
  return m_pNodes[iSection];
}
/////////////////////////////////////////////////////////////////
void
GSE_Octree::SetNode( Octree_Section iSection, GSE_Octree *pNode)
{
  m_pNodes[iSection] = pNode;
}
/////////////////////////////////////////////////////////////////
void
GSE_Octree::DeleteNode( Octree_Section iSection )
{
  delete m_pNodes[iSection];
  m_pNodes[iSection] = NULL;
}
/////////////////////////////////////////////////////////////////
GSE_Octree **
GSE_Octree::GetNodes(){

  return m_pNodes;
} 
/////////////////////////////////////////////////////////////////
GSE_GeometryOctree *
GSE_GeometryOctree::Split( VertexArray pVertices, 
			   std::list<unsigned int> & lstIndices,
			   unsigned int nMaxTreeDepth, 
			   unsigned int nMinFacesInNode,
			   float fWidth,
			   float fHeight,
			   float fLength,
			   const GSE_Vector3 &vCenter,
			   GSE_RenderGraph &rRenderGraph)
{
  
  GSE_GeometryOctree *pOctree  = NULL;
  
  std::list<unsigned int> lstNode1;
  std::list<unsigned int> lstNode2;
  std::list<unsigned int> lstNode3;
  std::list<unsigned int> lstNode4;
  std::list<unsigned int> lstNode5;
  std::list<unsigned int> lstNode6;
  std::list<unsigned int> lstNode7;
  std::list<unsigned int> lstNode8;
  std::list<unsigned int>::iterator it;
  
  GSE_Vector3 vTopLeftFront;
  GSE_Vector3 vTopLeftBack;
  GSE_Vector3 vTopRightFront;
  GSE_Vector3 vTopRightBack;
  
  GSE_Vector3 vBottomLeftFront;
  GSE_Vector3 vBottomLeftBack;
  GSE_Vector3 vBottomRightFront;
  GSE_Vector3 vBottomRightBack;
  
  GSE_Vector3 vVec1;
  GSE_Vector3 vVec2;
  GSE_Vector3 vVec3;
  GSE_Vector3 vTmp;
  float fWidthDiv2  = fWidth * 0.5f;
  float fHeightDiv2 = fHeight * 0.5f;
  float fLengthDiv2 = fLength * 0.5f;
  float fWidthDiv4  = fWidth * 0.25f;
  float fHeightDiv4 = fHeight * 0.25f;
  float fLengthDiv4 = fLength * 0.25f;

  int  bIndicesIncluded = 0;

  if ( lstIndices.size() > nMinFacesInNode && nMaxTreeDepth > 0){

    /////////////////////////////////////////////////////////////////
    // Check each face for does it belong to which of the child nodes.
    //
    for(it = lstIndices.begin(); it!=lstIndices.end();++it )
    {
      unsigned int index1 = (*it);
      unsigned int index2  =(*(++it));
      unsigned int index3  =(*(++it));

      bIndicesIncluded = 0;
      
      vVec1.UseExternalData(&(pVertices[index1*3]));
      vVec2.UseExternalData(&(pVertices[index2*3]));
      vVec3.UseExternalData(&(pVertices[index3*3]));

      vTmp[GSE_Vector3::X] = -fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  fHeightDiv4;
      vTmp[GSE_Vector3::Z] =  fLengthDiv4;
      vTopLeftFront  = vCenter + vTmp;
      vTmp[GSE_Vector3::X] = -fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  fHeightDiv4;
      vTmp[GSE_Vector3::Z] = -fLengthDiv4;
      vTopLeftBack   = vCenter + vTmp;
      vTmp[GSE_Vector3::X] =  fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  fHeightDiv4;
      vTmp[GSE_Vector3::Z] =  fLengthDiv4;
      vTopRightFront = vCenter + vTmp;
      vTmp[GSE_Vector3::X] =  fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  fHeightDiv4;
      vTmp[GSE_Vector3::Z] =  -fLengthDiv4;
      vTopRightBack  = vCenter + vTmp;
      /////////////////////////////////////////////////////////////////
      vTmp[GSE_Vector3::X] =  -fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  -fHeightDiv4;
      vTmp[GSE_Vector3::Z] =   fLengthDiv4;
      vBottomLeftFront  = vCenter + vTmp;
      vTmp[GSE_Vector3::X] =  -fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  -fHeightDiv4;
      vTmp[GSE_Vector3::Z] =  -fLengthDiv4;
      vBottomLeftBack   = vCenter + vTmp;
      vTmp[GSE_Vector3::X] =   fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  -fHeightDiv4;
      vTmp[GSE_Vector3::Z] =   fLengthDiv4;
      vBottomRightFront = vCenter + vTmp;
      vTmp[GSE_Vector3::X] =   fWidthDiv4;
      vTmp[GSE_Vector3::Y] =  -fHeightDiv4;
      vTmp[GSE_Vector3::Z] =  -fLengthDiv4;
      vBottomRightBack  = vCenter + vTmp;

      // Sort the indices by faces 
      if( IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode1.push_back(index1);
	lstNode1.push_back(index2);
	lstNode1.push_back(index3);
	bIndicesIncluded = 1;
      } 
      if( IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode2.push_back( index1);
	lstNode2.push_back( index2);
	lstNode2.push_back( index3);
	bIndicesIncluded = 1;
      }
      if( IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode3.push_back( index1);
	lstNode3.push_back( index2);
	lstNode3.push_back( index3);
	bIndicesIncluded = 1;
      }	  
      if( IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode4.push_back( index1);
	lstNode4.push_back( index2);
	lstNode4.push_back( index3);
	bIndicesIncluded = 1;
      }
      
      if( IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode5.push_back( index1);
	lstNode5.push_back( index2);
	lstNode5.push_back( index3);
	bIndicesIncluded = 1;
      } 
      if( IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode6.push_back( index1);
	lstNode6.push_back( index2);
	lstNode6.push_back( index3);
	bIndicesIncluded = 1;
      }
      if( IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode7.push_back( index1 );
	lstNode7.push_back( index2 );
	lstNode7.push_back( index3 );
	bIndicesIncluded = 1;
      }	  
      if( IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
	  IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
	  IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
      {
	lstNode8.push_back( index1 );
	lstNode8.push_back( index2 );
	lstNode8.push_back( index3 );
	bIndicesIncluded = 1;
      }

      //assert(bIndicesIncluded != 0 );
    }
    // Clear elements from this node's list since they aren't used any longer.
    lstIndices.clear();

    // Assign proper dimensions and position
    pOctree = new GSE_GeometryOctree();
    pOctree->SetWidth(fWidth);
    pOctree->SetHeight(fHeight);
    pOctree->SetLength(fLength);
    pOctree->SetPosition(vCenter);
    //cerr << "Subdividing at level " << nMaxTreeDepth << endl;
    // Call split for each child node recursively.
    pOctree->SetNode( GSE_Octree::TOP_LEFT_FRONT, 
		      GSE_GeometryOctree::Split( pVertices, lstNode1,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vTopLeftFront,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::TOP_LEFT_BACK, 
		      GSE_GeometryOctree::Split( pVertices, lstNode2,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vTopLeftBack,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::TOP_RIGHT_FRONT, 
		      GSE_GeometryOctree::Split( pVertices, lstNode3,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vTopRightFront,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::TOP_RIGHT_BACK, 
		      GSE_GeometryOctree::Split( pVertices, lstNode4,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vTopRightBack,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::BOTTOM_LEFT_FRONT, 
		      GSE_GeometryOctree::Split( pVertices, lstNode5,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vBottomLeftFront,
						 rRenderGraph));

    pOctree->SetNode( GSE_Octree::BOTTOM_LEFT_BACK, 
		      GSE_GeometryOctree::Split( pVertices, lstNode6,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vBottomLeftBack,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::BOTTOM_RIGHT_FRONT, 
		      GSE_GeometryOctree::Split( pVertices, lstNode7,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vBottomRightFront,
						 rRenderGraph));
    
    pOctree->SetNode( GSE_Octree::BOTTOM_RIGHT_BACK, 
		      GSE_GeometryOctree::Split( pVertices, lstNode8,
						 nMaxTreeDepth-1,nMinFacesInNode,
						 fWidthDiv2, fHeightDiv2,
						 fLengthDiv2, vBottomRightBack,
						 rRenderGraph));
    

  } 
  else
  {
    // If there aren't any vertices to add into this octree
    if ( lstIndices.size() == 0 )
    {
      
    } 
    else 
    {

      pOctree = new GSE_GeometryOctree();
      // Assign proper dimensions and position for the box
      pOctree->SetWidth(fWidth);
      pOctree->SetHeight(fHeight);
      pOctree->SetLength(fLength);
      pOctree->SetPosition(vCenter);

      // Generate indexbuffer
      GSE_IndexBuffer *pIndexBuffer = GSE_IndexBufferMgr::GetInstance()->CreateIndexBuffer();
      GSE_IndexBufferNode *pIndexBufferNode = rRenderGraph.CreateNode<GSE_IndexBufferNode>();
      pIndexBufferNode->SetIndexBuffer( pIndexBuffer );
      pOctree->SetIndexBufferNode( pIndexBufferNode );
      pOctree->GetIndexBufferNode()->GetIndexBuffer()->Init(lstIndices.size());
      
      // Copy indices to the indexbuffer
      unsigned int iIndex = 0;
      for(it = lstIndices.begin(); it!=lstIndices.end(); it++, iIndex++)
      {
	pIndexBuffer->m_pIndices[iIndex] = *it;
      }
      lstIndices.clear();

    }
  }
  return pOctree;
}
/////////////////////////////////////////////////////////////////
