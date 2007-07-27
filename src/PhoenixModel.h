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
/////////////////////////////////////////////////////////////////
using Phoenix::Graphics::CVertexDescriptor;
using Phoenix::Graphics::CIndexArray;
using Phoenix::Graphics::COglTexture;
using Phoenix::Default::TEXTURE_HANDLE;
using Phoenix::Default::VERTEX_HANDLE;
using Phoenix::Default::INDEX_HANDLE;
using std::vector;
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

      TEXTURE_HANDLE   	m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      VERTEX_HANDLE     m_aTextureCoordinateHandles[TEXTURE_HANDLE_COUNT];
      vector<TEXTURE_FILTER> m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      VERTEX_HANDLE    	m_VertexDescriptorHandle;
      INDEX_HANDLE      m_IndexArrayHandle;
      
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
      /// Returns handle to index array.
      /// \returns INDEX_HANDLE.
      INDEX_HANDLE	GetIndexHandle() const;
      ////////////////////
      /// Sets index array handle.
      /// \param handle Handle to index array.
      void		SetIndexHandle( INDEX_HANDLE handle );
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
      void   AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId = 0 );
    };
  }; // namespace Graphics
}; // namespace Phoenix
#endif
