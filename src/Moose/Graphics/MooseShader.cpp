#include "MooseShader.h"
#include "MooseGL.h"
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
/////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
inline int
LoadFile( const char * szFilename, std::string &sContents )
{

  // Temporary container for line
  string sLine;
  // The file stream
  ifstream fsFile;
  // open file
  fsFile.open( szFilename, ios::in );
  // test for failure
  if ( fsFile.is_open() )
  {
    // read line by line
    while( ! fsFile.eof() )
    {
      getline( fsFile, sLine );
      sContents.append( sLine );
      sContents.append( "\n" );
    }
    // close the file
    fsFile.close();
  }
  else
  {
    std::cerr << "The file " << szFilename
	      << " couldn't be opened."  << std::endl;
    return 1;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShader::CShader( ) 
{
  m_nShaderProgram  = glCreateProgram();
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShader::~CShader()
{
  // detach shaders from program.
  
  while( !m_lstVertexShaders.empty())
  {
    glDetachShader( GetProgram(), m_lstVertexShaders.front() );
    glDeleteShader( m_lstVertexShaders.front() );
    m_lstVertexShaders.pop_front();
  }
  while ( ! m_lstFragmentShaders.empty())
  {
    glDetachShader( GetProgram(), m_lstFragmentShaders.front() );
    glDeleteShader( m_lstFragmentShaders.front() );
    m_lstFragmentShaders.pop_front();
  }
  // delete program
  glDeleteProgram( GetProgram() );
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CShader::AttachVertexShader(GLuint nVertexShader ) 
{
  m_lstVertexShaders.push_back(nVertexShader);
  glAttachShader( GetProgram(), nVertexShader);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::AttachFragmentShader(GLuint nFragmentShader ) 
{
  m_lstFragmentShaders.push_back(nFragmentShader);
  glAttachShader( GetProgram(), nFragmentShader);
}
/////////////////////////////////////////////////////////////////
#define INFO_LOG_BUF_SIZE 1024
/////////////////////////////////////////////////////////////////
/// Parses shader/program info log into given string.
/// \param nObject From which shader/program log is retrieved.
/// \param strInfoLog String where log is stored.
/// \param bProgram True for retrieving program info log, False for shader info log
inline void
GetInfoLog( unsigned int nObject, std::string &strInfoLog, int bProgram )
{
  int iInfologLength   = 0;      // total length of log
  int iCharsRetrievedBatch  = 0; // how many chars were retrived from log in current pass
  int iCharsRetrievedTotal = 0;  // how many chars in total is retrieved.
  char sInfoLog[INFO_LOG_BUF_SIZE];

  // Get length of the info log
  if ( bProgram )      glGetProgramiv(nObject, GL_INFO_LOG_LENGTH, &iInfologLength);
  else		       glGetShaderiv(nObject, GL_INFO_LOG_LENGTH, &iInfologLength);

  // if info log exists, we parse it
  if (iInfologLength > 0)
  {
    strInfoLog.clear(); // clear existing string
    strInfoLog.reserve(iInfologLength); // allocate sufficient memory

    // read chars up to info log length
    while ( iCharsRetrievedTotal < iInfologLength )
    {
      if ( bProgram ) glGetProgramInfoLog(nObject, INFO_LOG_BUF_SIZE, &iCharsRetrievedBatch, sInfoLog);
      else            glGetShaderInfoLog(nObject, INFO_LOG_BUF_SIZE, &iCharsRetrievedBatch, sInfoLog);

      if ( iCharsRetrievedBatch > 0 )
      {
	iCharsRetrievedTotal += iCharsRetrievedBatch; // increase number of read chars
	strInfoLog.append(sInfoLog, iCharsRetrievedBatch); // append data to string
      }
    } // end while
    // ok, done
  }
}
/////////////////////////////////////////////////////////////////
/// Wrapper for retrieving shader info log.
/// \param nShader From which shader log is retrieved.
/// \param strInfoLog String where log is stored.
inline void
GetShaderInfoLog( unsigned int nShader, std::string &strInfoLog )
{
  GetInfoLog( nShader, strInfoLog, 0 );
}
/////////////////////////////////////////////////////////////////
/// Wrapper for retrieving program info log.
/// \param nProg From which program log is retrieved.
/// \param strInfoLog String where log is stored.
inline void
GetProgramInfoLog( unsigned int nProg, std::string &strInfoLog )
{
  GetInfoLog( nProg, strInfoLog, 1 );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::LoadVertexShader( const std::string & strVertexShader )
{
  return LoadVertexShader(strVertexShader.c_str());
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::LoadFragmentShader( const std::string & strFragmentShader )
{
  return LoadFragmentShader(strFragmentShader.c_str());
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::LoadVertexShader( const char * szVertexShader)
{

  string strVSSource;
  ////////////////////
  // Vertex shader loading
  if ( szVertexShader != NULL && strlen(szVertexShader) > 0 )
  {
    if ( LoadFile( szVertexShader, strVSSource ))
    {
      std::cerr << "Failed to load vertex shader '" << szVertexShader << "'" << std::endl;
    }

  }
  return CreateVertexShaderFromSource( strVSSource.size() > 0 ? strVSSource.c_str() : NULL,szVertexShader);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::LoadFragmentShader( const char * szFragmentShader)
{
  string strFSSource;
  ////////////////////
  // Fragment shader loading
  if ( szFragmentShader != NULL && strlen(szFragmentShader) > 0 )
  {
    if ( LoadFile( szFragmentShader, strFSSource ))
    {
      std::cerr << "Failed to load fragment shader '" << szFragmentShader << "'" << std::endl;
    }
  }
  return CreateFragmentShaderFromSource( strFSSource.size() > 0 ? strFSSource.c_str() : NULL, szFragmentShader );

}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::CreateVertexShaderFromSource( const char * szVertexShaderCode, const char * szVSname )
{

  int iState = 0; // compile status

  ////////////////////
  // Vertex shader loading
  if ( szVertexShaderCode != NULL && strlen(szVertexShaderCode) > 0 )
  {

    GLuint nVertexShader = glCreateShader( GL_VERTEX_SHADER );
    int nLength = strlen(szVertexShaderCode); // source code length
    const char *pStrCode = szVertexShaderCode;

    // compile source
    glShaderSource(nVertexShader,1, &pStrCode, &nLength );
    glCompileShader( nVertexShader );
    // get compile status
    glGetShaderiv( nVertexShader, GL_COMPILE_STATUS, &iState);
    if ( iState == GL_TRUE )
    {
      // compiling went ok
      AttachVertexShader( nVertexShader );
    }
    else
    {
      string strLog;
      GetShaderInfoLog( nVertexShader, strLog );
      std::cerr << "Error in vertex shader " << (szVSname == NULL  ? "" : szVSname) << " : " << strLog << std::endl;
    }

  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::CreateFragmentShaderFromSource( const char * szFragmentShaderCode, const char * szFSname)
{
  int iState = 0;
  ////////////////////
  // Fragment shader loading
  if ( szFragmentShaderCode != NULL && strlen(szFragmentShaderCode) > 0 )
  {

    GLuint nFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    int nLength = strlen(szFragmentShaderCode); // source code length
    const char *pStrCode = szFragmentShaderCode;

    glShaderSource(nFragmentShader,1, &pStrCode, &nLength );
    glCompileShader( nFragmentShader );

    // get compile status
    glGetShaderiv( nFragmentShader, GL_COMPILE_STATUS, &iState);
    if ( iState == GL_TRUE )
    {
      // compiling went ok
      AttachFragmentShader( nFragmentShader );
    }
    else
    {
      string strLog;
      GetShaderInfoLog( nFragmentShader, strLog );
      std::cerr << "Error in fragment shader " << (szFSname == NULL ? "" : szFSname) << strLog << std::endl;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CShader::Link()
{
  int iState;
  // link shader program
  glLinkProgram(GetProgram());
  glGetProgramiv(GetProgram(), GL_LINK_STATUS, &iState);

  // show linking errors, if any
  if ( iState == GL_FALSE )
  {
    string strLog;
    GetProgramInfoLog(GetProgram(), strLog);
    std::cerr << strLog << std::endl;
    
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CShader::Validate()
{
    GLint iStatus;
    std::string strInfoLog;
    glValidateProgram(GetProgram());
    GetProgramInfoLog( GetProgram(), strInfoLog);
    if (!strInfoLog.empty())
    {
        std::clog << "Program validate log: " << strInfoLog;
    }
    
    glGetProgramiv(GetProgram(), GL_VALIDATE_STATUS, &iStatus);
    return (iStatus != 0);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CShader::Apply()
{
    glUseProgram( GetProgram());
}
/////////////////////////////////////////////////////////////////
