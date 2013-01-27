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

/////////////////////////////////////////////////////////////////
#include <MooseMilkshapeLoader.h>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <cassert>
#include "MooseLogger.h"
/////////////////////////////////////////////////////////////////
#if TRISTRIPPER == 1
  #include <tri_stripper.h>
#endif
/////////////////////////////////////////////////////////////////
using std::fstream;
using std::ios;
using namespace Moose::Data;
using namespace Moose::Graphics;
using namespace Moose::Spatial;
using std::cerr;
using std::endl;
using std::vector;
using std::map;
using std::list;
/////////////////////////////////////////////////////////////////
Moose::Data::CMilkshapeLoader::CMilkshapeLoader()
{
  Init();
}
/////////////////////////////////////////////////////////////////
Moose::Data::CMilkshapeLoader::~CMilkshapeLoader()
{
  Destroy();
}
/////////////////////////////////////////////////////////////////
void 
Moose::Data::CMilkshapeLoader::Init()
{
  
  m_nNumVertices = 0;
  m_nNumTriangles =0 ;
  m_nNumGroups = 0;
  m_nNumMaterials = 0;
  m_nNumJoints = 0;
  
  m_pVertices = NULL;
  m_pTriangles = NULL;
  m_pGroups = NULL;
  m_pMaterials = NULL;
  
  m_Animationdata.fAnimationFPS = 0.0;
  m_Animationdata.fCurrentTime = 0.0;
  m_Animationdata.iTotalFrames = 0;

  m_pJoints = NULL;
  m_pVertexWeights = NULL;


  m_pPositions = NULL;
  m_pNormals = NULL;
  m_pColors = NULL;
  m_pTexCoords = NULL;
  m_pIndices = NULL;
}
/////////////////////////////////////////////////////////////////
// Frees the memory reserved by the model 
/////////////////////////////////////////////////////////////////
void 
Moose::Data::CMilkshapeLoader::Destroy()
{
  
  unsigned int i = 0;
  delete [] m_pVertices;
  delete [] m_pTriangles;
  delete [] m_pMaterials;
  // Groups have pointer members
  for( i=0;i<m_nNumGroups;i++ )
  {
    delete [] m_pGroups[i].pTriangleIndices;
  }
  delete [] m_pGroups;
  // Joints have pointer members
  for( i=0;i<m_nNumJoints;i++ )
  {
    delete [] m_pJoints[i].keyFramesRot;
    delete [] m_pJoints[i].keyFramesTrans;    
  }
  
  delete [] m_pJoints;
  delete [] m_pVertexWeights;  

  m_Animationdata.fAnimationFPS = 0.0;
  m_Animationdata.fCurrentTime = 0.0;
  m_Animationdata.iTotalFrames = 0;
  
  m_nNumVertices = 0;
  m_nNumTriangles = 0;
  m_nNumGroups = 0;
  m_nNumMaterials = 0;
  m_nNumJoints = 0;



}
/////////////////////////////////////////////////////////////////
int
Moose::Data::CMilkshapeLoader::Load(const std::string &sFilename)
{
  return Load( sFilename.c_str());
}
/////////////////////////////////////////////////////////////////
int
Moose::Data::CMilkshapeLoader::Load(const char *szFilename)
{

  fstream	fFile;
  int iRetval = 0;
  unsigned int iFilesize = 0;
  fFile.open( szFilename, ios::binary | ios::in );
  
  if ( !fFile )
  {
    g_Error << "Couldn't open " << szFilename << std::endl;
    return 1;
  }
  
  if ( Check_File(fFile) )
  {
  	g_Error << "Invalid file, loading cancelled." << std::endl;
    return 1;
  }
  Destroy();
  // --------------- READ DATA -----------------------------------
  
  unsigned char *pBuffer = Read_Into_Buffer(fFile, &iFilesize);

  if ( pBuffer == NULL )
  {
    cerr << "Error while reading " << szFilename << std::endl;
    return 1;
  }

  unsigned char *pWorkBuffer = pBuffer;
  
  // Read vertex data 
  pWorkBuffer = Handle_Vertices( pWorkBuffer, pBuffer+iFilesize );
  pWorkBuffer = Handle_Triangles( pWorkBuffer );
  pWorkBuffer = Handle_Groups( pWorkBuffer );
  pWorkBuffer = Handle_Materials( pWorkBuffer );
  pWorkBuffer = Handle_Keyframer( pWorkBuffer );
  pWorkBuffer = Handle_Joints( pWorkBuffer );
  
  
  // --------------- FINISHED ------------------------------------

  
  delete [] pBuffer;
  pBuffer = NULL;
  fFile.close();

  GenerateModelData();
  return iRetval;
}
/////////////////////////////////////////////////////////////////
int 
Moose::Data::CMilkshapeLoader::Check_File( std::fstream &File)
{
  
  int bRetval = 0;
  MS3D_Header_t MS3Dheader;  

  // -----------  Read header information ------------------------
  File.read( (char *)&MS3Dheader, sizeof(MS3D_Header_t));
  
  if ( File.bad())
  {
    g_Error << "Couldn't read file!" << std::endl;
    bRetval = 1;

  } 
  else 
  {
  	//cerr << "MS3Dheader.id = "      << MS3Dheader.id << std::endl;
    //cerr << "MS3Dheader.version = " << MS3Dheader.version << std::endl;
    // Check file type and version, exit if invalid
    if ( strncmp(MS3Dheader.id, "MS3D000000", 10) != 0 ){
      //cerr << "Not a Milkshape3D file!" << std::endl;
      bRetval = 1;
    } else if ( MS3Dheader.version < 3 || MS3Dheader.version > 4 ){
      //cerr << "Only ms3d versions 1.3 and 1.4 are supported!" << std::endl;
      bRetval = 1;
    }
  }
  
  return bRetval;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Read_Into_Buffer( std::fstream &File, unsigned int *pFilesize)
{

  unsigned char *pBuffer = NULL;
  // Determine file size
  File.seekg( 0, ios::end);
  long lFileSize = File.tellg();

  File.seekg(0, ios::beg);
  pBuffer = new unsigned char[lFileSize];

  if ( pFilesize != NULL )
  {
    *pFilesize  = lFileSize;
  }
  File.read( (char *)pBuffer, lFileSize);
  
  // check for reading errors
  if( File.bad())
  {
    delete [] pBuffer;
    pBuffer = NULL;
  }
  return pBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Vertices( unsigned char *pWorkBuffer, unsigned char *pEnd)
{
  if ( pWorkBuffer != NULL )
  {
    // Skip header
    pWorkBuffer+=sizeof(MS3D_Header_t);
    
    // Get the number of vertices in the file
    m_nNumVertices = *(WORD *)pWorkBuffer;
    pWorkBuffer += sizeof(WORD);
    
    //cerr << m_nNumVertices  << " vertices to be read..." << std::endl;
    
    if ( m_nNumVertices >= MAX_VERTICES )
    {
      cerr << "Bogus file - vertex count too large. This madness ends here!" << std::endl;
      pWorkBuffer = NULL;
    }
    
    if ( m_nNumVertices > 0 && m_nNumVertices < MAX_VERTICES )
    {
      m_pVertices = new MS3D_Vertex_t[m_nNumVertices];
     
      for(unsigned int i=0;i<m_nNumVertices;i++)
      {
	if ( (pWorkBuffer+sizeof(MS3D_Vertex_t)) >= pEnd )
	{
	  cerr << "Bogus file - faulty vertex count. This madness ends here!" << std::endl;
	  pWorkBuffer = NULL;
	  break;
	}
	memcpy(&m_pVertices[i], pWorkBuffer, sizeof(MS3D_Vertex_t));
	pWorkBuffer+=sizeof(MS3D_Vertex_t);
#ifdef DEBUG
#if DEBUG>1
	cerr << "v[0] = " << m_pVertices[i].vertex[0]
			  << "v[1] = " << m_pVertices[i].vertex[1]
			  << "v[2] = " << m_pVertices[i].vertex[2] 
			  << "boneid = " << (int)m_pVertices[i].boneId  << std::endl;
#endif
#endif

      }
    }
  }
  
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Triangles( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumTriangles = *(WORD *)pWorkBuffer;
    pWorkBuffer +=sizeof(WORD);

    if ( m_nNumTriangles > 0 && m_nNumTriangles < MAX_TRIANGLES )
    {
      m_pTriangles = new MS3D_Triangle_t[m_nNumTriangles];

      //cerr << m_nNumTriangles
			//<< " triangles to be read.." << std::endl;

      for(unsigned int i=0;i<m_nNumTriangles;i++)
      {
	// flags
	m_pTriangles[i].flags = *(WORD *)pWorkBuffer;
	pWorkBuffer+=sizeof(WORD);
	// vertexIndices
	m_pTriangles[i].vertexIndices[0] = *(WORD *)pWorkBuffer; pWorkBuffer+=sizeof(WORD);
	m_pTriangles[i].vertexIndices[1] = *(WORD *)pWorkBuffer; pWorkBuffer+=sizeof(WORD);
	m_pTriangles[i].vertexIndices[2] = *(WORD *)pWorkBuffer; pWorkBuffer+=sizeof(WORD);
	// vertexnormals
	memcpy(m_pTriangles[i].vertexNormals, pWorkBuffer, sizeof(float)*9);
	pWorkBuffer+=(sizeof(float)*9);
	// s
	memcpy(m_pTriangles[i].s, pWorkBuffer, sizeof(float)*3);
	pWorkBuffer+=(sizeof(float)*3);
	// t
	memcpy(m_pTriangles[i].t, pWorkBuffer, sizeof(float)*3);
	pWorkBuffer+=(sizeof(float)*3);
	
	m_pTriangles[i].smoothingGroup = *(BYTE *)pWorkBuffer;
	pWorkBuffer+=sizeof(BYTE);
	m_pTriangles[i].groupIndex = *(BYTE *)pWorkBuffer;
	pWorkBuffer+=sizeof(BYTE);

      }

    } 
    else 
    {
      cerr << "Bogus file - triangle count faulty. This madness ends here!"
	   << std::endl;
    }
  }
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Groups( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumGroups = *(WORD *)pWorkBuffer;    
    pWorkBuffer+=sizeof(WORD);

    if ( m_nNumGroups > 0 && m_nNumGroups < MAX_GROUPS )
    {
     // cerr << m_nNumGroups
	   //<< " groups to be read.." << std::endl;
      m_pGroups = new MS3D_Group_t[m_nNumGroups];

      for(WORD i=0;i<m_nNumGroups;i++)
      {
	// flags
	m_pGroups[i].flags = *(BYTE *)pWorkBuffer;
	pWorkBuffer+=sizeof(BYTE);
	// the group name
	strncpy(m_pGroups[i].name, (char *)pWorkBuffer, 32);
	pWorkBuffer+=sizeof(char)*32;
	// triangle count
	m_pGroups[i].nNumTriangles = *(WORD *)pWorkBuffer;	
	pWorkBuffer+=sizeof(WORD);

	m_pGroups[i].pTriangleIndices = new WORD[m_pGroups[i].nNumTriangles];
	// triangle indices
	memcpy(m_pGroups[i].pTriangleIndices, pWorkBuffer, sizeof(WORD)*m_pGroups[i].nNumTriangles);
	pWorkBuffer+=(sizeof(WORD)*m_pGroups[i].nNumTriangles);
	
	// material index
	m_pGroups[i].materialIndex = *(char *)pWorkBuffer;
	pWorkBuffer+=sizeof(char);
	
      }
    } 
    else 
    {
      cerr << "Bogus file - triangle count faulty. This madness ends here!"
	   << std::endl;
    }
  }
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Materials( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumMaterials = *(WORD *)pWorkBuffer;
    pWorkBuffer+=sizeof(WORD);

    if ( m_nNumMaterials < MAX_MATERIALS)
    {
      m_pMaterials = new MS3D_Material_t[m_nNumMaterials];
      //cerr << m_nNumMaterials << " materials to be read.." << std::endl;
      for(WORD i=0;i<m_nNumMaterials;i++)
      {
	memcpy( &m_pMaterials[i], pWorkBuffer, sizeof(MS3D_Material_t));
	pWorkBuffer+=sizeof(MS3D_Material_t);
      }
    } 
    else 
    {
      cerr << "Bogus file - material count faulty. This madness ends here!"
	   << std::endl;
    }
  }
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Keyframer( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_Animationdata.fAnimationFPS = *(float *)pWorkBuffer;
    pWorkBuffer+=sizeof(float);
    m_Animationdata.fCurrentTime =  *(float *)pWorkBuffer;
    pWorkBuffer+=sizeof(float);
    m_Animationdata.iTotalFrames = *(int *)pWorkBuffer;
    pWorkBuffer+=sizeof(int);
    /*
    cerr << "Animation FPS : " 
		      << m_Animationdata.fAnimationFPS
		      << ", #frames " 
		      << m_Animationdata.iTotalFrames << std::endl;
    */
  }
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Data::CMilkshapeLoader::Handle_Joints( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumJoints = *(WORD *)pWorkBuffer;
    pWorkBuffer+=sizeof(WORD);
    
    if ( m_nNumJoints == 0 ) 
    {
      //cerr <<  "No joints to be read" << endl;
    }
    else if (m_nNumJoints >= MAX_JOINTS) 
    {
      cerr << "Bogus file - joint count too large " 
	   << m_nNumJoints << " vs. " 
	   << MAX_JOINTS-1 << ". This madness ends here!" << endl;
    }
    else
    {
      m_pJoints = new MS3D_Joint_t[m_nNumJoints];
      /*cerr << m_nNumJoints 
	<< " joints to be read.." << std::endl;*/
      for(WORD i=0;i<m_nNumJoints;i++){
	// flags
	m_pJoints[i].flags = *(BYTE *)pWorkBuffer;
	pWorkBuffer+=sizeof(BYTE);
	// name
	strncpy(m_pJoints[i].name, (char *)pWorkBuffer, 32);
	pWorkBuffer+=sizeof(char)*32;
	/*cerr << "name of the joint : "
	  << m_pJoints[i].name << std::endl;*/
	// Parentname
	strncpy(m_pJoints[i].parentName, (char *)pWorkBuffer, 32);
	pWorkBuffer+=sizeof(char)*32;
	// Rotation
	memcpy( m_pJoints[i].rotation, pWorkBuffer, sizeof(float)*3);
	pWorkBuffer+=sizeof(float)*3;
	// Position
	memcpy( m_pJoints[i].position, pWorkBuffer, sizeof(float)*3);
	pWorkBuffer+=sizeof(float)*3;
	// Rotation keyframe count
	m_pJoints[i].nNumKeyFramesRot = *(WORD *)pWorkBuffer;
	pWorkBuffer+=sizeof(WORD);
	// Translation keyframe count
	m_pJoints[i].nNumKeyFramesTrans = *(WORD *)pWorkBuffer;
	pWorkBuffer+=sizeof(WORD);
	
	// Allocate keyframe arrays
	m_pJoints[i].keyFramesRot   = new MS3D_Keyframe_rot_t[m_pJoints[i].nNumKeyFramesRot];
	m_pJoints[i].keyFramesTrans = new MS3D_Keyframe_pos_t[m_pJoints[i].nNumKeyFramesTrans];

	// copy rotation keyframes
	for( WORD iR=0; iR<m_pJoints[i].nNumKeyFramesRot; iR++)
	{
	  m_pJoints[i].keyFramesRot[iR].time = *(float *)pWorkBuffer;
	  pWorkBuffer+=sizeof(float);
	  memcpy(m_pJoints[i].keyFramesRot[iR].rotation, pWorkBuffer, sizeof(float)*3);
	  pWorkBuffer+=sizeof(float)*3;
	}
	// copy translation keyframes
	for( WORD iT=0; iT<m_pJoints[i].nNumKeyFramesTrans; iT++)
	{
	  m_pJoints[i].keyFramesTrans[iT].time = *(float *)pWorkBuffer;
	  pWorkBuffer+=sizeof(float);
	  memcpy(m_pJoints[i].keyFramesTrans[iT].position, pWorkBuffer, sizeof(float)*3);
	  pWorkBuffer+=sizeof(float)*3;
	}
      }
    } 
  }
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
void
Moose::Data::CMilkshapeLoader::GenerateModelData()
{
  vector<CVertex> vecVertices;
  vector<unsigned int> vecIndices;

  CreateTriangleList( vecVertices, vecIndices );
  CreateGroupIndexMap( vecVertices );
  
  delete m_pPositions;
  delete m_pNormals;
  delete m_pTexCoords;
  delete m_pIndices;
  //cerr << "generating model data" << vecVertices.size() << endl;
  
  // Create new descriptors / array
  m_pPositions = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, vecVertices.size());
  m_pNormals = new CVertexDescriptor(ELEMENT_TYPE_NORMAL_3F, vecVertices.size());
  m_pTexCoords = new CVertexDescriptor(ELEMENT_TYPE_TEX_2F, vecVertices.size());
  m_pIndices = new CIndexArray(PRIMITIVE_TRI_LIST, vecIndices.size());
  unsigned int nIndex = 0;
  
  for(unsigned int i=0;i<vecVertices.size();i++)
  {
    nIndex = i*3;
    m_pPositions->GetPointer<float>()[nIndex]   = vecVertices[i].GetPosition()[0];
    m_pPositions->GetPointer<float>()[nIndex+1] = vecVertices[i].GetPosition()[1];
    m_pPositions->GetPointer<float>()[nIndex+2] = vecVertices[i].GetPosition()[2];

    m_pNormals->GetPointer<float>()[nIndex]   = vecVertices[i].GetNormal()[0];
    m_pNormals->GetPointer<float>()[nIndex+1] = vecVertices[i].GetNormal()[1];
    m_pNormals->GetPointer<float>()[nIndex+2] = vecVertices[i].GetNormal()[2];
    
    nIndex = i*2;
    m_pTexCoords->GetPointer<float>()[nIndex]   = vecVertices[i].GetTextureCoordinates()[0];
    m_pTexCoords->GetPointer<float>()[nIndex+1] = vecVertices[i].GetTextureCoordinates()[1];

  }
  unsigned int nMaxIndex = 0;
  for(unsigned int i=0;i<vecIndices.size();i++)
  {
    if ( vecIndices[i] > nMaxIndex ) nMaxIndex = vecIndices[i];
    
    if ( m_pIndices->IsShortIndices() )
    {
      m_pIndices->GetPointer<unsigned short int>()[i] = vecIndices[i];
    }
    else
    {
      m_pIndices->GetPointer<unsigned int>()[i] = vecIndices[i];
    }
  }
  
}
/////////////////////////////////////////////////////////////////
#define CREATE_VERTEX( VERTEX, TRIANGLE, T_VERTEXINDEX ) {			   \
										   \
 VERTEX.SetPosition(m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].vertex[0],  \
		    m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].vertex[1],  \
		    m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].vertex[2]); \
 VERTEX.m_vNormal.Set(TRIANGLE.vertexNormals[T_VERTEXINDEX]);				   \
 VERTEX.SetTextureCoordinates( TRIANGLE.s[T_VERTEXINDEX], TRIANGLE.t[T_VERTEXINDEX], 0)  ; \
 /*VERTEX.m_iBoneId = m_pVertices[TRIANGLE.vertexIndices[T_VERTEXINDEX]].boneId;*/	   \
 											   \
}
/////////////////////////////////////////////////////////////////

