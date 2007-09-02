#ifndef __PhoenixModel_h__
#define __PhoenixModel_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTexture.h"
#include "PhoenixDefaultEntities.h"
#include <vector>
#include <iostream>
#include <utility>
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
    ////////////////////
    /// Model class for 3D models with texture and vertex data.
    class CModel
    {
    protected:
      /// Handle to textures.
      TEXTURE_HANDLE   	m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      /// Handle to texture coordinates.
      VERTEX_HANDLE     m_aTextureCoordinateHandles[TEXTURE_HANDLE_COUNT];
      /// Texture filters for each texture.
      std::vector<TEXTURE_FILTER> m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      /// Handle to vertices. (position data)
      VERTEX_HANDLE    	m_VertexDescriptorHandle;
      /// Handle to vertex normals.
      VERTEX_HANDLE     m_VertexNormalHandle;
      /// Handle to index arrays for triangle list and/or Triangle strips.
      std::vector<INDEX_HANDLE> m_vecIndexArrayHandles;
      /// Handle to a shader
      SHADER_HANDLE     m_ShaderHandle;
      /// Shader parameters
      vector< std::pair<std::string, VERTEX_HANDLE > > m_vShaderParams;
    public:
      ////////////////////
      /// Constructor.
      CModel();
      ////////////////////
      /// Destructor.
      ~CModel();
      ////////////////////
      /// Returns handle to Texture.
      /// \param nId Which texture unit is handled. By default it is zero, the first.
      /// \returns TEXTURE_HANDLE
      TEXTURE_HANDLE    GetTextureHandle( unsigned int nId = 0) const;
      ////////////////////
      /// Sets texture handle.
      /// \param nId Which texture unit is handled. By default it is zero, the first.
      /// \param handle Handle to texture.
      void		SetTextureHandle( TEXTURE_HANDLE handle, unsigned int nId = 0 );
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE     GetVertexHandle() const;
      ////////////////////
      /// Sets vertex descriptor handle.
      /// \param handle Handle to vertex descriptor.
      void		SetVertexHandle( VERTEX_HANDLE handle );
      ////////////////////
      /// Sets vertex descriptor handle.
      /// \param handle Handle to vertex descriptor.
      void		SetNormalHandle( VERTEX_HANDLE handle );
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE     GetNormalHandle() const;
      ////////////////////
      /// Returns reference to a vector with handles to index arrays.
      /// \returns vector consisting of INDEX_HANDLEs.
      std::vector<INDEX_HANDLE>	& GetIndexHandles();
      ////////////////////
      /// Returns reference to a vector with handles to index arrays.
      /// \returns vector consisting of INDEX_HANDLEs.
      const std::vector<INDEX_HANDLE>	& GetIndexHandles() const;
      ////////////////////
      /// Adds index array handle to a vector.
      /// \param handle Handle to index array.
      void		AddIndexHandle( INDEX_HANDLE handle );
      ////////////////////
      /// Returns handle to shader.
      /// \returns SHADER_HANDLE.
      SHADER_HANDLE	GetShaderHandle() const;
      ////////////////////
      /// Sets shader handle.
      /// \param handle Handle to shader.
      void		SetShaderHandle( SHADER_HANDLE handle );
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param handle Handle to vertexdescriptor.
      void		SetShaderParameter( const char *sName, VERTEX_HANDLE handle );
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-VERTEX_HANDLE pairs.
      std::vector< std::pair<std::string, VERTEX_HANDLE> > & GetShaderParameters();
      ////////////////////
      /// Sets texture coordinate vertexdescriptor handle.
      /// \param handle Handle to vertexdescriptor with texture coordinates.
      /// \param nId To which texture unit these coordinates are applied. By default the first (zero).
      void		SetTextureCoordinateHandle( VERTEX_HANDLE handle, unsigned int nId = 0);
      ////////////////////
      /// Returns handle to Texture Coordinate vertex descriptor.
      /// \param nId From which texture unit these coordinates are retrieved. By default, the first one (zero).
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE     GetTextureCoordinateHandle( unsigned int nId = 0 );
      ////////////////////
      /// Returns texture filters for given texture.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1.  By default, it is first (zero).
      /// \returns Reference to texture filter vector. 
      vector<TEXTURE_FILTER> & GetTextureFilters( unsigned int nId = 0 );
      ////////////////////
      /// Adds texture filter to given texture.
      /// \param tTexFilter TEXTURE_FILTER to insert.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1. By default, it is first (zero).
      void		AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId = 0 );
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param model Model object.
      /// \returns Reference to output stream.
      friend std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CModel & model );
    };
  }; // namespace Graphics
}; // namespace Phoenix
#endif
