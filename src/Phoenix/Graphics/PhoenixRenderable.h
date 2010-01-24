#ifndef __PhoenixRenderable_h__
#define __PhoenixRenderable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixTagged.h"
#include "PhoenixTransform.h"
#include "PhoenixRenderState.h"
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
    /// Renderable interface.
    class PHOENIX_API CRenderable
    {
    protected:
      Phoenix::Graphics::CRenderState	      m_RenderState; 	///<! Renderstate.
      Phoenix::Math::CTransform		      *m_pTransform; 			///<! How renderable should be transformed.
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
      friend std::ostream & Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderable & renderable );
      ////////////////////
      /// Returns renderstate.
      /// \returns Current renderstate settings for renderable.
      Phoenix::Graphics::CRenderState & GetRenderState();
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( COglRenderer & renderer ) = 0;

      Phoenix::Math::CTransform * GetTransform();
      void	   SetTransform( Phoenix::Math::CTransform *pTransform );
      ////////////////////
      /// Sets identifier number for this renderable.
      /// \param iId Identifier number to be set.
      void     SetId( int iId );
      ////////////////////
      /// \returns identifier number of this renderable.
      int      GetId() const;
    };
    std::ostream & operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderable & renderable );
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
