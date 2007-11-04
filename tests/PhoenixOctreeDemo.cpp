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
using std::cerr;
using std::endl; 
unsigned int g_nDrawnNodes = 0;
/////////////////////////////////////////////////////////////////
void DrawNode ( COctreeNode<float> *pNode )
{
  g_nDrawnNodes++;
  float fHalfWidth = pNode->GetHalfWidth() ;
  
  CVector3<float> aCorners[8];
  aCorners[0] = pNode->GetPosition();
  aCorners[1] = pNode->GetPosition();
  aCorners[2] = pNode->GetPosition();
  aCorners[3] = pNode->GetPosition();
  aCorners[4] = pNode->GetPosition();
  aCorners[5] = pNode->GetPosition();
  aCorners[6] = pNode->GetPosition();
  aCorners[7] = pNode->GetPosition();
  
  aCorners[0][0] -= fHalfWidth;
  aCorners[0][1] -= fHalfWidth;
  aCorners[0][2] -= fHalfWidth;
  
  aCorners[1][0] += fHalfWidth;
  aCorners[1][1] -= fHalfWidth;
  aCorners[1][2] -= fHalfWidth;

  aCorners[2][0] += fHalfWidth;
  aCorners[2][1] -= fHalfWidth;
  aCorners[2][2] += fHalfWidth;

  aCorners[3][0] -= fHalfWidth;
  aCorners[3][1] -= fHalfWidth;
  aCorners[3][2] += fHalfWidth;

  aCorners[4][0] -= fHalfWidth;
  aCorners[4][1] += fHalfWidth;
  aCorners[4][2] -= fHalfWidth;
  
  aCorners[5][0] += fHalfWidth;
  aCorners[5][1] += fHalfWidth;
  aCorners[5][2] -= fHalfWidth;

  aCorners[6][0] += fHalfWidth;
  aCorners[6][1] += fHalfWidth;
  aCorners[6][2] += fHalfWidth;

  aCorners[7][0] -= fHalfWidth;
  aCorners[7][1] += fHalfWidth;
  aCorners[7][2] += fHalfWidth;

  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);

    glVertex3fv( aCorners[0].GetArray() );
    glVertex3fv( aCorners[1].GetArray() );
    glVertex3fv( aCorners[2].GetArray() );
    glVertex3fv( aCorners[3].GetArray() );

    glVertex3fv( aCorners[4].GetArray() );
    glVertex3fv( aCorners[5].GetArray() );
    glVertex3fv( aCorners[6].GetArray() );
    glVertex3fv( aCorners[7].GetArray() );
    
    glVertex3fv( aCorners[0].GetArray() );
    glVertex3fv( aCorners[1].GetArray() );
    glVertex3fv( aCorners[5].GetArray() );
    glVertex3fv( aCorners[4].GetArray() );

    glVertex3fv( aCorners[3].GetArray() );
    glVertex3fv( aCorners[2].GetArray() );
    glVertex3fv( aCorners[6].GetArray() );
    glVertex3fv( aCorners[7].GetArray() );

  glEnd();
  
}
/////////////////////////////////////////////////////////////////
void DrawOctree( COctreeNode<float> *pOctreeNode, CCamera &camera  )
{

  if ( pOctreeNode == NULL )
  {
    return;
  }

  
  int bSphere = Phoenix::Collision::SphereIntersectsAACube( camera.FrustumSphere(), *pOctreeNode );
  
  if ( bSphere && camera.Frustum().IntersectsCube( *pOctreeNode ))
  {
    //cerr << "****drawing octree: " << pOctreeNode->GetPosition() << ", w=" << pOctreeNode->GetWidth() << endl;
    // Draw this node
    DrawNode( pOctreeNode );
    // recursively draw children.
    DrawOctree( pOctreeNode->GetChild(TOP_LEFT_BACK), camera);
    DrawOctree( pOctreeNode->GetChild(TOP_LEFT_FRONT), camera);
    DrawOctree( pOctreeNode->GetChild(TOP_RIGHT_FRONT), camera);
    DrawOctree( pOctreeNode->GetChild(TOP_RIGHT_BACK), camera);
    DrawOctree( pOctreeNode->GetChild(BOTTOM_LEFT_FRONT), camera);
    DrawOctree( pOctreeNode->GetChild(BOTTOM_LEFT_BACK), camera);
    DrawOctree( pOctreeNode->GetChild(BOTTOM_RIGHT_FRONT), camera);
    DrawOctree( pOctreeNode->GetChild(BOTTOM_RIGHT_BACK), camera);
  }
}
/////////////////////////////////////////////////////////////////
void PrintCorners( CFrustum &frustum )
{
   cerr << "BOTTOM_NEAR_LEFT" <<  frustum.GetCorner(CFrustum::BOTTOM_NEAR_LEFT) << endl;
   cerr << "BOTTOM_NEAR_RIGHT" <<  frustum.GetCorner(CFrustum::BOTTOM_NEAR_RIGHT)<< endl;
   cerr << "BOTTOM_FAR_RIGHT" <<  frustum.GetCorner(CFrustum::BOTTOM_FAR_RIGHT)<< endl;
   cerr << "BOTTOM_FAR_LEFT" <<  frustum.GetCorner(CFrustum::BOTTOM_FAR_LEFT)<< endl;
  
   cerr << "TOP_NEAR_LEFT" <<  frustum.GetCorner(CFrustum::TOP_NEAR_LEFT)<< endl;
   cerr << "TOP_NEAR_RIGHT" <<  frustum.GetCorner(CFrustum::TOP_NEAR_RIGHT)<< endl;
   cerr << "TOP_FAR_RIGHT" <<  frustum.GetCorner(CFrustum::TOP_FAR_RIGHT)<< endl;
   cerr << "TOP_FAR_LEFT" <<  frustum.GetCorner(CFrustum::TOP_FAR_LEFT)<< endl;
}
/////////////////////////////////////////////////////////////////
void DrawFrustum( CCamera &camera )
{

  CFrustum &frustum = camera.Frustum();
  Phoenix::Volume::CSphere &sphere = camera.FrustumSphere();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_CULL_FACE);
  


