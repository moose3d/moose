#ifndef __PhoenixRenderableModelShared_h__
#define __PhoenixRenderableModelShared_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTexture.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixRenderable.h"
#include "PhoenixModel.h"
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
    ////////////////////
    /// Renderable class for 3D models with texture and vertex data.
    // TODO Add concept of model between renderablemodel and 3D data, and manage it using handles.
    class PHOENIX_API CRenderableModelShared : public Phoenix::Graphics::CRenderable
    {
    protected:
      MODEL_HANDLE m_hModel;
    public:
      ////////////////////
      /// Constructor.
      CRenderableModelShared();
      ////////////////////
      /// Destructor.
      virtual ~CRenderableModelShared();
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModelShared & renderable );
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( COglRenderer & renderer );
      ////////////////////
      /// \returns model handle.
      MODEL_HANDLE & GetModelHandle();
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
