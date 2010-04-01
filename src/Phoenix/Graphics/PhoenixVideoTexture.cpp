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
#include <GL/GLee.h>
#include <iostream>
#include <math.h>
#include <cassert>
#include "PhoenixVideoTexture.h"
/////////////////////////////////////////////////////////////////
using namespace std;
namespace prefix = Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
prefix::COglVideoTexture::COglVideoTexture( unsigned int id, const char *sFilename) : COglTexture(id, TEXTURE_VIDEO),CFFMpegStream()
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
prefix::COglVideoTexture::~COglVideoTexture()
{
  Destroy();
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::COglVideoTexture::Init()
{

  GetNextFrame();
  // Create rectangular texture (RGB) to match video frame size
  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, GetID());
  glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 3,  
		m_pCodecCtx->width, m_pCodecCtx->height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

  if ( m_bFrameBufferObjectSupport )
  {
    glGenFramebuffersEXT(1,&m_FrameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
			      GL_TEXTURE_RECTANGLE_ARB, GetID(), 0);
    
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    assert( status == GL_FRAMEBUFFER_COMPLETE_EXT && "OpenGL framebuffer was not complete!");
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
  }  

  glDisable(GL_TEXTURE_RECTANGLE_ARB );

  Update(0);
  

}
/////////////////////////////////////////////////////////////////
void
prefix::COglVideoTexture::Update( unsigned int nPassedTimeMS)
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
    //glRasterPos2i( 0, CScreen::GetScreen()->m_OglScreenParams.m_iHeight );
    glPixelZoom(1.0,-1.0f);      
    // draw m_pBuffer contentst on framebuffer
    glDrawPixels( m_pCodecCtx->width, m_pCodecCtx->height,
		  GL_RGB, GL_UNSIGNED_BYTE, m_pBuffer );
    glPixelZoom(1.0,1.0f);      
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
  } 
  else 
  {
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, GetID());
    glViewport(0,0,m_pCodecCtx->width, m_pCodecCtx->height );
    glRasterPos2i( 0, m_pCodecCtx->height /*CScreen::GetScreen()->m_OglScreenParams.m_iHeight*/ );
      
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
    

}// COglVideoTexture::Update
/////////////////////////////////////////////////////////////////
//CTexture *
//COglTextureMgr::CreateTextureFromVideo( char *pFilename)
//{
//
//  COglVideoTexture *pVidTex = new COglVideoTexture( pFilename);
//  Add(pVidTex);
//  return pVidTex;
//
//}
/////////////////////////////////////////////////////////////////
void 
prefix::COglVideoTexture::Destroy()
{
  CFFMpegStream::Destroy();
  if ( m_bFrameBufferObjectSupport )
  {
    glDeleteFramebuffersEXT(1, &m_FrameBuffer);
  }
  delete m_pBuffer;
}
/////////////////////////////////////////////////////////////////
