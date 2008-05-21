#ifndef __PhoenixTexture_h__
#define __PhoenixTexture_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixRenderable.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    enum TEXTURE_TYPE 
    {
      TEXTURE_2D = 0,
      TEXTURE_RECT = 1,
      TEXTURE_DEPTH2D
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
	T_WRAP_CLAMP,
	S_WRAP_CLAMP,
	T_WRAP_CLAMP_TO_EDGE,
	S_WRAP_CLAMP_TO_EDGE
      };
    /////////////////////////////////////////////////////////////////
    /// OpenGL base class.
    class COglBase
    {
    protected:
      /// OpenGL id.
      unsigned int m_nOglId;
    public:
      ////////////////////
      /// Constructor.
      /// \param nId OpenGL id number.
      COglBase( unsigned int nId );
      ////////////////////
      /// Returns OpenGL texture id.
      /// \returns Texture id.
      inline unsigned int & GetID() 
      {
	return m_nOglId;
      }
      ////////////////////
      /// Returns OpenGL texture id.
      /// \returns Texture id.
      inline const unsigned int & GetID() const
      {
	return m_nOglId;
      }
    };

    /////////////////////////////////////////////////////////////////
    /// Class for textures.
    class COglTexture : public COglBase,
			public Phoenix::Graphics::CRenderable
    {
    private:
      TEXTURE_TYPE		m_tTextureType;
    public:
      ////////////////////
      /// Constructor.
      /// \param nId OpenGL texture object id.
      /// \param tType TEXTURE_TYPE of this texture.
      COglTexture( unsigned int nId, TEXTURE_TYPE tType );
      ////////////////////
      /// Destructor.
      ~COglTexture();
      ////////////////////
      /// Returns OpenGL texture type.
      /// \returns Texture type.
      TEXTURE_TYPE GetType() const;
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
