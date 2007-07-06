/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_OglTexture_h__
#define __GSE_OglTexture_h__
/////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <GL/GLee.h>
#include <GL/gl.h>
#include <vector>
#include <list>
#include "GSE_Texture.h"
#include "GSE_structures.h"
#include "GSE_ffmpeg.h"
/////////////////////////////////////////////////////////////////
using std::list;
/////////////////////////////////////////////////////////////////
/// OpenGL texture class.
class GSE_OglTexture : public GSE_Texture
{
 protected:
  /// Texture name.
  GLuint m_nId;
 public:
  /// Default constructor.
  GSE_OglTexture()
  {
    glGenTextures(1, &m_nId);
  }
  /// Destructor.
  virtual ~GSE_OglTexture()
  {
    Destroy();
  }
  /// Destroys texture.
  void Destroy()
  {
    glDeleteTextures(1, &m_nId);
  }
  /// Returns texture name.
  GLuint &GetId()
  {
    return m_nId;
  }
};
/////////////////////////////////////////////////////////////////
//typedef GSE_OglTexture<GL_TEXTURE_1D, GLuint> GSE_OglTexture1D;
//typedef GSE_OglTexture<GL_TEXTURE_2D, GLuint> GSE_OglTexture2D;
//typedef GSE_OglTexture<GL_TEXTURE_3D, GLuint> GSE_OglTexture3D;
//typedef GSE_OglTexture<GL_TEXTURE_RECTANGLE_ARB, GLuint> GSE_OglTextureRectangle;
/////////////////////////////////////////////////////////////////
/// /brief 2D Video texture class. Provides a updateable texture which displays video.
class GSE_OglVideoTexture : public GSE_OglTexture,
			    public GSE_FFMpegStream
			    
{
  // Since our manager must be able to create VideoTextures as well
  friend class GSE_OglTextureMgr;
 protected:
  /// Framebuffer name.
  GLuint m_FrameBuffer;
  /// Is frame_buffer_object extension supported.
  int    m_bFrameBufferObjectSupport;
  /// The time which is left in current frame.
  float m_fRemainingTimeInFrame;
  /// The parametrized constructor.
  GSE_OglVideoTexture( char *sFilename );
  /// Initializes texture.
  void Init();
 public:

  /// Destructor.
  ~GSE_OglVideoTexture();
  /// Updates the video texture to display correct frame.
  /// \param nPassedTimeMS passed time in milliseconds.
  void Update( unsigned int nPassedTimeMS );
  /// Destroys texture.
  void Destroy();  

};
/////////////////////////////////////////////////////////////////
/// OpenGL texture manager class.
class GSE_OglTextureMgr : public GSE_TextureMgrInterface,
			  public GSE_Singleton<GSE_OglTextureMgr>,
			  public GSE_Container<GSE_Texture>
{
  // Singleton needs to be our friend
  friend class GSE_Singleton<GSE_OglTextureMgr>;

 private:
  ////////////////////
  /// Constructor.
  GSE_OglTextureMgr() {}
  ////////////////////
  // Deconstructor.
  virtual  ~GSE_OglTextureMgr();

  /// Loads texture into SDL_Surface.
  SDL_Surface *LoadImageIntoSDL_Surface( const char *pPath, 
					 char bUsePowerOfTwo,
					 int *pbHasAlphaChannel = NULL);

 public:
  ////////////////////
  ///  Returns reference to the value in the m_Textures[index]
  GSE_Texture & operator[](unsigned int iIndex);
  ////////////////////
  /// Loads Texture from file using SDL and adds it to the texture array.
  /// Returns pointer to the texture object.
  /// On error, NULL is returned.
  GSE_Texture *	CreateTextureFromImage( const char *pFilename, 
					char bUseAlpha, 
					TextureType::Type iTextureType,
					TextureQuality::Type iQuality);

  /// Creates empty texture with (iWidth * iHeight) pixels.
  /// \returns a pointer to created texture.
  GSE_Texture *	CreateEmptyTexture( int iWidth, int iHeight, 
				    char bUseAlpha, 
				    TextureType::Type iTextureType,
				    TextureQuality::Type iQuality);


  /// Creates an updateable texture which displays video stream.
  /// \param pFilename video filename.
  /// \returns a pointer to created texture.
  GSE_Texture * CreateTextureFromVideo( char *pFilename );

  /// Create a texture from rendered image.
  /// \param iStartX x-coordinate where image starts.
  /// \param iStartY y-coordinate where image starts.
  /// \param nWidth width of image.
  /// \param nHeight height of image.
  /// \param bUseAlpha should alpha channel be used.
  /// \param iType Texture type.
  /// \param iQuality Texture quality hint.
  /// \return A pointer to created texture.
  GSE_Texture *CreateTextureFromScreen( int iStartX, 
					int iStartY,
					unsigned int nWidth,
					unsigned int nHeight,
					char bUseAlpha,
					TextureType::Type iType,
					TextureQuality::Type iQuality);
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// history:
//
//		$Log: GSE_OglTexture.h,v $
//		Revision 1.10  2007/06/04 09:03:28  entity
//		GLee fix
//
//		Revision 1.9  2007/06/01 08:55:43  entity
//		doxygen comments added
//
//		Revision 1.8  2007/05/21 18:34:36  entity
//		const stuff
//
//		Revision 1.7  2007/05/21 12:50:00  entity
//		glew to GLee
//
//		Revision 1.6  2007/05/17 19:29:46  entity
//		videotexture updates
//
//		Revision 1.5  2007/05/17 19:28:05  entity
//		VideoTexture updates
//
//		Revision 1.4  2007/05/16 12:59:38  entity
//		Video buffer can be accessed via texture now
//
//		Revision 1.3  2007/04/03 12:53:57  entity
//		texture quality
//
//		Revision 1.2  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.1  2007/03/31 18:56:26  entity
//		renamed
//
//		Revision 1.6  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.5  2006/12/21 09:10:32  entity
//		RenderToTexture feature added
//
//		Revision 1.4  2006/10/13 14:41:20  entity
//		some code cleanups
//
//		Revision 1.3  2006/10/10 08:27:42  entity
//		header fixes
//
/////////////////////////////////////////////////////////////////
