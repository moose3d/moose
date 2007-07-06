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
/////////////////////////////////////////////////////////////////
#ifndef __GSE_OglShaders_h__
#define __GSE_OglShaders_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "GSE_shaders.h"
#include "GSE_Logger.h"
/////////////////////////////////////////////////////////////////
using Core::GSE_Logger;
/////////////////////////////////////////////////////////////////
/// The base class for GLSL shaders.
class GSE_OglShaderBase 
{ 
 protected:  
  // The GLSL shader id 
  GLuint m_iShader;
 public:
  // Returns the GLSL shader id 
  inline GLuint GetShaderId();
  // Sets the GLSL shader id 
  inline void SetShaderId( GLuint iShader);

};
/////////////////////////////////////////////////////////////////
/// The GLSL vertex shader class. 
class GSE_OglVertexShader : public GSE_VertexShader,
			    public GSE_OglShaderBase
{

 public:
  /// Constructor.
  GSE_OglVertexShader( char *pSourceFile );
  /// Destructor.
  virtual ~GSE_OglVertexShader();
  /// Initializes the shader.
  int InitShader();

};
/////////////////////////////////////////////////////////////////
/// The GLSL fragment shader class
class GSE_OglFragmentShader : public GSE_FragmentShader,
			      public GSE_OglShaderBase
{
 public:
  // The default constructor
  GSE_OglFragmentShader( char *pSourceFile );
  // The destructor
  virtual ~GSE_OglFragmentShader();
  /// initializes the shader.
  int InitShader();

};
/////////////////////////////////////////////////////////////////
/// The GLSL shader program class.
class GSE_OglShaderProgram : public GSE_ShaderProgram
{
 protected:
  /// Shader program name.
  GLuint m_iProgram;
 public:  
  /// Parametrized constructor.
  /// \param pShaderName name of shader.
  GSE_OglShaderProgram(char *pShaderName); 
  /// Destructor.
  virtual ~GSE_OglShaderProgram();
  /// Adds new vertexshader to this program.
  /// \param pShader a pointer to a VertexShader.
  /// \return non-zero on error.
  int AddShader( GSE_VertexShader *pShader );
  /// Adds new fragmentshader to this program.
  /// \param pShader a pointer to a FragmentShader.
  /// \return non-zero on error.
  int AddShader( GSE_FragmentShader *pShader );
  /// Removes a fragmentshader from this program.
  /// \param pShader fragment shader to be removed.
  /// \return non-zero on error.
  int RemoveShader( GSE_FragmentShader *pShader );
  /// Removes a vertexshader from this program.
  /// \param pShader vertex shader to be removed.
  /// \return non-zero on error.
  int RemoveShader( GSE_VertexShader *pShader );
  /// Links compiled shaders to the program.
  /// \return non-zero on error.
  int InitProgram();
  /// Returns program identifier.
  /// \param program name.
  GLuint GetProgramId();
  /// Adds new parameter to this shader program.
  /// \param pShaderParam a pointer to shaderparam.
  /// \return non-zero on error.
  int AddParameter( GSE_ShaderParam *pShaderParam );
};

/////////////////////////////////////////////////////////////////
/// The GLSL shader uniform parameter.
template < std::size_t ELEMENT_SIZE, /// how many TYPEs does one element hold.
	   typename TYPE>	    /// type of the storage element.
class GSE_OglShaderParam : public GSE_ShaderParam
{
 public:

 protected:

  /// The shader parameter location.
  GLint  m_iLoc;
  /// parameter values.
  TYPE	 *m_pValues;
  /// number of paramters.
  unsigned int m_iNumElements;
  /// number of values in one parameter
  unsigned int m_iNumInElement;

