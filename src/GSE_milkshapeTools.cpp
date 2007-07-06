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
#include "GSE_milkshapeTools.h"
#include "GSE_OglTexture.h"
#include "GSE_animation.h"
#include "GSE_GeometryData.h"
#include "GSE_SceneGraph.h"
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Core;
using std::map;
using std::string;
using std::vector;
/////////////////////////////////////////////////////////////////
/// Helps in the loading process of MS3D files.
class GSE_MS3DVertex : public GSE_Vertex
{
public:
  /// Which bone does this vertex belong to.
  int m_iBoneId;
  /// Default constructor.
  GSE_MS3DVertex()
  {
    m_iBoneId = -1;
  }
  /// Comparision operator.
  /// \param vert The vertex this is compared to.
  /// \return true if equal.
  bool operator==( GSE_MS3DVertex vert)
  {
    return (m_vPosition == vert.m_vPosition &&
            m_vNormal   == vert.m_vNormal   &&
	    m_vTexCoord == vert.m_vTexCoord &&
	    m_iBoneId   == vert.m_iBoneId );
  }
};
/////////////////////////////////////////////////////////////////
typedef std::list<GSE_MS3DVertex>		           MS3DVertexList;
typedef std::map<unsigned int, std::list< unsigned int > > MS3DBoneIndexMap;
typedef std::map<std::string, GSE_JointNode * >		   MS3DStringToJointMap;
typedef std::map<unsigned int, GSE_JointNode * >	   MS3DBoneToJointMap;


