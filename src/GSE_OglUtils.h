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

#ifndef __GSE_OglUtils_h__
#define __GSE_OglUtils_h__
#include <GL/GLee.h>
#include <GL/gl.h>

//
// GL-related utility class
//
class GSE_OglUtils 
{
 public:
  // Checks if GL_ERRORS exist, non-zero if yes, 0 if no errors.
  static int  IsGLErrors() __attribute__((deprecated));
  // Reports GL_ERRORS currently queued with GSE_Logger
  static void ShowGLErrors(  );
  
  // Reports the errors in GLSL
  static void ShowGLSLProgramOutput(GLuint iProgram);
  static void ShowGLSLShaderOutput(GLuint iShader);
  // These methods check does a shader or shaderprogram have errors messages queued
  static int IsGLSLCompileErrors( GLuint iShader );
  static int IsGLSLLinkErrors( GLuint iProgram );
  static void DeleteVBOs( unsigned int nSize, GLuint *pVBObuffer);
  static void LoadColorMatrixYUV2RGB();
  static void ResetColorMatrix();
};
#endif
