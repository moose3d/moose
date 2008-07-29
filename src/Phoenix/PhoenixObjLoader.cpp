#include "PhoenixObjLoader.h"
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
SkipWhiteSpace( const char *szStr )
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
  const char *end = SkipWhiteSpace(begin);

  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No vertex coords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  vert.x = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = SkipWhiteSpace(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one vertex coord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  vert.y = strtof(buf, (char **)NULL);

  // parse for z-coord
  begin = end+1;
  end = SkipWhiteSpace(begin);
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
  const char *end = SkipWhiteSpace(begin);

  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No normal coords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  normal.x = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = SkipWhiteSpace(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one normal coord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  normal.y = strtof(buf, (char **)NULL);

  // parse for z-coord
  begin = end+1;
  end = SkipWhiteSpace(begin);
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
  const char *end = SkipWhiteSpace(begin);
  
  // parse for x-coord.
  if ( end[0] == '\0' ) throw CBadFormatException("No texcoords available.", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  texc.u = strtof(buf, (char **)NULL);
  
  // parse for y-coord
  begin = end+1;
  end = SkipWhiteSpace(begin);
  if ( begin[0] == '\0' ) throw CBadFormatException("Only one texcoord available", m_currLine);
  strncpy( buf, begin, end-begin);
  buf[end-begin]='\0';
  texc.v = strtof(buf, (char **)NULL);
  
  

  // insert normal into normal vector
  m_TexCoords.push_back( texc );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Data::CObjLoader::ParseFace( const char *szLine )
{
    
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
      if ( strstr( line, "v " ) == line )
      {
 	ParsePosition( line );
      }
      else if ( strstr( line, "vt ") == line )
      {
	ParseTexCoord( line );
      }
      else if ( strstr( line, "vn " ) == line )
      {
	ParseNormal( line );
      }
      else if ( strstr( line, "f ") == line )
      {
	ParseFace( line );
      }
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
  cerr << "#vert: " << m_Vertices.size() << endl;
  cerr << "#norm: " << m_Normals.size() << endl;
  cerr << "#texc: " << m_TexCoords.size() << endl;
  delete pBuffer;
  return iRetval;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetTexCoordArray( size_t nTexUnit )
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetColorArray()
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetNormalArray()
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetVertexArray()
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CVertexDescriptor * 
Phoenix::Data::CObjLoader::GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType)
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CIndexArray *	     
Phoenix::Data::CObjLoader::GetIndices()
{
  return NULL;
}
/////////////////////////////////////////////////////////////////
