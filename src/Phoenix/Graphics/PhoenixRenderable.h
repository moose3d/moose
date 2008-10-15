#ifndef __PhoenixRenderable_h__
#define __PhoenixRenderable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTexture.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixTransform.h"
#include "PhoenixRenderState.h"
#include "PhoenixAPI.h"
#include <vector>
#include <iostream>
#include <utility>
#include <map>
/////////////////////////////////////////////////////////////////
using Phoenix::Graphics::CVertexDescriptor;
using Phoenix::Graphics::CIndexArray;
using Phoenix::Graphics::COglTexture;
using Phoenix::Default::TEXTURE_HANDLE;
using Phoenix::Default::VERTEX_HANDLE;
using Phoenix::Default::INDEX_HANDLE;
using Phoenix::Default::SHADER_HANDLE;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    class COglRenderer;
    // typedefs for readability (and maintanence)
    typedef std::vector< std::pair<std::string, VERTEX_HANDLE * > > ShaderParams;
    typedef std::map< std::string, int >	       ShaderIntParams;
    typedef std::map< std::string, float >	       ShaderFloatParams;
    ////////////////////
    /// Renderable class for 3D models with texture and vertex data.
    class PHOENIX_API CRenderable
    {
    protected:
      /// Handle to textures.
      TEXTURE_HANDLE			      m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      /// Handle to texture coordinates.
      VERTEX_HANDLE			      m_aTextureCoordinateHandles[TEXTURE_HANDLE_COUNT];
      /// Texture filters for each texture.
      std::vector<TEXTURE_FILTER>	      m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      /// Handle to vertices. (position data)
      VERTEX_HANDLE			      m_VertexDescriptorHandle;
      /// Handle to vertex normals.
      VERTEX_HANDLE			      m_VertexNormalHandle;
      /// Handle to vertex colors.
      VERTEX_HANDLE			      m_VertexColorHandle;
      /// Handle to index arrays for triangle list and/or Triangle strips.
      INDEX_HANDLE			      m_hIndices;
      /// Handle to a shader
      SHADER_HANDLE			      m_ShaderHandle;
      
      /// Shader parameters
      ShaderParams			      m_vShaderParams;
      /// Shader integer parameters
      ShaderIntParams			      m_vShaderIntParams;
      /// Shader float parameters
      ShaderFloatParams			      m_vShaderFloatParams;
      
      /// Transparency flag.
      //bool				      m_bTransparent;      
      /// Renderstate.
      Phoenix::Graphics::CRenderState	      m_RenderState;
      /// How renderable should be transformed.
      Phoenix::Math::CTransform		      *m_pTransform;
    public:
      ////////////////////
      /// Constructor.
      CRenderable();
      ////////////////////
      /// Destructor.
      virtual ~CRenderable();
      ////////////////////
      /// Returns handle to Texture.
      /// \param nId Which texture unit is handled. By default it is zero, the first.
      /// \returns TEXTURE_HANDLE
      TEXTURE_HANDLE    & GetTextureHandle( unsigned int nId = 0);
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE     & GetVertexHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE    & GetNormalHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE    & GetColorHandle();
      ////////////////////
      /// Returns reference to a handle with triangle lists indices .
      /// \returns INDEX_HANDLE.
      INDEX_HANDLE &	GetIndices();
      ////////////////////
      /// Returns handle to shader.
      /// \returns SHADER_HANDLE.
      SHADER_HANDLE  & GetShaderHandle();
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param handle Handle to vertexdescriptor.
      void		AddShaderParameter( const char *sName, VERTEX_HANDLE * pHandle );
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param fValue Parameter value.
      void		AddShaderParameter( const char *sName, float fValue );
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param iValue Parameter value.
      void		AddShaderParameter( const char *sName, int iValue );
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-VERTEX_HANDLE pairs.
      ShaderParams & GetShaderParameters();
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-int pairs.
      ShaderIntParams & GetShaderIntParameters();
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-float pairs.
      ShaderFloatParams & GetShaderFloatParameters();
      ////////////////////
      /// Returns handle to Texture Coordinate vertex descriptor.
      /// \param nId From which texture unit these coordinates are retrieved. By default, the first one (zero).
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE & GetTextureCoordinateHandle( unsigned int nId = 0 );
      ////////////////////
      /// Returns texture filters for given texture.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1.  By default, it is first (zero).
      /// \returns Reference to texture filter vector. 
      std::vector<TEXTURE_FILTER> & GetTextureFilters( unsigned int nId = 0 );
      ////////////////////
      /// Adds texture filter to given texture.
      /// \param tTexFilter TEXTURE_FILTER to insert.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1. By default, it is first (zero).
      void		AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId = 0 );
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderable & renderable );
      ////////////////////
      /// Returns renderstate.
      /// \returns Current renderstate settings for renderable.
      Phoenix::Graphics::CRenderState & GetRenderState();
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( COglRenderer & renderer );
      
      Phoenix::Math::CTransform * GetTransform();

      void	   SetTransform( Phoenix::Math::CTransform *pTransform );

    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
