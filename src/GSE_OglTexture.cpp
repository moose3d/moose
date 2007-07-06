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
#include <SDL.h>
#include <SDL_image.h>
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include "GSE_Screen.h"
#include "GSE_OglTexture.h"
#include "GSE_Logger.h"
#include "GSE_OglUtils.h"
#include "GSE_debug.h"
/////////////////////////////////////////////////////////////////
using namespace std;
using namespace Core;
/////////////////////////////////////////////////////////////////
GSE_OglTextureMgr::~GSE_OglTextureMgr()
{
  //GSE_Logger::Error() << "GSE_OglTextureMgr destructor.\n";
  GSE_Container<GSE_Texture>::Clear();
}
/////////////////////////////////////////////////////////////////
void ProcessTexture( void *pPixelData, int bMipMap, 
		     GLint iInternalFormat, 
		     GSE_OglTexture *pTexture,
		     int iWidth, int iHeight    )
{

  switch( pTexture->GetType() )
  {
  case TextureType::GSE_TEX1D:
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,pTexture->GetId());
    if ( bMipMap )
    {
      gluBuild1DMipmaps(GL_TEXTURE_1D, iInternalFormat, 
			iWidth > iHeight ? iWidth : iHeight, GL_RGBA, 
			GL_UNSIGNED_BYTE, pPixelData);
    } 
    else 
    {
      glTexImage1D( GL_TEXTURE_1D, 0, iInternalFormat, 
		    iWidth > iHeight ? iWidth : iHeight,0,
		    GL_RGBA,  GL_UNSIGNED_BYTE, pPixelData);
    }
    
    glDisable(GL_TEXTURE_1D);
    break;
  case TextureType::GSE_TEX2D:
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pTexture->GetId());
    if ( bMipMap )
    {
      gluBuild2DMipmaps(GL_TEXTURE_2D, iInternalFormat, iWidth, iHeight, 
			GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);
    }
    else
    {
      glTexImage2D( GL_TEXTURE_2D, 0, iInternalFormat, iWidth, iHeight,0,
		    GL_RGBA, GL_UNSIGNED_BYTE, pPixelData );
    }
    glDisable(GL_TEXTURE_2D);
    break;
  case TextureType::GSE_TEXR2D:
    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB,pTexture->GetId());
    glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, iInternalFormat,
		  iWidth, iHeight, 0, GL_RGBA,
		  GL_UNSIGNED_BYTE, pPixelData );
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    break;
  }
  
}
/////////////////////////////////////////////////////////////////
GSE_Texture *
GSE_OglTextureMgr::CreateTextureFromImage( const char *pFilename, 
					   char bUseAlpha, 
					   TextureType::Type iTextureType,
					   TextureQuality::Type iQuality)
{
  int bImageHasAlphaChannel = 0;


  SDL_Surface *pSurface = NULL;
  // Load image and determine does it have alpha channel
  pSurface = LoadImageIntoSDL_Surface(pFilename, 
				      iTextureType == TextureType::GSE_TEX2D, 
				      &bImageHasAlphaChannel);        

  GSE_OglTexture *pOglTexture = NULL;
  if ( pSurface != NULL ) 
  {
    
    int		iWidth   = pSurface->w;
    int		iHeight  = pSurface->h;

    GLint	iInternalFormat = (iQuality == TextureQuality::QUALITY) ? GL_RGB : GL_RGB4;
    // if alpha channel is enabled, texture format is RGBA
    if ( bImageHasAlphaChannel && bUseAlpha ) 
    { 
      iInternalFormat = (iQuality == TextureQuality::QUALITY) ? GL_RGBA : GL_RGBA4;
    }
    pOglTexture = new GSE_OglTexture();
    pOglTexture->SetType(iTextureType);
    ProcessTexture( pSurface->pixels, 1, iInternalFormat,
		    pOglTexture, iWidth, iHeight);
    Add(pOglTexture);
    ////////////////////
    if ( bImageHasAlphaChannel && bUseAlpha ) 
    {
      pOglTexture->SetTransparentHint(1);
    }
    ////////////////////
    SDL_FreeSurface(pSurface);
    pSurface = NULL;  

  } 
  else 
  {
    GSE_ERR("Psurface == NULL");
  }


  return static_cast<GSE_Texture *>(pOglTexture);
}  
/////////////////////////////////////////////////////////////////
GSE_Texture *
GSE_OglTextureMgr::CreateEmptyTexture(int iWidth, int iHeight, 
				      char bUseAlpha, 
				      TextureType::Type iTextureType,
				      TextureQuality::Type iQuality)
{
  
  GSE_OglTexture *pTexture = new GSE_OglTexture();
  GLint	iInternalFormat = (iQuality == TextureQuality::QUALITY) ? 3 : GL_RGB4;
  // if alpha channel is enabled, texture format is RGBA
  if ( bUseAlpha ) 
  { 
    iInternalFormat = (iQuality == TextureQuality::QUALITY) ? 4 : GL_RGBA4;
  }

  pTexture->SetType(iTextureType);
  ProcessTexture( NULL, 0, iInternalFormat, pTexture, iWidth, iHeight );
  Add(pTexture);
  if ( bUseAlpha ) 
  { 
    pTexture->SetTransparentHint(1);
  }
  return static_cast<GSE_Texture *>(pTexture);
}
/////////////////////////////////////////////////////////////////
GSE_Texture *
GSE_OglTextureMgr::CreateTextureFromScreen( int iStartX, 
					    int iStartY,
					    unsigned int nWidth,
					    unsigned int nHeight,
					    char bUseAlpha,
					    TextureType::Type iTextureType,
					    TextureQuality::Type iQuality)
{
  
  GSE_OglTexture *pTexture = NULL;
  GLint	iInternalFormat = (iQuality == TextureQuality::QUALITY) ? 3 : GL_RGB4;
  // if alpha channel is enabled, texture format is RGBA
  if ( bUseAlpha ) 
  { 
    iInternalFormat = (iQuality == TextureQuality::QUALITY) ? 4 : GL_RGBA4;
  }

  pTexture = new GSE_OglTexture();
  pTexture->SetType(iTextureType);
  ProcessTexture( NULL, 0, iInternalFormat, pTexture, nWidth, nHeight);
  Add(pTexture);

  switch ( iTextureType ){
  case TextureType::GSE_TEX1D:
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,pTexture->GetId());
    glCopyTexSubImage1D( GL_TEXTURE_1D, 0, 0, iStartX, iStartY, nWidth > nHeight ? nWidth : nHeight );
    glDisable(GL_TEXTURE_1D);
    break;
  case TextureType::GSE_TEX2D:
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pTexture->GetId());
    glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0,0, 
			 iStartX, iStartY, nWidth, nHeight );
    glDisable(GL_TEXTURE_2D);
    break;
  case TextureType::GSE_TEXR2D:
    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, pTexture->GetId());
    glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 
			 iStartX, iStartY, nWidth, nHeight );
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    break;
  }
  // If alpha channel is used, we hint the
  if ( bUseAlpha ) 
  { 
    pTexture->SetTransparentHint(1);
  }

  return static_cast<GSE_Texture *>(pTexture);
}
/////////////////////////////////////////////////////////////////
// Loads an image into a SDL_Surface
// Returns the SDL_Surface containing the image.
SDL_Surface *
GSE_OglTextureMgr::LoadImageIntoSDL_Surface( const char *pPath, 
					     char bUsePowerOfTwo,
					     int *pbHasAlphaChannel)
{
  // 
  // check byte order so correct mask can be applied.
  //
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int iRedMask   = 0xff000000;
  int iGreenMask = 0x00ff0000;
  int iBlueMask  = 0x0000ff00;
  int iAlphaMask = 0x000000ff;
#else
  int iRedMask   = 0x000000ff;
  int iGreenMask = 0x0000ff00;
  int iBlueMask  = 0x00ff0000;
  int iAlphaMask = 0xff000000;
#endif
  int iBpp    = 32;
  int iWidth  = 0;
  int iHeight = 0;

  SDL_Surface *pTmpSurface  = IMG_Load(pPath);

  int bImageHasAlphaChannel = 0;

  if ( pTmpSurface != NULL ) 
  {
    bImageHasAlphaChannel = (pTmpSurface->flags & (SDL_SRCALPHA | SDL_SRCCOLORKEY));
  }
  // Set the information about alpha channel
  if ( pbHasAlphaChannel != NULL )
  {
    *pbHasAlphaChannel = bImageHasAlphaChannel;
  }

  SDL_Surface *pRGBASurface = NULL;
  
  if ( pTmpSurface == 0 )
  {
    GSE_ERR( "IMG_Load error: " << IMG_GetError() );
  } 
  else 
  {

    if ( bUsePowerOfTwo )
    {
      float fLog2Height = log(pTmpSurface->h)/log(2);
      float fLog2Width  = log(pTmpSurface->w)/log(2);

      // Rounding errors might cause nasty effects
      if ( (fLog2Height - (int)fLog2Height) < 0.00001 )
      {
	fLog2Height = (int)fLog2Height;
      }
      if ( (fLog2Width - (int)fLog2Width)   < 0.00001 ) 
      {
	fLog2Width  = (int)fLog2Width;
      }
      // round dimensions up to closest power of two
      iWidth  = (int)pow(2, ceil(fLog2Width));
      iHeight = (int)pow(2, ceil(fLog2Height));

    }
    else
    {
      iWidth = pTmpSurface->w;
      iHeight = pTmpSurface->h;
    }

    // create new surface
    pRGBASurface = SDL_CreateRGBSurface(SDL_HWSURFACE,iWidth,iHeight,iBpp,
					iRedMask, iGreenMask, iBlueMask,
					iAlphaMask);
    if ( pRGBASurface == NULL )
    {
      GSE_ERR("SDL_CreateRGBSurface() failed." );
    }
    else
    {
      // blit loaded image into newly created SDL_Surface
      SDL_BlitSurface( pTmpSurface, 0, pRGBASurface, 0); 
    }
    // Cleanup; free the surface of the temporary image 
    SDL_FreeSurface( pTmpSurface);
    pTmpSurface = 0;

  }
  
  return pRGBASurface;
}
/////////////////////////////////////////////////////////////////
GSE_OglVideoTexture::GSE_OglVideoTexture( char *sFilename) : GSE_OglTexture(),GSE_FFMpegStream()
{
  m_bFrameBufferObjectSupport = (GLEE_EXT_framebuffer_object != 0);
  
  if ( m_bFrameBufferObjectSupport )
  {
    cerr << "GL_framebuffer_object is supported, yay!" << endl;
  }
  SetStream(sFilename);
  Init();
}
/////////////////////////////////////////////////////////////////
GSE_OglVideoTexture::~GSE_OglVideoTexture()
{
  Destroy();
}
void
GSE_OglVideoTexture::Init()
{

  GetNextFrame();
  // Create rectangular texture (RGB) to match video frame size
  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, GetId());
  glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 3,  
		m_pCodecCtx->width, m_pCodecCtx->height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

  if ( m_bFrameBufferObjectSupport )
  {
    glGenFramebuffersEXT(1,&m_FrameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
			      GL_TEXTURE_RECTANGLE_ARB, GetId(), 0);
    
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    assert( status == GL_FRAMEBUFFER_COMPLETE_EXT && "OpenGL framebuffer was not complete!");
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
  }  

  glDisable(GL_TEXTURE_RECTANGLE_ARB );

  Update(0);
  SetType( TextureType::GSE_TEXR2D );
  
}
/////////////////////////////////////////////////////////////////
void
GSE_OglVideoTexture::Update( unsigned int nPassedTimeMS)
{

  if ( !IsPlaying() )
  {
    return;
  } 
  
  SeekNextFrameFromStream( nPassedTimeMS );

  if ( m_bFrameBufferObjectSupport )
  {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_FrameBuffer);
    // Set viewport
    glViewport(0,0,m_pCodecCtx->width, m_pCodecCtx->height );
    // Set raster position
    glRasterPos2i( 0, GSE_Screen::GetScreen()->m_OglScreenParams.m_iHeight );
    glPixelZoom(1.0,-1.0f);      
    // draw m_pBuffer contentst on framebuffer
    glDrawPixels( m_pCodecCtx->width, m_pCodecCtx->height,
		  GL_RGB, GL_UNSIGNED_BYTE, m_pBuffer );
    glPixelZoom(1.0,1.0f);      
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
  } 
  else 
  {
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, GetId());
    glViewport(0,0,m_pCodecCtx->width, m_pCodecCtx->height );
    glRasterPos2i( 0, m_pCodecCtx->height /*GSE_Screen::GetScreen()->m_OglScreenParams.m_iHeight*/ );
      
    glPixelZoom(1.0,-1.0f);
    // draw m_pBuffer contents on framebuffer
    glDrawPixels( m_pCodecCtx->width, 
		  m_pCodecCtx->height,
		  GL_RGB, GL_UNSIGNED_BYTE, m_pBuffer );
    glPixelZoom(1.0,1.0f);      
    glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 
			 0,0, 0,0, 
			 m_pCodecCtx->width, m_pCodecCtx->height);
  }
    

}// GSE_OglVideoTexture::Update 
/////////////////////////////////////////////////////////////////
GSE_Texture * 
GSE_OglTextureMgr::CreateTextureFromVideo( char *pFilename)
{
  
  GSE_OglVideoTexture *pVidTex = new GSE_OglVideoTexture( pFilename);
  Add(pVidTex);
  return pVidTex;

}
/////////////////////////////////////////////////////////////////
void 
GSE_OglVideoTexture::Destroy()
{
  GSE_FFMpegStream::Destroy();
  GSE_OglTexture::Destroy();
  if ( m_bFrameBufferObjectSupport )
  {
    glDeleteFramebuffersEXT(1, &m_FrameBuffer);
  }
  delete m_pBuffer;
}
/////////////////////////////////////////////////////////////////
// history:    
//
//		$Log: GSE_OglTexture.cpp,v $
//		Revision 1.15  2007/06/04 09:03:34  entity
//		GLee fix
//
//		Revision 1.14  2007/05/21 18:34:36  entity
//		const stuff
//
//		Revision 1.13  2007/05/21 12:50:00  entity
//		glew to GLee
//
//		Revision 1.12  2007/05/18 17:25:08  entity
//		ATI fix: framebuffer is disabled after creation
//
//		Revision 1.11  2007/05/18 10:24:05  entity
//		playable and stoppable stream
//
//		Revision 1.10  2007/05/18 06:17:05  entity
//		framebuffer gets deleted with texture now
//
//		Revision 1.9  2007/05/17 19:29:56  entity
//		videotexture updates
//
//		Revision 1.8  2007/05/16 22:21:09  entity
//		stream doesn't segfault now when it ends
//
//		Revision 1.7  2007/05/16 19:55:15  entity
//		working FBO support with fallback to glCopyTexSubImage2D
//
//		Revision 1.6  2007/05/16 13:00:50  entity
//		VideoTexture with initial FBO support, fallback into glCopyTexSubImage
//
//		Revision 1.5  2007/04/03 12:53:57  entity
//		texture quality
//
//		Revision 1.4  2007/04/01 21:05:50  entity
//		separated textures from material
//
//		Revision 1.3  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.2  2007/04/01 08:17:54  entity
//		some constant renames
//
//		Revision 1.1  2007/03/31 18:56:14  entity
//		renamed
//
//		Revision 1.9  2007/03/28 21:09:29  entity
//		unnecessary #ifdef WIN32 removed
//
//		Revision 1.8  2007/03/22 19:54:00  entity
//		texture flipping disabled
//
//		Revision 1.7  2007/03/14 10:57:00  entity
//		internal texture size change GL_RGB4
//
//		Revision 1.6  2007/03/13 13:57:49  entity
//		alpha channel to textures only if we really want is
//
//		Revision 1.5  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.4  2006/12/21 09:10:32  entity
//		RenderToTexture feature added
//
//		Revision 1.3  2006/10/13 14:50:28  entity
//		Code cleanups
//
//		Revision 1.2  2006/10/10 08:20:52  entity
//		added few comments
//
//
//		10082006  AG/eNtity : added CreatePlainTexture()
//		way back AG/eNtity : initial version
/////////////////////////////////////////////////////////////////