 public:
  /// Parametrized constructor.
  /// \param pName shader name.
  /// \param iNumElements number of elements.
  GSE_OglShaderParam( char * pName, unsigned int iNumElements ) : GSE_ShaderParam(pName)
  {
    m_iNumElements  = iNumElements;
    m_iNumInElement = ELEMENT_SIZE;
    m_pValues	    = new TYPE[m_iNumElements * m_iNumInElement];
    // The initialization 
    for(unsigned int i=0;i<m_iNumElements*m_iNumInElement;i++){
      m_pValues[i] = 0; // handy, this goes for boolean, float, short and int.
    }
    m_iLoc = 0;
    
  }
  /// Copy constructor.
  GSE_OglShaderParam( const GSE_OglShaderParam &ref)
  {

    m_iNumElements = ref.m_iNumElements;
    m_iNumInElement = ref.m_iNumInElement;
    m_iLoc = ref.m_iLoc;
    m_pValues = new TYPE[m_iNumElements * m_iNumInElement];
    SetValues( ref.m_pValues );

  }
  /// Assignment operator.
  /// \param ref Shader parameter.
  /// \return a reference to this.
  GSE_OglShaderParam &operator=( GSE_OglShaderParam ref )
  {
    m_iNumElements = ref.m_iNumElements;
    m_iNumInElement = ref.m_iNumInElement;
    m_iLoc = ref.m_iLoc;
    if ( m_pValues != NULL ) delete m_pValues;
    m_pValues = new TYPE[m_iNumElements * m_iNumInElement];
    SetValues( ref.m_pValues );
  }

  /// Initializes the location.
  /// \param pProgram shader program where this parameter is linked to.
  void Init( GSE_ShaderProgram *pProgram )
  {
    GSE_Logger::Log() << "Calling Init() for " << GetParamName() << std::endl;
    
    m_iLoc = glGetUniformLocation( ((GSE_OglShaderProgram *)pProgram)->GetProgramId(), GetParamName() );
    GSE_Logger::Log() << "Program " 
		      << ((GSE_OglShaderProgram *)pProgram)->GetProgramId() 
		      << " param '" 
		      <<  GetParamName() 
		      << "' is at " << m_iLoc << std::endl;
  }
  /// Returns the values array.
  /// \return pointer to values array.
  TYPE *GetValues()
  {
    return m_pValues;
  }
  /// Assigns values to this parameter.
  /// \param pValues parameter values.
  void SetValues( TYPE *pValues )
  {
    if ( pValues != NULL )
    {
      memcpy( m_pValues, pValues, sizeof(TYPE)* m_iNumElements * m_iNumInElement);

      //for(unsigned int i=0;i<m_iNumElements * m_iNumInElement;i++){
      //	m_pValues[i] = pValues[i];
      //}
    } else {
      GSE_Logger::Error() << __FUNCTION__ 
			  << "Can't set NULL as m_pValues." << std::endl;
    }
  }
};
/////////////////////////////////////////////////////////////////
/// The GLSL uniform shader parameter for single floating point values.
class GSE_OglShaderParam1fv : public GSE_OglShaderParam<1,float>
{
 public:
  /// default constructor.
  GSE_OglShaderParam1fv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<1,float>(pName,iNumElements) {}
  /// Applies parameters to program.
  void Apply()
  {
    glUniform1fv( m_iLoc, m_iNumElements, m_pValues);
  }
};
/////////////////////////////////////////////////////////////////
/// The GLSL uniform shader parameter for two floating point values.
class GSE_OglShaderParam2fv : public GSE_OglShaderParam<2,float>
{
 public:
  /// default constructor.
  GSE_OglShaderParam2fv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<2,float>(pName,iNumElements) {}
  /// Applies parameters to program.
  void Apply()
  {
    glUniform2fv( m_iLoc, m_iNumElements, m_pValues);
  }
};
/////////////////////////////////////////////////////////////////
/// The GLSL uniform shader parameter for three floating point values.
class GSE_OglShaderParam3fv : public GSE_OglShaderParam<3,float>
{
 public:
  /// default constructor.
  GSE_OglShaderParam3fv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<3,float>(pName,iNumElements) {}
  /// Applies parameters to program.
  void Apply()
  {
    glUniform3fv( m_iLoc, m_iNumElements, m_pValues);
  }
};
/////////////////////////////////////////////////////////////////
/// The GLSL uniform shader parameter for four floating point values.
class GSE_OglShaderParam4fv : public GSE_OglShaderParam<4,float>
{
 public:
  /// default constructor.
  GSE_OglShaderParam4fv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<4,float>(pName,iNumElements) {}
  /// Applies parameters to program.
  void Apply()
  {
    glUniform4fv( m_iLoc, m_iNumElements, m_pValues);
  }
};
// ----------------------------------------------------------------------------
//
// The GLSL uniform shader parameter for integer values
//
// ----------------------------------------------------------------------------
class GSE_OglShaderParam1iv : public GSE_OglShaderParam<1,int>
{
 public:
  GSE_OglShaderParam1iv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<1,int>(pName,iNumElements) {}
  void Apply()
  {
    glUniform1iv( m_iLoc, m_iNumElements, m_pValues);
  }
};