struct TriInd
{
  MS3D_Triangle_t *ptr;
  int index[3];
};
/////////////////////////////////////////////////////////////////
void 
PrintDiff( MS3D_Triangle_t &a , MS3D_Triangle_t & b, int whichVertex )
{
  cerr << "--****-----" << endl;
  cerr << a.vertexNormals[whichVertex][0] << " != " << b.vertexNormals[whichVertex][0]  << endl;
  cerr << a.vertexNormals[whichVertex][1] << " != " << b.vertexNormals[whichVertex][1] << endl;
  cerr << a.vertexNormals[whichVertex][2] << " != " << b.vertexNormals[whichVertex][2] << endl;
  cerr << a.s[whichVertex] << " != " << b.s[whichVertex] << endl;
  cerr << a.t[whichVertex] << " != " << b.t[whichVertex] << endl; 
  cerr << "-------" << endl;
}
/////////////////////////////////////////////////////////////////
/// Compares two triangles by normals and texcoords if they are same.
inline bool 
CloseEnough( MS3D_Triangle_t &a , int whichVertex, MS3D_Triangle_t & b, int whichVertexPrev )
{
  return ( QUITE_CLOSE_TO( a.vertexNormals[whichVertex][0], b.vertexNormals[whichVertexPrev][0] ) &&  
	   QUITE_CLOSE_TO( a.vertexNormals[whichVertex][1], b.vertexNormals[whichVertexPrev][1] ) &&
	   QUITE_CLOSE_TO( a.vertexNormals[whichVertex][2], b.vertexNormals[whichVertexPrev][2] ) &&
	   QUITE_CLOSE_TO( a.s[whichVertex], b.s[whichVertexPrev] ) &&
	   QUITE_CLOSE_TO( a.t[whichVertex], b.t[whichVertexPrev] ));
}
/////////////////////////////////////////////////////////////////
void 
Moose::Data::CMilkshapeLoader::CreateTriangleList( vector<CVertex> &vecVertices, vector<unsigned int> &vecIndices)
{
  
  // Collect all triangles connected to 
  vector< TriInd > * vertTriangles = new vector<TriInd>[m_nNumVertices];
  
  for(size_t nT=0; nT<m_nNumTriangles; nT++)
  {
    for( int v=0;v<3;++v)
    {
      TriInd ti;
      ti.ptr = &m_pTriangles[nT];
      ti.index[0] = -1;
      ti.index[1] = -1;
      ti.index[2] = -1;
      vertTriangles[ m_pTriangles[nT].vertexIndices[v] ].push_back( ti );
    }
  }

#ifdef DEBUG
#if DEBUG>1
  for( size_t i=0; i< m_nNumVertices; ++i)
  {
    vector< TriInd > & tris = vertTriangles[i];
    cerr << "vertex " << i ;
    cerr << "(" ;
    cerr << m_pVertices[i].vertex[0] << ",";
    cerr << m_pVertices[i].vertex[1] << ",";
    cerr << m_pVertices[i].vertex[2] << ")";
    cerr << " has " << tris.size() << "triangles." << endl;
  }
#endif
#endif
  vecVertices.clear();
  vecIndices.clear();
  
  // insert new vertices and store indices into TriInd structs.
  for( size_t i=0; i< m_nNumVertices; ++i)
  {
    vector< TriInd > & tris = vertTriangles[i];
    for( size_t t=0;t< tris.size(); ++t)
    {
      bool foundExisting = false;
      int whichVertex = 0;
      
      // check which index we must handle
      if ( tris[t].ptr->vertexIndices[0] == i)        whichVertex = 0;
      else if ( tris[t].ptr->vertexIndices[1] == i)   whichVertex = 1;
      else whichVertex = 2; /*( tris[t].ptr->vertexIndices[2] == i)*/  
      
      
      // go trough all triangles before current 
      for( size_t p=0;p<t; ++p)
      {
	// another vertex might have _different_ index...
	int whichVertexPrev = 0;
	if ( tris[p].ptr->vertexIndices[0] == i)        whichVertexPrev = 0;
	else if ( tris[p].ptr->vertexIndices[1] == i)   whichVertexPrev = 1;
	else whichVertexPrev = 2; /*( tris[t].ptr->vertexIndices[2] == i)*/  
	
	// if identical vertex exists, use it. (comparing normals and texcoords).
	if ( CloseEnough( *tris[t].ptr, whichVertex, *tris[p].ptr, whichVertexPrev) )
	{
	  foundExisting = true;
	  tris[t].index[whichVertex] = tris[p].index[whichVertexPrev];
	  break;
	}

      }
      // if existing vertex was not found
      if ( ! foundExisting )
      {
	// insert new vertex, normal, texcoord and index
	CVertex vertex;    
	CREATE_VERTEX( vertex, (*tris[t].ptr), whichVertex );
	vecVertices.push_back( vertex );
	tris[t].index[whichVertex] = vecVertices.size()-1;

#ifdef DEBUG
#if DEBUG>1
	cerr << "checking vert" << i << ", inserting : " << vertex << endl;
#endif
#endif

      }
    } // tris
  } // vertices

#ifdef DEBUG
#if DEBUG>1
  cerr << "copying index data" << endl;
  cerr << "#different vertices: " << vecVertices.size() << endl;
#endif
#endif
  //
  // copy index data from TriInd to actual triangles, so it can be 
  // used to create triangle index list
  for( size_t i=0; i< m_nNumVertices; ++i)
  {
    vector< TriInd > & tris = vertTriangles[i];

    for( size_t t=0;t< tris.size(); ++t)
    {
      // copy index position only if set (not -1)
      if ( tris[t].index[0] > -1 )
      {
	assert( tris[t].index[0] > -1 && (size_t)tris[t].index[0] < vecVertices.size());
	tris[t].ptr->vertexIndices[0] = tris[t].index[0];
      } 

      if ( tris[t].index[1] > -1)
      {
	assert( tris[t].index[1] > -1 && (size_t)tris[t].index[1] < vecVertices.size());
	tris[t].ptr->vertexIndices[1] = tris[t].index[1];
      }

      if ( tris[t].index[2] > -1)
      {
	assert( tris[t].index[2] > -1 && (size_t)tris[t].index[2] < vecVertices.size());
	tris[t].ptr->vertexIndices[2] = tris[t].index[2];
      }
    }
  }
#ifdef DEBUG
#if DEBUG>1
  cerr << "creating triangle list" << endl;
#endif
#endif
  // go through each triangle and create a list.
  
  for(size_t nT=0; nT<m_nNumTriangles; ++nT)
  {
    MS3D_Triangle_t & tri = m_pTriangles[nT];
    assert( tri.vertexIndices[0] < vecVertices.size() );
    assert( tri.vertexIndices[1] < vecVertices.size() );
    assert( tri.vertexIndices[2] < vecVertices.size() );
    vecIndices.push_back( tri.vertexIndices[0] );
    vecIndices.push_back( tri.vertexIndices[1] );
    vecIndices.push_back( tri.vertexIndices[2] );
  }

  delete [] vertTriangles;

}  // separatevertices
/////////////////////////////////////////////////////////////////
void 
Moose::Data::CMilkshapeLoader::CreateGroupIndexMap( std::vector<Moose::Spatial::CVertex> &vecVertices  )
{

  m_mapGroups.clear();
  std::list<size_t> lstIndices;
  // for each group
  for( size_t g=0;g<m_nNumGroups;g++)
  {
    struct MS3D_Group_t group = m_pGroups[g];
    
    lstIndices.clear();
    
    // for each triangle in Group
    for( size_t t=0; t<group.nNumTriangles; t++)
    {
      MS3D_Triangle_t & triangle = m_pTriangles[group.pTriangleIndices[t]];

      // at this point, triangle indices have been optimized 
      lstIndices.push_back( triangle.vertexIndices[0] );
      lstIndices.push_back( triangle.vertexIndices[1] );
      lstIndices.push_back( triangle.vertexIndices[2] );
      
    } // for each tri

    // Create index array from indices
    CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, lstIndices.size());
    if ( pIndices->IsShortIndices())
    {
      std::list<size_t>::iterator it = lstIndices.begin();
      for( size_t i=0;i<lstIndices.size();i++, it++)
      {
	pIndices->GetPointer<unsigned short int>()[i] = *it;
      }
    } 
    else
    {
      std::list<size_t>::iterator it = lstIndices.begin();
      for( size_t i=0;i<lstIndices.size();i++, it++)
      {
	pIndices->GetPointer<unsigned int>()[i] = *it;
      }
    }
    // Insert indexarray under group name 
    m_mapGroups.insert( make_pair( std::string(group.name), pIndices ) );
  }
}
/////////////////////////////////////////////////////////////////
// void
// Moose::Data::CMilkshapeLoader::Stripify()
// {
// #ifdef TRISTRIPPER
//   triangle_stripper::indices triangleIndices;
//   CIndexArray *pIndices = NULL;
  
