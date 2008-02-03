#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using std::cerr;
using std::endl; 
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int g_bGlow = 0;
const float g_fRotationSpeedPerSec = 56.15f;
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 800;

const unsigned int FRAMEBUFFER_WIDTH =256;
const unsigned int FRAMEBUFFER_HEIGHT = (unsigned int)(FRAMEBUFFER_WIDTH*((float)SCREEN_HEIGHT/(float)SCREEN_WIDTH));


/////////////////////////////////////////////////////////////////
int main()
{
  
  CSDLScreen::m_SDLScreenParams.m_iWidth = SCREEN_WIDTH;
  CSDLScreen::m_SDLScreenParams.m_iHeight= SCREEN_HEIGHT;
  CSDLScreen::m_SDLScreenParams.m_iVideoModeFlags |= SDL_FULLSCREEN;

  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,43.0f);
  camera.SetViewport( 0,0, SCREEN_WIDTH, SCREEN_HEIGHT );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 500.0f );
  camera.SetFieldOfView( 43.0f);

  
  CCamera cameraScreenQuad;
  cameraScreenQuad.SetPosition( 0, 0.0f,0.0f);
  cameraScreenQuad.SetViewport( 0,0, SCREEN_WIDTH,SCREEN_HEIGHT );
  cameraScreenQuad.SetNearClipping( -2);
  cameraScreenQuad.SetFarClipping( 20 );
  cameraScreenQuad.SetViewOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

  CCamera cameraFBOQuad;
  cameraFBOQuad.SetPosition( 0, 0.0f,0.0f);
  cameraFBOQuad.SetViewport( 0,0, FRAMEBUFFER_WIDTH,FRAMEBUFFER_HEIGHT );
  cameraFBOQuad.SetNearClipping( -2);
  cameraFBOQuad.SetFarClipping( 20 );
  cameraFBOQuad.SetViewOrtho( 0, FRAMEBUFFER_WIDTH, 0, FRAMEBUFFER_HEIGHT);

  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();

  Phoenix::Data::CMilkshapeLoader loader;
  assert ( loader.Load( std::string("./Resources/Models/1701-e.ms3d") ) == 0 && "Could not open model file!");
  loader.GenerateModelData();

  CModel shipModel;

  assert( g_DefaultVertexManager->Create( loader.GetVertices(),  "ship_vertices",  shipModel.GetVertexHandle()  ) == 0);  
  assert( g_DefaultVertexManager->Create( loader.GetTexCoords(), "ship_texcoords", shipModel.GetTextureCoordinateHandle() ) == 0);
  assert( g_DefaultVertexManager->Create( loader.GetNormals(),   "ship_normals",   shipModel.GetNormalHandle()   ) == 0);

  g_DefaultVertexManager->DuplicateHandle( shipModel.GetTextureCoordinateHandle(0), shipModel.GetTextureCoordinateHandle(1));
  
  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "ship_indices" << i;
    shipModel.AddIndexHandle( INDEX_HANDLE() );
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], stream.str(), shipModel.GetIndexHandles().back() ) == 0);

  }

  shipModel.AddTextureFilter( ENV_REPLACE, 0 );
  shipModel.AddTextureFilter( MIN_LINEAR, 0 );
  shipModel.AddTextureFilter( MAG_LINEAR, 0 );
  shipModel.AddTextureFilter( T_WRAP_REPEAT, 0 );
  shipModel.AddTextureFilter( S_WRAP_REPEAT, 0 );
  
  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetIndices();


  COglTexture *pTextureReal  = pOglRenderer->CreateTexture("./Resources/Textures/sovereign.tga");
  COglTexture *pTextureReal2 = pOglRenderer->CreateTexture("./Resources/Textures/sovereign_glow.tga");

  COglTexture *pFBOTexture     = pOglRenderer->CreateTexture( FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, TEXTURE_RECT);
  COglTexture *pFBOTexture2    = pOglRenderer->CreateTexture( FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, TEXTURE_RECT);

  
  TEXTURE_HANDLE hFramebuffer;
  TEXTURE_HANDLE hFramebuffer2;


  g_DefaultTextureManager->Create( pFBOTexture,     "framebuffer_texture",   hFramebuffer );
  g_DefaultTextureManager->Create( pFBOTexture2,    "framebuffer_texture2",  hFramebuffer2 );

  g_DefaultTextureManager->Create( pTextureReal,    "ship_texture_real",      shipModel.GetTextureHandle(0));
  g_DefaultTextureManager->Create( pTextureReal2,   "ship_texture_glow",      shipModel.GetTextureHandle(1));
  
  
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );



  ////////////////////
  // Transform stuff which makes things go round and round
  Phoenix::Math::CTransform transform;

  CQuaternion q;
  CTimer timer;
  timer.Reset();
  
  CFrameBufferObject *pFrameBuffer = pOglRenderer->CreateFramebuffer( FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, FBO_DEPTH_BUFFER );
  assert( pFrameBuffer != NULL );
  assert( pOglRenderer->AttachTextureToFramebuffer( *pFrameBuffer, hFramebuffer,  0 ) == 0);
  assert( pOglRenderer->AttachTextureToFramebuffer( *pFrameBuffer, hFramebuffer2, 1 ) == 0);

  
  CShader *pShader = pOglRenderer->CreateShader( "./Resources/Shaders/fbotest.vertex.glsl",
						 "./Resources/Shaders/fbotest.frag.glsl");
  
  CShader *pBlurShader = pOglRenderer->CreateShader( "./Resources/Shaders/blur.vertex.glsl",
						     "./Resources/Shaders/blur.frag.glsl");

  int bMousePressed = 0;
  CVector2<int> vMouseStart, vMouseEnd;
  CVector4<unsigned char> vWhite(255,255,255,255);
  CFpsCounter fps;
  fps.Reset();

  while( g_bLoop )
  {
    fps.Update();
    while ( SDL_PollEvent(&event ))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  g_bLoop = 0;
	}
	else if ( event.key.keysym.sym == SDLK_UP )
	{
	  camera.Move( 1.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  camera.Move( -1.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 1.1f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -1.1f );
	} 
	else if ( event.key.keysym.sym == SDLK_SPACE )
	{
	  g_bGlow = !g_bGlow;
	  if ( g_bGlow )
	  {

	  }
	} 
	break;
      case SDL_MOUSEBUTTONDOWN:
	bMousePressed = 1;
	vMouseStart[0] = event.button.x;
	vMouseStart[1] = camera.GetViewport()[3]-event.button.y;
	std::cerr << "Got down at : " << vMouseStart << std::endl;
	break;
      case SDL_MOUSEBUTTONUP:
	bMousePressed = 0;
	break;
      case SDL_MOUSEMOTION:
	if ( bMousePressed )
	{
	  vMouseEnd[0] = event.motion.x;
	  vMouseEnd[1] = camera.GetViewport()[3]-event.button.y;

	  
	  camera.VirtualTrackball( CVector3<float>(0,0,0),
				   vMouseStart,
				   vMouseEnd);

	  vMouseStart = vMouseEnd;

	} 
	break;
      default:
	break;
      }
    }

    q.CreateFromAxisAngle( 1,0,0, g_fRotationSpeedPerSec * (timer.GetPassedTime().GetSeconds()+(float)timer.GetPassedTime().GetMilliSeconds()*0.001f));
    timer.Reset();
    transform.Rotate( q );
    //cerr << "Collected: " << nCollected << endl;
 
   
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glClearColor(0,0,0,0);
    g_DefaultTextureManager->Release( shipModel.GetTextureHandle(1) );

    pOglRenderer->CommitCamera( camera );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitState( STATE_DEPTH_TEST);
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->CommitState( STATE_FACECULLING );
    pOglRenderer->CommitColor( vWhite );

    // Rotate 
    pOglRenderer->CommitTransform( transform);
      pOglRenderer->CommitModel( shipModel );
    pOglRenderer->CommitShader( NULL );



    if ( g_bGlow )
    {
      g_DefaultTextureManager->AttachHandle( "ship_texture_glow",  shipModel.GetTextureHandle(1));
    /////////////////////////////////////////////////////////////////
    // Render all into framebuffer texture
    glClearColor( 0,0,0,0);
    pOglRenderer->CommitFrameBufferSingle( *pFrameBuffer, 0 );
       camera.SetViewport(0,0, FRAMEBUFFER_WIDTH,FRAMEBUFFER_HEIGHT);
       camera.UpdateProjection();
       camera.UpdateView();
       pOglRenderer->CommitCamera( camera );
       pOglRenderer->ClearBuffer( COLOR_BUFFER );
       pOglRenderer->ClearBuffer( DEPTH_BUFFER );
       pOglRenderer->CommitState( STATE_DEPTH_TEST);
       pOglRenderer->DisableState( STATE_LIGHTING );
       pOglRenderer->CommitState( STATE_FACECULLING );
       // Rotate 
       pOglRenderer->CommitTransform( transform);
         pOglRenderer->CommitShader( pShader );
	 pOglRenderer->CommitUniformShaderParam( *pShader, "tex0", 0);
	 pOglRenderer->CommitUniformShaderParam( *pShader, "tex1", 1);
         pOglRenderer->CommitModel( shipModel );
 	pOglRenderer->CommitShader( NULL );
	pOglRenderer->RollbackTransform();
    pOglRenderer->RollbackFrameBuffer( *pFrameBuffer );

    camera.SetViewport(0,0, SCREEN_WIDTH,SCREEN_HEIGHT);
    camera.UpdateProjection();
    camera.UpdateView();

    pOglRenderer->DisableTexture(1);
    pOglRenderer->DisableTexture(0);

    pOglRenderer->CommitShader( pBlurShader );
     /////////////////////////////////////////////////////////////////
     pOglRenderer->CommitFrameBufferSingle( *pFrameBuffer, 1 );

        pOglRenderer->CommitCamera( cameraFBOQuad );
        pOglRenderer->ClearBuffer( COLOR_BUFFER );
        pOglRenderer->ClearBuffer( DEPTH_BUFFER );
        pOglRenderer->DisableState( STATE_DEPTH_TEST);
        pOglRenderer->DisableState( STATE_LIGHTING );
        pOglRenderer->CommitState( STATE_FACECULLING );
        pOglRenderer->CommitTexture( 0, pFBOTexture);
	pOglRenderer->CommitShader( pBlurShader );
	pOglRenderer->CommitUniformShaderParam( *pBlurShader, "tex0",      0);
        pOglRenderer->CommitUniformShaderParam( *pBlurShader, "bVertical", 1);
	// Render vertical blur 
	pOglRenderer->CommitQuad( *pFrameBuffer );
        //pOglRenderer->DisableTexture( 0, pFBOTexture);
     pOglRenderer->RollbackFrameBuffer( *pFrameBuffer );

    pOglRenderer->CommitFrameBufferSingle( *pFrameBuffer, 0 );
       pOglRenderer->CommitCamera( cameraFBOQuad );
       pOglRenderer->ClearBuffer( COLOR_BUFFER );
       pOglRenderer->ClearBuffer( DEPTH_BUFFER );
       pOglRenderer->DisableState( STATE_DEPTH_TEST);
       pOglRenderer->DisableState( STATE_LIGHTING );
       pOglRenderer->CommitState( STATE_FACECULLING );

       pOglRenderer->CommitTexture( 0, pFBOTexture2);

       //pOglRenderer->CommitUniformShaderParam( *pBlurShader, "tex0",      0);
       pOglRenderer->CommitUniformShaderParam( *pBlurShader, "bVertical", 0);
       // Render horizontal blur.
       pOglRenderer->CommitQuad( *pFrameBuffer );



    pOglRenderer->RollbackFrameBuffer( *pFrameBuffer );

    pOglRenderer->CommitShader( NULL );

    glClearColor( 0,0,0,1 );
    pOglRenderer->CommitCamera( cameraScreenQuad );
    pOglRenderer->CommitColor( vWhite );
    //pOglRenderer->ClearBuffer( COLOR_BUFFER );
    //pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );    
    pOglRenderer->DisableState( STATE_DEPTH_TEST );
    pOglRenderer->CommitState( STATE_BLENDING );
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE );
    pOglRenderer->CommitTexture( 0, pFBOTexture);
    pOglRenderer->DisableTexture(1);

    glColor3f( 1, 1,1);
    // Render finished blur quad
    pOglRenderer->CommitQuad( cameraScreenQuad, *pFrameBuffer );

    pOglRenderer->DisableState( STATE_BLENDING );
    /////////////////////////////////////////////////////////////////


    }

    pOglRenderer->Finalize();
    fps++;
    if ( fps.HasPassed(1,0))
    {
      cerr << "FPS: " << fps << endl;
      fps.Reset();
    }
    CSDLScreen::GetInstance()->SwapBuffers();
    timer.Update();
  }
  delete pFrameBuffer;
  pFrameBuffer = NULL;
  CSDLScreen::DestroyInstance();
  return 0;
}
