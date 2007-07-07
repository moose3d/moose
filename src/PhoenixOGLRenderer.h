/////////////////////////////////////////////////////////////////
#ifndef __PhoenixOGLRenderer_h__
#define __PhoenixOGLRenderer_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "PhoenixVertexArray.h"
#include "PhoenixIndexArray.h"
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    enum VERTEX_BUFFER_TYPE 
    {
      VB_VERTEX_BUFFER = 0,
      VB_COLOR_BUFFER = 1
    };

    /////////////////////////////////////////////////////////////////
    /// \brief A class which tells which OpenGL features are supported 
    /// by underlying hardware
    class COglRendererFeatures
    {
    protected:
      /// vertex_program support flag.
      int m_bARB_vertex_program;
      /// vertex_shader support flag.
      int m_bARB_vertex_shader;
      /// fragment_shader support flag.
      int m_bARB_fragment_shader;
      /// vertex_array support flag.
      int m_bEXT_vertex_array;
      /// vertex_buffer_object support flag.
      int m_bARB_vertex_buffer_object;
      /// multitexture support flag.
      int m_bARB_multitexture;
      /// shader_objects support flag.
      int m_bARB_shader_objects;
      /// Max number of supported lights.
      int m_iMaxLights;
      /// GL_MAX_ELEMENTS_VERTICES, used to create optimal render batches.
      int m_iMaxElementsVertices; 
      /// GL_MAX_ELEMENTS_INDICES, used to create optimal render batches.
      int m_iMaxElementsIndices;  

    public:
      /// Default constructor.
      COglRendererFeatures();
      /// Is vertex_program extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexProgram() const;
      /// Is vertex_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexShader() const;
      /// Is fragment_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasFragmentShader() const;
      /// Is vertex_array supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexArray() const;
      /// Is vertex_buffer_object supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexBufferObject() const;
      /// Is multitexture supported.
      /// \return 1 if supported, 0 otherwise.
      int HasMultitexture() const;
      /// Is shader_objects supported.
      /// \return 1 if supported, 0 otherwise.
      int HasShaderObjects() const;
      /// Get maximum number of lights.
      /// \return maximum number of lights.
      int  GetMaxLights() const;
      /// Get max elements in vertex arrays.
      /// \return maximum number of elements in vertex arrays.
      int  GetMaxElementsVertices() const;
      /// Get max elements in index arrays.
      /// \return maximum number of elements in index arrays.
      int  GetMaxElementsIndices() const;
      /// for printing out supported features.
      friend std::ostream &operator<<(std::ostream &stream, 
				      const COglRendererFeatures &obj);
    protected:
      /// Initializes the supported values.
      void Init();
    };
    /// BufferType
    enum BUFFER_TYPE 
    { 
      COLOR_BUFFER = 0,
      DEPTH_BUFFER = 1
    };
    /////////////////////////////////////////////////////////////////
    /// Renderer object for OpenGL.
    class COglRenderer
    {
    protected:
      /// Supported features.
      COglRendererFeatures *m_pFeatures;
    public:
      ////////////////////
      /// Default constructor
      COglRenderer();
      ////////////////////
      /// Destructor
      ~COglRenderer();
      ////////////////////
      /// Clears buffer
      void ClearBuffer( BUFFER_TYPE tType);
      ////////////////////
      /// Swaps the buffers if double buffering is used, otherwise 
      /// it doesn't do a thing.
      void Finalize();
      ////////////////////
      /// Commits vertex descriptor.
      /// \param pBuffer buffer to be applied.
      void CommitVertexArray( CVertexArray *pBuffer );
      ////////////////////
      /// Draws the elements from previously set arrays.
      /// \param pIndexBuffer which indices are used and what primitives will be created.
      void CommitPrimitive( CIndexArray *pIndexBuffer );
      ////////////////////
      /// Sets vertex color for drawing.
      /// \param vColor RGBA Color vector.
      void CommitVertexColor( CVector4<unsigned char> &vColor );
      ////////////////////
      /// Disable vertex, color and normal arrays.
      /// \param tType VERTEX_BUFFER_TYPE to be disabled.
      void DisableBuffer( VERTEX_BUFFER_TYPE tType );
    };
    /////////////////////////////////////////////////////////////////  
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
