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
      inline unsigned int GetID() 
      {
	return m_nOglId;
      }
    };
    ////////////////////
    /// 
//     class CRenderBufferObject : public COglBase
//     {
//     public:
//       CRenderBufferObject( unsigned int nRenderBufferId );
//     };
//     /////////////////////////////////////////////////////////////////
//     /// Class for framebuffer objects.
//     class CFrameBufferObject : public COglBase
//     {
//     protected:
//       std::vector<CRenderBufferObject *> m_vecRenderBuffers;
//     public:
//       ////////////////////
//       /// Constructor.
//       CFrameBufferObject( unsigned int nFrameBufferId );
//       ////////////////////
//       /// Destructor.
//       ~CFrameBufferObject();
//       ////////////////////
//       /// Attaches renderbuffers to this framebuffer.
//       //void AttachRenderBuffer( COglBase *pObject );
      
//     };
    /////////////////////////////////////////////////////////////////
    /// Class for textures.
    class COglTexture : public COglBase
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
      ////////////////////
      /// Returns framebuffer.
      /// \returns Reference to framebuffer.
      //Phoenix::Graphics::CFrameBufferObject & GetFrameBuffer();
      ////////////////////
      /// Returns depthbuffer.
      /// \returns Reference to depthbuffer.
      //Phoenix::Graphics::CRenderBufferObject & GetDepthBuffer();
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