//   if ( GetIndices().size() == 1 )
//   {
//     pIndices = GetIndices()[0];
//     if ( pIndices == NULL ) return;
//   }
//   else
//   {
//     cerr << "Too many index lists for stripification! I can use only one!" << endl;
//     return;
//   }

//   for(unsigned int i=0; i<pIndices->GetNumIndices(); i++)
//   {
//     if ( pIndices->IsShortIndices())
//     {
//       triangleIndices.push_back(pIndices->GetPointer<unsigned short int>()[i]);
//     }
//     else
//     {
//       triangleIndices.push_back(pIndices->GetPointer<unsigned int>()[i]);
//     }
//   }
//   triangle_stripper::primitive_vector primitiveVector;
//   triangle_stripper::tri_stripper triStripper( triangleIndices );
  
//   triStripper.SetMinStripSize(2);
//   triStripper.SetCacheSize(0);
//   triStripper.SetBackwardSearch(true);
//   triStripper.Strip( &primitiveVector );
  
//   unsigned int nStripCount = 0;
//   unsigned int nListCount = 0;
//   unsigned int nListLength = 0;
//   unsigned int nStripAvgLength = 0;
//   unsigned int nStripTriangleCount = 0;

//   for( unsigned int i=0;i<primitiveVector.size();i++)
//   {
//     if( primitiveVector[i].Type == triangle_stripper::TRIANGLE_STRIP)
//     {
//       nStripCount++;
//       nStripAvgLength += primitiveVector[i].Indices.size();
//       nStripTriangleCount += primitiveVector[i].Indices.size()-2;
//     }
//     else
//     {
//       nListCount++;
//       nListLength += primitiveVector[i].Indices.size();

