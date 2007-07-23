#ifndef __PhoenixModel_h__
#define __PhoenixModel_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTexture.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
using Phoenix::Graphics::CVertexDescriptor;
using Phoenix::Graphics::CIndexArray;
using Phoenix::Graphics::COglTexture;
using Phoenix::Default::TEXTURE_HANDLE;
using Phoenix::Default::VERTEX_HANDLE;
using Phoenix::Default::INDEX_HANDLE;
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

      TEXTURE_HANDLE   	m_TextureHandle;
      VERTEX_HANDLE    	m_VertexDescriptorHandle;
      INDEX_HANDLE	m_IndexArrayHandle;
      
    public:
      ////////////////////
      /// Constructor.
      CModel();
      ////////////////////
      /// Destructor.
      ~CModel();
      ////////////////////
      /// Returns handle to Texture.
      /// \returns TEXTURE_HANDLE
      TEXTURE_HANDLE    GetTextureHandle( unsigned int nId ) const;
      ////////////////////
      /// Sets texture handle.
      /// \param handle Handle to texture.
      void		SetTextureHandle( unsigned int nId, TEXTURE_HANDLE handle );
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      VERTEX_HANDLE     GetVertexHandle() const;
      ////////////////////
      /// Sets vertex descriptor handle.
      /// \param handle Handle to vertex descriptor.
      void		SetVertexHandle( VERTEX_HANDLE handle );
      ////////////////////
      /// Returns handle to index array.
      /// \returns INDEX_HANDLE.
      INDEX_HANDLE	GetIndexHandle() const;
      ////////////////////
      /// Sets index array handle.
      /// \param handle Handle to index array.
      void		SetIndexHandle( INDEX_HANDLE handle );
    };
  }; // namespace Graphics
}; // namespace Phoenix
#endif
