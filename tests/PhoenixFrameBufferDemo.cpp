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
const float g_fRotationSpeedPerSec = 56.15f;
/////////////////////////////////////////////////////////////////
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,3.0f);
  //camera.SetViewport( 480,340, 160, 120 );
  camera.SetViewport( 0,0, 320, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 43.0f);
  camera.RotateAroundRight(0.0f);
  
  CCamera camera2;
  camera2.SetPosition( 0, 0.0f,0.0f);
  camera2.SetViewport( 0,0, 512,512 );
  camera2.SetNearClipping( -2);
  camera2.SetFarClipping( 20 );
  camera2.SetViewOrtho( -2, 2, -2, 2);
  
  CCamera camera3;
  camera3.SetPosition( 0, 0.0f,3.0f);
  //camera.SetViewport( 480,340, 160, 120 );
  camera3.SetViewport( 320,0, 320, 480 );
  camera3.SetNearClipping( 0.1f);
  camera3.SetFarClipping( 100.0f );
  camera3.SetFieldOfView( 43.0f);
  camera3.RotateAroundRight(0.0f);

  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();

  Phoenix::Data::CMilkshapeLoader loader;
  assert ( loader.Load( std::string("./Resources/Models/box.ms3d") ) == 0 && "Could not open model file!");
  loader.GenerateModelData();

  CModel boxModel;

  assert( g_DefaultVertexManager->Create( loader.GetVertices(),  "box_vertices",  boxModel.GetVertexHandle()  ) == 0);  
  assert( g_DefaultVertexManager->Create( loader.GetTexCoords(), "box_texcoords", boxModel.GetTextureCoordinateHandle() ) == 0);
  assert( g_DefaultVertexManager->Create( loader.GetNormals(),   "box_normals",   boxModel.GetNormalHandle()   ) == 0);
  
  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "box_indices" << i;
    boxModel.AddIndexHandle( INDEX_HANDLE() );
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], stream.str(), boxModel.GetIndexHandles().back() ) == 0);

  }

  boxModel.AddTextureFilter( ENV_REPLACE, 0 );
  boxModel.AddTextureFilter( MIN_LINEAR, 0 );
  boxModel.AddTextureFilter( MAG_LINEAR, 0 );
  boxModel.AddTextureFilter( T_WRAP_REPEAT, 0 );
  boxModel.AddTextureFilter( S_WRAP_REPEAT, 0 );
  
  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetIndices();


  COglTexture *pBoxTextureReal = pOglRenderer->CreateTexture("./Resources/Textures/wall.tga");
  COglTexture *pFBOTexture     = pOglRenderer->CreateTexture( 512, 512, TEXTURE_2D);
  COglTexture *pFBOTexture2    = pOglRenderer->CreateTexture( 512, 512, TEXTURE_2D);

  TEXTURE_HANDLE hWallTexture;
  TEXTURE_HANDLE hFramebuffer;
  TEXTURE_HANDLE hFramebuffer2;

  g_DefaultTextureManager->Create( pFBOTexture,     "framebuffer_texture",   hFramebuffer );
  g_DefaultTextureManager->Create( pFBOTexture2,    "framebuffer_texture2",  hFramebuffer2 );
  g_DefaultTextureManager->Create( pBoxTextureReal, "box_texture_real",      hWallTexture );
  
  g_DefaultTextureManager->DuplicateHandle( hWallTexture, boxModel.GetTextureHandle() );
  
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );



  ////////////////////
  // Transform stuff which makes things go round and round
  Phoenix::Math::CTransform transform;
  Phoenix::Math::CTransform transform2;
  CQuaternion q;
  CTimer timer;
  timer.Reset();
  
  CFrameBufferObject *pFrameBuffer = pOglRenderer->CreateFramebuffer( 512, 512, FBO_DEPTH_BUFFER );
  assert( pFrameBuffer != NULL );
  assert( pOglRenderer->AttachTextureToFramebuffer( *pFrameBuffer, hFramebuffer,  0 ) == 0);
  assert( pOglRenderer->AttachTextureToFramebuffer( *pFrameBuffer, hFramebuffer2, 1 ) == 0);

  CShader *pShader = pOglRenderer->CreateShader( "./Resources/Shaders/fbotest.vertex.glsl",
						 "./Resources/Shaders/fbotest.frag.glsl");
  
  int bMousePressed = 0;
  CVector2<int> vMouseStart, vMouseEnd;
  camera.UpdateProjection();
  camera.UpdateView();
  camera2.UpdateProjection();
  camera2.UpdateView();
  while( g_bLoop )
  {
    
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

    q.CreateFromAxisAngle( 1,1,1, g_fRotationSpeedPerSec * (timer.GetPassedTime().GetSeconds()+(float)timer.GetPassedTime().GetMilliSeconds()*0.001f));
    timer.Reset();
    transform.Rotate( q );
    //cerr << "Collected: " << nCollected << endl;
   
    
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    // Switch texture to stone wall
    g_DefaultTextureManager->Release( boxModel.GetTextureHandle());
    g_DefaultTextureManager->DuplicateHandle( hWallTexture, boxModel.GetTextureHandle());

    // Render all into framebuffer texture
    glClearColor( 0,1,0,1);
    pOglRenderer->CommitFrameBuffer( *pFrameBuffer, 2 );
      pOglRenderer->CommitCamera( camera2 );
      pOglRenderer->ClearBuffer( COLOR_BUFFER );
      pOglRenderer->ClearBuffer( DEPTH_BUFFER );
      pOglRenderer->CommitState( STATE_DEPTH_TEST);
      pOglRenderer->DisableState( STATE_LIGHTING );
      pOglRenderer->DisableState( STATE_FACECULLING );
      // Rotate cube
      pOglRenderer->CommitTransform( transform);

        pOglRenderer->CommitShader( pShader );
	pOglRenderer->CommitUniformShaderParam( *pShader, "tex0", 0);
        pOglRenderer->CommitModel( boxModel );
	pOglRenderer->CommitShader( NULL );
      pOglRenderer->RollbackTransform();
    pOglRenderer->RollbackFrameBuffer( *pFrameBuffer );

    // Switch texture to framebuffer1 texture
    g_DefaultTextureManager->Release( boxModel.GetTextureHandle() );
    g_DefaultTextureManager->DuplicateHandle( hFramebuffer, boxModel.GetTextureHandle());

    // Render cube with previously rendered image as cube side texture
    glClearColor( 0,0,0,1);
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );

    pOglRenderer->CommitModel( boxModel );
     
    // Render cube with previously rendered image as cube side texture (but with camera 3)
    // Switch texture to framebuffer1 texture
    g_DefaultTextureManager->Release( boxModel.GetTextureHandle() );
    g_DefaultTextureManager->DuplicateHandle( hFramebuffer2, boxModel.GetTextureHandle());
    pOglRenderer->CommitCamera( camera3 );
   
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );

    pOglRenderer->CommitModel( boxModel );
    
    camera.UpdateProjection();
    camera.UpdateView();
    camera2.UpdateProjection();
    camera2.UpdateView();
    
    pOglRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
    timer.Update();
  }
  delete pFrameBuffer;
  pFrameBuffer = NULL;
  CSDLScreen::DestroyInstance();
  return 0;
}
