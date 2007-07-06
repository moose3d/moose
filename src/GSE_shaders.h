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

#ifndef __GSE_shaders_h__
#define __GSE_shaders_h__

#include "GSE_structures.h"
#include <algorithm>
#include <vector>
#include <string.h>
#include <map>
#include <string.h>

using std::vector;
using std::map;


/// \brief An abstract generic shader class, provides a base class for all shaders in GSE.
class GSE_Shader
{
 private:
  /// The default constructor.
  GSE_Shader(){}
 protected:
  /// The filename which points to the shader file.
  char *m_pFilename; 
  /// The parametrized constructor for derived classes. Sets the shader filename.
  /// \param pSourceFile The filename for the shader source.
  GSE_Shader( char *pSourceFile){
    if ( pSourceFile != NULL )
    {
      m_pFilename = strdup(pSourceFile);
    } 
    else
    {
      m_pFilename = NULL;
    }
  }
 public:
  /// \brief The copy constructor.
  /// \param ref A reference to GSE_Shader object.
  GSE_Shader(const GSE_Shader &ref)
  {
    m_pFilename = strdup(ref.m_pFilename);
  }
  /// \brief The assignment operator.
  /// \param shader A GSE_Shader object.
  GSE_Shader &operator=(GSE_Shader &shader)
  {
    SetShaderFilename( shader.m_pFilename);
    return *this;
  }
  /// \brief The destructor.
  virtual ~GSE_Shader()
  {
    if ( m_pFilename != NULL ) delete m_pFilename;
    m_pFilename = NULL;
  }
  /// \brief Sets the shader filename.
  /// \param pString the shader filename.
  inline void SetShaderFilename( char *pString )
  {
    if ( m_pFilename != NULL ) delete m_pFilename;
    m_pFilename = strdup(pString);
  }
  /// \brief Returns the shader filename.
  /// \return const char * - a pointer to a filename.
  inline const char *GetShaderFilename()
  {
    return m_pFilename;
  }
  /// \brief Loads the shader source and compiles it. 
  /// \returns Zero on success. On error, non-zero.
  virtual int InitShader() = 0;

};

/// \brief The Fragment shader baseclass. 
/// The baseclass for pixel and fragment shaders.
class GSE_FragmentShader : public GSE_Shader 
{
 protected:
  /// Constructor.
  GSE_FragmentShader( char *pSourceFile) : GSE_Shader( pSourceFile )
  {
    
  }
 public:
  /// Destructor.
  virtual ~GSE_FragmentShader(){}

};
// ----------------------------------------------------------------------------
/// The Vertex shader baseclass
// ----------------------------------------------------------------------------
class GSE_VertexShader : public GSE_Shader
{
 protected:
  /// Parametrized constructor.
  /// \param pSourceFile the source code for the shader.
  GSE_VertexShader( char *pSourceFile ) : GSE_Shader( pSourceFile)
  {
    
  } 

};

class GSE_ShaderProgram;
// ----------------------------------------------------------------------------
//
/// \brief A primitive-wide shader parameter baseclass. 
/// This variable cannot be changed during rendering.
//
class GSE_ShaderParam
{
 protected:
  /// The shader parameter variable name in shader.
  char * m_pParamName;
 public:
  /// \brief The constructor.
  /// \param pParamName The name of the parameter.
  GSE_ShaderParam(char *pParamName)
  {
    m_pParamName = strdup(pParamName);
  }
  /// \brief The copy constructor.
  /// \param ref A reference to GSE_ShaderParam object.
  GSE_ShaderParam(const GSE_ShaderParam &ref)
  {
    m_pParamName = strdup(ref.m_pParamName);
  }
  /// \brief Assignment operator.
  /// \param ref A GSE_ShaderParam object which attributes are assigned to this.
  GSE_ShaderParam &operator=(GSE_ShaderParam &ref)
  {
    m_pParamName = strdup(m_pParamName);
    return *this;
  }
  /// \brief The destructor.
  virtual ~GSE_ShaderParam()
  {
    if ( m_pParamName != NULL ){
      delete m_pParamName;
    }
  }
  /// \brief Returns the parameter name
  /// \return const char * parameter name as zero-terminated char array.
  inline const char * GetParamName() {
    return (const char *)m_pParamName;
  }
  /// \brief Binds the parameter to a program.
  /// \param pProgram the GSE_ShaderProgram which this parameter will be bound.
  virtual void Init( GSE_ShaderProgram *pProgram) = 0;
  /// \brief Assigns the parameter values to current shader.
  virtual void Apply() = 0;
};
// ----------------------------------------------------------------------------

