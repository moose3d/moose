#define __STDC_CONSTANT_MACROS 
#include "MooseRenderableVideo.h"
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
    assert( g_TextureMgr->Create(pTexture, g_UniqueNameStr,  GetRenderState().GetTextureHandle()) == 0 );

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
