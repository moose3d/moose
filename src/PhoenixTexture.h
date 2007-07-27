#ifndef __PhoenixTexture_h__
#define __PhoenixTexture_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    enum TEXTURE_TYPE 
    {
      TEXTURE_2D = 0
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
    ////////////////////    
    /// Class for textures.
    class COglTexture
    {
    private:
      unsigned int m_nOglId;
      TEXTURE_TYPE m_tTextureType;
    public:
      COglTexture( unsigned int nId, TEXTURE_TYPE tType );
      ~COglTexture();
      unsigned int GetID() const;
      TEXTURE_TYPE GetType() const;
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
