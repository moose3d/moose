/////////////////////////////////////////////////////////////////
#include "PhoenixMilkshapeLoader.h"
#include <fstream>
#include <vector>
#ifndef WIN32
// Win32 support for tri_stripper needs some tweaking.
// it is not too crucial to be left out.
#include <tri_stripper.h>
#endif
/////////////////////////////////////////////////////////////////
#define DELETE(OBJ) if(OBJ != NULL ) delete OBJ; OBJ=NULL;
/////////////////////////////////////////////////////////////////
using std::fstream;
using std::ios;
using namespace Phoenix::Data;
using namespace Phoenix::Graphics;
using Phoenix::Spatial::CVertex;
using std::cerr;
using std::endl;
using std::vector;

/////////////////////////////////////////////////////////////////
Phoenix::Data::CMilkshapeLoader::CMilkshapeLoader()
{
  Init();
}
/////////////////////////////////////////////////////////////////
Phoenix::Data::CMilkshapeLoader::~CMilkshapeLoader()
{
  Destroy();
}
/////////////////////////////////////////////////////////////////
Phoenix::Data::CMilkshapeLoader::CMilkshapeLoader( const CMilkshapeLoader &ref)
{
  // Get sizes 
  m_nNumVertices = ref.m_nNumVertices;
  m_nNumTriangles = ref.m_nNumTriangles ;
  m_nNumGroups = ref.m_nNumGroups;
  m_nNumMaterials = ref.m_nNumMaterials;
  m_nNumJoints = ref.m_nNumJoints;

  // Allocate memory 
  m_pVertices	= new MS3D_Vertex_t[m_nNumVertices];
  m_pTriangles	= new MS3D_Triangle_t[m_nNumTriangles];
  m_pGroups	= new MS3D_Group_t[m_nNumGroups];
  m_pMaterials  = new MS3D_Material_t[m_nNumMaterials];
  m_pJoints     = new MS3D_Joint_t[m_nNumJoints];

  // Copy data
  memcpy(m_pVertices, ref.m_pVertices,  sizeof(ref.m_pVertices));
  memcpy(m_pTriangles,ref.m_pTriangles, sizeof(ref.m_pTriangles));
  memcpy(m_pGroups,   ref.m_pGroups,    sizeof(ref.m_pGroups) );

  for(unsigned int i=0;i<m_nNumGroups;i++){
    m_pGroups[i].pTriangleIndices = new WORD[m_pGroups[i].nNumTriangles];
    memcpy(m_pGroups[i].pTriangleIndices, ref.m_pGroups[i].pTriangleIndices, sizeof(WORD)*m_pGroups[i].nNumTriangles);
  }
  
  memcpy(m_pMaterials,   ref.m_pMaterials,    sizeof(m_pMaterials) );
  
  m_Animationdata.fAnimationFPS = ref.m_Animationdata.fAnimationFPS;
  m_Animationdata.fCurrentTime  = ref.m_Animationdata.fCurrentTime;
  m_Animationdata.iTotalFrames  = ref.m_Animationdata.iTotalFrames;
 
  memcpy(m_pJoints, ref.m_pJoints, sizeof(ref.m_pJoints));

  for(unsigned int i=0;i<m_nNumJoints;i++)
  {
    m_pJoints[i].keyFramesRot   = new MS3D_Keyframe_rot_t[m_pJoints[i].nNumKeyFramesRot];
    m_pJoints[i].keyFramesTrans = new MS3D_Keyframe_pos_t[m_pJoints[i].nNumKeyFramesTrans];
    memcpy(m_pJoints[i].keyFramesRot,   
	   ref.m_pJoints[i].keyFramesRot,   
	   sizeof(ref.m_pJoints[i].keyFramesRot));
    memcpy(m_pJoints[i].keyFramesTrans, 
	   ref.m_pJoints[i].keyFramesTrans, 
	   sizeof(ref.m_pJoints[i].keyFramesTrans));
  }

  memcpy(m_pVertexWeights, ref.m_pVertexWeights, sizeof(ref.m_pVertexWeights));
  m_bHasBeenLoaded = ref.m_bHasBeenLoaded;
}
/////////////////////////////////////////////////////////////////
CMilkshapeLoader &
Phoenix::Data::CMilkshapeLoader::operator=( CMilkshapeLoader obj )
{
  Destroy();
  // Get sizes 
  m_nNumVertices = obj.m_nNumVertices;
  m_nNumTriangles = obj.m_nNumTriangles ;
  m_nNumGroups = obj.m_nNumGroups;
  m_nNumMaterials = obj.m_nNumMaterials;
  m_nNumJoints = obj.m_nNumJoints;

  // Allocate memory 
  m_pVertices	= new MS3D_Vertex_t[m_nNumVertices];
  m_pTriangles	= new MS3D_Triangle_t[m_nNumTriangles];
  m_pGroups	= new MS3D_Group_t[m_nNumGroups];
  m_pMaterials  = new MS3D_Material_t[m_nNumMaterials];
  m_pJoints     = new MS3D_Joint_t[m_nNumJoints];

  // Copy data
  memcpy(m_pVertices, obj.m_pVertices,  sizeof(obj.m_pVertices));
  memcpy(m_pTriangles,obj.m_pTriangles, sizeof(obj.m_pTriangles));
  memcpy(m_pGroups,   obj.m_pGroups,    sizeof(obj.m_pGroups) );

  for(unsigned int i=0;i<m_nNumGroups;i++){
    m_pGroups[i].pTriangleIndices = new WORD[m_pGroups[i].nNumTriangles];
    memcpy(m_pGroups[i].pTriangleIndices, 
	   obj.m_pGroups[i].pTriangleIndices, 
	   sizeof(WORD)*m_pGroups[i].nNumTriangles);
  }
  
  memcpy(m_pMaterials,   obj.m_pMaterials,    sizeof(m_pMaterials) );

  m_Animationdata.fAnimationFPS = obj.m_Animationdata.fAnimationFPS;
  m_Animationdata.fCurrentTime  = obj.m_Animationdata.fCurrentTime;
  m_Animationdata.iTotalFrames  = obj.m_Animationdata.iTotalFrames;
 
  memcpy(m_pJoints, obj.m_pJoints, sizeof(obj.m_pJoints));

  for(unsigned int i=0;i<m_nNumJoints;i++){
    m_pJoints[i].keyFramesRot   = new MS3D_Keyframe_rot_t[m_pJoints[i].nNumKeyFramesRot];
    m_pJoints[i].keyFramesTrans = new MS3D_Keyframe_pos_t[m_pJoints[i].nNumKeyFramesTrans];
    memcpy(m_pJoints[i].keyFramesRot,   
	   obj.m_pJoints[i].keyFramesRot,   
	   sizeof(obj.m_pJoints[i].keyFramesRot));
    memcpy(m_pJoints[i].keyFramesTrans, 
	   obj.m_pJoints[i].keyFramesTrans, 
	   sizeof(obj.m_pJoints[i].keyFramesTrans));
  }

  memcpy(m_pVertexWeights, 
	 obj.m_pVertexWeights, 
	 sizeof(obj.m_pVertexWeights));
  m_bHasBeenLoaded = obj.m_bHasBeenLoaded;
  return *this;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CMilkshapeLoader::Init()
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
  m_bHasBeenLoaded = 0;

  m_pPositions = NULL;
  m_pNormals = NULL;
  m_pColors = NULL;
  m_pTexCoords = NULL;
}
/////////////////////////////////////////////////////////////////
// Frees the memory reserved by the model 
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CMilkshapeLoader::Destroy()
{
  
  unsigned int i = 0;
  DELETE(m_pVertices);
  DELETE(m_pTriangles);
  DELETE(m_pMaterials);
  // Groups have pointer members
  for( i=0;i<m_nNumGroups;i++ )
  {
    DELETE(m_pGroups[i].pTriangleIndices);
  }
  DELETE(m_pGroups);
  // Joints have pointer members
  for( i=0;i<m_nNumJoints;i++ )
  {
    DELETE( m_pJoints[i].keyFramesRot);
    DELETE( m_pJoints[i].keyFramesTrans);    
  }
  
  DELETE(m_pJoints);
  DELETE(m_pVertexWeights);  
  DELETE(m_pPositions);
  DELETE(m_pNormals);
  DELETE(m_pColors);
  DELETE(m_pTexCoords);
  while( GetIndices().size() > 0 )
  {
    DELETE(GetIndices()[GetIndices().size()-1]);
    GetIndices().pop_back();
  }

  
  m_Animationdata.fAnimationFPS = 0.0;
  m_Animationdata.fCurrentTime = 0.0;
  m_Animationdata.iTotalFrames = 0;

  m_nNumVertices = 0;
  m_nNumTriangles = 0;
  m_nNumGroups = 0;
  m_nNumMaterials = 0;
  m_nNumJoints = 0;

  m_bHasBeenLoaded = 0;

}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CMilkshapeLoader::Load(const std::string &sFilename)
{

  fstream	fFile;
  int iRetval = 0;
  unsigned int iFilesize = 0;
  fFile.open( sFilename.c_str(), ios::binary | ios::in );
  
  if ( !fFile )
  {
    cerr << "Couldn't open " << sFilename << std::endl;
    return 1;
  }
  
  if ( Check_File(fFile) )
  {
    cerr << "Invalid file, loading cancelled." << std::endl;
    return 1;
  }

  if ( m_bHasBeenLoaded ) Destroy();
  // --------------- READ DATA -----------------------------------
  
  unsigned char *pBuffer = Read_Into_Buffer(fFile, &iFilesize);

  if ( pBuffer == NULL )
  {
    cerr << "Error while reading " << sFilename << std::endl;
    return 1;
  }

  unsigned char *pWorkBuffer = pBuffer;
  
  // Read vertex data 
  pWorkBuffer = Handle_Vertices( pWorkBuffer, pBuffer+iFilesize );
  // Read Triangle data
  pWorkBuffer = Handle_Triangles( pWorkBuffer );
  // Read Group data
  pWorkBuffer = Handle_Groups( pWorkBuffer );
  // Read Material data
  pWorkBuffer = Handle_Materials( pWorkBuffer );
  // Read Keyframe data 
  pWorkBuffer = Handle_Keyframer( pWorkBuffer );
  // Read Joints data 
  pWorkBuffer = Handle_Joints( pWorkBuffer );
  
  
  // --------------- FINISHED ------------------------------------

  if ( pBuffer != NULL ) 
  {
    delete pBuffer;
  }
  pBuffer = NULL;
  fFile.close();
  m_bHasBeenLoaded = 1;

  return iRetval;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Data::CMilkshapeLoader::Check_File( std::fstream &File)
{
  
  int bRetval = 0;
  MS3D_Header_t MS3Dheader;  

  // -----------  Read header information ------------------------
  File.read( (char *)&MS3Dheader, sizeof(MS3D_Header_t));
  
  if ( File.bad())
  {
    cerr << "Couldn't read file!" << std::endl;
    bRetval = 1;

  } 
  else 
  {
    cerr << "MS3Dheader.id = "      << MS3Dheader.id << std::endl;
    cerr << "MS3Dheader.version = " << MS3Dheader.version << std::endl;
    // Check file type and version, exit if invalid
    if ( strncmp(MS3Dheader.id, "MS3D000000", 10) != 0 ){
      cerr << "Not a Milkshape3D file!" << std::endl;
      bRetval = 1;
    } else if ( MS3Dheader.version < 3 || MS3Dheader.version > 4 ){
      cerr << "Only ms3d versions 1.3 and 1.4 are supported!" << std::endl;
      bRetval = 1;
    }
  }
  
  return bRetval;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Phoenix::Data::CMilkshapeLoader::Read_Into_Buffer( std::fstream &File, unsigned int *pFilesize)
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
    delete pBuffer;
    pBuffer = NULL;
  }
  return pBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Phoenix::Data::CMilkshapeLoader::Handle_Vertices( unsigned char *pWorkBuffer, unsigned char *pEnd)
{
  if ( pWorkBuffer != NULL )
  {
    // Skip header
    pWorkBuffer+=sizeof(MS3D_Header_t);
    
    // Get the number of vertices in the file
    m_nNumVertices = *(WORD *)pWorkBuffer;
    pWorkBuffer += sizeof(WORD);
    
    cerr << m_nNumVertices 
	 << " vertices to be read..." << std::endl;
    
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
	cerr << "v[0] = " << m_pVertices[i].vertex[0]
			  << "v[1] = " << m_pVertices[i].vertex[1]
			  << "v[2] = " << m_pVertices[i].vertex[2] 
			  << "boneid = " << (int)m_pVertices[i].boneId  << std::endl;
#endif

      }
    }
  }
  
  return pWorkBuffer;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Phoenix::Data::CMilkshapeLoader::Handle_Triangles( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumTriangles = *(WORD *)pWorkBuffer;
    pWorkBuffer +=sizeof(WORD);

    if ( m_nNumTriangles > 0 && m_nNumTriangles < MAX_TRIANGLES )
    {
      m_pTriangles = new MS3D_Triangle_t[m_nNumTriangles];

      cerr << m_nNumTriangles 
			<< " triangles to be read.." << std::endl;

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
Phoenix::Data::CMilkshapeLoader::Handle_Groups( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumGroups = *(WORD *)pWorkBuffer;    
    pWorkBuffer+=sizeof(WORD);

    if ( m_nNumGroups > 0 && m_nNumGroups < MAX_GROUPS )
    {
      cerr << m_nNumGroups 
	   << " groups to be read.." << std::endl;
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
Phoenix::Data::CMilkshapeLoader::Handle_Materials( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumMaterials = *(WORD *)pWorkBuffer;
    pWorkBuffer+=sizeof(WORD);

    if ( m_nNumMaterials > 0 && m_nNumMaterials < MAX_MATERIALS)
    {
      m_pMaterials = new MS3D_Material_t[m_nNumMaterials];
      cerr << m_nNumMaterials << " materials to be read.." << std::endl;
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
Phoenix::Data::CMilkshapeLoader::Handle_Keyframer( unsigned char *pWorkBuffer)
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
Phoenix::Data::CMilkshapeLoader::Handle_Joints( unsigned char *pWorkBuffer)
{
  if ( pWorkBuffer != NULL )
  {
    m_nNumJoints = *(WORD *)pWorkBuffer;
    pWorkBuffer+=sizeof(WORD);
    
    if ( m_nNumJoints == 0 ) 
    {
      cerr <<  "No joints to be read" << endl;
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
Phoenix::Data::CMilkshapeLoader::GenerateModelData()
{
  vector<CVertex> vecVertices;
  vector<unsigned int> vecIndices;
  CreateTriangleList( vecVertices, vecIndices);
  
  DELETE(m_pPositions);
  DELETE(m_pNormals);
  DELETE(m_pTexCoords);
  // delete all previous index arrays, if any
  while( GetIndices().size() > 0 )
  {
    delete GetIndices()[GetIndices().size()-1];
    GetIndices().pop_back();
  }
  // Create new descriptors / array
  m_pPositions = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, vecVertices.size());
  m_pNormals = new CVertexDescriptor(ELEMENT_TYPE_NORMAL_3F, vecVertices.size());
  m_pTexCoords = new CVertexDescriptor(ELEMENT_TYPE_TEX_2F, vecVertices.size());
  CIndexArray *pIndices = new CIndexArray(PRIMITIVE_TRI_LIST, vecIndices.size());
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
  
  for(unsigned int i=0;i<vecIndices.size();i++)
  {
    if ( pIndices->IsShortIndices() )
    {
      pIndices->GetPointer<unsigned short int>()[i] = vecIndices[i];
    }
    else
    {
      pIndices->GetPointer<unsigned int>()[i] = vecIndices[i];
    }
  }
 
  GetIndices().push_back( pIndices );
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
void 
Phoenix::Data::CMilkshapeLoader::CreateTriangleList( vector<CVertex> &vecVertices,
						     vector<unsigned int> &vecIndices)
{
  vecVertices.clear();
  vecIndices.clear();
  for(unsigned int nT=0; nT<m_nNumTriangles; nT++)
  {
    struct MS3D_Triangle_t triangle;
    triangle = m_pTriangles[nT];
    

    for( unsigned int nV=0;nV<3;nV++)
    {
      CVertex vertex;    
      CREATE_VERTEX( vertex, triangle, nV );

      // Check does the vertex exist in the list already
      bool bFoundVertex = false;
      unsigned int nIndex = 0;
      for( ; nIndex < vecVertices.size(); nIndex++ )
      {
	if ( vecVertices[nIndex] == vertex ) 
	{
	  bFoundVertex = true;
	  break;
	} 
      }
      
      if ( bFoundVertex )
      {
	vecIndices.push_back( nIndex );
      }
      else
      {
	vecVertices.push_back( vertex );
	vecIndices.push_back( vecVertices.size() - 1 );
      }
    } // for each vertex in triangle
  } // for each triangle
}  // separatevertices
/////////////////////////////////////////////////////////////////
void
Phoenix::Data::CMilkshapeLoader::Stripify()
{
#ifndef WIN32
  triangle_stripper::indices triangleIndices;
  CIndexArray *pIndices = NULL;
  
  if ( GetIndices().size() == 1 )
  {
    pIndices = GetIndices()[0];
    if ( pIndices == NULL ) return;

  }
  else
  {
    cerr << "Too many index lists for stripification! I can use only one!" << endl;
    return;
  }

  for(unsigned int i=0;i<pIndices->GetNumIndices();i++)
  {
    if ( pIndices->IsShortIndices())
    {
      triangleIndices.push_back(pIndices->GetPointer<unsigned short int>()[i]);
    }
    else
    {
      triangleIndices.push_back(pIndices->GetPointer<unsigned int>()[i]);
    }
  }
  triangle_stripper::primitive_vector primitiveVector;
  triangle_stripper::tri_stripper triStripper( triangleIndices );
  
  triStripper.SetMinStripSize(2);
  triStripper.SetCacheSize(0);
  triStripper.SetBackwardSearch(true);
  triStripper.Strip( &primitiveVector );
  
  unsigned int nStripCount = 0;
  unsigned int nListCount = 0;
  unsigned int nListLength = 0;
  unsigned int nStripAvgLength = 0;
  unsigned int nStripTriangleCount = 0;

  for( unsigned int i=0;i<primitiveVector.size();i++)
  {
    if( primitiveVector[i].Type == triangle_stripper::TRIANGLE_STRIP)
    {
      nStripCount++;
      nStripAvgLength += primitiveVector[i].Indices.size();
      nStripTriangleCount += primitiveVector[i].Indices.size()-2;
    }
    else
    {
      nListCount++;
      nListLength += primitiveVector[i].Indices.size();

    }
  }
  nStripAvgLength /= nStripCount;
  cerr << "Created " << nStripCount << " strips. Average length " << nStripAvgLength << ", in total " << nStripTriangleCount << " triangles drawn as stripped." << endl;
  cerr << "Created " << nListCount << " lists. " << nListLength / 3  << " triangles drawn unstripped." << endl;

  // Remove original triangle list.
  delete GetIndices()[0];
  GetIndices().clear();
  pIndices = NULL;
  
  // for each batch of primitives
  for( unsigned int i=0;i<primitiveVector.size();i++)
  {
    // Check which type of primitive we got
    if ( primitiveVector[i].Type == triangle_stripper::TRIANGLE_STRIP )
    {
      pIndices = new CIndexArray(PRIMITIVE_TRI_STRIP, primitiveVector[i].Indices.size());
    }
    else
    {
      pIndices = new CIndexArray(PRIMITIVE_TRI_LIST, primitiveVector[i].Indices.size());
    }
    // Copy indices into indexarray
    for(unsigned int p=0;p<primitiveVector[i].Indices.size();p++)
    {
      if (pIndices->IsShortIndices())
      {
	pIndices->GetPointer<unsigned short int>()[p] = primitiveVector[i].Indices[p];
      }
      else
      {
	pIndices->GetPointer<unsigned int>()[p] = primitiveVector[i].Indices[p];
      }
    }
    // Put index array into vector
    GetIndices().push_back( pIndices );
  }
  #endif
}
/////////////////////////////////////////////////////////////////
