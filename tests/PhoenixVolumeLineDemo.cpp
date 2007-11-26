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
#define PLASMA_TEXTURE "EnergyBeamTexture_Plasma"
#define LASER_TEXTURE "EnergyBeamTexture_Laser"
class CPlasmaBeam : public CEnergyBeam 
{
private:
  
public:
  ////////////////////
  /// Constructor.
  CPlasmaBeam()
  {
    g_DefaultTextureManager->AttachHandle(PLASMA_TEXTURE, GetTextureHandle());
    AddTextureFilter( MIN_NEAREST );
    AddTextureFilter( MAG_LINEAR );
    AddTextureFilter( S_WRAP_REPEAT );
  }
  ////////////////////
  /// Creates necessary resources for this class.
  static void CreateResources( Phoenix::Graphics::COglRenderer &renderer )
  {
    COglTexture *pBeamTexture = renderer.CreateTexture( "Resources/Textures/beam.tga");
    assert( g_DefaultTextureManager->Create( pBeamTexture, PLASMA_TEXTURE) == 0);
    
    CShader *pShader = g_DefaultShaderManager->GetResource("EnergyBeamShader");
    if ( pShader == NULL )
    {
      pShader = renderer.CreateShader( std::string("Resources/Shaders/line_vertex.glsl"), 
					    std::string("Resources/Shaders/line_frag.glsl") );
      assert( pShader != NULL ) ;
      assert(g_DefaultShaderManager->Create( pShader, "EnergyBeamShader") == 0 );
    }

  }
};
/////////////////////////////////////////////////////////////////
class CLaserBeam : public CEnergyBeam 
{
private:
  
public:
  ////////////////////
  /// Constructor.
  CLaserBeam()
  {
    g_DefaultTextureManager->AttachHandle(LASER_TEXTURE, GetTextureHandle());
    AddTextureFilter( MIN_NEAREST );
    AddTextureFilter( MAG_LINEAR );
    AddTextureFilter( S_WRAP_REPEAT );
  }
  ////////////////////
  /// Creates necessary resources for this class.
  static void CreateResources( Phoenix::Graphics::COglRenderer &renderer )
  {
    COglTexture *pBeamTexture = renderer.CreateTexture( "Resources/Textures/beam2.tga");
    assert( g_DefaultTextureManager->Create( pBeamTexture, LASER_TEXTURE) == 0);
    CShader *pShader = g_DefaultShaderManager->GetResource("EnergyBeamShader");
    if ( pShader == NULL )
    {
      pShader = renderer.CreateShader( std::string("Resources/Shaders/line_vertex.glsl"), 
				       std::string("Resources/Shaders/line_frag.glsl") );
      assert( pShader != NULL ) ;
      assert(g_DefaultShaderManager->Create( pShader, "EnergyBeamShader") == 0 );
    }
  }
};

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

  
  CPlasmaBeam::CreateResources( *pOglRenderer );
  CLaserBeam::CreateResources( *pOglRenderer );
  


  float fAngle = 0.0f;
  float fMagnitude = 12.0f;
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  
  
  
  CPlasmaBeam *pBeam;
  CLaserBeam *pBeam2;
  pBeam = new CPlasmaBeam();
  pBeam2 = new CLaserBeam();
  pBeam->Initialize( vStartPos, vEndPos, fThickness );
  pBeam2->Initialize( vStartPos, vEndPos, fThickness );
  
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
    fAngle += 0.00001f;
    pBeam->IncreaseTime(0.025f);
    pBeam2->IncreaseTime(0.025f);
    
  }
  delete pBeam;
  delete pBeam2;
  



  CSDLScreen::DestroyInstance();

  return 0;
}