/// \brief An struct holding the array where vertex-bound parameters are stored
/// for shaders to use 'em.
class GSE_VertexAttribStruct
{
 protected:
  /// The parameter name.
  char *m_pParamName;
  /// \brief The constructor. 
  /// \param pName the parameter name as zero-terminated char array.
  GSE_VertexAttribStruct( char *pName ){
    m_pParamName = strdup(pName);
  }
  
 public:
  /// \brief The copy constructor.
  /// \param ref A reference to GSE_VertexAttribStruct object.
  GSE_VertexAttribStruct(const GSE_VertexAttribStruct &ref)
  {
    m_pParamName = strdup( ref.m_pParamName);
  }
  /// \brief The assignment operator.
  /// \param obj A reference to a GSE_VertexAttribStruct object.
  GSE_VertexAttribStruct &operator=(GSE_VertexAttribStruct & obj)
  {
    if( m_pParamName != NULL ) delete m_pParamName;
    m_pParamName = strdup( obj.m_pParamName );
    return *this;
  }
  /// \brief The destructor.
  virtual ~GSE_VertexAttribStruct() 
  {
    if ( m_pParamName != NULL ) delete m_pParamName;
    m_pParamName = NULL;
  }
  /// \brief Returns the parameter name.
  /// \return The name as zero-terminated char array.
  inline const char * GetParamName() {
    return (const char *)m_pParamName;
  }
  /// \brief Activates the struct.
  virtual void Activate()   = 0;
  /// \brief Deactives the struct.
  virtual void Deactivate() = 0;
  /// \brief Initializes the struct.
  virtual void Init() = 0;
};
// ----------------------------------------------------------------------------

/// \brief A manager class from vertex attributes in vertex shaders.
/// \author eNtity / AG
class GSE_VertexAttribStructMgr : public GSE_Container<GSE_VertexAttribStruct>,
				  public GSE_Singleton<GSE_VertexAttribStructMgr>
{
  friend class GSE_Singleton<GSE_VertexAttribStructMgr>;
 private:
  /// \brief The constructor.
  GSE_VertexAttribStructMgr() : GSE_Container<GSE_VertexAttribStruct>(){}
 public:
  /// \brief The destructor.
  ~GSE_VertexAttribStructMgr(){}
  /// \brief Initializes all attributes which have been inserted into the manager.
  void InitAttributes()
  {
    std::vector<GSE_VertexAttribStruct *>::iterator it = m_Objects.begin();
    for(;it!=m_Objects.end();it++)
    {
      (*it)->Init();
    }
  }
};
// ----------------------------------------------------------------------------
//
/// \brief A shader program class. This program can use several vertex and fragment
/// shaders.
/// \author eNtity / AG
class GSE_ShaderProgram 
{
 protected:
  /// The vector containing the vertex shader indices.
  std::vector<GSE_VertexShader *> m_vVertexShaders;
  /// The vector containing the fragment shader indices.
  std::vector<GSE_FragmentShader *> m_vFragmentShaders;
  /// The vector containing the shader parameter attributes.
  std::vector<GSE_ShaderParam *> m_vShaderParameters;
  /// The shader name which identifies the shader.
  char *m_pShaderName;


  /// \brief The default constructor.
  GSE_ShaderProgram( char *pShaderName )
  { 
    m_pShaderName = NULL;
    SetShaderName( pShaderName );
  }  
  
