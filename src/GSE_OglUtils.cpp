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

#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "GSE_OglUtils.h"
#include "GSE_Logger.h"
#include "GSE_debug.h"
using namespace std;
using namespace Core;

int GSE_OglUtils::IsGLErrors() 
{ 
  return (glGetError()!=GL_NO_ERROR);
}
  
void 
GSE_OglUtils::ShowGLErrors()
{
  
  GLenum iGLError       = glGetError();
  // if error occurred during 
  while( iGLError != GL_NO_ERROR) {
    
    GSE_Logger::Error() << " GL_ERROR : " 
		      << gluErrorString(iGLError) 
		      << endl;
    iGLError = glGetError();
    GSE_Logger::Error().flush();

  }

  
}

void
GSE_OglUtils::ShowGLSLProgramOutput(GLuint iProgram)
{

  int iInfologLength   = 0;
  int iCharsRetrieved  = 0;
  char *pInfoLog;
  // Get length of the info log
  glGetShaderiv(iProgram, GL_INFO_LOG_LENGTH, &iInfologLength);
  
  if (iInfologLength > 0)
  {
    pInfoLog = (char *)malloc(iInfologLength);
    if ( pInfoLog != NULL ) {
      glGetProgramInfoLog(iProgram, iInfologLength, &iCharsRetrieved, pInfoLog);
      GSE_Logger::Error() << "----------GLSL program output begins---------" << std::endl;
      GSE_Logger::Error() << pInfoLog << std::endl;
      GSE_Logger::Error() << "----------GLSL program output ends ---------" << std::endl;
      free(pInfoLog);
    } else {

      GSE_Logger::Error() << __FUNCTION__ << "Couldn't malloc()" << std::endl;
    }

  } else {
      GSE_Logger::Error() << "----------GLSL program output begins---------" << std::endl;
      GSE_Logger::Error() << "No program output." << std::endl;
      GSE_Logger::Error() << "----------GLSL program output ends ---------" << std::endl;

  }

}

void
GSE_OglUtils::ShowGLSLShaderOutput(GLuint iShader)
{



  int iInfologLength   = 0;
  int iCharsRetrieved  = 0;
  char *pInfoLog;
  // Get length of the info log
  glGetShaderiv(iShader, GL_INFO_LOG_LENGTH, &iInfologLength);
  
  if (iInfologLength > 0)
  {
    pInfoLog = (char *)malloc(iInfologLength);
    if ( pInfoLog != NULL ) {
      glGetShaderInfoLog(iShader, iInfologLength, &iCharsRetrieved, pInfoLog);
      GSE_Logger::Error() << "----------GLSL shader output begins---------" << std::endl;
      GSE_Logger::Error() << pInfoLog << std::endl;
      GSE_Logger::Error() << "----------GLSL shader output ends ---------" << std::endl;
      free(pInfoLog);
    } else {
      GSE_Logger::Error() << __FUNCTION__ << "Couldn't malloc()" << std::endl;
    }

  } else {
    GSE_Logger::Error() << "----------GLSL shader output begins---------" << std::endl;
    GSE_Logger::Error() << "No shader output." << std::endl;
    GSE_Logger::Error() << "----------GLSL shader output ends ---------" << std::endl;
      

  }
}

int 
GSE_OglUtils::IsGLSLLinkErrors( GLuint iProgram )
{
  int iState = 0;
  glGetProgramiv(iProgram, GL_LINK_STATUS, &iState);

  if ( iState == GL_TRUE ) return 0;
  else			   return 1;

}
/////////////////////////////////////////////////////////////////
int 
GSE_OglUtils::IsGLSLCompileErrors( GLuint iShader )
{
  int iState = 0;
  glGetShaderiv(iShader, GL_COMPILE_STATUS, &iState);

  if ( iState == GL_TRUE ) return 0;
  else			   return 1;
}
/////////////////////////////////////////////////////////////////
void 
GSE_OglUtils::DeleteVBOs( unsigned int nSize, GLuint *pVBObuffer)
{
  if ( pVBObuffer == NULL ) {
    GSE_Logger::Error() << DEBUG_HEADER << "I won't delete NULLs." << std::endl;
  } else {
    glDeleteBuffersARB( nSize, pVBObuffer );
    *pVBObuffer = 0;    
  } 
}
/////////////////////////////////////////////////////////////////
void
GSE_OglUtils::LoadColorMatrixYUV2RGB()
{
  GLfloat mYUV2RGB[16] = {
    1.0f,    1.0f,      1.0f,   0.0f,
    0.0f,   -0.344136f, 1.773f, 0.0f,
    1.402f, -0.714136f, 0.0f,   0.0f,
    0.0f,    0.0f,      0.0f,   0.0f
  };

  glMatrixMode(GL_COLOR);
  glLoadMatrixf(mYUV2RGB);
  glPixelTransferf( GL_GREEN_BIAS, -0.5f);
  glPixelTransferf( GL_BLUE_BIAS, -0.5f);
  glMatrixMode(GL_MODELVIEW);
}

void 
GSE_OglUtils::ResetColorMatrix()
{
  glMatrixMode(GL_COLOR);
  glLoadIdentity();
  glPixelTransferf( GL_GREEN_BIAS, 0.0f);
  glPixelTransferf( GL_BLUE_BIAS, 0.0f);
  glMatrixMode(GL_MODELVIEW);
}
/////////////////////////////////////////////////////////////////
