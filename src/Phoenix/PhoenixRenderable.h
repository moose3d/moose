#ifndef __PhoenixRenderable_h__
#define __PhoenixRenderable_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCore.h>
#include <PhoenixDefaultEntities.h>

/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics 
  {
    class COglRenderer;
    ///////////////////
    /// Interface for rendereable objects.
    class CRenderable 
    {
    public:

      virtual ~CRenderable() {}
      ////////////////////
      /// All rendering should be here.
      virtual void		Render( Phoenix::Graphics::COglRenderer & renderer ) = 0;
      ////////////////////
      /// Is object transparent or not.
      /// \returns true for transparency, false otherwise.
      virtual bool  IsTransparent() const = 0 ;
      ////////////////////
      /// Returns texture handle to object.
      /// \param nTexId For which texture unit id should be returned - by default, 0.
      /// \returns Reference to texture handle.
      virtual const Phoenix::Default::TEXTURE_HANDLE &	GetTextureHandle( size_t nTexId = 0) const = 0;
      ////////////////////
      /// Returns shader handle to object.
      /// \returns Reference to shared handle.
      virtual const Phoenix::Default::SHADER_HANDLE  &	GetShaderHandle() const = 0;
      ////////////////////
      /// Returns vertex handle to object.
      /// \returns Reference to vertex handle.
      virtual const Phoenix::Default::VERTEX_HANDLE  &  GetVertexHandle() const = 0;
    };
  } // namespace Graphics
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