/////////////////////////////////////////////////////////////////
/// Creates a new FrontMaterialNode from material at iMaterialIndex. The
/// material is stored in GSE_MaterialMgr.
/// \param rModel The model where material is
/// \param iMaterialIndex The index of the material in material array.
/// \returns GSE_FrontMaterialNode The node which has the material applied.
GSE_FrontMaterialNode *
CreateMaterialNode( GSE_MS3DModel &rModel, int iMaterialIndex, GSE_RenderGraph *pRenderGraph )
{

  GSE_MaterialMgr *pMaterialMgr = GSE_MaterialMgr::GetInstance();
  GSE_Material *pMaterial = NULL;
  GSE_FrontMaterialNode *pFrontMaterial = NULL;

  
  // check that the index is valid
  if ( iMaterialIndex > -1 && iMaterialIndex < rModel.m_nNumMaterials)
  {
    // This helps the readablity a bit:
    MS3D_Material_t *pMS3DMaterial = &(rModel.m_pMaterials[iMaterialIndex]);    
    pMaterial = pMaterialMgr->Find(std::string(pMS3DMaterial->name));
    
    if (pMaterial == NULL)
    {
      
      // Create new material object
      pMaterial = new GSE_Material();
      // Set values 
      pMaterial->SetAmbient(GSE_Color(pMS3DMaterial->ambient));
      pMaterial->SetDiffuse(GSE_Color(pMS3DMaterial->diffuse));
      pMaterial->SetSpecular(GSE_Color(pMS3DMaterial->specular));
      pMaterial->SetEmission(GSE_Color(pMS3DMaterial->emission));
      pMaterial->SetShininess(pMS3DMaterial->shininess / 128.0f);
      pMaterial->SetName( pMS3DMaterial->name );

      // Add material to manager
      pMaterialMgr->Add(pMaterial);
    } 
    else 
    {
      GSE_DEBUG("Using existing material" << pMaterial->GetName());
    }
    // Create new node
    pFrontMaterial = pRenderGraph->CreateNode<GSE_FrontMaterialNode>();
    // Assign material to it
    pFrontMaterial->SetMaterial(pMaterial);
    pFrontMaterial->SetName( pMS3DMaterial->name );

  } 
  else 
  {

    if ( iMaterialIndex == -1 ) 
    {
      GSE_WARN( "Material index out of bounds -  " 
	       << "Resorting to default material NOW!");
    } 
    else 
    {
      GSE_WARN( "No material assigned, using default.");
    }
    
    // If material is not assigned or it is bogus, 
    pFrontMaterial = pRenderGraph->CreateNode<GSE_FrontMaterialNode>();
    pFrontMaterial->SetMaterial( pMaterialMgr->At(0) );
    
  }
  
  return pFrontMaterial;
  
}
/////////////////////////////////////////////////////////////////
/// Creates a new GSE_TextureNode.
/// \param rModel The model where material is
/// \param iMaterialIndex The index of the material in material array where texture is retrieved.
/// \param iQuality The texture quality flag.
/// \returns GSE_FrontMaterialNode The node which has the material applied.
GSE_TextureNode *
CreateTextureNode( GSE_MS3DModel &rModel, int iMaterialIndex, 
		   GSE_RenderGraph *pRenderGraph, TextureQuality::Type iQuality,
		   const string &strPath)
{
  
  GSE_TextureNode *pTextureNode = NULL;
  GSE_Texture *pTexture         = NULL;
  
  // check that the index is valid
  if ( iMaterialIndex > -1 && iMaterialIndex < rModel.m_nNumMaterials)
  {
    MS3D_Material_t *pMS3DMaterial = &(rModel.m_pMaterials[iMaterialIndex]);    

    pTexture = GSE_OglTextureMgr::GetInstance()->CreateTextureFromImage(
	(strPath+string("/")+string(pMS3DMaterial->texture)).c_str(), 0 ,
	TextureType::GSE_TEX2D, iQuality);

    pTextureNode = pRenderGraph->CreateNode<GSE_TextureNode>();
    pTextureNode->SetName( pMS3DMaterial->texture );
    pTextureNode->SetTexture( pTexture );

  } 
  return pTextureNode;
  
}
/////////////////////////////////////////////////////////////////
/// Macro for parsing vertex information from MS3D model data
#define CREATE_VERTEX( GSE_MS3DVERTEX, MODEL, TRIANGLE, T_VERTEXINDEX ) {\
 \
 GSE_MS3DVERTEX.m_vPosition.Set(MODEL.m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].vertex);\
 GSE_MS3DVERTEX.m_vNormal.Set(TRIANGLE.vertexNormals[T_VERTEXINDEX]);\
 GSE_MS3DVERTEX.m_vTexCoord[GSE_Vector2::X]=TRIANGLE.s[T_VERTEXINDEX];\
 GSE_MS3DVERTEX.m_vTexCoord[GSE_Vector2::Y]=TRIANGLE.t[T_VERTEXINDEX];\
 GSE_MS3DVERTEX.m_iBoneId = MODEL.m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].boneId;\
 \
}
/////////////////////////////////////////////////////////////////
/// Creates new vertex for every occasion where position is same
/// but normal and/or texture coordinate is different.
void 
SeparateVertices( GSE_MS3DModel &rModel, MS3DVertexList &lstVertices )
{

  for(unsigned int nT=0; nT<rModel.m_nNumTriangles; nT++)
  {
    struct MS3D_Triangle_t triangle;
    triangle = rModel.m_pTriangles[nT];
    
    for( unsigned int nV=0;nV<3;nV++)
    {
      // Create vertex
      GSE_MS3DVertex vertex;
      CREATE_VERTEX( vertex, rModel, triangle, nV );

      // Check does the vertex exist in the list already
      MS3DVertexList::iterator it = lstVertices.begin();
      bool bFoundVertex = false;
      for( ; it != lstVertices.end(); it++ )
      {
	if ( *it == vertex ) 
	{
	  bFoundVertex = true;
	  break;
	} 
      }
      
      if ( !bFoundVertex )
      {
	//cerr << "okay, found a vertex which does not exist yet" << endl;
	
	lstVertices.push_back( vertex );
	GSE_DEBUG( "pushing vertex " << vertex.m_vPosition);
      }
      else 
      {
	//cerr << "same vertex used" << endl;
      }
    }
  }
} 
/////////////////////////////////////////////////////////////////
/// \brief Goes through every triangle and their vertices, creating an elementlist for 
/// for the faces to be drawn.
/// \param rModel The Milkshape model data
/// \param g The group index
/// \oaram lstVertices the list where all vertices in the group are pushed.
GSE_IndexBuffer *
ParseGroupTriangles( GSE_MS3DModel &rModel, unsigned int g, MS3DVertexList &lstVertices )
{

  GSE_IndexBufferMgr *pIndexBufferMgr = GSE_IndexBufferMgr::GetInstance();
  GSE_IndexBuffer *pIndexBuffer = pIndexBufferMgr->CreateIndexBuffer();

  pIndexBuffer->Init(rModel.m_pGroups[g].nNumTriangles*3);

  
  unsigned int nIndexBufferIndex = 0;
  
  unsigned int nVertexIndex = 0; // The vertex index in GeometryData.m_pVertices

  // for each triangle in the group
  for(unsigned int t=0;t<rModel.m_pGroups[g].nNumTriangles;t++)
  {
    // get the triangle index
    unsigned int iTriangleInd = rModel.m_pGroups[g].pTriangleIndices[t];
      
    // sanity check for triangle index
    if (iTriangleInd >= 0 && iTriangleInd < rModel.m_nNumTriangles) 
    {
	
      struct MS3D_Triangle_t triangle;
      triangle = rModel.m_pTriangles[iTriangleInd];

      // For each triangle has three vertices
      for( unsigned int nVertex=0; nVertex<3; nVertex++)
      {
	GSE_MS3DVertex vertex;
	CREATE_VERTEX( vertex, rModel, triangle, nVertex );
	  
	MS3DVertexList::iterator it = lstVertices.begin();
	bool bFoundVertex = false;

	for( nVertexIndex = 0 ; it != lstVertices.end(); it++,nVertexIndex++ )
	{
	  if ( *it == vertex ) 
	  {
	    bFoundVertex = true;
	    break;
	  } 
	}
      
	if ( bFoundVertex )
	{
	  // Copy vertex indices to element list 
	  pIndexBuffer->m_pIndices[nIndexBufferIndex++] = nVertexIndex;
	} 
	else
	{
	  GSE_ERR("UH_OH: Vertex missing?");
	}
      }
    } 
    else 
    {
	  
      GSE_ERR( "Triangle index faulty; " 
	       << "This madness ends here." );
    }
  }
  
  return pIndexBuffer;
  

}
/////////////////////////////////////////////////////////////////
/// Parses the Joints and constructs the initial graph.
/// \param sRootName The name of the root joint.
/// \returns A pointer to the RootNode of the animation.
GSE_JointNode *
CreateJoints( GSE_MS3DModel &rModel, 
	      GSE_AnimationGraph *pAnimGraph,
	      MS3DStringToJointMap & mapJoints,
	      MS3DBoneToJointMap & mapIndexJoints,
	      std::string sRootName )
{

  GSE_JointNode *pRootJointNode = NULL;
  GSE_JointMgr *pJointMgr = GSE_JointMgr::GetInstance();

  GSE_LOG( "Loading Joint keyframe data...");
  
  // Load keyframedata from each joint
  for(unsigned int j=0;j<rModel.m_nNumJoints;j++)
  {
      
    // Get the number of keyframes
    unsigned int nRotFrames   = rModel.m_pJoints[j].nNumKeyFramesRot;
    unsigned int nTransFrames = rModel.m_pJoints[j].nNumKeyFramesTrans;
      
    // Create new joint
    GSE_Joint *pJoint = new GSE_Joint(nTransFrames, nRotFrames);
    GSE_JointNode *pJointNode = pAnimGraph->CreateNode<GSE_JointNode>();

    pJointNode->SetJoint( pJoint );
    pJointNode->SetName( rModel.m_pJoints[j].name);    
    
    pJoint->m_vLocalTranslation.Set( rModel.m_pJoints[j].position );
    pJoint->m_vLocalRotation.Set(    rModel.m_pJoints[j].rotation );
    
    // Add to jointmgr and get index
    int iJointIndex = pJointMgr->Add(pJoint);
	
    // Convert translation keyframes
    for(unsigned int kf=0;kf<nTransFrames;kf++)
    {
      pJoint->m_pTranslationKeyframes[kf].m_iJointIndex = iJointIndex;
      // Get time in milliseconds
      pJoint->m_pTranslationKeyframes[kf].m_iTime = (int)(rModel.m_pJoints[j].keyFramesTrans[kf].time * 1000);
      pJoint->m_pTranslationKeyframes[kf].m_vValues.Set(rModel.m_pJoints[j].keyFramesTrans[kf].position);

    }
    
    // Convert rotation keyframes
    for(unsigned int kf=0;kf<nRotFrames;kf++)
    {
      pJoint->m_pRotationKeyframes[kf].m_iJointIndex = iJointIndex;
      
      // Get time in milliseconds
      pJoint->m_pRotationKeyframes[kf].m_iTime = (int)(rModel.m_pJoints[j].keyFramesRot[kf].time * 1000);
      pJoint->m_pRotationKeyframes[kf].m_vValues.Set(rModel.m_pJoints[j].keyFramesRot[kf].rotation);
      
    }
    
    // Create mapping to for relation construction later on
    std::string sName = rModel.m_pJoints[j].name;
    
    mapJoints[sName]  = pJointNode;
    mapIndexJoints[j] = pJointNode;

  }
  
  // Create links between parents and children.
  for(unsigned int j=0;j<rModel.m_nNumJoints;j++)
  {
      
    std::string sParentName = rModel.m_pJoints[j].parentName;
    std::string sName       = rModel.m_pJoints[j].name;
    
    GSE_LOG("node '" << sName << "' with parent '" 
	    << sParentName << "'");
    
    if ( sName == sRootName)
    {
      GSE_LOG("node " << sName << " set as root");
      pRootJointNode = mapJoints[sName];
      mapJoints[sName]->GetJoint()->m_bRoot = 1;
    } 
    else if ( sParentName.size() > 0)// Add child node for parent
    {
      
      mapJoints[sParentName]->AddEdge( mapJoints[sName] );
    } 
    
  }
  
  return pRootJointNode;
}
/////////////////////////////////////////////////////////////////
/// Parses the vertices and generates a mapping from boneID to 
/// indices of the vertices that have been assigned to the given bone.
/// \param lstVertices A list of GSE_MS3DVertices
/// \param mapBoneElements mapping from unsigned int to list of unsigned ints.
void
ParseVertexIndicesForBones( MS3DVertexList &lstVertices,  
			    MS3DBoneIndexMap & mapBoneElements)
{
  MS3DVertexList::iterator it = lstVertices.begin();
  for(unsigned int i=0; i<lstVertices.size(); i++, it++)
  {
    // if the bone index is negative, there's no bone
    if ( (*it).m_iBoneId >= 0 )
    {
      mapBoneElements[(unsigned int)(*it).m_iBoneId].push_back(i);  
    }
  }
}
/////////////////////////////////////////////////////////////////
/// Assigns vertices to the to the joints by attaching an
/// elementlist to the GSE_JointNode.
/// \param mapBoneElements map of unsigned ints to list of unsigned ints
/// \param mapIndexJoints map of unsigned ints to GSE_JointNodes.
/// \param pGeometryData The geometry
/// \param pGeometryDataWorld the World geometry which will be updated by animation.
void 
AssignVerticesToBones( MS3DBoneIndexMap & mapBoneElements,
		       MS3DBoneToJointMap & mapIndexJoints,
		       GSE_GeometryData *pGeometryData, 
		       GSE_GeometryData *pGeometryDataWorld)
{

  MS3DBoneIndexMap::iterator itBoneVertices;
  itBoneVertices = mapBoneElements.begin();

  for( ;itBoneVertices != mapBoneElements.end(); itBoneVertices++)
  {
    unsigned int iBoneId = itBoneVertices->first;
    std::list<unsigned int> lstElements = itBoneVertices->second;

    // Create elementlist
    GSE_IndexBuffer *pIndexBuffer = GSE_IndexBufferMgr::GetInstance()->CreateIndexBuffer();
    pIndexBuffer->Init(lstElements.size());

    // for each index in the list
    std::list<unsigned int>::iterator it = lstElements.begin();
    unsigned int iCounter = 0;
    for(;it!=lstElements.end();it++)
    {
      // add index to elementlist
      pIndexBuffer->m_pIndices[iCounter] = *it;
      iCounter++;
    }
    // bind elementlist to the jointnode
    mapIndexJoints[iBoneId]->SetElements(pIndexBuffer);
      
    // assign vertex data to the jointnode
    mapIndexJoints[iBoneId]->SetGeometryDataOrig(pGeometryData);
    mapIndexJoints[iBoneId]->SetGeometryDataWorld(pGeometryDataWorld);
  }
}
/////////////////////////////////////////////////////////////////
/// The implementation of Convert of MS3DModel. 
int
GSE_MS3DObjectConverter::Convert( GSE_MS3DModel &rModel, 
				  GSE_RenderGraph *pRenderGraph,
				  GSE_AnimationGraph *pAnimGraph,
				  GSE_GeometryNode **ppGeometryNode,
				  GSE_JointNode **ppJointNode,
				  std::string sRootName,
				  TextureQuality::Type iQuality)
{
  GSE_LOG("Conversion begins");

  // mapping for speeding up the selection process to O(log #Vertices)
  std::map<GSE_MS3DVertex, unsigned int>	       mapVertexToIndex;
  // list of all separate vertices (different normals, texcoords)
  std::list<GSE_MS3DVertex>		lstVertices;
  
  MS3DStringToJointMap	mapJoints;
  MS3DBoneToJointMap	mapIndexJoints;
  MS3DBoneIndexMap      mapBoneElements;
  GSE_GeometryDataMgr *pGeometryDataMgr = GSE_GeometryDataMgr::GetInstance();

  if ( pRenderGraph == NULL ) 
  {
    GSE_ERR("RenderGraph == NULL");
    *ppGeometryNode = NULL;
    *ppJointNode = NULL;
    return -1;
  }
  
  // Check which vertices must be separate
  SeparateVertices( rModel, lstVertices);
  
  GSE_GeometryData *pGeometryData = new GSE_GeometryData();
  pGeometryDataMgr->Add(pGeometryData);
  pGeometryData->SetVertexCount( lstVertices.size() );
  GSE_LOG("Got " << lstVertices.size() << " separate vertices.");

  pGeometryData->InitVertices();
  pGeometryData->InitNormals();
  pGeometryData->InitTexCoords(0);

  // Create geometrynode for holding the objstruct
  GSE_GeometryNode *pGeomNode = pRenderGraph->CreateNode<GSE_GeometryNode>();
  
  pGeomNode->SetGeometryData(pGeometryData);

  std::list<GSE_MS3DVertex>::iterator it = lstVertices.begin();
  for( unsigned int nVertexCounter = 0; nVertexCounter < lstVertices.size(); 
       it++, nVertexCounter++)
  {
    pGeometryData->SetVertex(nVertexCounter, (*it).m_vPosition);
    pGeometryData->SetNormal(nVertexCounter, (*it).m_vNormal);
    pGeometryData->SetTexCoords(0, nVertexCounter, (*it).m_vTexCoord);
  }
  /////////////////////////////////////////////////////////////////
  // For every group in the model
  for(unsigned int g=0;g<rModel.m_nNumGroups;g++)
  {
    
    GSE_FrontMaterialNode *pMatNode = NULL;
    GSE_TextureNode *pTexNode = NULL;
    GSE_IndexBuffer *pIndexBuffer = NULL;
    GSE_IndexBufferNode *pIndexBufferNode = NULL;
    
    int iMaterialIndex = rModel.m_pGroups[g].materialIndex;
    
    pMatNode  = CreateMaterialNode( rModel, iMaterialIndex, pRenderGraph);
    pTexNode  = CreateTextureNode( rModel, iMaterialIndex, pRenderGraph, iQuality, GetTexturePath() );
    pIndexBuffer = ParseGroupTriangles( rModel, g, lstVertices);
    
    pIndexBufferNode = pRenderGraph->CreateNode<GSE_IndexBufferNode>();
    pIndexBufferNode->SetIndexBuffer(pIndexBuffer);
    
    // Check does the texture exist
    if ( pTexNode != NULL )
    {
      pGeomNode->AddEdge( pTexNode );
      pTexNode->AddEdge(pMatNode);
      pMatNode->AddEdge(pIndexBufferNode);
    } 
    else
    {
      pGeomNode->AddEdge( pMatNode );
      pMatNode->AddEdge(pIndexBufferNode);
    }
    
  }
  *ppGeometryNode = pGeomNode;
  if ( pAnimGraph != NULL )
  {

    
    GSE_JointNode *pJointNode = CreateJoints( rModel, pAnimGraph, 
					      mapJoints, mapIndexJoints, 
					      sRootName);
    
    ParseVertexIndicesForBones( lstVertices, mapBoneElements );
    GSE_GeometryData *pGeometryDataWorld = new GSE_GeometryData(*pGeometryData);
    pGeometryDataMgr->Add(pGeometryDataWorld);

    AssignVerticesToBones( mapBoneElements, mapIndexJoints, pGeometryData, pGeometryDataWorld);
    pGeomNode->SetGeometryDataWorld(pGeometryDataWorld);    
    if ( ppJointNode != NULL ) *ppJointNode = pJointNode;
  } 
  else
  {
    // No animation, world struct points to original and possibly wild pointer 
    // will be NULLified.
    pGeomNode->SetGeometryDataWorld(pGeometryData);    
    if ( ppJointNode != NULL ) *ppJointNode = NULL;
  }

  lstVertices.clear();
  return 0;
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_MS3DObjectConverter::CreateSceneLimbsFromJoints( GSE_SceneNode *pSceneNode,
						     GSE_JointNode *pJointRoot )
{
  queue< GSE_JointNode * > queueJointNodes;
  //queue< GSE_SceneNode *>  queueSceneNodes;
  list< GSE_GraphEdge<Animation::NodeType> *>::iterator it;  
  /////////////////////////////////////////////////////////////////
  //GSE_SceneNode *pSNode  = NULL;
  GSE_SceneLimbNode *pSChild = NULL;
  GSE_JointNode *pJNode  = NULL;
  GSE_JointNode *pJChild = NULL;
  /////////////////////////////////////////////////////////////////
  queueJointNodes.push( pJointRoot );
  //queueSceneNodes.push( pSceneNode );
  /////////////////////////////////////////////////////////////////
  while ( ! queueJointNodes.empty())
  {
    pJNode = queueJointNodes.front(); queueJointNodes.pop();
    //pSNode = queueSceneNodes.front(); queueSceneNodes.pop();
    if ( pJNode->HasLeavingEdges())
    {
      it = pJNode->GetLeavingEdges().begin();
      // For each joint child create a corresponding scene limb child
      for( ; it != pJNode->GetLeavingEdges().end();it++)
      {

	pJChild = (GSE_JointNode *)(*it)->GetToNode();
	// Create new node
	pSChild = ((GSE_SceneGraph *)pSceneNode->GetGraph())->CreateNode<GSE_SceneLimbNode>();
	// Transform follows the animation in relative to the parent joint
	pSChild->AddTransformHook( &(pJChild->GetJoint()->m_mFinal));
	pSChild->SetName( pJChild->GetName() );

	// Add edge from object root to child in scenegraph
	pSceneNode->AddEdge( pSChild );

	// Calculate the bounding volume for this scene limb, if it has vertices assigned
	if ( pJChild->GetGeometryDataOrig() != NULL )
	{
	  GSE_Sphere boundingSphere = 
	  Geometry::CalculateBoundingSphereTight(pJChild->GetGeometryDataOrig()->m_pVertices,
						 *pJChild->GetElements());
	  
	  GSE_OrientedBox boundingBox =
	  Geometry::CalculateOrientedBoundingBox( pJChild->GetGeometryDataOrig()->m_pVertices,
						  *pJChild->GetElements());

	  pSChild->SetLocalBoundingSphere( boundingSphere );
	  pSChild->SetLocalBoundingBox( boundingBox );
	  //std::cerr << "Child: bounding sphere " << boundingSphere << endl;
	  //std::cerr << "Child: bounding box " << boundingBox << endl;
	}
	// The child joint might have children also, check it 
	queueJointNodes.push( pJChild );
	//queueSceneNodes.push( pSChild );
      }
    }
  }
}
/////////////////////////////////////////////////////////////////
GSE_Sphere 
GSE_MS3DObjectConverter::GetBoundingSphereForJoint ( const GSE_MS3DModel &rModel, 
						     const std::string & sJoint )
{
  float *pVertices = NULL;
  int nJoint = 0;
  std::list< GSE_Vector3 > lstVertices;
  // Seek the joint id
  for( ; nJoint < rModel.m_nNumJoints; nJoint++)
  {
    if ( strncmp( rModel.m_pJoints[nJoint].name, sJoint.c_str(), 32 ) == 0 ) break;
  }
  // sanity check
  if ( nJoint >= rModel.m_nNumJoints )
  {
    GSE_WARN( "The model has no such joint '" << sJoint << "'");
    return GSE_Sphere();
  }
  /////////////////////////////////////////////////////////////////(
  // Seek the vertices bound to this joint
  for ( unsigned int nVertex = 0; nVertex<rModel.m_nNumVertices; nVertex++)
  {
    if ( rModel.m_pVertices[nVertex].boneId == nJoint )
    {
      lstVertices.push_back( GSE_Vector3(rModel.m_pVertices[nVertex].vertex));
    }
  }
  pVertices = new float[lstVertices.size()*3];
  // copy vertex data into the array
  std::list< GSE_Vector3 >::iterator it = lstVertices.begin();

  for (unsigned int count = 0 ; it != lstVertices.end(); it++,count++)
  {
    pVertices[count*3]     = (*it).m_pValues[0];
    pVertices[(count*3)+1] = (*it).m_pValues[1];
    pVertices[(count*3)+2] = (*it).m_pValues[2];
  }
  
  GSE_Sphere sphere = Geometry::CalculateBoundingSphereTight( pVertices, 
							      lstVertices.size());
  // delete array
  delete pVertices;
  lstVertices.clear();
  return sphere;
}
/////////////////////////////////////////////////////////////////
GSE_OrientedBox
GSE_MS3DObjectConverter::GetOBBForJoint ( const GSE_MS3DModel &rModel, 
					  const std::string & sJoint )
{
  float *pVertices = NULL;
  int nJoint = 0;
  std::list< GSE_Vector3 > lstVertices;
  // Seek the joint id
  for( ; nJoint < rModel.m_nNumJoints; nJoint++)
  {
    if ( strncmp( rModel.m_pJoints[nJoint].name, sJoint.c_str(), 32 ) == 0 ) break;
  }
  // sanity check
  if ( nJoint >= rModel.m_nNumJoints )
  {
    GSE_WARN( "The model has no such joint '" << sJoint << "'");
    return GSE_OrientedBox();
  }
  // Seek the vertices bound to this joint
  for ( unsigned int nVertex = 0; nVertex<rModel.m_nNumVertices; nVertex++)
  {
    if ( rModel.m_pVertices[nVertex].boneId == nJoint )
    {
      lstVertices.push_back( GSE_Vector3(rModel.m_pVertices[nVertex].vertex));
    }
  }
  pVertices = new float[lstVertices.size()*3];
  // copy vertex data into the array
  std::list< GSE_Vector3 >::iterator it = lstVertices.begin();

  for (unsigned int count = 0 ; it != lstVertices.end(); it++,count++)
  {
    pVertices[count*3]     = (*it).m_pValues[0];
    pVertices[(count*3)+1] = (*it).m_pValues[1];
    pVertices[(count*3)+2] = (*it).m_pValues[2];
  }
  
  GSE_OrientedBox box = Geometry::CalculateOrientedBoundingBox( pVertices, 
								lstVertices.size());
  // delete array
  lstVertices.clear();
  delete pVertices;
  return box;
}
/////////////////////////////////////////////////////////////////
GSE_OrientedBox 
GSE_MS3DObjectConverter::GetOBBForUnanimated ( const GSE_MS3DModel &rModel)
{
  float *pVertices = NULL;
  std::list< GSE_Vector3 > lstVertices;
  GSE_OrientedBox box;
  // Seek the vertices without bone
  for ( unsigned int nVertex = 0; nVertex<rModel.m_nNumVertices; nVertex++)
  {
    if ( rModel.m_pVertices[nVertex].boneId <= -1 )
    {
      lstVertices.push_back( GSE_Vector3(rModel.m_pVertices[nVertex].vertex));
    }
  }
  if ( lstVertices.size() == 0 )
  {
    lstVertices.clear();
    return box;
  }
  pVertices = new float[lstVertices.size()*3];
  // copy vertex data into the array
  std::list< GSE_Vector3 >::iterator it = lstVertices.begin();

  for (unsigned int count = 0 ; it != lstVertices.end(); it++,count++)
  {
    pVertices[count*3]     = (*it).m_pValues[0];
    pVertices[(count*3)+1] = (*it).m_pValues[1];
    pVertices[(count*3)+2] = (*it).m_pValues[2];
  }
  
  box = Geometry::CalculateOrientedBoundingBox( pVertices, 
						lstVertices.size());
  // delete array
  delete pVertices;
  lstVertices.clear();
  return box;
}
/////////////////////////////////////////////////////////////////
GSE_Sphere 
GSE_MS3DObjectConverter::GetBoundingSphereForUnanimated ( const GSE_MS3DModel &rModel)
{
  float *pVertices = NULL;
  std::list< GSE_Vector3 > lstVertices;
  GSE_Sphere sphere;
  // Seek the vertices without bone
  for ( unsigned int nVertex = 0; nVertex<rModel.m_nNumVertices; nVertex++)
  {
    if ( rModel.m_pVertices[nVertex].boneId <= -1 )
    {
      lstVertices.push_back( GSE_Vector3(rModel.m_pVertices[nVertex].vertex));
    }
  }
  if( lstVertices.size() == 0 )
  {
    lstVertices.clear();
    return sphere;
  }
  pVertices = new float[lstVertices.size()*3];
  // copy vertex data into the array
  std::list< GSE_Vector3 >::iterator it = lstVertices.begin();

  for (unsigned int count = 0 ; it != lstVertices.end(); it++,count++)
  {
    pVertices[count*3]     = (*it).m_pValues[0];
    pVertices[(count*3)+1] = (*it).m_pValues[1];
    pVertices[(count*3)+2] = (*it).m_pValues[2];
  }
  
  sphere = Geometry::CalculateBoundingSphereTight( pVertices, 
						   lstVertices.size());
  // delete array
  delete pVertices;
  lstVertices.clear();
  return sphere;
}
/////////////////////////////////////////////////////////////////
void 
GSE_MS3DObjectConverter::SetTexturePath( const string &strPath )
{
  m_strTexturePath = strPath;
}
/////////////////////////////////////////////////////////////////
const string &
GSE_MS3DObjectConverter::GetTexturePath() const
{
  return m_strTexturePath;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_milkshapeTools.cpp,v $
// Revision 1.16  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.15  2007/05/21 18:33:36  entity
// texture path added
//
// Revision 1.14  2007/04/03 12:51:43  entity
// texture quality added
//
// Revision 1.13  2007/04/01 21:04:23  entity
// CreateTextureNode added
//
// Revision 1.12  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.11  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.10  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.9  2007/03/30 12:56:32  entity
// SetElements->SetVertexCount rename
//
// Revision 1.8  2007/03/28 08:24:57  entity
// IndexBuffer from Mgr Object
//
// Revision 1.7  2007/03/25 15:35:29  entity
// Limb stuff: inserted as a child of given object node with object world final matrix
//
// Revision 1.6  2007/03/23 16:08:53  entity
// removed bughunt output
//
// Revision 1.5  2007/03/23 13:30:26  entity
// fixed ugly segfault on GetBoundingSphereFor...()
//
// Revision 1.4  2007/03/23 12:49:37  entity
// added GetBoundingSphereForJoint,GetOBBForJoint,
// GetBoundingSphereForUnanimated,GetOBBForUnanimated
//
// Revision 1.3  2007/03/23 11:52:30  entity
// unneeded function param removed
//
// Revision 1.2  2007/03/23 11:51:20  entity
// CreateSceneLimbsFromJoints() added
//
// Revision 1.1  2007/03/23 07:31:51  entity
// separated from GSE_sgutils.cpp
//
/////////////////////////////////////////////////////////////////
