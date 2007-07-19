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
