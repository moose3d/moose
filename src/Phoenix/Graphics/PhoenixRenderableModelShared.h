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
namespace Phoenix
{
  namespace Graphics
  {
    class COglRenderer;
    ////////////////////
    /// Renderable class for 3D models with texture and vertex data.
    class PHOENIX_API CRenderableModelShared : public Phoenix::Graphics::CRenderable
    {
    protected:
      Phoenix::Default::MODEL_HANDLE m_hModel;
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
      friend std::ostream & Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModelShared & renderable );
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( Phoenix::Graphics::COglRenderer & renderer );
      ////////////////////
      /// \returns model handle.
      Phoenix::Default::MODEL_HANDLE & GetModelHandle();
    };
    std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModelShared & renderable );

  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
