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

#include <string>
#include <fstream>
#include "GSE_Logger.h"
#include "GSE_OglShaders.h"
#include "GSE_OglUtils.h"
using std::string;
using std::ifstream;
using std::ios;
using Core::GSE_Logger;




// ----------------------------------------------------------------------------
// Name:	LoadFile
// Desc:        Loads a text file pointed by path pSourceFile
// History:	12.4.2006 Initial version AG
// Param:	char *pSourceFile  - path to the text file 
// Returns:	std::string containing the text in the pSourceFile. 
// ----------------------------------------------------------------------------
string
LoadFile(char *pSourceFile )
{
  // The string where file text is stored 
  string sContents;
  // Temporary container for line 
  string sLine;
  // The file stream 
  ifstream fsFile;

  if ( pSourceFile != NULL ) {

    // open file 
    fsFile.open( pSourceFile );
    // test for failure 
    if ( fsFile.is_open(), ios::in) {
      
      // read line by line 
      while( ! fsFile.eof() )
	{
	  getline( fsFile, sLine );
	  sContents.append( sLine );
	}
      // close the file 
      fsFile.close();
      
    } else {
      GSE_Logger::Error() << "The file " 
			  << pSourceFile 
			  << " couldn't be opened."  << std::endl;
    }

  }

  return sContents;

}  
// ----------------------------------------------------------------------------


GLuint
GSE_OglShaderBase::GetShaderId()
{
  return m_iShader;
}

void
GSE_OglShaderBase::SetShaderId( GLuint iShader)
{
  m_iShader = iShader;
}

// ----------------------------------------------------------------------------
GSE_OglVertexShader::GSE_OglVertexShader( char *pSourceFile ) : 
					GSE_VertexShader( pSourceFile ) 
{
  m_iShader = glCreateShader( GL_VERTEX_SHADER );
}

GSE_OglVertexShader::~GSE_OglVertexShader()
{
  glDeleteShader( m_iShader );
}
int
GSE_OglVertexShader::InitShader()
{
  string sSource = LoadFile ( m_pFilename );
  // glShaderSource accepts only const char *
  const char *pCString = sSource.c_str();
  
  if ( sSource.length() == 0 ){
    GSE_Logger::Error() << "No such file " 
			<<  m_pFilename 
			<< " or file empty." << std::endl;
    return 1;
  }

  glShaderSource(m_iShader, 1, &pCString, NULL);
  glCompileShader(m_iShader);
  
  if ( GSE_OglUtils::IsGLSLCompileErrors(m_iShader)){
    GSE_OglUtils::ShowGLSLShaderOutput( m_iShader );
    return 1;
  } else {
    GSE_Logger::Error() << "VertexShader " 
			<< m_pFilename 
			<< " loaded and compiled." << std::endl;
  }

  return 0;
}

// ----------------------------------------------------------------------------

GSE_OglFragmentShader::GSE_OglFragmentShader( char *pSourceFile ) : 
					GSE_FragmentShader( pSourceFile ) 
{
  m_iShader = glCreateShader( GL_FRAGMENT_SHADER );
  
}

GSE_OglFragmentShader::~GSE_OglFragmentShader() 
{
  glDeleteShader( m_iShader );
}
int
GSE_OglFragmentShader::InitShader()
{
  string sSource = LoadFile ( m_pFilename );
  // glShaderSource accepts only const char *
  const char *pCString = sSource.c_str();
  if ( sSource.length() == 0 ){
    GSE_Logger::Error() << "No such file " 
			<<  m_pFilename 
			<< " or file empty." << std::endl;
    return 1;
  }
  glShaderSource(m_iShader, 1, &pCString, NULL);
  glCompileShader(m_iShader);
  

  if ( GSE_OglUtils::IsGLSLCompileErrors( m_iShader)){
    GSE_OglUtils::ShowGLSLShaderOutput( m_iShader );
    return 1;
  } else {
    GSE_Logger::Error() << "FragmentShader " 
			<< m_pFilename 
			<< " loaded and compiled." << std::endl;
  }

  return 0;
}


// ----------------------------------------------------------------------------

