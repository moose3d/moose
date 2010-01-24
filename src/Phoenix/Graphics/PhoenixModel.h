#ifndef __PhoenixModel_h__
#define __PhoenixModel_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTexture.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixRenderable.h"
#include "PhoenixAPI.h"
#include <vector>
#include <iostream>
#include <utility>
#include <map>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
  	////////////////////
    /// class for 3D models with texture and vertex data.
    class PHOENIX_API CModel
    {
    protected:
      /// Handle to texture coordinates.
  	  Phoenix::Default::VERTEX_HANDLE			      m_aTextureCoordinateHandles[TEXTURE_HANDLE_COUNT];
      /// Handle to vertices. (position data)
  	  Phoenix::Default::VERTEX_HANDLE			      m_VertexDescriptorHandle;
      /// Handle to vertex normals.
  	  Phoenix::Default::VERTEX_HANDLE			      m_VertexNormalHandle;
      /// Handle to vertex colors.
  	  Phoenix::Default::VERTEX_HANDLE			      m_VertexColorHandle;
      /// Handle to index arrays for triangle list and/or Triangle strips.
      Phoenix::Default::INDEX_HANDLE			      m_hIndices;


    public:
      ////////////////////
      /// Constructor.
      CModel();
      ////////////////////
      /// For duplicating properly.
      /// \param model Existing model.
      CModel( const CModel & model );
      ////////////////////
      /// Destructor.
      virtual ~CModel();

      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Phoenix::Default::VERTEX_HANDLE     & GetVertexHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Phoenix::Default::VERTEX_HANDLE    & GetNormalHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Phoenix::Default::VERTEX_HANDLE    & GetColorHandle();
      ////////////////////
      /// Returns reference to a handle with triangle lists indices .
      /// \returns INDEX_HANDLE.
      Phoenix::Default::INDEX_HANDLE &	GetIndices();
      ////////////////////
      /// Returns handle to Texture Coordinate vertex descriptor.
      /// \param nId From which texture unit these coordinates are retrieved. By default, the first one (zero).
      /// \returns VERTEX_HANDLE.
      Phoenix::Default::VERTEX_HANDLE & GetTextureCoordinateHandle( unsigned int nId = 0 );
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CModel & model );

    };

  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
