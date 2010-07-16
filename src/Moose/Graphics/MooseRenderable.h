#ifndef __MooseRenderable_h__
#define __MooseRenderable_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseTagged.h"
#include "MooseTransform.h"
#include "MooseRenderState.h"
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
    /// Renderable interface.
    class MOOSE_API CRenderable
    {
    protected:
      Moose::Graphics::CRenderState	      m_RenderState; 	///<! Renderstate.
      Moose::Math::CTransform		      *m_pTransform; 			///<! How renderable should be transformed.
      int 																m_iId; 						///<! Identifier for marking renderables in some manner. Comes handy with CRenderableProperty and CGameObject.
    public:
      ////////////////////
      /// Constructor.
      CRenderable();
      ////////////////////
      /// Destructor.
      virtual ~CRenderable();
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderable & renderable );
      ////////////////////
      /// Returns renderstate.
      /// \returns Current renderstate settings for renderable.
      Moose::Graphics::CRenderState & GetRenderState();
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( COglRenderer & renderer ) = 0;

      Moose::Math::CTransform * GetTransform();
      void	   SetTransform( Moose::Math::CTransform *pTransform );
      ////////////////////
      /// Sets identifier number for this renderable.
      /// \param iId Identifier number to be set.
      void     SetId( int iId );
      ////////////////////
      /// \returns identifier number of this renderable.
      int      GetId() const;
    };
    std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CRenderable & renderable );
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