//     }
//   }
//   nStripAvgLength /= nStripCount;
//   cerr << "Created " << nStripCount << " strips. Average length " << nStripAvgLength << ", in total " << nStripTriangleCount << " triangles drawn as stripped." << endl;
//   cerr << "Created " << nListCount << " lists. " << nListLength / 3  << " triangles drawn unstripped." << endl;

//   // Remove original triangle list.
//   delete GetIndices()[0];
//   GetIndices().clear();
//   pIndices = NULL;
  
//   // for each batch of primitives
//   for( unsigned int i=0;i<primitiveVector.size();i++)
//   {
//     // Check which type of primitive we got
//     if ( primitiveVector[i].Type == triangle_stripper::TRIANGLE_STRIP )
//     {
//       pIndices = new CIndexArray(PRIMITIVE_TRI_STRIP, primitiveVector[i].Indices.size());
//     }
//     else
//     {
//       pIndices = new CIndexArray(PRIMITIVE_TRI_LIST, primitiveVector[i].Indices.size());
//     }
//     // Copy indices into indexarray
//     for(unsigned int p=0;p<primitiveVector[i].Indices.size();p++)
//     {
//       if (pIndices->IsShortIndices())
//       {
// 	pIndices->GetPointer<unsigned short int>()[p] = primitiveVector[i].Indices[p];
//       }
//       else
//       {
// 	pIndices->GetPointer<unsigned int>()[p] = primitiveVector[i].Indices[p];
//       }
//     }
//     // Put index array into vector
//     GetIndices().push_back( pIndices );
//   }
// #endif
// }
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
Moose::Data::CMilkshapeLoader::GetVertexArray( float fScale ) const
{
  return new CVertexDescriptor( *m_pPositions);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
Moose::Data::CMilkshapeLoader::GetNormalArray() const
{
  return new CVertexDescriptor( *m_pNormals);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
Moose::Data::CMilkshapeLoader::GetTexCoordArray( size_t nTexUnit ) const
{
  return new CVertexDescriptor( *m_pTexCoords);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
Moose::Data::CMilkshapeLoader::GetColorArray() const
{
  return new CVertexDescriptor( *m_pColors);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CIndexArray * 
Moose::Data::CMilkshapeLoader::GetIndexArray( const char *szGroupName  ) const
{
  CIndexArray *pIndexArray = NULL;
  if ( szGroupName == NULL )
  {
    pIndexArray = new CIndexArray( *m_pIndices );
  }
  else
  {
    std::map<std::string, Moose::Graphics::CIndexArray *>::const_iterator it;
    it = m_mapGroups.find(std::string(szGroupName));
    // Check that we found it...
    if ( it != m_mapGroups.end())
    {
      // create new indexarray.
      pIndexArray = new CIndexArray( *((*it).second) );
    }
    
  }
  return pIndexArray;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
Moose::Data::CMilkshapeLoader::GetInterleavedArray( Moose::Graphics::ELEMENT_TYPE tType ) const
{
  CVertexDescriptor *pData = new CVertexDescriptor( tType, m_pPositions->GetSize() );
  for( size_t i=0;i<m_pPositions->GetSize(); ++i)
  {
    pData->GetPointer<float>(i)[0] = m_pPositions->GetPointer<float>(i)[0];
    pData->GetPointer<float>(i)[1] = m_pPositions->GetPointer<float>(i)[1];
    pData->GetPointer<float>(i)[2] = m_pPositions->GetPointer<float>(i)[2];

    pData->GetPointer<float>(i)[3] = m_pNormals->GetPointer<float>(i)[0];
    pData->GetPointer<float>(i)[4] = m_pNormals->GetPointer<float>(i)[1];
    pData->GetPointer<float>(i)[5] = m_pNormals->GetPointer<float>(i)[2];

    pData->GetPointer<float>(i)[6] = m_pTexCoords->GetPointer<float>(i)[0];
    pData->GetPointer<float>(i)[7] = m_pTexCoords->GetPointer<float>(i)[1];

  }
  return pData;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Data::CMilkshapeLoader::IsMilkshapeFile( const char *szFilename )
{
	CMilkshapeLoader loader;

	fstream fFile;
	fFile.open( szFilename, ios::binary | ios::in );

  if ( !fFile )
  {
    //cerr << "Couldn't open " << szFilename << std::endl;
    return false;
  }

  if ( loader.Check_File(fFile) )
  {
    //cerr << "Invalid file, loading cancelled." << std::endl;
    return false;
  }
  return true;
}
