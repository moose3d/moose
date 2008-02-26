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
using namespace Phoenix::Collision;
using std::cerr;
using std::endl; 
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int g_bGlow = 0;
const float g_fRotationSpeedPerSec = 56.15f;
const unsigned int SCREEN_WIDTH  = 640;
const unsigned int SCREEN_HEIGHT = 480;

float fOffset = 0.0f;
/////////////////////////////////////////////////////////////////
int main()
{
  
  CSDLScreen::m_SDLScreenParams.m_iWidth = SCREEN_WIDTH;
  CSDLScreen::m_SDLScreenParams.m_iHeight= SCREEN_HEIGHT;
  //CSDLScreen::m_SDLScreenParams.m_iVideoModeFlags |= SDL_FULLSCREEN;

  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  
  CCamera camera;
  camera.SetPosition( 4.0, 0.50f,1.0f);
  camera.SetViewport( 0,0, SCREEN_WIDTH, SCREEN_HEIGHT );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 500.0f );
  camera.SetFieldOfView( 43.0f);
  camera.RotateAroundUp(90.0f);
  

  

  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();

  CVertex v0,v1,v2;
  v0.SetPosition(0,0,-1);
  v1.SetPosition(1,0,0);
  v2.SetPosition(1,1,0.5);
  
  CLine line;
  line.SetStart( CVector3<float>(0,0.5,0.25));
  line.SetEnd(   CVector3<float>(0,0.5,-0.25));
 
  CTimer timer;
  timer.Reset();
  
  CVector4<unsigned char> vWhite(255,255,255,255);
  CVector4<unsigned char> vPurple(255,55,255,255);
  CVector4<unsigned char> vYellow(255,255,0,255);
  CFpsCounter fps;
  fps.Reset();
  CVector2<int> vMousePos;
  CTransform tr;
  tr.SetTranslation(2,0,0);
  tr.SetRotation( 0.4, 0.104, 0.0);

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
	else if ( event.key.keysym.sym == SDLK_a )
	{
	  camera.RotateAroundY(1.0f);
	}
	else if ( event.key.keysym.sym == SDLK_d )
	{
	  camera.RotateAroundY(-1.0f);
	}
	else if ( event.key.keysym.sym == SDLK_w )
	{
	  camera.Move(camera.GetForwardVector());
	}
	else if ( event.key.keysym.sym == SDLK_s )
	{
	  camera.Move(-camera.GetForwardVector());
	}
	else if ( event.key.keysym.sym == SDLK_UP )
	{
	  line.SetStart( line.GetStart()+CVector3<float>(0,0,0.01f));
	  line.SetEnd(   line.GetEnd()  +CVector3<float>(0,0,0.01f));  
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  line.SetStart( line.GetStart()-CVector3<float>(0,0,0.01f));
	  line.SetEnd(   line.GetEnd()  -CVector3<float>(0,0,0.01f));
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  line.SetStart( line.GetStart()-CVector3<float>(0.01,0,0));
	  line.SetEnd(   line.GetEnd()  -CVector3<float>(0.01,0,0));	  
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  line.SetStart( line.GetStart()+CVector3<float>(0.01,0,0));
	  line.SetEnd(   line.GetEnd()  +CVector3<float>(0.01,0,0));
	} 
	else if ( event.key.keysym.sym == SDLK_PAGEUP )
	{
	  line.SetStart( line.GetStart()+CVector3<float>(0, 0.01,0));
	  line.SetEnd(   line.GetEnd()  +CVector3<float>(0, 0.01,0));
	} 
	else if ( event.key.keysym.sym == SDLK_PAGEDOWN )
	{
	  line.SetStart( line.GetStart()-CVector3<float>(0, 0.01,0));
	  line.SetEnd(   line.GetEnd()  -CVector3<float>(0, 0.01 ,0));
	} 
	break;
      case SDL_MOUSEBUTTONDOWN:
	break;
      case SDL_MOUSEBUTTONUP:
	break;
      case SDL_MOUSEMOTION:
	{
// 	  CVector2<int> vMousePosCurrent(event.motion.x, event.motion.y);
// 	  CVector2<int> vMouseDiff = vMousePos - vMousePosCurrent;
// 	  vMouseDiff[0] = -vMouseDiff[0];
// 	  camera.RotateAroundY(vMouseDiff[0]);
// 	  camera.RotateAroundX(vMouseDiff[1]);	
// 	  vMousePos = vMousePosCurrent;
	}
	break;
      default:
	break;
      }
    }


   
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glClearColor(0,0,0,0);
    
    
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitState( STATE_DEPTH_TEST);
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );

    pOglRenderer->DisableState( STATE_BLENDING );
    pOglRenderer->CommitTransform( tr );
    
    glBegin(GL_TRIANGLES);

      pOglRenderer->CommitColor( CVector4<unsigned char>(255,0,0,255));
      pOglRenderer->CommitVertex(v0);

      pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0,255));
      pOglRenderer->CommitVertex(v1);

      pOglRenderer->CommitColor( CVector4<unsigned char>(0,0,255,255));
      pOglRenderer->CommitVertex(v2);

    glEnd();
    pOglRenderer->RollbackTransform();
    CVector3<float> vTmp;
    CLine lineTmp;
    CMatrix4x4<float> inv;
    InverseMatrix( tr.GetMatrix(), inv );
    lineTmp.SetStart( Transform( line.GetEnd(), inv));
    lineTmp.SetEnd( Transform( line.GetStart(), inv));

    if ( LineIntersectsTriangle( lineTmp, v0.GetPosition(), v1.GetPosition(), v2.GetPosition(), vTmp) )
    {
      pOglRenderer->CommitColor( CVector4<unsigned char>(0,255,0,255));    
      cerr << "Ossuuu" << endl;
    } 
    else
    {
      pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    }
    glBegin(GL_LINES);
      glVertex3fv(line.GetStart().GetArray());
      glVertex3fv(line.GetEnd().GetArray());
    glEnd();
    
    
    pOglRenderer->Finalize();
    fps++;
    if ( fps.HasPassed(1,0))
    {
      cerr << "FPS: " << fps << endl;
      fps.Reset();
    }
    CSDLScreen::GetInstance()->SwapBuffers();
    timer.Update();
    fOffset+= 0.1f;
    if ( fOffset > SCREEN_HEIGHT )
      fOffset = 0.0f;
  }


  CSDLScreen::DestroyInstance();
  return 0;
}