class GSE_OglShaderParam2iv : public GSE_OglShaderParam<2,int>
{
 public:
  GSE_OglShaderParam2iv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<2,int>(pName,iNumElements) {}
  void Apply()
  {
    glUniform2iv( m_iLoc, m_iNumElements, m_pValues);
  }
};

class GSE_OglShaderParam3iv : public GSE_OglShaderParam<3,int>
{
 public:
  GSE_OglShaderParam3iv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<3,int>(pName,iNumElements) {}
  void Apply()
  {
    glUniform3iv( m_iLoc, m_iNumElements, m_pValues);
  }
};

class GSE_OglShaderParam4iv : public GSE_OglShaderParam<4,int>
{
 public:
  GSE_OglShaderParam4iv( char *pName, unsigned int iNumElements ) : GSE_OglShaderParam<4,int>(pName,iNumElements) {}
  void Apply()
  {
    glUniform4iv( m_iLoc, m_iNumElements, m_pValues);
  }
};
// ----------------------------------------------------------------------------
//
// The GLSL uniform shader parameter for matrices. According to the specs,
// only float 4x4, 3x3 and 2x2 are accepted.
//
// ----------------------------------------------------------------------------
class GSE_OglShaderParam4x4fv : public GSE_OglShaderParam<16,float>
{
 public:
  GSE_OglShaderParam4x4fv( char *pName, unsigned int iNumElements) : GSE_OglShaderParam<16,float>(pName, iNumElements){}
   void Apply()
  {
    glUniformMatrix4fv( m_iLoc, m_iNumElements, 0, m_pValues);
  }
  
};
class GSE_OglShaderParam3x3fv : public GSE_OglShaderParam<9,float>
{
 public:
  GSE_OglShaderParam3x3fv( char *pName, unsigned int iNumElements) : GSE_OglShaderParam<9,float>(pName, iNumElements){}
   void Apply()
  {
    glUniformMatrix3fv( m_iLoc, m_iNumElements, 0, m_pValues);
  }
  
};
class GSE_OglShaderParam2x2fv : public GSE_OglShaderParam<4,float>
{
 public:
  GSE_OglShaderParam2x2fv( char *pName, unsigned int iNumElements) : GSE_OglShaderParam<4,float>(pName, iNumElements){}
   void Apply()
  {
    glUniformMatrix3fv( m_iLoc, m_iNumElements, 0, m_pValues);
  }
  
};
// ----------------------------------------------------------------------------
//
// The GLSL attribute variable array template. Can be used to create various 
// vertex attribute arrays with different types.
//
// ----------------------------------------------------------------------------
template< typename TYPE,	    // The internal storage type of the array
	  GLenum PARAMTYPE,	    // GL_FLOAT, GL_SHORT, etc.
	  std::size_t ELEM_SIZE>    // The number of values per element
class GSE_OglVertexAttribStruct : public GSE_VertexAttribStruct
{

 protected:

