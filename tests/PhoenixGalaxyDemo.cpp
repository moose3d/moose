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
const float g_fRotationSpeedPerSec = 1.15f;
/////////////////////////////////////////////////////////////////
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  CCamera camera;
  camera.SetPosition( 0, 0.2f,0.0f);
  //camera.SetViewport( 480,340, 160, 120 );
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 43.0f);
  camera.RotateAroundRight(-90.0f);

  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  


  COglTexture *pMilkyWayTexture = pOglRenderer->CreateTexture("Resources/Textures/MilkyWay.tga");
  COglTexture *pMilkyWayTextureSide = pOglRenderer->CreateTexture("Resources/Textures/MilkyWaySide.tga");

  CVertex v1, v2, v3, v4;
  CVector4<unsigned char> white(255,255,255,255);
  
  v1.SetPosition(-1,0,1);
  v1.SetColor( white );
  v1.SetTextureCoordinates(0,0);

  v2.SetPosition( 1,0,1);
  v2.SetColor( white);
  v2.SetTextureCoordinates(1,0);

  v3.SetPosition( 1,0,-1);
  v3.SetColor( white );  
  v3.SetTextureCoordinates(1,1);

  v4.SetPosition( -1,0,-1);
  v4.SetColor( white);  
  v4.SetTextureCoordinates(0,1);

//   CVertex vs1, vs2, vs3, vs4;
  
//   vs1.SetPosition(0,-0.26,0);
//   vs1.SetColor( white );
//   vs1.SetTextureCoordinates(0,0);

//   vs2.SetPosition(0, -0.26, 0);
//   vs2.SetColor( white);
//   vs2.SetTextureCoordinates(1,0);

//   vs3.SetPosition( 0,0.26f,0);
//   vs3.SetColor( white );  
//   vs3.SetTextureCoordinates(1,1);

//   vs4.SetPosition( 0,-0.26,0);
//   vs4.SetColor( white);  
//   vs4.SetTextureCoordinates(0,1);

  int i=0;

  CVertexDescriptor *pVertexDescriptor = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, 8);
  pVertexDescriptor->GetPointer<float>()[i++] = -1.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  1.0;

  pVertexDescriptor->GetPointer<float>()[i++] = 1.0;
  pVertexDescriptor->GetPointer<float>()[i++] = 0.0;
  pVertexDescriptor->GetPointer<float>()[i++] = 1.0;

  pVertexDescriptor->GetPointer<float>()[i++] =  1.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] = -1.0;
  
  pVertexDescriptor->GetPointer<float>()[i++] = -1.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] = -1.0;
  
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  -0.130;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  -0.130;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.130;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.130;
  pVertexDescriptor->GetPointer<float>()[i++] =  0.0;

  i=0;  
  CVertexDescriptor *pTexCoords = new CVertexDescriptor(ELEMENT_TYPE_TEX_2F, 8);
  pTexCoords->GetPointer<float>()[i++] =  0.0;
  pTexCoords->GetPointer<float>()[i++] =  0.0;

  pTexCoords->GetPointer<float>()[i++] = 1.0;
  pTexCoords->GetPointer<float>()[i++] = 0.0;

  pTexCoords->GetPointer<float>()[i++] =  1.0;
  pTexCoords->GetPointer<float>()[i++] =  1.0;
  
  pTexCoords->GetPointer<float>()[i++] =  0.0;
  pTexCoords->GetPointer<float>()[i++] =  1.0;

  pTexCoords->GetPointer<float>()[i++] =  0.0;
  pTexCoords->GetPointer<float>()[i++] =  0.0;
  
  pTexCoords->GetPointer<float>()[i++] =  1.0;
  pTexCoords->GetPointer<float>()[i++] =  0.0;

  pTexCoords->GetPointer<float>()[i++] =  1.0;
  pTexCoords->GetPointer<float>()[i++] =  1.0;
  
  pTexCoords->GetPointer<float>()[i++] =  0.0;
  pTexCoords->GetPointer<float>()[i++] =  1.0;
  
  i=0;
  CVertexDescriptor *pShaderAttrib = new CVertexDescriptor(ELEMENT_TYPE_UNIFORM_3F,1);
  pShaderAttrib->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib->GetPointer<float>()[i++] = 0.0;

  i=0;
  CVertexDescriptor *pShaderAttrib2 = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F,8);

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;

  // Actual shader stuff begins
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;

  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = -1.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 0.0;
  pShaderAttrib2->GetPointer<float>()[i++] = 1.0;

  CIndexArray *pIndices = new CIndexArray(PRIMITIVE_QUAD_LIST, 4);
  pIndices->GetPointer<unsigned short int>()[0] = 4;
  pIndices->GetPointer<unsigned short int>()[1] = 5;
  pIndices->GetPointer<unsigned short int>()[2] = 6;
  pIndices->GetPointer<unsigned short int>()[3] = 7;

  ////////////////////
  // Transform stuff which makes things go round and round
  Phoenix::Math::CTransform transform;
  CQuaternion q;
  CTimer timer;
  timer.Reset();
  CShader *pBillBoardShader = pOglRenderer->CreateShader( "Resources/Shaders/bbquad_vertex.glsl", 
							  "Resources/Shaders/bbquad_frag.glsl" );
  int bMousePressed = 0;
  CVector2<int> vMouseStart, vMouseEnd;
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
    pShaderAttrib->GetPointer<float>()[0] = camera.GetPosition()[0];
    pShaderAttrib->GetPointer<float>()[1] = camera.GetPosition()[1];
    pShaderAttrib->GetPointer<float>()[2] = camera.GetPosition()[2];

    q.CreateFromAxisAngle( 0,1,0, g_fRotationSpeedPerSec * ((float)timer.GetPassedTimeMS())/1000.0f);
    timer.Reset();
    transform.Rotate( q );
    //cerr << "Collected: " << nCollected << endl;
    
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->CommitState( STATE_BLENDING );
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA );
    pOglRenderer->CommitCamera( camera );
    
    
    pOglRenderer->CommitTransform( transform );

      pOglRenderer->CommitTexture( 0, pMilkyWayTexture );
      pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitQuad( v1,v2,v3,v4); 
   pOglRenderer->RollbackTransform();
      
      pOglRenderer->CommitTexture( 0, pMilkyWayTextureSide );
      //pOglRenderer->DisableTexture( 0, pMilkyWayTexture );


      pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitVertexDescriptor( pVertexDescriptor );
      pOglRenderer->CommitVertexDescriptor( pTexCoords );
      pOglRenderer->CommitShader(pBillBoardShader);
      //pOglRenderer->CommitShaderParam( *pBillBoardShader, "camerapos", *pShaderAttrib);
        pOglRenderer->CommitShaderParam( *pBillBoardShader, "dirWidth", *pShaderAttrib2);
        pOglRenderer->CommitUniformShaderParam( *pBillBoardShader, "beamTexture", 0 );
        pOglRenderer->CommitPrimitive( pIndices );
        //pOglRenderer->CommitQuad( vs1,vs2,vs3,vs4); 
      pOglRenderer->CommitShader( NULL );





    camera.UpdateProjection();
    camera.UpdateView();
    camera.CalculateFrustum();    
    camera.CalculateBoundingSphere();
    camera.CalculateBoundingCone();

    pOglRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
    timer.Update();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
