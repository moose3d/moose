#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
#include "PhoenixEnergyBeam.h"
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  string strVertexTest("VertexTest");
  string tmpName;
  CVector3<float> vStartPos(0,0,0);
  CVector3<float> vEndPos(13,0,0);
  //CVector3<float> vDir = vEndPos - vStartPos;
  float fThickness = 0.25f;
  ////////////////////

  COglRenderer *pOglRenderer = new COglRenderer();
  

  CCamera camera;
  camera.SetPosition( 0,0,2.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 45.0f);
  SDL_Event event;
  CVector3<float> vMove(0,0,0.2f);
  
  
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  CShader *pShader = pOglRenderer->CreateShader( std::string("Resources/Shaders/line_vertex.glsl"), 
						 std::string("Resources/Shaders/line_frag.glsl") );
  
  assert( pShader != NULL ) ;
  SHADER_HANDLE hShaderHandle;
  assert(g_DefaultShaderManager->Create( pShader, "EnergyBeamShader", hShaderHandle) == 0 );
  

  


  float fAngle = 0.0f;
  float fMagnitude = 12.0f;
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  COglTexture *pBeamTexture = pOglRenderer->CreateTexture( std::string("Resources/Textures/beam.tga"));
  COglTexture *pBeamTexture2 = pOglRenderer->CreateTexture( std::string("Resources/Textures/beam2.tga"));
  
  CEnergyBeam *pBeam, *pBeam2;
  pBeam = new CEnergyBeam();
  pBeam2 = new CEnergyBeam();
  pBeam->Initialize( vStartPos, vEndPos, fThickness );
  assert( g_DefaultTextureManager->Create( pBeamTexture, "EnergyBeamTexture_Red", pBeam->GetTextureHandle()) == 0);
  pBeam->AddTextureFilter( MIN_NEAREST );
  pBeam->AddTextureFilter( MAG_LINEAR );
  pBeam->AddTextureFilter( S_WRAP_REPEAT );
  
  pBeam2->Initialize( vStartPos, vEndPos, fThickness );
  assert( g_DefaultTextureManager->Create( pBeamTexture2, "EnergyBeamTexture_Another", pBeam2->GetTextureHandle()) == 0);
  pBeam2->AddTextureFilter( MIN_NEAREST );
  pBeam2->AddTextureFilter( MAG_LINEAR );
  pBeam2->AddTextureFilter( S_WRAP_REPEAT );
  
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
	  camera.Move( 0.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  camera.Move( -0.3f );
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

    pOglRenderer->CommitState( STATE_BLENDING);
    pOglRenderer->CommitBlending( BLEND_SRC_ONE, BLEND_DST_ONE );
    
    pOglRenderer->CommitModel( *pBeam );
    pOglRenderer->CommitModel( *pBeam2 );
    
    pOglRenderer->Finalize();
    vStartPos[2] = cosf(fAngle) * 2.0f;
    vStartPos[1] = sinf(fAngle) * 2.0f;
    
    pBeam->Initialize(vStartPos, vEndPos, fThickness );

    vStartPos[2] = cosf(-fAngle) * 1.0f;
    vStartPos[1] = sinf(fAngle) * 1.0f;
    
    pBeam2->Initialize(vStartPos, vEndPos, fThickness );
    pOglRenderer->DisableState( STATE_BLENDING);
    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
    fAngle += 0.001f;
    pBeam->IncreaseTime(0.025f);
    pBeam2->IncreaseTime(0.025f);
    
  }
  delete pBeam;
  delete pBeam2;
  



  CSDLScreen::DestroyInstance();

  return 0;
}
