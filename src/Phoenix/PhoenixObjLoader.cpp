#include "PhoenixObjLoader.h"
#include "PhoenixSpatial.h"
#include <fstream>
#include <string.h>
#include <string>
#include <exception>
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::fstream;
using std::ios;
using std::exception;
using std::cerr;
using std::endl;
using std::ios_base;
using std::string;
using namespace Phoenix::Data;
using namespace Phoenix::Spatial;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
#define DELETE(OBJ) if(OBJ != NULL ) delete OBJ; OBJ=NULL;
/////////////////////////////////////////////////////////////////
struct printIt_t
{
  void operator()( ObjVertex & v )
  {
    cerr << "v "
	 << v.x << " "
	 << v.y << " "
	 << v.z << endl;
  }

  void operator()( ObjNormal & v )
  {
    cerr << "vn "
	 << v.x << " "
	 << v.y << " "
	 << v.z << endl;
  }
  
  void operator()( ObjTexCoord & tex )
  {
    cerr << "vt "
	 << tex.u << " "
	 << tex.v << endl;
  }

  void operator()( ObjFace & face )
  {
    cerr << "f ";
    cerr << face.v1 << "/" ;
    if ( face.t1 > -1 ) cerr << face.t1 ;
    cerr << "/";
    if ( face.n1 > -1) cerr << face.n1;
    cerr << " ";

    cerr << face.v2 << "/" ;
    if ( face.t2 > -1 ) cerr << face.t2 ;
    cerr << "/";
    if ( face.n2 > -1) cerr << face.n2;
    cerr << " ";

    cerr << face.v3 << "/" ;
    if ( face.t3 > -1 ) cerr << face.t3 ;
    cerr << "/";
    if ( face.n3 > -1) cerr << face.n3;

    cerr << endl;  
  }
} printIt;
/////////////////////////////////////////////////////////////////
class CObjException: public exception
{
private:
  const char *m_Msg;
  size_t      m_line;
public:
  CObjException( const char *msg, size_t line ) : m_Msg(msg), m_line(line) {}
  virtual const char* what() const throw()
  {
    return m_Msg;
  }
  size_t GetLine() const 
  {
    return m_line;
  }
};
/////////////////////////////////////////////////////////////////
class CParseException: public CObjException
{
public:
  CParseException( const char *msg, size_t line ) : CObjException( msg,line )  {}
};
/////////////////////////////////////////////////////////////////
class CBadFormatException: public CObjException
{
public:
  CBadFormatException( const char *msg, size_t line ) : CObjException( msg,line )  {}
};
/////////////////////////////////////////////////////////////////
const char *
FindSeparator( const char *szStr )
{
  const char *ptr = szStr;
  if ( szStr == NULL ) return NULL;
  while( ptr[0] != '\t' && 
	 ptr[0] != ' '  &&
	 ptr[0] != '\0' )
  {
    ++ptr;
  }
  return ptr;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CObjLoader::ParsePosition( const char *szLine )
{
  ObjVertex vert;
  
  char buf[256];

  const char *begin = szLine+2;
  const char *end = FindSeparator(begin);

  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No vertex coords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  vert.x = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = FindSeparator(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one vertex coord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  vert.y = strtof(buf, (char **)NULL);

  // parse for z-coord
  begin = end+1;
  end = FindSeparator(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only two vertex coords available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  vert.z = strtof(buf, (char **)NULL);

  // insert vertex into vertex vector
  m_Vertices.push_back( vert );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CObjLoader::ParseNormal( const char *szLine )
{
  ObjNormal normal;
  
  char buf[256];

  const char *begin = szLine+2;
  const char *end = FindSeparator(begin);

  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No normal coords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  normal.x = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = FindSeparator(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one normal coord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  normal.y = strtof(buf, (char **)NULL);

  // parse for z-coord
  begin = end+1;
  end = FindSeparator(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only two normal coords available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  normal.z = strtof(buf, (char **)NULL);

  // insert normal into normal vector
  m_Normals.push_back( normal );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CObjLoader::ParseTexCoord( const char *szLine )
{
  ObjTexCoord texc;
  
  char buf[256];

  const char *begin = szLine+2;
  const char *end = FindSeparator(begin);
  
  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No texcoords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  texc.u = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = FindSeparator(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one texcoord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  texc.v = strtof(buf, (char **)NULL);

  // insert normal into normal vector
  m_TexCoords.push_back( texc );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Data::CObjLoader::ParseGroups( const char *szLine )
{
  m_currGroups.clear();
  char buf[256];

  const char *begin = szLine+2;
  const char *end = FindSeparator(begin);

  // there can be several groups
  while ( end[0] != '\0' )
  {
    strncpy( buf, begin, 256);
    buf[end-begin]='\0';
    if ( strlen( buf) > 0 )
    {
      m_currGroups.push_back( std::string(buf) );  
      m_mapGroupFaces[ std::string(buf) ];
    }
    // parse for y-coord
    begin = end+1;
    end = FindSeparator(begin);
  }
  // copy last group, if such exists.
  strncpy( buf, begin, 256);
  buf[end-begin]='\0';
  if ( strlen( buf) > 0 )
  {
    m_currGroups.push_back( std::string(buf) );  
    m_mapGroupFaces[std::string(buf)];
  }
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Data::CObjLoader::ParseFace( const char *szLine )
{
  ObjFace face;
  char buf[256];

  const char *begin = szLine+2;
  const char *end = FindSeparator(begin);
  bool hasMore = true;
  
  for( int i=0; i<3 && hasMore;i++)
  {
    if ( end[0] == '\0' )  
    {
      hasMore = false;
    }
    // copy current def block to buf and terminate string
    strncpy( buf, begin, end-begin);
    buf[end-begin]='\0';
    // check total length of buf.
    size_t bufSize = strlen(buf);

    // replace slashes with \0
    char *faceLine = buf;
    while ( (faceLine = index(faceLine, '/')) != NULL ) 
    {
      faceLine[0] = '\0';
      faceLine++;
    }
  
    // parse vertex index
    faceLine = buf;

    switch( i )
    {
    case 0:
      face.v1 = strtol(faceLine, (char **)NULL, 10) - 1;  
      break;
    case 1: 
      face.v2 = strtol(faceLine, (char **)NULL, 10) - 1;  
      break;
    case 2: 
      face.v3 = strtol(faceLine, (char **)NULL, 10) - 1;  
      break;
    }
    // parse texcoord
    faceLine+=(strlen(faceLine)+1);
    if ( faceLine < buf+bufSize )
    {
      // texcoord is optional
      if ( strlen( faceLine) > 0 )
      {
	switch( i )
	{
	case 0:
	  face.t1 = strtol(faceLine, (char **)NULL, 10) - 1;  
	  break;
	case 1: 
	  face.t2 = strtol(faceLine, (char **)NULL, 10) - 1;  
	  break;
	case 2: 
	  face.t3 = strtol(faceLine, (char **)NULL, 10) - 1;  
	  break;
	}

      }
      // parse normal
      faceLine+=(strlen(faceLine)+1);
      if ( faceLine < buf+bufSize )
      {
	// normal is optional
	if ( strlen(faceLine) > 0 )
	{
	  switch( i )
	  {
	  case 0:
	    face.n1 = strtol(faceLine, (char **)NULL, 10) - 1;  
	    break;
	  case 1: 
	    face.n2 = strtol(faceLine, (char **)NULL, 10) - 1;  
	    break;
	  case 2: 
	    face.n3 = strtol(faceLine, (char **)NULL, 10) - 1;  
	    break;
	  }
	}
      } 
    } 
    
    // parse next face vertex definition
    begin = (end+1);
    end = FindSeparator( begin );
  } 

  m_Faces.push_back(face);
  
  // insert face pointer into 
  GroupList::iterator it = m_currGroups.begin();
  GroupFaces::iterator faceListIt;
  for ( ; it != m_currGroups.end(); it++)
  {
    m_mapGroupFaces[(*it)].push_back( &m_Faces.back() );
  }
  
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Data::CObjLoader::Load( const char *szFilename )
{
  
  fstream	fFile;
  int iRetval = 0;
  size_t nFilesize = 0;
  m_currLine = 0;

  fFile.open( szFilename, ios::in );
  
  if ( !fFile )
  {
    cerr << "Couldn't open " << szFilename << std::endl;
    return 1;
  }
  

  // move to end of file
  fFile.seekg(0, ios_base::end);
  nFilesize = fFile.tellg();
  fFile.seekg(0, ios_base::beg);
  
  char *pBuffer = new char[nFilesize];
  fFile.read( pBuffer, nFilesize);
  fFile.close();

  // replace line breaks with \0
  char *line = pBuffer;
  while ( (line = index(line, '\n')) != NULL ) 
  {
    line[0] = '\0';
    line++;
  }
  
  line = pBuffer;
  size_t nChars = 0;

  try 
  {
    while( nChars < nFilesize )
    {
      if      ( strstr( line, "v "  ) == line ) ParsePosition( line );     
      else if ( strstr( line, "vt " ) == line )	ParseTexCoord( line );
      else if ( strstr( line, "vn " ) == line ) ParseNormal( line );
      else if ( strstr( line, "f "  ) == line )	ParseFace( line );
      else if ( strstr( line, "g "  ) == line )	ParseGroups( line );
      // proceed to next line
      size_t strsize = (strlen(line) + 1);
      line += strsize;
      nChars+=strsize;
      ++m_currLine;
    }
  } 
  catch ( CBadFormatException & ex )
  {
    cerr << ex.what() << " at " << ex.GetLine() <<  endl;
  }  

#ifdef DEBUG
  cerr << "#vert: " << m_Vertices.size() << endl;
  cerr << "#norm: " << m_Normals.size() << endl;
  cerr << "#texc: " << m_TexCoords.size() << endl;
  cerr << "#indices: " << m_Faces.size() << endl;
  for_each( m_Vertices.begin(), m_Vertices.end(), printIt);
  for_each( m_Normals.begin(), m_Normals.end(), printIt);
  for_each( m_TexCoords.begin(), m_TexCoords.end(), printIt);
  for_each( m_Faces.begin(), m_Faces.end(), printIt);
#endif
  GenerateModelData();

  delete pBuffer;
  return iRetval;
}
/////////////////////////////////////////////////////////////////
struct ObjFaceInd
{
  ObjFace *ptr;
  int index[3];
};
/////////////////////////////////////////////////////////////////
void
SelectTexCoordAndNormalIndex( int currVertIndex, const ObjFace & f, int & texcInd, int & normInd, int & whichVertex )
{
  /// check which vertex index we must handle
  if ( f.v1  == currVertIndex )  
  {
    texcInd = f.t1;
    normInd = f.n1;
    whichVertex = 0;
  }
  else if ( f.v2  == currVertIndex ) 
  {
    texcInd = f.t2;
    normInd = f.n2;
    whichVertex = 1;
  }
  else 
  {
    texcInd = f.t3;
    normInd = f.n3;
    whichVertex = 2;
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Data::CObjLoader::GenerateModelData()
{
  vector<CVertex> vecVertices;
  vector<size_t> vecIndices;

  // group triangles by shared vertices
  vector< ObjFaceInd > * allFaces = new vector< ObjFaceInd >[m_Vertices.size()];

  for( size_t face=0;face<m_Faces.size();++face)
  {
    {
      ObjFaceInd fi;
      fi.ptr = &m_Faces[face];
      fi.index[0] = -1;
      fi.index[1] = -1;
      fi.index[2] = -1;
      
      allFaces[ m_Faces[face].v1].push_back( fi );
    }
    {
      ObjFaceInd fi;
      fi.ptr = &m_Faces[face];
      fi.index[0] = -1;
      fi.index[1] = -1;
      fi.index[2] = -1;
      
      allFaces[ m_Faces[face].v2].push_back( fi );
    }
    {
      ObjFaceInd fi;
      fi.ptr = &m_Faces[face];
      fi.index[0] = -1;
      fi.index[1] = -1;
      fi.index[2] = -1;
      
      allFaces[ m_Faces[face].v3].push_back( fi );
    }
  }
  
  // insert new vertices and store indices into ObjFaceInd structs.
  for( size_t v=0;v< m_Vertices.size(); ++v)
  {
    vector< ObjFaceInd > & faces = allFaces[v];
#ifdef DEBUG
    cerr << "#Faces in " << v << " : " <<  faces.size() << endl;
#endif
    for( size_t f=0; f<faces.size(); ++f)
    {
      bool foundExisting = false;
      int  texcInd = 0;
      int  normInd = 0;
      int  whichVertex;
      SelectTexCoordAndNormalIndex( v, *faces[f].ptr, texcInd, normInd, whichVertex );
      
      // go through all faces before current
      for( size_t p=0;p<f;++p)
      {
	int texcIndPrev = 0;
	int normIndPrev = 0;
	int whichVertexPrev;
	// another vertex might have _different_ index...
	SelectTexCoordAndNormalIndex( v, *faces[p].ptr, texcIndPrev, normIndPrev, whichVertexPrev );
	
	if ( texcInd == texcIndPrev && normInd == normIndPrev )
	{
	  foundExisting = true;
	  faces[f].index[whichVertex] = faces[p].index[whichVertexPrev];
	  break;
	}
      } // for( size_t p
      
      if ( !foundExisting )
      {
	  
	CVertex vertex;
	int vertIndex = 0;
	int normIndex = 0;
	int texcIndex = 0;
	  
	switch ( whichVertex )
	{
	case 0:
	  vertIndex = faces[f].ptr->v1;
	  normIndex = faces[f].ptr->n1;
	  texcIndex = faces[f].ptr->t1;
	  break;
	case 1:
	  vertIndex = faces[f].ptr->v2;
	  normIndex = faces[f].ptr->n2;
	  texcIndex = faces[f].ptr->t2;
	  break;
	case 2:
	  vertIndex = faces[f].ptr->v3;
	  normIndex = faces[f].ptr->n3;
	  texcIndex = faces[f].ptr->t3;
	  break;
	default:
	  assert( NULL && "Wrong whichVertex? This is impossible" );
	  break;
	}
	vertex.SetPosition( m_Vertices[vertIndex].x,
			    m_Vertices[vertIndex].y,
			    m_Vertices[vertIndex].z );
	// if normal index is defined
	if ( normIndex > -1 )
	{
	  vertex.SetNormal( m_Normals[normIndex].x,
			    m_Normals[normIndex].y,
			    m_Normals[normIndex].z );
	}
	// if texcoord index is defined
	if ( texcIndex > -1 )
	{
	  vertex.SetTextureCoordinates( m_TexCoords[texcIndex].u,
					m_TexCoords[texcIndex].v );
	}
	// insert vertex into array
	vecVertices.push_back( vertex );
	faces[f].index[whichVertex] = vecVertices.size()-1;
      }
    } // for( size_t f=
  } //  for( size_t v=0;

  // new vertices and indices have been created, so 
  // copy index data from ObjFaceInd to actual faces, so it can be 
  // used to create triangle index list
  for( size_t v=0; v< m_Vertices.size(); ++v)
  {
    vector< ObjFaceInd > & faces = allFaces[v];

    for( size_t f=0;f< faces.size(); ++f)
    {
      // copy index position only if set (not -1)
      if ( faces[f].index[0] > -1 )
      {
	assert( faces[f].index[0] > -1 && (size_t)faces[f].index[0] < vecVertices.size());
	faces[f].ptr->v1 = faces[f].index[0];
      } 
      
      if ( faces[f].index[1] > -1)
      {
	assert( faces[f].index[1] > -1 && (size_t)faces[f].index[1] < vecVertices.size());
	faces[f].ptr->v2 = faces[f].index[1];
      }

      if ( faces[f].index[2] > -1)
      {
	assert( faces[f].index[2] > -1 && (size_t)faces[f].index[2] < vecVertices.size());
	faces[f].ptr->v3 = faces[f].index[2];
      }
    }
  }
  
  // go through each faces and create a list.
  for( size_t f=0;f<m_Faces.size();f++)
  {
    ObjFace & face = m_Faces[f];
#ifdef DEBUG
    cerr << "v1: " << face.v1 << endl;
    cerr << "v2: " << face.v2 << endl;
    cerr << "v3: " << face.v3 << endl;
    cerr << "vertices total: " << vecVertices.size() << endl;
#endif
    assert( face.v1 < (int)vecVertices.size());
    assert( face.v2 < (int)vecVertices.size());
    assert( face.v3 < (int)vecVertices.size());

    vecIndices.push_back( face.v1 );
    vecIndices.push_back( face.v2 );
    vecIndices.push_back( face.v2 );
  }

  delete [] allFaces;

  DELETE(m_pPositions);
  DELETE(m_pNormals);
  DELETE(m_pTexCoords);
  DELETE(m_pIndices );

  // Create new descriptors / array
  m_pPositions = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, vecVertices.size());
  m_pNormals   = new CVertexDescriptor(ELEMENT_TYPE_NORMAL_3F, vecVertices.size());
  m_pTexCoords = new CVertexDescriptor(ELEMENT_TYPE_TEX_2F, vecVertices.size());
  m_pIndices   = new CIndexArray(PRIMITIVE_TRI_LIST, vecIndices.size());
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


  // Create indices for groups
  GroupFaces::iterator it = m_mapGroupFaces.begin();
  for( ; it != m_mapGroupFaces.end(); it++)
  {

    // Create index array; each triangle has three vertices
    CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, (it->second).size()*3);

    std::list<ObjFace *> & faceList = it->second;
    std::list< ObjFace *>::iterator facePtrIt = faceList.begin();

    size_t nIndex = 0;    
    // copy indices into array
    for( ; facePtrIt != faceList.end(); facePtrIt++ )
    {
      if ( pIndices->IsShortIndices())
      {
	pIndices->GetPointer<unsigned short int>()[nIndex++] = (*facePtrIt)->v1;
	pIndices->GetPointer<unsigned short int>()[nIndex++] = (*facePtrIt)->v2;
	pIndices->GetPointer<unsigned short int>()[nIndex++] = (*facePtrIt)->v3;
      }
    }
    
    // insert index array into map
    m_mapGroups[it->first] = pIndices;
    
  }
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetVertexArray() const
{
  return new CVertexDescriptor( *m_pPositions);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetNormalArray() const
{
  return new CVertexDescriptor( *m_pNormals);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetTexCoordArray( size_t nTexUnit ) const
{
  return new CVertexDescriptor( *m_pTexCoords);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetColorArray() const
{
  return new CVertexDescriptor( *m_pColors);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CIndexArray * 
Phoenix::Data::CObjLoader::GetIndexArray( const char *szGroupName  ) const
{
  CIndexArray *pIndexArray = NULL;
  if ( szGroupName == NULL )
  {
    pIndexArray = new CIndexArray( *m_pIndices );
  }
  else
  {
    std::map<std::string, Phoenix::Graphics::CIndexArray *>::const_iterator it;
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
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType ) const
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