  TYPE          *m_pValues;
  GLint         m_iLoc;
  GLboolean	m_bNormalized;
  unsigned int  m_iNumVertices;
  //int		m_iProgramIndex;
  GSE_OglShaderProgram *m_pSHprog;
 public:
  // The constructor
  GSE_OglVertexAttribStruct(char *pName, 
			    unsigned int iNumVertices ) : 
  GSE_VertexAttribStruct(pName)
  {
    m_iNumVertices = iNumVertices;
    m_bNormalized = 0;
    m_pValues = new TYPE[m_iNumVertices*ELEM_SIZE];
    m_iLoc = 0;
    m_pSHprog = NULL;
  }
  // The copy constructor
  GSE_OglVertexAttribStruct(const GSE_OglVertexAttribStruct &ref) : GSE_VertexAttribStruct( ref )
  {
    m_iNumVertices = ref.m_iNumVertices;
    m_bNormalized = ref.m_bNormalized;
    m_pValues = new TYPE[m_iNumVertices*ELEM_SIZE];
    m_iLoc = ref.m_iLoc;
    // copy values to new buffer
    memcpy(m_pValues, ref.m_pValues, sizeof(TYPE)*m_iNumVertices*ELEM_SIZE);
    m_pSHprog = ref.m_pSHprog;
  }
  // The assignment operator
  GSE_OglVertexAttribStruct &operator=(GSE_OglVertexAttribStruct ref)
  {
    m_iNumVertices = ref.m_iNumVertices;
    m_bNormalized = ref.m_bNormalized;
    m_iLoc = ref.m_iLoc;
    // delete previous array
    if ( m_pValues != NULL ) delete m_pValues;
    // allocate new space
    m_pValues = new TYPE[m_iNumVertices*ELEM_SIZE];
    // copy values to new buffer
    memcpy(m_pValues, ref.m_pValues, sizeof(TYPE)*m_iNumVertices*ELEM_SIZE);
    m_pSHprog = ref.m_pSHprog;
    return *this;

  }
  // The destructor
  ~GSE_OglVertexAttribStruct()
  {
    if ( m_pValues != NULL ){
      delete m_pValues;
    }
    m_pValues = NULL;
  }
  // Define will the values passed to the shader be normalized before sending.
  inline void SetNormalized(GLboolean bFlag )
  {
    m_bNormalized = bFlag;
  }
  // Whether values passed to shader will be normalized, true for yes.
  inline char IsNormalized() 
  {
    return m_bNormalized;
  }
  // 
  void Activate() 
  {
    glEnableVertexAttribArray(m_iLoc);
    glVertexAttribPointer(m_iLoc, ELEM_SIZE, PARAMTYPE , m_bNormalized, 0, m_pValues);
  }
  
  void Deactivate() 
  {
    glDisableVertexAttribArray(m_iLoc);
  }
  TYPE * GetValues()
  {
    return m_pValues;
  }
  void SetValues( unsigned int iVertex, TYPE *pArray )
  {
    if ( pArray == NULL ){
      GSE_Logger::Error() << __FUNCTION__ 
			  << " : Invalid src array" << std::endl;

    } else if( iVertex >= m_iNumVertices){
      GSE_Logger::Error() << __FUNCTION__ 
			  << " : Invalid vertex number" << std::endl;
      
    } else {
      memcpy( &m_pValues[iVertex*ELEM_SIZE], pArray, sizeof(TYPE)*ELEM_SIZE );
    }
        
  }
  
  void BindToProgram( GSE_OglShaderProgram *pSHprog)
  {
    m_pSHprog = pSHprog;
  }
  void Init()
  {
    if ( m_pSHprog != NULL )
    {
      m_iLoc = glGetAttribLocation( m_pSHprog->GetProgramId(),  m_pParamName);
    }
  }


};
// ----------------------------------------------------------------------------
//
// Allowed vertex attributes for shaders (according to GLSL Spec 1.10.59)
//
// ----------------------------------------------------------------------------
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 4> GSE_OglVertexAttrib4f;
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 3> GSE_OglVertexAttrib3f;
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 2> GSE_OglVertexAttrib2f;
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 1> GSE_OglVertexAttrib1f;

typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 16> GSE_OglVertexAttribMatrix4x4f;
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 9 > GSE_OglVertexAttribMatrix3x3f;
typedef GSE_OglVertexAttribStruct<GLfloat, GL_FLOAT, 4 > GSE_OglVertexAttribMatrix2x2f;


// An example: 
// 66 vertices in a mesh and each of them receives vec4 (four floating-point values)
// and the variable name in shader is 'tangent'.
// GSE_OglVertexAttrib4f *pVertexAttrib = new GSE_OglVertexAttrib4f("tangent", 66);
//
#endif
