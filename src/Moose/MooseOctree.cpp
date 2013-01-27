/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseOctree.h"

#include <iostream>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
// int IsInside(  CVector3 &vCenter,
// 	       float fHalfWidth, float fHalfHeight, float fHalfLength,
// 	       CVector3 &vPosition)
// {
//   int bRetval = 0;
//   //#define EPSILON 0.0001f
//   if ( vPosition[CVector3::X] <= vCenter[CVector3::X]+(fHalfWidth)+EPSILON &&
//        vPosition[CVector3::X] >= vCenter[CVector3::X]-(fHalfWidth)-EPSILON){
    
//     if ( vPosition[CVector3::Y] <= vCenter[CVector3::Y]+(fHalfHeight)+EPSILON &&
// 	 vPosition[CVector3::Y] >=  vCenter[CVector3::Y]-(fHalfHeight)-EPSILON){
//       if ( vPosition[CVector3::Z] <= vCenter[CVector3::Z]+(fHalfLength)+EPSILON &&
// 	   vPosition[CVector3::Z] >= vCenter[CVector3::Z]-(fHalfLength)-EPSILON){
// 	bRetval = 1;
//       } 
//     } 
//   }
  
//   return bRetval;
// }


// /////////////////////////////////////////////////////////////////

// CGeometryOctree *
// CGeometryOctree::Split( VertexDescriptor pVertices, 
// 			   std::list<unsigned int> & lstIndices,
// 			   unsigned int nMaxTreeDepth, 
// 			   unsigned int nMinFacesInNode,
// 			   float fWidth,
// 			   float fHeight,
// 			   float fLength,
// 			   const CVector3 &vCenter,
// 			   CRenderGraph &rRenderGraph)
// {
  
//   CGeometryOctree *pOctree  = NULL;
  
//   std::list<unsigned int> lstNode1;
//   std::list<unsigned int> lstNode2;
//   std::list<unsigned int> lstNode3;
//   std::list<unsigned int> lstNode4;
//   std::list<unsigned int> lstNode5;
//   std::list<unsigned int> lstNode6;
//   std::list<unsigned int> lstNode7;
//   std::list<unsigned int> lstNode8;
//   std::list<unsigned int>::iterator it;
  
//   CVector3 vTopLeftFront;
//   CVector3 vTopLeftBack;
//   CVector3 vTopRightFront;
//   CVector3 vTopRightBack;
  
//   CVector3 vBottomLeftFront;
//   CVector3 vBottomLeftBack;
//   CVector3 vBottomRightFront;
//   CVector3 vBottomRightBack;
  
//   CVector3 vVec1;
//   CVector3 vVec2;
//   CVector3 vVec3;
//   CVector3 vTmp;
//   float fWidthDiv2  = fWidth * 0.5f;
//   float fHeightDiv2 = fHeight * 0.5f;
//   float fLengthDiv2 = fLength * 0.5f;
//   float fWidthDiv4  = fWidth * 0.25f;
//   float fHeightDiv4 = fHeight * 0.25f;
//   float fLengthDiv4 = fLength * 0.25f;

//   int  bIndicesIncluded = 0;

//   if ( lstIndices.size() > nMinFacesInNode && nMaxTreeDepth > 0){

//     /////////////////////////////////////////////////////////////////
//     // Check each face for does it belong to which of the child nodes.
//     //
//     for(it = lstIndices.begin(); it!=lstIndices.end();++it )
//     {
//       unsigned int index1 = (*it);
//       unsigned int index2  =(*(++it));
//       unsigned int index3  =(*(++it));

//       bIndicesIncluded = 0;
      
//       vVec1.UseExternalData(&(pVertices[index1*3]));
//       vVec2.UseExternalData(&(pVertices[index2*3]));
//       vVec3.UseExternalData(&(pVertices[index3*3]));

//       vTmp[CVector3::X] = -fWidthDiv4;
//       vTmp[CVector3::Y] =  fHeightDiv4;
//       vTmp[CVector3::Z] =  fLengthDiv4;
//       vTopLeftFront  = vCenter + vTmp;
//       vTmp[CVector3::X] = -fWidthDiv4;
//       vTmp[CVector3::Y] =  fHeightDiv4;
//       vTmp[CVector3::Z] = -fLengthDiv4;
//       vTopLeftBack   = vCenter + vTmp;
//       vTmp[CVector3::X] =  fWidthDiv4;
//       vTmp[CVector3::Y] =  fHeightDiv4;
//       vTmp[CVector3::Z] =  fLengthDiv4;
//       vTopRightFront = vCenter + vTmp;
//       vTmp[CVector3::X] =  fWidthDiv4;
//       vTmp[CVector3::Y] =  fHeightDiv4;
//       vTmp[CVector3::Z] =  -fLengthDiv4;
//       vTopRightBack  = vCenter + vTmp;
//       /////////////////////////////////////////////////////////////////
//       vTmp[CVector3::X] =  -fWidthDiv4;
//       vTmp[CVector3::Y] =  -fHeightDiv4;
//       vTmp[CVector3::Z] =   fLengthDiv4;
//       vBottomLeftFront  = vCenter + vTmp;
//       vTmp[CVector3::X] =  -fWidthDiv4;
//       vTmp[CVector3::Y] =  -fHeightDiv4;
//       vTmp[CVector3::Z] =  -fLengthDiv4;
//       vBottomLeftBack   = vCenter + vTmp;
//       vTmp[CVector3::X] =   fWidthDiv4;
//       vTmp[CVector3::Y] =  -fHeightDiv4;
//       vTmp[CVector3::Z] =   fLengthDiv4;
//       vBottomRightFront = vCenter + vTmp;
//       vTmp[CVector3::X] =   fWidthDiv4;
//       vTmp[CVector3::Y] =  -fHeightDiv4;
//       vTmp[CVector3::Z] =  -fLengthDiv4;
//       vBottomRightBack  = vCenter + vTmp;

