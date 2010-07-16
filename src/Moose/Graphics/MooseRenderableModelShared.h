#ifndef __MooseRenderableModelShared_h__
#define __MooseRenderableModelShared_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseResourceManager.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTexture.h"
#include "MooseDefaultEntities.h"
#include "MooseRenderable.h"
#include "MooseModel.h"
#include "MooseAPI.h"
#include <vector>
#include <iostream>
#include <utility>
#include <map>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    class COglRenderer;
    ////////////////////
    /// Renderable class for 3D models with texture and vertex data.
    class MOOSE_API CRenderableModelShared : public Moose::Graphics::CRenderable
    {
    protected:
      Moose::Default::MODEL_HANDLE m_hModel;
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
      friend std::ostream & Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModelShared & renderable );
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( Moose::Graphics::COglRenderer & renderer );
      ////////////////////
      /// \returns model handle.
      Moose::Default::MODEL_HANDLE & GetModelHandle();
    };
    std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModelShared & renderable );

  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