 public:
  /// \brief The destructor.
  virtual ~GSE_ShaderProgram();
  /// \brief The copy constructor
  GSE_ShaderProgram(const GSE_ShaderProgram &ref)
  {
    m_pShaderName = NULL;
    SetShaderName(ref.m_pShaderName);
  }
  /// \brief The assignment operator. 
  /// \param program A referece to GSE_ShaderProgram.
  /// \return A reference to this.
  GSE_ShaderProgram &operator=(GSE_ShaderProgram & program)
  {
    SetShaderName(program.m_pShaderName);
    m_vVertexShaders = program.m_vVertexShaders;
    m_vFragmentShaders = program.m_vFragmentShaders;
    m_vShaderParameters = program.m_vShaderParameters;
    return *this;
  }
  /// \brief Assigns the shader name.
  /// \param pString the zero-terminated char array which 
  // represents the shader name.
  inline void SetShaderName( char *pString )
  {
    if ( m_pShaderName != NULL ) delete m_pShaderName;
    m_pShaderName = strdup( pString );
  }
  /// Returns the shader name.
  /// \return shader name as zero-terminated char array.
  inline const char *GetShaderName()
  {
    return m_pShaderName;
  }
  /// \brief Assigns shader to a program. 
  /// \param pShader A pointer to the GSE_VertexShader to be added.
  /// \return zero on success, non-zero on error.
  virtual int AddShader( GSE_VertexShader *pShader ) = 0;
  /// \brief Assigns shader to a program
  /// \return zero on success, non-zero on error 
  /// \param pShader pointer to GSE_FragmentShader to be added.
  virtual int AddShader( GSE_FragmentShader *pShader ) = 0;
  /// \brief Removes a fragment shader from the program.
  /// \param pShader pointer to GSE_FragmentShader to be removed.
  /// \return zero on success, non-zero on error 
  virtual int RemoveShader( GSE_FragmentShader *pShader ) = 0;
  /// \brief Removes a vertex shader from the program.
  /// \param pShader pointer to GSE_VertexShader to be removed.
  /// \return zero on success, non-zero on error 
  virtual int RemoveShader( GSE_VertexShader *pShader ) = 0;
  /// \brief Initializes the program. Links shaders to the program.
  /// \return zero on success, non-zero on error
  virtual int InitProgram() = 0;
  /// \brief Adds shader parameter to the program.
  /// \param pShaderParam GSE_ShaderParam to be added.
  /// \return zero on success, non-zero on error
  virtual int AddParameter( GSE_ShaderParam *pShaderParam ) = 0;
  /// \brief Applies the uniform parameters to the program.
  void SetUniformParameters()
  {
    std::vector<GSE_ShaderParam *>::iterator it = m_vShaderParameters.begin();
    for(;it!=m_vShaderParameters.end();it++)
    {
      (*it)->Apply();
    }
  }

};
// ----------------------------------------------------------------------------

/// \brief  A Singleton manager class for handling shader programs
/// \author eNtity / AG
class GSE_ShaderProgramMgr : public GSE_Container<GSE_ShaderProgram>,
			     public GSE_Singleton<GSE_ShaderProgramMgr>
{
  friend class GSE_Singleton<GSE_ShaderProgramMgr>;
 private:
  /// \brief The constructor.
  GSE_ShaderProgramMgr() : GSE_Container<GSE_ShaderProgram>() {}
  /// \brief The destructor.
  ~GSE_ShaderProgramMgr() {}
 public:

  /// \brief Initializes the programs which are managed.
  /// \return zero on success, non-zero on error
  int InitPrograms();
};

// ----------------------------------------------------------------------------

/// \brief A Vertex Shader manager class. 
/// Handles every vertex shader in the system
class GSE_VertexShaderMgr : public GSE_Singleton< GSE_VertexShaderMgr >,
			    public GSE_Container< GSE_VertexShader >
{
  friend class GSE_Singleton< GSE_VertexShaderMgr >;
 private:  
  /// \brief The constructor.
  GSE_VertexShaderMgr() : GSE_Container< GSE_VertexShader >()
  {
    
  }
 public:
  /// \brief  Initializes the stored shaders.
  /// \return zero on success, non-zero on error.
  int InitShaders();
};
// ----------------------------------------------------------------------------

/// \brief A Fragment Shader manager class. 
/// Handles every fragment shader in the system. 
class GSE_FragmentShaderMgr : public GSE_Singleton< GSE_FragmentShaderMgr >,
			      public GSE_Container< GSE_FragmentShader >
{
  friend class GSE_Singleton< GSE_FragmentShaderMgr >;
 private:
  /// \brief the constructor.
  GSE_FragmentShaderMgr() : GSE_Container< GSE_FragmentShader >()
  {
    
  }
 public:
  /// \brief Initializes the stored shaders.
  /// \return zero on success, non-zero on error.
  int InitShaders();
}; 

#endif
