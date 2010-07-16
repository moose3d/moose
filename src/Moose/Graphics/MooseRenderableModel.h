#ifndef __MooseRenderableModel_h__
#define __MooseRenderableModel_h__
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
    class MOOSE_API CRenderableModel: public Moose::Graphics::CRenderable
    {
    protected:
      Moose::Graphics::CModel * m_pModel;
        bool                      m_bDestroyModel; ///!< Whether to destroy model when renderablemodel is destroyed.
    public:
      ////////////////////
      /// Constructor.
      CRenderableModel();
      ////////////////////
      /// Destructor.
      virtual ~CRenderableModel();
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModel & renderable );
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( Moose::Graphics::COglRenderer & renderer );
      ////////////////////
      /// \returns model handle.
      Moose::Graphics::CModel * GetModel();
      ////////////////////
      /// Sets model.
      void SetModel( Moose::Graphics::CModel *pModel );
      void SetDestroyModel( bool bValue );
    };
    std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModel & renderable );
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
