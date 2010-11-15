#ifndef __MooseTexture_h__
#define __MooseTexture_h__
/////////////////////////////////////////////////////////////////
#include "MooseGL.h"
#include "MooseAPI.h"
//#include <MooseRenderable.h>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    enum TEXTURE_TYPE
    {
      TEXTURE_2D,      /* Normal 2D texture */
#if !defined(MOOSE_APPLE_IPHONE)
      TEXTURE_RECT,    /* Rectangular 2D texture */
#endif
      TEXTURE_DEPTH2D, /* 2D depth texture */
      TEXTURE_CUBE     /* Cube texture map */
    };
    /// Texture filter modes.
    enum TEXTURE_FILTER
      {
	ENV_MODULATE = 0,
	ENV_DECAL,
	ENV_BLEND,
	ENV_REPLACE,
	ENV_COMBINE_INCR,
	ENV_COMBINE_REPLACE,
	MIN_NEAREST,
	MIN_LINEAR,
	MIN_MIP_NEAREST,
	MIN_MIP_LINEAR,
	MAG_LINEAR,
	MAG_NEAREST,
	T_WRAP_REPEAT,
	S_WRAP_REPEAT,
	R_WRAP_REPEAT,
	T_WRAP_CLAMP,
	S_WRAP_CLAMP,
	R_WRAP_CLAMP,
	T_WRAP_CLAMP_TO_EDGE,
	S_WRAP_CLAMP_TO_EDGE,
	R_WRAP_CLAMP_TO_EDGE
      };
    /////////////////////////////////////////////////////////////////
    /// OpenGL base class.
    class MOOSE_API COglBase
    {
    protected:
      /// OpenGL id.
      GLuint m_nOglId;
    public:
      ////////////////////
      /// Constructor.
      /// \param nId OpenGL id number.
      COglBase( GLuint nId );
      ////////////////////
      /// Destructor.
      virtual ~COglBase();
      ////////////////////
      /// Returns OpenGL texture id.
      /// \returns Texture id.
      inline GLuint & GetID()
      {
	return m_nOglId;
      }
      ////////////////////
      /// Returns OpenGL texture id.
      /// \returns Texture id.
      inline const GLuint & GetID() const
      {
	return m_nOglId;
      }
    };

    /////////////////////////////////////////////////////////////////
    /// Class for textures.
    typedef unsigned int ResolutionType;
    class MOOSE_API COglTexture : public COglBase
    {
    private:
      TEXTURE_TYPE		m_tTextureType;
      ResolutionType		m_nWidth;
      ResolutionType		m_nHeight;
    public:
      ////////////////////
      /// Constructor.
      /// \param nId OpenGL texture object id.
      /// \param tType TEXTURE_TYPE of this texture.
      COglTexture( unsigned int nId, TEXTURE_TYPE tType );
      ////////////////////
      /// Destructor.
      virtual ~COglTexture();
      ////////////////////
      /// Returns OpenGL texture type.
      /// \returns Texture type.
      TEXTURE_TYPE GetType() const;
      ////////////////////
      /// Returns texture width.
      /// \returns texture width.
      ResolutionType GetWidth() const;
      ////////////////////
      /// Returns texture height.
      /// \returns texture height.
      ResolutionType GetHeight() const;
      ////////////////////
      /// Sets texture width.
      /// \param width Texture width.
      void SetWidth( ResolutionType width );
      ////////////////////
      /// Sets texture height.
      /// \param height Texture height.
      void SetHeight( ResolutionType height );
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
