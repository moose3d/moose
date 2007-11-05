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
  string strTexFighterBump="Resources/Textures/omega_normal.tga";

  string strVertexTest("VertexTest");
  string strTexCoords("TexCoordTest");
  string strIndexTest("IndexTest");
  string strTextureTest("TextureTest");
  string strTextureTest2("Texture2");
  string tmpName;

  ////////////////////
  CVertexDescriptor *pVertices = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F, 4 );

  pVertices->GetPointer<float>()[0] = -1.0f;
  pVertices->GetPointer<float>()[1] = 1.0f;
  pVertices->GetPointer<float>()[2] = 0.0f;
  
  pVertices->GetPointer<float>()[3] = -1.0f;
  pVertices->GetPointer<float>()[4] = -1.0f;
  pVertices->GetPointer<float>()[5] = 0.0f;

  pVertices->GetPointer<float>()[6] = 1.0f;
  pVertices->GetPointer<float>()[7] = 1.0f;
  pVertices->GetPointer<float>()[8] = 0.0f;

  pVertices->GetPointer<float>()[9] =  1.0f;
  pVertices->GetPointer<float>()[10] = -1.0f;
  pVertices->GetPointer<float>()[11] = 0.0f;
  ////////////////////  
  CVertexDescriptor *pColors = new CVertexDescriptor( ELEMENT_TYPE_COLOR_4UB, 4 );
  
  pColors->GetPointer<unsigned char>()[0] = 255;
  pColors->GetPointer<unsigned char>()[1] = 0;
  pColors->GetPointer<unsigned char>()[2] = 0;
  pColors->GetPointer<unsigned char>()[3] = 255;

  pColors->GetPointer<unsigned char>()[4] = 0;
  pColors->GetPointer<unsigned char>()[5] = 255;
  pColors->GetPointer<unsigned char>()[6] = 0;
  pColors->GetPointer<unsigned char>()[7] = 255;

  pColors->GetPointer<unsigned char>()[8] = 0;
  pColors->GetPointer<unsigned char>()[9] = 0;
  pColors->GetPointer<unsigned char>()[10] = 255;
  pColors->GetPointer<unsigned char>()[11] = 255;

  pColors->GetPointer<unsigned char>()[12] = 255;
  pColors->GetPointer<unsigned char>()[13] = 255;
  pColors->GetPointer<unsigned char>()[14] = 0;
  pColors->GetPointer<unsigned char>()[15] = 255;
  ////////////////////
  CVertexDescriptor *pTexCoords = new CVertexDescriptor( ELEMENT_TYPE_TEX_2F, 4 );
  pTexCoords->GetPointer<float>()[0] = 0.0f;
  pTexCoords->GetPointer<float>()[1] = 1.0f;
  pTexCoords->GetPointer<float>()[2] = 0.0f;
  pTexCoords->GetPointer<float>()[3] = 0.0f;
  pTexCoords->GetPointer<float>()[4] = 1.0f;
  pTexCoords->GetPointer<float>()[5] = 1.0f;
  pTexCoords->GetPointer<float>()[6] = 1.0f;
  pTexCoords->GetPointer<float>()[7] = 0.0f;
  ////////////////////
  CVertexDescriptor *pNormals = new CVertexDescriptor( ELEMENT_TYPE_NORMAL_3F, 4);
  pNormals->GetPointer<float>()[0] = 0.0f;
  pNormals->GetPointer<float>()[1] = 0.0f;
  pNormals->GetPointer<float>()[2] = 1.0f;
  pNormals->GetPointer<float>()[3] = 0.0f;
  pNormals->GetPointer<float>()[4] = 0.0f;
  pNormals->GetPointer<float>()[5] = 1.0f;
  pNormals->GetPointer<float>()[6] = 0.0f;
  pNormals->GetPointer<float>()[7] = 0.0f;
  pNormals->GetPointer<float>()[8] = 1.0f;
  pNormals->GetPointer<float>()[9] = 0.0f;
  pNormals->GetPointer<float>()[10] = 0.0f;
  pNormals->GetPointer<float>()[11] = 1.0f;

  CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, 6 );
  pIndices->GetPointer<unsigned short int>()[0] = 0;
  pIndices->GetPointer<unsigned short int>()[1] = 1;
  pIndices->GetPointer<unsigned short int>()[2] = 2;
  pIndices->GetPointer<unsigned short int>()[3] = 1;
  pIndices->GetPointer<unsigned short int>()[4] = 3;
  pIndices->GetPointer<unsigned short int>()[5] = 2;
  COglRenderer *pOglRenderer = new COglRenderer();



  COglTexture *pTexture  = pOglRenderer->CreateTexture(strTexFilename);
  COglTexture *pTexture2 = pOglRenderer->CreateTexture(strTexFilename2);
  COglTexture *pTextureRock = pOglRenderer->CreateTexture((strTexFilename2="Resources/Textures/wall_transparent.tga"));
  COglTexture *pTextureBump = pOglRenderer->CreateTexture((strTexFilename2="Resources/Textures/wall_normal.tga"));

  COglTexture *pTextureShip = pOglRenderer->CreateTexture((strTexFilename2="Resources/Textures/spaceship.tga"));
  COglTexture *pTextureShipBump = pOglRenderer->CreateTexture((strTexFilename2="Resources/Textures/spaceship_normal.tga"));
 

  COglTexture *pTextureFighterBump = pOglRenderer->CreateTexture( strTexFighterBump );
  
  assert( pTexture2 != NULL);


  INDEX_HANDLE hIndexHandle;


  

  
  
  
  CCamera camera;
  camera.SetPosition( 0,0,2.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 45.0f);
  SDL_Event event;
  CVector3<float> vMove(0,0,0.2f);
  
  CLight light;
  light.SetPosition(0,0.0f,3.0f);
  light.SetSpotExponent(1.0f);
  CVector3<float> vDir(0,0,-1);
  vDir.Normalize();
  light.SetDirection( vDir );
  light.SetType(SPOTLIGHT);
  light.SetConstantAttenuation(0.0f);
  light.SetLinearAttenuation(0.01f);
  light.SetQuadraticAttenuation(0.01f);
  light.SetSpotAngle(1.0f);
  CVector4<unsigned char> vColor(155,155,155,255);
  light.SetDiffuseColor(vColor);
  light.SetAmbientColor(vColor = CVector4<unsigned char>(10,10,10,255));
  light.SetSpecularColor(vColor = CVector4<unsigned char>(255,255,255,255));
  

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);



  
  
  CMaterial material;
  material.SetDiffuse( CVector4<float>(0.86,0.86,0.86,1.0f));
  material.SetAmbient( CVector4<float>(0.26,0.26,0.26,1.0f));
  material.SetSpecular( CVector4<float>(1,1,1,1));
  material.SetShininess( 0.8000000f);

  CMaterial materialShip;
  materialShip.SetDiffuse( CVector4<float>(0.86,0.86,0.86,1.0f));
  materialShip.SetAmbient( CVector4<float>(0.26,0.26,0.26,1.0f));
  materialShip.SetSpecular( CVector4<float>(0.15f,0.15f,0.15f,1.0f));
  materialShip.SetShininess( 0.138f);


  

  float fAngle = 0.0f;
  float fMagnitude = 12.0f;
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  CFpsCounter fpsCounter;
  fpsCounter.Reset();
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
    
    ////////////////////
    // Rotate light using  angle.
    light.SetPosition( fMagnitude * sinf(fAngle), fMagnitude * cosf(fAngle), light.GetPosition()[2]);
    fAngle += 0.004f;
    if ( fAngle > 360.0f ) fAngle -= 360.0f;
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitState( STATE_LIGHTING );
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->CommitLight( light );    
    
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //glEnable(GL_DEPTH_TEST);
    
    
    pOglRenderer->CommitMaterial( material );
    // Draw colored triangle
    pOglRenderer->CommitVertexDescriptor( pVertices );
    //pOglRenderer->CommitVertexDescriptor( pColors );
    pOglRenderer->CommitVertexDescriptor( pNormals );
    pOglRenderer->CommitVertexDescriptor( pTexCoords );
    pOglRenderer->CommitVertexDescriptor( pTexCoords, 1);
    pOglRenderer->CommitTexture( 0, pTextureRock );
    pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
    pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);
    pOglRenderer->CommitTexture( 1, pTextureBump );
    pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
    pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);


    //glRotatef( 40.0f, 0,1,0);
    //glTranslatef( 2,0,0);
    pOglRenderer->CommitPrimitive( pIndices );

    glPushMatrix();
      glTranslatef(-2.5f,0,0);
      pOglRenderer->CommitTexture( 0, pTextureShip );
      pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitTexture( 1, pTextureShipBump );
      pOglRenderer->CommitFilter( MIN_MIP_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitFilter( MAG_LINEAR, TEXTURE_2D);
      pOglRenderer->CommitMaterial( materialShip );
      pOglRenderer->CommitPrimitive( pIndices );
    glPopMatrix();

    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableLight( 0 );
    // Draw textured / transparent 
    pOglRenderer->CommitTexture( 0, pTexture );
    glPushMatrix();
    glTranslatef(2.5f,0,0);
    // glEnable(GL_ALPHA_TEST);
//     glAlphaFunc( GL_GREATER, 0.3f);
//     glTranslatef( 0,0,0.3f);

    pOglRenderer->CommitVertexDescriptor( pTexCoords );
    pOglRenderer->DisableClientState( CLIENT_STATE_COLOR_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_VERTEX_ARRAY );
    //pOglRenderer->DisableClientState( CLIENT_STATE_TEX0_ARRAY );
    
    CVector4<unsigned char> vWhite(255,255,255,255);
    pOglRenderer->CommitColor( vWhite );

    pOglRenderer->CommitTexture( 0, pTexture );
    pOglRenderer->CommitPrimitive( pIndices );

    pOglRenderer->CommitShader( NULL );
    glPopMatrix();
    
    pOglRenderer->DisableClientState( CLIENT_STATE_VERTEX_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_COLOR_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX0_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX1_ARRAY );
    pOglRenderer->DisableTexture( 0, pTexture );
    pOglRenderer->DisableTexture( 1, pTexture );
    


    pOglRenderer->DisableState( STATE_ALPHA_TEST );
    pOglRenderer->DisableState( STATE_DEPTH_TEST );
    pOglRenderer->Finalize();
    
    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter.AddFrame();
    if ( fpsCounter.HasPassedMS(1000) )
    {
      std::cerr << "FPS: " << fpsCounter.GetFPS() << std::endl;
      fpsCounter.Reset();
    }
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
