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
  
  string strVertexTest("VertexTest");
  string tmpName;
  CVector3<float> vStartPos(0,0,0);
  CVector3<float> vEndPos(3,0,0);
  CVector3<float> vDir = vEndPos - vStartPos;
  float fThickness = 0.25f;
  ////////////////////
  CVertexDescriptor *pVertices = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F, 4 );

  pVertices->GetPointer<float>()[0] = vStartPos[0];
  pVertices->GetPointer<float>()[1] = vStartPos[1];
  pVertices->GetPointer<float>()[2] = vStartPos[2];
  
  pVertices->GetPointer<float>()[3] = vStartPos[0];
  pVertices->GetPointer<float>()[4] = vStartPos[1];
  pVertices->GetPointer<float>()[5] = vStartPos[2];

  pVertices->GetPointer<float>()[6] = vEndPos[0];
  pVertices->GetPointer<float>()[7] = vEndPos[1];
  pVertices->GetPointer<float>()[8] = vEndPos[2];
  
  pVertices->GetPointer<float>()[9] =  vEndPos[0];
  pVertices->GetPointer<float>()[10] = vEndPos[1];
  pVertices->GetPointer<float>()[11] = vEndPos[2];
  
  CIndexArray *pIndices = new CIndexArray( PRIMITIVE_QUAD_LIST, 4 );
  pIndices->GetPointer<unsigned short int>()[0] = 0;
  pIndices->GetPointer<unsigned short int>()[1] = 1;
  pIndices->GetPointer<unsigned short int>()[2] = 2;
  pIndices->GetPointer<unsigned short int>()[3] = 3;
  
  COglRenderer *pOglRenderer = new COglRenderer();
  
  
  CVertexDescriptor *pLineDirAndThickness = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F, 4);
  
  pLineDirAndThickness->GetPointer<float>()[0] = vDir[0];
  pLineDirAndThickness->GetPointer<float>()[1] = vDir[1];
  pLineDirAndThickness->GetPointer<float>()[2] = vDir[2];
  pLineDirAndThickness->GetPointer<float>()[3] = fThickness;

  pLineDirAndThickness->GetPointer<float>()[4] = vDir[0];
  pLineDirAndThickness->GetPointer<float>()[5] = vDir[1];
  pLineDirAndThickness->GetPointer<float>()[6] = vDir[2];
  pLineDirAndThickness->GetPointer<float>()[7] = -fThickness;

  pLineDirAndThickness->GetPointer<float>()[8] =  -vDir[0];
  pLineDirAndThickness->GetPointer<float>()[9] =  -vDir[1];
  pLineDirAndThickness->GetPointer<float>()[10] = -vDir[2];
  pLineDirAndThickness->GetPointer<float>()[11] = fThickness;

  pLineDirAndThickness->GetPointer<float>()[12] = -vDir[0];
  pLineDirAndThickness->GetPointer<float>()[13] = -vDir[1];
  pLineDirAndThickness->GetPointer<float>()[14] = -vDir[2];
  pLineDirAndThickness->GetPointer<float>()[15] = -fThickness;
  
  VERTEX_HANDLE hVertexHandle;
  INDEX_HANDLE hIndexHandle;
  
  // Create vertex resource
  assert( g_DefaultVertexManager->Create( pVertices, "lineVertices", hVertexHandle ) == 0);
  assert( g_DefaultIndexManager->Create( pIndices, "lineIndices", hIndexHandle ) == 0);

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
  assert(g_DefaultShaderManager->Create( pShader, "lineShader", hShaderHandle) == 0 );

  CVertexDescriptor *pCameraPos = new CVertexDescriptor(ELEMENT_TYPE_UNIFORM_3F,1);
  pCameraPos->GetPointer<float>()[0] = camera.GetPosition()[0];
  pCameraPos->GetPointer<float>()[1] = camera.GetPosition()[1];
  pCameraPos->GetPointer<float>()[2] = camera.GetPosition()[2];
  
  VERTEX_HANDLE hCameraPos, hLinedirThickness;
  assert(g_DefaultVertexManager->Create( pCameraPos, "cameraPosParam", hCameraPos ) == 0);  
  assert(g_DefaultVertexManager->Create( pLineDirAndThickness, "linedirThicknessParam", hLinedirThickness ) == 0);  

  float fAngle = 0.0f;
  float fMagnitude = 12.0f;
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
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
    pCameraPos->GetPointer<float>()[0] = camera.GetPosition()[0];
    pCameraPos->GetPointer<float>()[1] = camera.GetPosition()[1];
    pCameraPos->GetPointer<float>()[2] = camera.GetPosition()[2];

    
    //CVector4<unsigned char> vWhite(255,255,255,255);
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitCamera( camera );

    pOglRenderer->CommitShader( pShader );
    pOglRenderer->CommitShaderParam( *pShader, "cameraPos" , *pCameraPos );
    pOglRenderer->CommitShaderParam( *pShader, "linedirThickness" , *pLineDirAndThickness);
    //pOglRenderer->CommitColor( vWhite );    
    // Draw colored triangle
    pOglRenderer->CommitVertexDescriptor( pVertices );
    //pOglRenderer->CommitVertexDescriptor( pColors );
    
    pOglRenderer->CommitPrimitive( pIndices );

    /*    glBegin(GL_QUADS);
      glVertex3f(0,1,0);
      glVertex3f(0,0,0);
      glVertex3f(3,0,0);
      glVertex3f(3,1,0);
      glEnd(); */
    
    pOglRenderer->Finalize();
    
    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
