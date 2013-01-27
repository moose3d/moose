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

#ifndef __MooseShader_h__
#define __MooseShader_h__
/////////////////////////////////////////////////////////////////
//#include <MooseRenderable.h>

#include "MooseGL.h"
#include <list>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for programmable shaders.
    class MOOSE_API CShader
    {
    private:
      /// OpenGL program id.
      GLuint m_nShaderProgram;
      std::list<GLuint> m_lstVertexShaders;
      std::list<GLuint> m_lstFragmentShaders;
    public:
      ////////////////////
      /// Creates a program id (assuming that glContext is set up properly).
      CShader();
      ////////////////////
      /// Destructor.
      ~CShader();
      ////////////////////
      /// Assigns vertex shader.
      /// \param nVertexShader Vertex shader id. Must be created with glCreateShader( GL_VERTEX_SHADER );
      void AttachVertexShader( GLuint nVertexShader );
      ////////////////////
      /// Assigns fragment shader.
      /// \param nFragmentShader Fragment shader id. Must be created with glCreateShader( GL_FRAGMENT_SHADER );
      void AttachFragmentShader( GLuint nFragmentShader );
      ////////////////////
      /// Returns program id.
      /// \returns Program number.
      inline unsigned int GetProgram() const 
      { 
	return m_nShaderProgram; 
      }
      ////////////////////
      /// Links shader.
      bool Link();
      void LoadVertexShader( const std::string & strVertexShader );
      void LoadVertexShader( const char * szVertexShader );
      void LoadFragmentShader( const std::string & strFragmentShader );
      void LoadFragmentShader( const char * szFragmentShader );
      void CreateVertexShaderFromSource( const char * szVertexShaderCode, const char * szVSname );
      void CreateFragmentShaderFromSource( const char * szFragmentShaderCode, const char * szFSname);
      bool Validate();
      void Apply();
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
