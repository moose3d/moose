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
      virtual void		Render( Phoenix::Graphics::COglRenderer & renderer ) = 0;
      virtual bool		IsTransparent() = 0;
      //virtual Phoenix::Default::TEXTURE_HANDLE	GetTextureHandle( size_t nTexId = 0) = 0;
      //virtual Phoenix::Default::SHADER_HANDLE	GetShaderHandle() = 0;
    };
  } // namespace Graphics
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
