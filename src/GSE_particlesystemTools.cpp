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
#include "GSE_particlesystemTools.h"
#include "GSE_GeometryData.h"
#include "GSE_OglTexture.h"
#include "GSE_IndexBuffer.h"
#include "GSE_Material.h"
/////////////////////////////////////////////////////////////////
/// Creates animation and render graph nodes for particlesystem.
void
GSE_ParticleSystemConverter::Convert( GSE_ParticleSystemBase *pParticleSystem,
				      GSE_Texture *pTexture,
				      GSE_TextureNode **ppTextureNode,
				      GSE_ParticleSystemNode **ppParticleSystemNode,
				      GSE_AnimParticleSystemNode **ppAnimNode,
				      GSE_AnimationGraph *pAnimGraph,
				      GSE_RenderGraph *pRenderGraph)
{

  // Check are passed parameters ok
  char bSanityFailed = 0;
  // Number of vertices in each particle
  unsigned int nNumVertices = 3;
  
  if ( pParticleSystem  == NULL ) 
  {
    GSE_ERR("I can't create anything from NULL particlesystem!" );
    bSanityFailed = 1;
  } 
  if ( pTexture == NULL )
  {
    GSE_ERR("I can't create anything from NULL texture!" );
    bSanityFailed = 1;
  }

  // The point of no return
  if ( bSanityFailed ) return;
  

  GSE_IndexBufferMgr *pIndexBufferMgr = GSE_IndexBufferMgr::GetInstance();
  /////////////////////////////////////////////////////////////////
  // Create element list for particlesystem; the system must be initialized with
  // UPDATE pass by animrenderer.
  GSE_IndexBuffer *pIndexBuffer = pIndexBufferMgr->CreateIndexBuffer();
  pIndexBuffer->Init(pParticleSystem->GetMaxParticles()*nNumVertices);
  pIndexBuffer->m_nVerticesInPrimitive = nNumVertices;
  /////////////////////////////////////////////////////////////////
  // Since updating a particlesystem sorts the particles, indices will always be 
  // ordered from 0 to pIndexBuffer->m_nNumIndices, so we assign the values here.
  unsigned short *pIndices = pIndexBuffer->m_pIndices;
  unsigned int nP = 0;
  while(nP<pIndexBuffer->m_nNumIndices) *(pIndices++)=nP++;

  // Batching wreaks havoc on particlesystem implementation at the moment,
  // drop it.
  pIndexBuffer->m_bBatchingEnabled = false;
  // Create Animation node
  GSE_AnimParticleSystemNode *pAnimNode = pAnimGraph->CreateNode<GSE_AnimParticleSystemNode>();
  pAnimNode->SetParticleSystem(pParticleSystem);
  pAnimNode->SetElements( pIndexBuffer );
  
  // Create indexbuffer node for geometry
  GSE_IndexBufferNode *pIndexBufferNode = pRenderGraph->CreateNode<GSE_IndexBufferNode>();
  pIndexBufferNode->SetIndexBuffer( pIndexBuffer );
  
  // Create particlesystem node
  GSE_ParticleSystemNode *pPSNode = pRenderGraph->CreateNode<GSE_ParticleSystemNode>();
  //pRenderGraph->AddNode(pPSNode);
  pPSNode->SetParticleSystem( pParticleSystem );
  pPSNode->AddEdge( pIndexBufferNode );
  
  // Create texture Node
  GSE_TextureNode *pTextureNode = pRenderGraph->CreateNode<GSE_TextureNode>();
  pTextureNode->SetTexture(pTexture);
  pTextureNode->AddEdge( pPSNode );
  
  // Create geometry
  GSE_GeometryData *pGeometryData = GSE_GeometryDataMgr::GetInstance()->CreateGeometryData();
  
  // This baby has got vertex, texture coordinate and color buffers
  pGeometryData->SetVertexCount( pParticleSystem->GetMaxParticles()*nNumVertices);
  pGeometryData->InitVertices();
  pGeometryData->InitTexCoords(0);      // Only one texture is allowed with particles!
  pGeometryData->InitNormals();
  pGeometryData->InitColors();
  // Set initial values for buffers, vertex values and normals are 
  // calculated in renderer for each frame since those depend on camera 
  // orientation.
  const GSE_Particle *particle = NULL;
  unsigned int nIndex = 0;

  for(unsigned int nP=0;nP<pParticleSystem->GetMaxParticles();nP++)
  {
    particle = &(pParticleSystem->GetParticles()[nP]);
    // Calculate the index of the first vertex for current particle
    nIndex = nP * nNumVertices;     
    // Assign basecolor
    pGeometryData->SetColor(nIndex,  particle->m_vColor);
    pGeometryData->SetColor(nIndex+1,particle->m_vColor);
    pGeometryData->SetColor(nIndex+2,particle->m_vColor);
    // Assign texture coordinates
    pGeometryData->SetTexCoords(0, nIndex,   GSE_Vector2(0,0));
    pGeometryData->SetTexCoords(0, nIndex+1, GSE_Vector2(0,1));
    pGeometryData->SetTexCoords(0, nIndex+2, GSE_Vector2(1,1));
  }
  // Assign objstruct
  pPSNode->SetGeometryData(pGeometryData);  
  *ppTextureNode = pTextureNode;
  *ppAnimNode = pAnimNode;
  *ppParticleSystemNode = pPSNode;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_particlesystemTools.cpp,v $
// Revision 1.9  2007/04/04 20:58:37  entity
// retrieval of psnode
//
// Revision 1.8  2007/04/03 20:26:07  entity
// normal init added back
//
// Revision 1.7  2007/04/03 12:51:16  entity
// renamed Helper->Converter and added texture quality
//
// Revision 1.6  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.5  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.4  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.3  2007/03/30 12:56:32  entity
// SetElements->SetVertexCount rename
//
// Revision 1.2  2007/03/28 08:31:00  entity
// pIndexBufferMgr->CreateIndexBuffer
//
// Revision 1.1  2007/03/23 07:38:03  entity
//  separated from GSE_sgutils.cpp
//
/////////////////////////////////////////////////////////////////