//   std::cerr << "NEAR :" << frustum.GetPlane(Phoenix::Volume::BACK) << std::endl;
//   std::cerr << "FAR :" << frustum.GetPlane(Phoenix::Volume::FRONT) << std::endl;
//   std::cerr << "TOP :" << frustum.GetPlane(Phoenix::Volume::TOP) << std::endl;
//   std::cerr << "BOTTOM :" << frustum.GetPlane(Phoenix::Volume::BOTTOM) << std::endl;
//   std::cerr << "LEFT :" << frustum.GetPlane(Phoenix::Volume::LEFT) << std::endl;
//   std::cerr << "RIGHT :" << frustum.GetPlane(Phoenix::Volume::RIGHT) << std::endl;

  
  

  glColor3f(1,0,0);
  glBegin(GL_QUADS);
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_LEFT).GetArray());

    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_LEFT).GetArray());
    
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_LEFT).GetArray());

    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_RIGHT).GetArray());

  glEnd();
  glColor3f(0,1,0);
  GLUquadric * q = gluNewQuadric();
  //cerr << "sphere radius: " << sphere.GetRadius() << endl;
  //cerr << "sphere pos: " << sphere.GetPosition() << endl;
  gluQuadricDrawStyle(q, GLU_SILHOUETTE);
  glPushMatrix();
    glTranslatef( sphere.GetPosition()[0], 
		  sphere.GetPosition()[1],
		  sphere.GetPosition()[2]);
    glRotatef( 90.0f, 1, 0, 0);
    gluDisk(q, 1.0, sphere.GetRadius(), 24, 24);
  glPopMatrix();
  gluDeleteQuadric(q);
}
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }

  CCamera test;
  test.SetPosition( 0.0f, 0.0f,0.0f);
  test.SetViewport( 0,0, 640, 480 );
  test.SetNearClipping( 0.1f);
  test.SetFarClipping( 20.0f );
  test.SetViewOrtho( -200,200, -150, 150);
  test.SetFieldOfView( 45.0f);

  CCamera camera;
  camera.SetPosition( 0, 10.0f,0.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 800.0f );
  camera.SetViewOrtho( -200,200, -150, 150);

  camera.RotateAroundRight( -90.0f);
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  

  COctree<float> *pOctree = new COctree<float>(5,100.0f);
  
  COctreeNode<float> *pOctreeNode = new COctreeNode<float>();
  pOctreeNode->SetPosition( 0, 0, 0 );
  pOctreeNode->SetWidth( 20.0f);
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
	  test.Move( 1.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  test.Move( -1.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  test.RotateAroundUp( 2.93f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  test.RotateAroundUp( -2.93f );
	} 
	else if ( event.key.keysym.sym == SDLK_RETURN )
	{
	  cerr << test.FrustumSphere() << endl;
	  std::cerr << "Number of visible nodes " 
		    << g_nDrawnNodes << std::endl;
	}
	break;
      default:
	break;
      }
    }

    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->CommitCamera( camera );

    test.UpdateProjection();
    test.UpdateView();
    test.CalculateFrustum();    
    test.CalculateBoundingSphere();
    test.CalculateBoundingCone();
    CVector4<unsigned char> color(255,255,255,255);
    pOglRenderer->CommitColor(color );
    DrawFrustum( test);
    g_nDrawnNodes = 0;
    DrawOctree( pOctree->GetRoot(), test );

    pOglRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
