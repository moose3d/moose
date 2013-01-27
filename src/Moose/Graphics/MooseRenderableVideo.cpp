/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseRenderableVideo.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseVideoTexture.h"
#include "MooseRenderable.h"
#include "MooseDefaultEntities.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Default;
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
prefix::CRenderableVideo::CRenderableVideo( const std::string & strFilename )
{
    SetStream(strFilename.c_str());
    //SeekNextFrameFromStream(0);
    COglRenderer r;

    COglTexture *pTexture = r.CreateTexture(GetWidth(), GetHeight(), TEXTURE_RECT );
    assert( pTexture != NULL );
    int iRetval = g_TextureMgr->Create(pTexture, g_UniqueNameStr,  GetRenderState().GetTextureHandle());
    assert( iRetval == 0 );

    m_pFrameBufferObj = r.CreateFramebuffer( GetWidth(), GetHeight(), FBO_COLOR_BUFFER );
    r.AttachTextureToFramebuffer(*m_pFrameBufferObj, GetRenderState().GetTextureHandle(), 0 );
}
///////////////////////////////////////////////////////////////////////////////
prefix::CRenderableVideo::~CRenderableVideo()
{
    delete m_pFrameBufferObj;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CRenderableVideo::Render( Moose::Graphics::COglRenderer & r )
{

    r.CommitRenderState(GetRenderState());
    glBegin(GL_QUADS);

       glTexCoord2f(0.0f,                  0.0f);                  glVertex2f(0.0f, 0.0f);
       glTexCoord2f(m_pCodecCtx->width,    0.0f);                  glVertex2f(1.0f, 0.0f);
       glTexCoord2f(m_pCodecCtx->width,    m_pCodecCtx->height);   glVertex2f(1.0f, 1.0f);
       glTexCoord2f(0.0f,                  m_pCodecCtx->height);   glVertex2f(0.0f, 1.0f);

    glEnd();

}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CRenderableVideo::Update( float fSeconds )
{
     // decode next frame from the stream.
     SeekNextFrameFromStream(static_cast<int>(fSeconds*1000.0f));
     //UpdateBuffer();
     // prepare frame buffer for rendering
     m_Renderer.CommitFrameBufferSingle(*m_pFrameBufferObj, 0);
     // flip image vertically
     glRasterPos2i( 0, 240);
     glPixelZoom(1,-1);
     //glRasterPos2i(0,0);
     // put pixels into buffer
     glDrawPixels(GetWidth(),GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, GetBuffer());
     // restore things back to normal
     glPixelZoom(1,1);
     m_Renderer.RollbackFrameBuffer( *m_pFrameBufferObj );
}
///////////////////////////////////////////////////////////////////////////////
#endif
