#include <Phoenix.h>
#include <iostream>
#include <list>
#include <vector>
#include <SDL.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::Collision;
using namespace Phoenix::AI;
using namespace Phoenix::Data;
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl; 
using std::list;
using std::vector;
/////////////////////////////////////////////////////////////////
int CalculateHitpoint( CLine &line, CVertexDescriptor & vertices, 
		       CIndexArray & indices, CVector3<float> & point, 
		       CVector3<float> & vTangent )
{
  CVector3<float> vVertices[3];
  
  list< CVector3<float> >::iterator it;
  CVector3<float> vPoint;
  
  for( size_t i=0;i<indices.GetNumIndices();i+=3)
  {
	  
    unsigned short int *pTmp = indices.GetPointer<unsigned short int>();
    
    vVertices[0][0] = vertices.GetPointer<float>()[pTmp[i]*3];
    vVertices[0][1] = vertices.GetPointer<float>()[pTmp[i]*3+1];
    vVertices[0][2] = vertices.GetPointer<float>()[pTmp[i]*3+2];
    
    vVertices[1][0] = vertices.GetPointer<float>()[pTmp[i+1]*3];
    vVertices[1][1] = vertices.GetPointer<float>()[pTmp[i+1]*3+1];
    vVertices[1][2] = vertices.GetPointer<float>()[pTmp[i+1]*3+2];
    
    vVertices[2][0] = vertices.GetPointer<float>()[pTmp[i+2]*3];
    vVertices[2][1] = vertices.GetPointer<float>()[pTmp[i+2]*3+1];
    vVertices[2][2] = vertices.GetPointer<float>()[pTmp[i+2]*3+2];
    
    //cerr << "Checking: " << line.GetStart() << " and " << line.GetEnd() << endl;
    if ( LineIntersectsTriangle( line, vVertices[0], vVertices[1], vVertices[2], point))
    {
      vTangent = (vVertices[1]-vVertices[0]).GetNormalized();
      return 1;
    }
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  

  CMilkshapeLoader *pLoader = new CMilkshapeLoader();
  CVertexDescriptor *pVertexDescriptor = NULL; 
  CVertexDescriptor *pNormals = NULL;
  assert(pLoader->Load("Resources/Models/sphere.ms3d") == 0);
  
  pLoader->GenerateModelData( VERTEX_COMP_POSITION | VERTEX_COMP_NORMAL  );
  pVertexDescriptor = pLoader->GetVertices();
  pLoader->ResetVertices();

  assert( pLoader->GetIndices().size() == 1 );
  CIndexArray *pIndices = pLoader->GetIndices()[0];
  pLoader->ResetIndices();
  
  pNormals = pLoader->GetNormals();
  pLoader->ResetNormals();

  delete pLoader;
      
  if (  CSDLScreen::GetInstance() == NULL )
  {
    cerr << "Can't open screen" << endl;
  }

  CCamera camera;
  camera.SetViewport( 0,0, CSDLScreen::m_SDLScreenParams.m_iWidth, CSDLScreen::m_SDLScreenParams.m_iHeight );
  camera.SetPosition( 0,0, 30.0f);
  camera.SetNearClipping(0.1f);
  camera.SetFarClipping(600.0f);
  camera.SetFieldOfView(45.0f);
  camera.SetTrackballDistance(45.0f);
  SDL_Event event;
  int bLoop = 1;
  
  COglRenderer *pRenderer = new COglRenderer();
  CVector2<int> vMousePos((int)(camera.GetViewport()[2]*0.5f),  (int)(camera.GetViewport()[3]*0.5f)) ;
  int bMousePressed = 0;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  CFpsCounter fpsCounter;
  fpsCounter.Reset();

  pRenderer->CommitCache( *pVertexDescriptor );
  pRenderer->CommitCache( *pIndices );

  CLine line;
  line.SetStart( CVector3<float>(15,0,0) );
  line.SetEnd( CVector3<float>(3,0,0) );
  

  COglTexture *pTexture = pRenderer->CreateTexture( "Resources/Textures/crater.tga");
  while (  bLoop  )
  {
    fpsCounter.Update();
    while ( SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  bLoop = 0;
	} 
	else if ( event.key.keysym.sym == SDLK_s)
	{
	  camera.Move(camera.GetForwardVector()*-5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_w)
	{
	  camera.Move(camera.GetForwardVector()*5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_a)
	{
	  camera.Strafe(-5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_d)
	{
	  camera.Strafe(5.0f);
	}
	else if ( event.key.keysym.sym == SDLK_RIGHT)
	{
	  line.SetStart( line.GetStart() + CVector3<float>(1,0,0));
	  line.SetEnd( line.GetEnd() + CVector3<float>(1,0,0));
	}
	else if ( event.key.keysym.sym == SDLK_LEFT)
	{
	  line.SetStart( line.GetStart() - CVector3<float>(1,0,0));
	  line.SetEnd( line.GetEnd() - CVector3<float>(1,0,0));
	}
	else if ( event.key.keysym.sym == SDLK_DOWN)
	{
	  line.SetStart( line.GetStart() - CVector3<float>(0,1,0));
	  line.SetEnd( line.GetEnd() - CVector3<float>(0,1,0));
	}
	else if ( event.key.keysym.sym == SDLK_UP)
	{
	  line.SetStart( line.GetStart() + CVector3<float>(0,1,0));
	  line.SetEnd( line.GetEnd() + CVector3<float>(0,1,0));
	}
	else if ( event.key.keysym.sym == SDLK_PAGEUP)
	{
	  line.SetStart( line.GetStart() + CVector3<float>(0,0,1));
	  line.SetEnd( line.GetEnd() + CVector3<float>(0,0,1));
	}
	else if ( event.key.keysym.sym == SDLK_PAGEDOWN)
	{
	  line.SetStart( line.GetStart() - CVector3<float>(0,0,1));
	  line.SetEnd( line.GetEnd() - CVector3<float>(0,0,1));
	}
	break;
      case SDL_MOUSEBUTTONDOWN:
	bMousePressed = 1;
	break;
      case SDL_MOUSEBUTTONUP:
	bMousePressed = 0;
	break;
      case SDL_MOUSEMOTION:
	{
	  CVector2<int> vMousePosCurrent(event.motion.x, event.motion.y);
	  CVector2<int> vMouseDiff = vMousePos - vMousePosCurrent;
	  vMouseDiff[0] = -vMouseDiff[0];
	  //camera.RotateAroundUp(vMouseDiff[0]);
	  //camera.RotateAroundRight(vMouseDiff[1]);	
	  if ( bMousePressed ){
	    camera.VirtualTrackball( vMousePos,
				     vMousePosCurrent );
	  }
	  
	  vMousePos = vMousePosCurrent;
	  
	}
	break;
      default:
	break;
      }     
    }
    pRenderer->ClearBuffer( COLOR_BUFFER );
    pRenderer->ClearBuffer( DEPTH_BUFFER );
    pRenderer->CommitState( STATE_DEPTH_TEST );
    pRenderer->CommitState( STATE_FACECULLING );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
    pRenderer->CommitCamera( camera );
    pRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    pRenderer->CommitVertexDescriptor( pVertexDescriptor );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

    pRenderer->CommitPrimitive( pIndices );
    
    pRenderer->CommitColor( CVector4<unsigned char>(0,0,0,255));
    CVector3<float> vTmp = camera.WorldCoordinatesToEye( CVector3<float>(0,0,0));
    camera.SetDecalOffset( 0.1, vTmp[2] );
    camera.UpdateProjection();
    pRenderer->CommitCamera( camera );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    pRenderer->CommitPrimitive( pIndices );

    CVector3<float> vPoint;
    CVector3<float> vTangent;

    std::list< std::list< CVertex > > lstTriFans;
    int bHits = 0;
    if ( CalculateHitpoint( line, *pVertexDescriptor, *pIndices, vPoint, vTangent ))
    {
      bHits = 1;
      CDecalVolume decal( vPoint, -line.GetDirection(), vTangent, 3.0f, 3.0f,0.20f );
      CalculateDecalMesh( decal, *pVertexDescriptor, *pNormals, *pIndices, lstTriFans );
      CVector3<float> vTmp = camera.WorldCoordinatesToEye( vPoint );
      camera.SetDecalOffset( 0.1, vTmp[2] );
      camera.UpdateProjection();
      pRenderer->CommitCamera( camera );
    }
    else 
    {
      bHits = 0;
    }

    glBegin(GL_LINES);

      glColor3f(1,0,0);
      glVertex3fv( line.GetStart().GetArray() );
      if ( !bHits )        
	glColor3f(0,1,0);
      glVertex3fv( line.GetEnd().GetArray() );
    glEnd();
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
    // Draw decal mesh
    std::list< std::list< CVertex > >::iterator it;
    pRenderer->CommitTexture( 0, pTexture );
    pRenderer->CommitState( STATE_BLENDING );
    pRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA );
    for( it = lstTriFans.begin(); it != lstTriFans.end(); it++)
    {

      std::list< CVertex >::iterator fanIt;
      glColor3f(1,1,1);
      pRenderer->CommitState( STATE_DEPTH_TEST );
      pRenderer->DisableState ( STATE_DEPTH_WRITE );
      glBegin(GL_TRIANGLE_FAN);
      for( fanIt = (*it).begin(); fanIt != (*it).end(); fanIt++)
      {
	pRenderer->CommitVertex( *fanIt );
      }
      glEnd();
    }
    pRenderer->DisableTexture( 0, pTexture );
    pRenderer->CommitState ( STATE_DEPTH_WRITE );
    pRenderer->DisableState( STATE_BLENDING );
    glColor3f(1,1,1);
    camera.ResetDecalOffset();
    camera.UpdateProjection();
    
    pRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter++;
    if ( fpsCounter.HasPassed(1,0) )
    {
      cerr << "FPS: " << fpsCounter << endl;
      fpsCounter.Reset();
    }    
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
