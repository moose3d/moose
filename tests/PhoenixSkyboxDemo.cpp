#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  string strTexFilename("Resources/Textures/painting.tga");
  string strTexFilename2("Resources/Textures/lightmap.tga");

  
  string strVertexTest("VertexTest");
  string strTexCoords("TexCoordTest");
  string strIndexTest("IndexTest");
  string strTextureTest("TextureTest");
  string strTextureTest2("Texture2");
  string tmpName;


  COglRenderer *pOglRenderer = new COglRenderer();
  CCamera camera;
  camera.SetPosition( 0,0,0.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 45.0f);

  SDL_Event event;
  CVector3<float> vMove(0,0,0.2f);
  

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  CSkybox skybox;
  COglTexture *pTex = pOglRenderer->CreateTexture("Resources/Textures/otop7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_FLOOR_TEXTURE", skybox.GetTextureHandle(0));

  pTex = pOglRenderer->CreateTexture("Resources/Textures/otop7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_CEILING_TEXTURE", skybox.GetTextureHandle(1));

  pTex = pOglRenderer->CreateTexture("Resources/Textures/ofront7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_FRONT_TEXTURE", skybox.GetTextureHandle(2));

  pTex = pOglRenderer->CreateTexture("Resources/Textures/oback7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_BACK_TEXTURE", skybox.GetTextureHandle(3));

  pTex = pOglRenderer->CreateTexture("Resources/Textures/oleft7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_LEFT_TEXTURE", skybox.GetTextureHandle(4));

  pTex = pOglRenderer->CreateTexture("Resources/Textures/oright7.tga");
  g_DefaultTextureManager->Create( pTex, "SKYBOX_RIGHT_TEXTURE", skybox.GetTextureHandle(5));

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
	  //camera.Move( 0.3f );
	  camera.RotateAroundRight( 2.93f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  //camera.Move( -0.3f );
	  camera.RotateAroundRight( -2.93f );
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 2.93f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -2.93f );
	} 
	else if ( event.key.keysym.sym == SDLK_PAGEUP )
	{
	  camera.Elevate( 0.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_PAGEDOWN )
	{
	  camera.Elevate( -0.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_RETURN )
	{
	  std::cerr << camera.GetPosition() << std::endl;
	  std::cerr << camera.GetViewMatrix() << std::endl;
	}
	break;
      default:
	break;
      }
    }

    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitCamera( camera );
    glColor3f(1,1,1);
    glEnable(GL_CULL_FACE);
    pOglRenderer->CommitSkybox( skybox, camera );
    pOglRenderer->DisableState( STATE_ALPHA_TEST );
    pOglRenderer->DisableState( STATE_DEPTH_TEST );
    pOglRenderer->Finalize();
    
    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
