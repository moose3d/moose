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
#include "GSE_3dsTools.h"
#include "GSE_IndexBuffer.h"
#include "GSE_GeometryData.h"
#include <lib3ds/file.h>
#include <lib3ds/node.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <map>
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::map;
using std::list;
using std::cerr;
using std::endl;
typedef map< string, list<unsigned int> > MaterialNameToIndexMap;
/////////////////////////////////////////////////////////////////
#define HAS_MATERIAL(A) ( A.material[0] ) 
/////////////////////////////////////////////////////////////////
void SortFacesByMaterial( MaterialNameToIndexMap &rMapMaterialBuckets, 
			  Lib3dsMesh *pMesh )
{
  /////////////////////////////////////////////////////////////////
  MaterialNameToIndexMap::iterator it;
  rMapMaterialBuckets.clear();
  // Create an mapping for default material
  list<unsigned int> lstIndices;
  rMapMaterialBuckets.insert( make_pair(string("GSE_DEFAULT"),lstIndices) );
  /////////////////////////////////////////////////////////////////
  cerr << "The mesh has " << pMesh->faces << " faces" << endl;
  for(unsigned int f=0;f<pMesh->faces;f++)
  {
    Lib3dsFace &rFace = pMesh->faceL[f];
    if ( HAS_MATERIAL(rFace) ) 
    {
      it = rMapMaterialBuckets.find(string(rFace.material));
      /////////////////////////////////////////////////////////////////
      if ( it == rMapMaterialBuckets.end()) 
      {
	// Create a new list for indices
	lstIndices.clear();
	lstIndices.push_back( rFace.points[0]);
	lstIndices.push_back( rFace.points[1]);
	lstIndices.push_back( rFace.points[2]);
	rMapMaterialBuckets.insert( make_pair( string(rFace.material),lstIndices));
      }
      else 
      {
	// Push indices into old list
	(*it).second.push_back( rFace.points[0]);
	(*it).second.push_back( rFace.points[1]);
	(*it).second.push_back( rFace.points[2]);
      }
      /////////////////////////////////////////////////////////////////
    }
    else 
    {
      // Push indices into default list
      it = rMapMaterialBuckets.find(string("GSE_DEFAULT"));
      (*it).second.push_back( rFace.points[0]);
      (*it).second.push_back( rFace.points[1]);
      (*it).second.push_back( rFace.points[2]);

    }
  } // for( ...
}
/////////////////////////////////////////////////////////////////
/// Converts the given Lib3dsMaterial into GSE_Material.
/// \param pMat the material to be converted.
GSE_Material *
ConvertMaterial ( Lib3dsMaterial *pMat )
{
  GSE_Material   *pGseMat = GSE_MaterialMgr::GetInstance()->CreateMaterial(pMat->name);
  pGseMat->SetAmbient( GSE_Color( pMat->ambient[0], 
				  pMat->ambient[1], 
				  pMat->ambient[2], 
				  pMat->ambient[3]));
  pGseMat->SetDiffuse( GSE_Color( pMat->diffuse[0], 
				  pMat->diffuse[1], 
				  pMat->diffuse[2], 
				  pMat->diffuse[3]));
  pGseMat->SetSpecular( GSE_Color( pMat->specular[0], 
				   pMat->specular[1], 
				   pMat->specular[2], 
				   pMat->specular[3]));
  pGseMat->SetShininess( pMat->shininess );
  return pGseMat;
}
/////////////////////////////////////////////////////////////////
#define ABSOLUTE_NODE_NAME_LENGTH 10
/////////////////////////////////////////////////////////////////
int
GSE_3dsConverter::Convert( const string &sFilename,
			   const string &sNodename,
			   GSE_RenderGraph *pRenderGraph,     
			   GSE_GeometryNode **ppGeometryNode )
{

  Lib3dsFile *pFile = NULL;
  Lib3dsMesh *pMesh = NULL;
  Lib3dsVector *parrNormals;
  MaterialNameToIndexMap mapMaterialBuckets;
  pFile       = lib3ds_file_load(sFilename.c_str());
  /////////////////////////////////////////////////////////////////
  if ( pFile == NULL )
  {
    GSE_ERR("Could not open " << sFilename);
    return -1;
  }
  /////////////////////////////////////////////////////////////////
  // The lib3ds assumes that all node names are buffed with
  // whitespace up to 10 characters.
  /////////////////////////////////////////////////////////////////
  string sNodenameBuffed = sNodename;

  pMesh       = lib3ds_file_mesh_by_name(pFile, sNodenameBuffed.c_str());
  if ( pMesh == NULL )
  {
    GSE_ERR("No such mesh node " << sNodename << " in " << sFilename);
    return -2;
  }
  /////////////////////////////////////////////////////////////////
  if ( pMesh->faces <= 0)
  {
    GSE_ERR("The mesh " << sNodename << " has no faces, bailing out.");
    return -3;
  }
  /////////////////////////////////////////////////////////////////
  parrNormals = new Lib3dsVector[3*pMesh->faces];
  lib3ds_mesh_calculate_normals(pMesh, parrNormals);
  
  SortFacesByMaterial( mapMaterialBuckets, pMesh );
  // Create geometrynode
  *ppGeometryNode = pRenderGraph->CreateNode<GSE_GeometryNode>();
  
  GSE_GeometryData *pGeometryData = GSE_GeometryDataMgr::GetInstance()->CreateGeometryData();
  pGeometryData->SetVertexCount( pMesh->points );
  pGeometryData->InitVertices( );
  pGeometryData->InitNormals();
  // Static object, so cache it for performance.
  pGeometryData->SetCacheMethod( GSE_GeometryData::VERTEX_ARRAY, GSE_GeometryData::REQUEST_VBO_CACHE );
  pGeometryData->SetCacheMethod( GSE_GeometryData::NORMAL_ARRAY, GSE_GeometryData::REQUEST_VBO_CACHE );
  // Copy vertices and normals into the geometry 
  float *pVertices = pGeometryData->m_pVertices;
  float *pNormals  = pGeometryData->m_pNormals;
  
  for( unsigned int nP = 0;nP<pMesh->points;nP++)
  {
    // Swap the y and z axis since they are used differently in 3ds
    pVertices[0] = pMesh->pointL[nP].pos[0];
    pVertices[1] = pMesh->pointL[nP].pos[2];
    pVertices[2] = pMesh->pointL[nP].pos[1];

    pNormals[0] =  parrNormals[nP][0];
    pNormals[1] =  parrNormals[nP][2];
    pNormals[2] =  parrNormals[nP][1];

    pVertices+=3;
    pNormals+=3;
  }
  
  (*ppGeometryNode)->SetGeometryData( pGeometryData );
  (*ppGeometryNode)->SetGeometryDataWorld( pGeometryData );

  MaterialNameToIndexMap::iterator it = mapMaterialBuckets.begin();
  /////////////////////////////////////////////////////////////////
  for( ;it != mapMaterialBuckets.end(); it++)
  {

    if ( (*it).second.size() > 0 )
    {
      GSE_IndexBuffer *pIndexBuffer = GSE_IndexBufferMgr::GetInstance()->CreateIndexBuffer();
      pIndexBuffer->Init( (*it).second.size());
      list< unsigned int >::iterator indexIt = (*it).second.begin();
    
      // Copy each index into the elementlist
      for(unsigned int nCount=0; indexIt != (*it).second.end(); indexIt++,nCount++)
      {
	pIndexBuffer->m_pIndices[nCount] = *indexIt;
      }
      // Since this is intended to be a static object, this should'n hurt.
      pIndexBuffer->SetCacheMethod( GSE_IndexBuffer::REQUEST_VBO_CACHE );
      pIndexBuffer->m_bBatchingEnabled = 1;
      GSE_IndexBufferNode *pIndexBufferNode =  pRenderGraph->CreateNode<GSE_IndexBufferNode>();      
      pIndexBufferNode->SetIndexBuffer(pIndexBuffer);
      /////////////////////////////////////////////////////////////////
      if ( (*it).first != "GSE_DEFAULT")
      {
	Lib3dsMaterial *pMat    = lib3ds_file_material_by_name(pFile,(*it).first.c_str() );
	GSE_Material   *pGseMat = ConvertMaterial(pMat);
      
	GSE_FrontMaterialNode *pFrontMat = pRenderGraph->CreateNode<GSE_FrontMaterialNode>();
	pFrontMat->SetMaterial(pGseMat);

	(*ppGeometryNode)->AddEdge( pFrontMat );
	pFrontMat->AddEdge( pIndexBufferNode);
      }
      else
      {
	(*ppGeometryNode)->AddEdge( pIndexBufferNode );
      }
    }
    /// Clear the index list
    (*it).second.clear();
  }
  /////////////////////////////////////////////////////////////////
  // Cleanup
  mapMaterialBuckets.clear();
  delete [] parrNormals;
  lib3ds_file_free( pFile);
  return 0;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_3dsTools.cpp,v $
// Revision 1.9  2007/04/06 11:55:07  entity
// removed unnecessary creation of empty indexbuffers
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
// Revision 1.5  2007/03/30 12:57:59  entity
// SetElements->SetVertexCount rename
//
// Revision 1.4  2007/03/29 13:15:42  entity
// added vertexbufferobject cache requests
//
// Revision 1.3  2007/03/28 11:31:45  entity
// fixed the 3ds axis problem
//
// Revision 1.2  2007/03/28 11:19:43  entity
// the name buffering off
//
// Revision 1.1  2007/03/27 10:11:20  entity
// Initial version
//
/////////////////////////////////////////////////////////////////