//       // Sort the indices by faces 
//       if( IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vTopLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode1.push_back(index1);
// 	lstNode1.push_back(index2);
// 	lstNode1.push_back(index3);
// 	bIndicesIncluded = 1;
//       } 
//       if( IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vTopLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode2.push_back( index1);
// 	lstNode2.push_back( index2);
// 	lstNode2.push_back( index3);
// 	bIndicesIncluded = 1;
//       }
//       if( IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vTopRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode3.push_back( index1);
// 	lstNode3.push_back( index2);
// 	lstNode3.push_back( index3);
// 	bIndicesIncluded = 1;
//       }	  
//       if( IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vTopRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode4.push_back( index1);
// 	lstNode4.push_back( index2);
// 	lstNode4.push_back( index3);
// 	bIndicesIncluded = 1;
//       }
      
//       if( IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vBottomLeftFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode5.push_back( index1);
// 	lstNode5.push_back( index2);
// 	lstNode5.push_back( index3);
// 	bIndicesIncluded = 1;
//       } 
//       if( IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vBottomLeftBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode6.push_back( index1);
// 	lstNode6.push_back( index2);
// 	lstNode6.push_back( index3);
// 	bIndicesIncluded = 1;
//       }
//       if( IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vBottomRightFront, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode7.push_back( index1 );
// 	lstNode7.push_back( index2 );
// 	lstNode7.push_back( index3 );
// 	bIndicesIncluded = 1;
//       }	  
//       if( IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec1) ||
// 	  IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec2) ||
// 	  IsInside(vBottomRightBack, fWidthDiv4, fHeightDiv4, fLengthDiv4, vVec3))
//       {
// 	lstNode8.push_back( index1 );
// 	lstNode8.push_back( index2 );
// 	lstNode8.push_back( index3 );
// 	bIndicesIncluded = 1;
//       }

//       //assert(bIndicesIncluded != 0 );
//     }
//     // Clear elements from this node's list since they aren't used any longer.
//     lstIndices.clear();

//     // Assign proper dimensions and position
//     pOctree = new CGeometryOctree();
//     pOctree->SetWidth(fWidth);
//     pOctree->SetHeight(fHeight);
//     pOctree->SetLength(fLength);
//     pOctree->SetPosition(vCenter);
//     //cerr << "Subdividing at level " << nMaxTreeDepth << endl;
//     // Call split for each child node recursively.
//     pOctree->SetNode( COctree::O_TOP_LEFT_FRONT, 
// 		      CGeometryOctree::Split( pVertices, lstNode1,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vTopLeftFront,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::O_TOP_LEFT_BACK, 
// 		      CGeometryOctree::Split( pVertices, lstNode2,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vTopLeftBack,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::TOP_RIGHT_FRONT, 
// 		      CGeometryOctree::Split( pVertices, lstNode3,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vTopRightFront,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::TOP_RIGHT_BACK, 
// 		      CGeometryOctree::Split( pVertices, lstNode4,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vTopRightBack,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::BOTTOM_LEFT_FRONT, 
// 		      CGeometryOctree::Split( pVertices, lstNode5,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vBottomLeftFront,
// 						 rRenderGraph));

//     pOctree->SetNode( COctree::BOTTOM_LEFT_BACK, 
// 		      CGeometryOctree::Split( pVertices, lstNode6,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vBottomLeftBack,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::BOTTOM_RIGHT_FRONT, 
// 		      CGeometryOctree::Split( pVertices, lstNode7,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vBottomRightFront,
// 						 rRenderGraph));
    
//     pOctree->SetNode( COctree::BOTTOM_RIGHT_BACK, 
// 		      CGeometryOctree::Split( pVertices, lstNode8,
// 						 nMaxTreeDepth-1,nMinFacesInNode,
// 						 fWidthDiv2, fHeightDiv2,
// 						 fLengthDiv2, vBottomRightBack,
// 						 rRenderGraph));
    

//   } 
//   else
//   {
//     // If there aren't any vertices to add into this octree
//     if ( lstIndices.size() == 0 )
//     {
      
//     } 
//     else 
//     {

//       pOctree = new CGeometryOctree();
//       // Assign proper dimensions and position for the box
//       pOctree->SetWidth(fWidth);
//       pOctree->SetHeight(fHeight);
//       pOctree->SetLength(fLength);
//       pOctree->SetPosition(vCenter);

//       // Generate indexbuffer
//       CIndexBuffer *pIndexBuffer = CIndexBufferMgr::GetInstance()->CreateIndexBuffer();
//       CIndexBufferNode *pIndexBufferNode = rRenderGraph.CreateNode<CIndexBufferNode>();
//       pIndexBufferNode->SetIndexBuffer( pIndexBuffer );
//       pOctree->SetIndexBufferNode( pIndexBufferNode );
//       pOctree->GetIndexBufferNode()->GetIndexBuffer()->Init(lstIndices.size());
      
//       // Copy indices to the indexbuffer
//       unsigned int iIndex = 0;
//       for(it = lstIndices.begin(); it!=lstIndices.end(); it++, iIndex++)
//       {
// 	pIndexBuffer->m_pIndices[iIndex] = *it;
//       }
//       lstIndices.clear();

//     }
//   }
//   return pOctree;
// }
// /////////////////////////////////////////////////////////////////