GSE_OglShaderProgram::GSE_OglShaderProgram(char *pShaderName) : 
				GSE_ShaderProgram(pShaderName)
{ 
  m_iProgram = glCreateProgram();
  if ( m_iProgram == 0 ){
    GSE_Logger::Error() << "Couldn't create program!" << std::endl;
  }
}
GSE_OglShaderProgram::~GSE_OglShaderProgram()
{
  // detach every vertexshader
  for( unsigned int iVs=0;iVs<m_vVertexShaders.size();iVs++){
    glDetachShader( m_iProgram, 
		    ((GSE_OglVertexShader *)
		     m_vVertexShaders[iVs])->GetShaderId());
  }
  // clear vector
  m_vVertexShaders.clear();

}
int 
GSE_OglShaderProgram::AddShader( GSE_VertexShader *pShader )
{
  // failsafe
  if ( pShader == NULL ) return -1;
  
  m_vVertexShaders.push_back(pShader);
  /*glAttachShader( m_iProgram, 
		  ((GSE_OglVertexShader *)pShader)->GetShaderId());
  */
  return 0;
}
  
int 
GSE_OglShaderProgram::AddShader( GSE_FragmentShader *pShader ) 
{
  // failsafe
  if ( pShader == NULL ) return -1;
  
  m_vFragmentShaders.push_back(pShader);
  /*glAttachShader( m_iProgram, 
		  ((GSE_OglFragmentShader *)pShader)->GetShaderId());
  */
  return 0;
}

int 
GSE_OglShaderProgram::RemoveShader( GSE_FragmentShader *pShader )
{
  int iRetval = 1;
  vector<GSE_FragmentShader *>::iterator itNeedle;
  itNeedle = find( m_vFragmentShaders.begin(),
		   m_vFragmentShaders.end(),
		   pShader);
  // if shader is found, detach and remove from vector
  if ( itNeedle != m_vFragmentShaders.end()){
    glDetachShader( m_iProgram, 
		    ((GSE_OglFragmentShader *)(*itNeedle))->GetShaderId());
    m_vFragmentShaders.erase(itNeedle);
    iRetval = 0;
  }
  return iRetval;
}
  
int 
GSE_OglShaderProgram::RemoveShader( GSE_VertexShader *pShader )
{  
  int iRetval = 1;
  vector<GSE_VertexShader *>::iterator itNeedle;
  
  itNeedle = find( m_vVertexShaders.begin(),
		     m_vVertexShaders.end(),
		   pShader);
  
  if ( itNeedle != m_vVertexShaders.end()){
    glDetachShader( m_iProgram, 
		    ((GSE_OglVertexShader *)(*itNeedle))->GetShaderId());
    m_vVertexShaders.erase(itNeedle);
    iRetval = 0;
  }
  return iRetval;
}

int
GSE_OglShaderProgram::InitProgram()
{
  // attach every VERTEX shader 
  for( unsigned int iVS=0;iVS<m_vVertexShaders.size();iVS++)
  {
    glAttachShader( m_iProgram,
		    ((GSE_OglVertexShader *)m_vVertexShaders[iVS])->GetShaderId());
  }
  // attach every FRAGMENT shader
  for( unsigned int iFS=0;iFS<m_vFragmentShaders.size();iFS++)
  {
    glAttachShader( m_iProgram,
		    ((GSE_OglFragmentShader *)m_vFragmentShaders[iFS])->GetShaderId());
  }
  // Link the program 
  glLinkProgram( m_iProgram );

  GSE_OglUtils::ShowGLErrors();
  if ( GSE_OglUtils::IsGLSLLinkErrors(m_iProgram)){
    GSE_OglUtils::ShowGLSLProgramOutput( m_iProgram );
    return 1;
  } else {

    GSE_Logger::Error() << "ShaderProgram " 
			<< m_pShaderName 
			<< " linked." << std::endl;

    
  }
  glUseProgram( m_iProgram );
  std::vector<GSE_ShaderParam *>::iterator it = m_vShaderParameters.begin();
  for(;it!=m_vShaderParameters.end();it++)
  {
    (*it)->Init(this);
  }
  glUseProgram(0 );
  return 0;
}

GLuint
GSE_OglShaderProgram::GetProgramId()
{
  return m_iProgram;
}

int
GSE_OglShaderProgram::AddParameter( GSE_ShaderParam *pShaderParam )
{
  GSE_Logger::Log() << __PRETTY_FUNCTION__ << std::endl;
  if ( pShaderParam != NULL ){
    m_vShaderParameters.push_back( pShaderParam );
    // NOT HERE, adding parameter might occur when programs aren't linked.
    //pShaderParam->Init( this );
    return 0;
  } else {
    return 1;
  }

}  
// ----------------------------------------------------------------------------
