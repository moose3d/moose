#ifndef __PhoenixRenderable_h__
#define __PhoenixRenderable_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCore.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics 
  {
    enum RENDERABLE_TYPE
    {
      RT_BLENDING = 0,
      RT_ALPHA_TEST,
      RT_VERTEXDESCRIPTOR,
      RT_INDEXARRAY,
      RT_TEXTURE,
      RT_SHADER,
      RT_STATE,
      RT_MATERIAL,
      RT_TRANSFORM
    };
    /////////////////////////////////////////////////////////////////
    /// Base class for all renderable things. Useful when sorting them
    /// in RenderQueue.
    class CRenderable 
    {
    protected: 
      /// Type of this renderable.
      RENDERABLE_TYPE m_tRenderableType;
      ////////////////////
      /// Constructor, forces type.
      /// \param type Type of this renderable.
      CRenderable( RENDERABLE_TYPE type ) : m_tRenderableType(type)
      {
	
      }
    public:
      ////////////////////
      /// Returns type of this renderable.
      /// \returns Type of this renderable.
      RENDERABLE_TYPE GetRenderableType()
      {
	return m_tRenderableType;
      }
    };
  }
}

/////////////////////////////////////////////////////////////////
#endif
