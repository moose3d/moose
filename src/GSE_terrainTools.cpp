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
#include "GSE_terrainTools.h"
#include "GSE_Logger.h"
#include "GSE_GeometryData.h"
#include "GSE_SceneGraph.h"
#include "GSE_Octree.h"
#include <SDL.h> 
#include <SDL_image.h>
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::list;
using Core::GSE_Logger;
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
int
GSE_TerrainConverter::LoadGeometry( const string &rsFilename, 
				    GSE_GeometryData **ppGeometry, 
				    std::list<unsigned int> &lstIndices,
				    float fStepSize,
				    float fMaxHeight,
				    const GSE_Vector3 &vPosition)
{

  /////////////////////////////////////////////////////////////////
  SDL_Surface *pTmpSurface  = IMG_Load(rsFilename.c_str());
  if ( pTmpSurface == 0 )
  {
    GSE_ERR( "Image loading error: " << IMG_GetError() );
    return 1;
  } 
  /////////////////////////////////////////////////////////////////
  if ( pTmpSurface->format->BytesPerPixel != 1 )
  { 
    GSE_ERR( "Not grayscale image, bytesperpixel = " 
	     << pTmpSurface->format->BytesPerPixel);
    SDL_FreeSurface( pTmpSurface);
    return 2;
  }
  /////////////////////////////////////////////////////////////////
  unsigned int nWidth  = pTmpSurface->w;
  unsigned int nHeight = pTmpSurface->h;
  GSE_ERR( "Got terrain  " << nWidth << " x " << nHeight );
  /////////////////////////////////////////////////////////////////
  GSE_GeometryData *pGeometryData = GSE_GeometryDataMgr::GetInstance()->CreateGeometryData();
  pGeometryData->SetVertexCount( nWidth * nHeight );
  pGeometryData->InitVertices();
  pGeometryData->InitTexCoords(0);
  pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::REQUEST_VBO_CACHE);
  pGeometryData->SetCacheMethod( GSE_GeometryData::TEXCOORD0_ARRAY, GSE_GeometryData::REQUEST_VBO_CACHE);
  /////////////////////////////////////////////////////////////////
  Uint8 *pPixels = (Uint8 *)pTmpSurface->pixels;  
  float fHeightFactor = fMaxHeight / 255.0f;
  unsigned int nIndex = 0;
  /////////////////////////////////////////////////////////////////
  float fTexCoordXFactor = 1.0f / (((float)nWidth)*0.05f);
  float fTexCoordYFactor = 1.0f / (((float)nHeight)*0.05f);
  float fXoffset = vPosition.m_pValues[GSE_Vector3::X]-nWidth*fStepSize*0.5f;
  float fYoffset = vPosition.m_pValues[GSE_Vector3::Y];
  float fZoffset = vPosition.m_pValues[GSE_Vector3::Z]-nHeight*fStepSize*0.5f;
  
  for(unsigned int h=0;h<nHeight;h++)
  {
    for(unsigned int w=0;w<nWidth;w++)
    {
      nIndex = (h*nWidth)+w;
      GSE_Vector3 vVertex( ((float)w)*fStepSize + fXoffset, 
			   ((float)pPixels[nIndex]) * fHeightFactor + fYoffset,
			   ((float)h)*fStepSize + fZoffset);
      // Set vertex
      pGeometryData->SetVertex(nIndex, vVertex );
      pGeometryData->SetTexCoords( 0, nIndex, GSE_Vector2(((float)w)*fTexCoordXFactor,((float)h)*fTexCoordYFactor));
      // Determine face indices, starting from second row.
      if ( h > 0 )
      {
	////////////////////
	// *  *  *  *  *
        //   /|       /|
	//  / | and  / |
	// *--*     *--*
	if ( w != 0 || w == nWidth - 1)
	{
	  lstIndices.push_back(nIndex-nWidth);
	  lstIndices.push_back(nIndex-1);
	  lstIndices.push_back(nIndex);
	  
	} 

	////////////////////
	// *--*
        // | /
	// |/ and others
	// *
	if ( w < nWidth - 1 )
	{
	  lstIndices.push_back(nIndex-nWidth);
	  lstIndices.push_back(nIndex);
	  lstIndices.push_back(nIndex-nWidth+1);
	}
	
      }
    }
  }
  *ppGeometry = pGeometryData;
  SDL_FreeSurface( pTmpSurface);
  return 0;
}
/////////////////////////////////////////////////////////////////
void
GSE_TerrainConverter::CreateTerrainOctreeFromFile( const string &rsFilename,
						   GSE_SceneGraph &rSceneGraph,
						   GSE_GeometryOctree **ppGeometryOctree,
						   GSE_GeometryNode **ppGeometryNode,
						   float fStepSize,
						   float fMaxHeight,
						   unsigned int nMaxTreeDepth,
						   unsigned int nMinFacesInNode,
						   const GSE_Vector3 &vPosition )
{


  GSE_GeometryData *pGeometryData = NULL;
  list<unsigned int> lstIndices;
  if ( LoadGeometry( rsFilename, &pGeometryData, lstIndices, fStepSize, fMaxHeight, vPosition) )
  {
    return;
  }
  /////////////////////////////////////////////////////////////////
  GSE_GeometryNode *pGeomNode = rSceneGraph.GetRenderGraph().CreateNode<GSE_GeometryNode>();
  pGeomNode->SetGeometryData( pGeometryData );
  pGeomNode->SetGeometryDataWorld( pGeometryData );
  *ppGeometryNode = pGeomNode;
  
  // Construct an GeometryOctree from the vertices.
  GSE_AxisAlignedBox aaBox = Geometry::CalculateAlignedBox( pGeometryData->m_pVertices,
							    pGeometryData->m_iNumElements);
  cerr << "got box: " 
       << aaBox.GetPosition()  << " | " 
       << aaBox.GetWidth() << "," << aaBox.GetHeight() << "," << aaBox.GetLength() << endl;
  
  *ppGeometryOctree = GSE_GeometryOctree::Split( pGeometryData->m_pVertices,
						 lstIndices, nMaxTreeDepth,
						 nMinFacesInNode,
						 aaBox.GetWidth(),
						 aaBox.GetHeight(),
						 aaBox.GetLength(),
						 aaBox.GetPosition(), 
						 rSceneGraph.GetRenderGraph());
  /////////////////////////////////////////////////////////////////

}
/////////////////////////////////////////////////////////////////
void 
GSE_TerrainConverter::CreateTerrainObjectFromFile(  const string &rsFilename,
						    GSE_SceneGraph &rSceneGraph,
						    GSE_GeometryNode **ppGeometryNode,
						    float fStepSize,float fMaxHeight, const GSE_Vector3 &vPosition )
{
  GSE_GeometryData *pGeometryData = NULL;
  list<unsigned int> lstIndices;
  if ( LoadGeometry( rsFilename, &pGeometryData, lstIndices, fStepSize, fMaxHeight, vPosition) )
  {
    return;
  }
  /////////////////////////////////////////////////////////////////
  GSE_GeometryNode *pGeomNode = rSceneGraph.GetRenderGraph().CreateNode<GSE_GeometryNode>();
  GSE_IndexBufferNode *pIndexBufferNode = rSceneGraph.GetRenderGraph().CreateNode<GSE_IndexBufferNode>();
  GSE_IndexBuffer *pIndexBuffer =  GSE_IndexBufferMgr::GetInstance()->CreateIndexBuffer();
  pGeomNode->SetGeometryData( pGeometryData );
  pGeomNode->SetGeometryDataWorld( pGeometryData );
  *ppGeometryNode = pGeomNode;
  pGeomNode->AddEdge(pIndexBufferNode);
  pIndexBuffer->Init( lstIndices.size() );
  pIndexBufferNode->SetIndexBuffer( pIndexBuffer );
  
  list<unsigned int>::iterator it = lstIndices.begin();
  for(unsigned int n=0;it!=lstIndices.end();n++,it++)
  {
    pIndexBuffer->m_pIndices[n] = *it;
  }
  
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_terrainTools.cpp,v $
// Revision 1.10  2007/04/04 08:59:25  entity
// terrain texcoords are now cached
//
// Revision 1.9  2007/04/02 12:55:13  entity
// terrain center
//
// Revision 1.8  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.7  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.6  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.5  2007/03/30 12:56:32  entity
// SetElements->SetVertexCount rename
//
// Revision 1.4  2007/03/29 21:15:57  entity
// texture coordinates added
//
// Revision 1.3  2007/03/29 13:11:37  entity
// more modular implementation
//
// Revision 1.2  2007/03/29 06:54:06  entity
// added nMaxTreeDepth and nMinfacesInNode arguments for 
// CreateGeometryOctreeFromFile and finished the implementation.
//
// Revision 1.1  2007/03/28 21:03:55  entity
// first version
//
/////////////////////////////////////////////////////////////////
