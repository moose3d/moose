#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
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

  CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_STRIP, 4 );
  pIndices->GetPointer<unsigned short int>()[0] = 0;
  pIndices->GetPointer<unsigned short int>()[1] = 1;
  pIndices->GetPointer<unsigned short int>()[2] = 2;
  pIndices->GetPointer<unsigned short int>()[3] = 3;

  COglRenderer *pOglRenderer = new COglRenderer();
  string strTexFilename("Resources/Textures/painting.tga");
  string strTexFilename2("Resources/Textures/lightmap.tga");
  COglTexture *pTexture  = pOglRenderer->CreateTexture(strTexFilename);
  COglTexture *pTexture2 = pOglRenderer->CreateTexture(strTexFilename2);
  assert( pTexture2 != NULL);
  VERTEX_HANDLE hVertexHandle;
  VERTEX_HANDLE hTexCoordHandle;
  TEXTURE_HANDLE hTextureHandle;
  TEXTURE_HANDLE hTextureHandle2;
  INDEX_HANDLE hIndexHandle;
  string strVertexTest("VertexTest");
  string strTexCoords("TexCoordTest");
  string strIndexTest("IndexTest");
  string strTextureTest("TextureTest");
  string strTextureTest2("Texture2");
  // Create vertex resource
  assert( g_DefaultVertexManager->Create( pVertices, strVertexTest, hVertexHandle ) == 0);
  // create texture coordinate resource
  assert( g_DefaultVertexManager->Create( pTexCoords, strTexCoords, hTexCoordHandle ) == 0);
  // create index array resource
  assert( g_DefaultIndexManager->Create( pIndices, strIndexTest, hIndexHandle ) == 0);
  // create texture resource
  assert( g_DefaultTextureManager->Create( pTexture, strTextureTest, hTextureHandle ) == 0);
  // create another texture resource 
  assert( g_DefaultTextureManager->Create( pTexture2, strTextureTest2, hTextureHandle2 ) == 0);
  // check that index resource actually exists
  assert( g_DefaultIndexManager->GetResource( hIndexHandle ) != NULL );

  CCamera camera;
  camera.SetPosition( 0,0,2.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 45.0f);
  SDL_Event event;
  CVector3<float> vMove(0,0,0.2f);

  CModel model;
  model.SetVertexHandle( hVertexHandle );
  model.SetIndexHandle( hIndexHandle );
  model.SetTextureHandle( hTextureHandle );
  model.SetTextureHandle( hTextureHandle2, 1 );
  model.SetTextureCoordinateHandle( hTexCoordHandle );
  model.SetTextureCoordinateHandle( hTexCoordHandle, 1);
  model.AddTextureFilter( ENV_REPLACE, 0 );
  model.AddTextureFilter( MIN_MIP_LINEAR, 0 );
  model.AddTextureFilter( MAG_LINEAR, 0 );
  model.AddTextureFilter( ENV_MODULATE, 1 );
  model.AddTextureFilter( MIN_MIP_LINEAR, 1 );
  model.AddTextureFilter( MAG_LINEAR, 1 );
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  CShader *pShader = pOglRenderer->CreateShader( std::string("Resources/Shaders/vertex.glsl"), std::string("Resources/Shaders/fragment.glsl") );
  assert(pShader != NULL );
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
	break;
      default:
	break;
      }
    }
    
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitCamera( camera );
    // Draw colored triangle
    pOglRenderer->CommitVertexDescriptor( pVertices );
    pOglRenderer->CommitVertexDescriptor( pColors );
    pOglRenderer->CommitPrimitive( pIndices );
    // Draw textured / transparent triangle
    //pOglRenderer->CommitTexture( 0, pTexture );
    glPushMatrix();
    glTranslatef(1.0f,0,0);
    // glEnable(GL_ALPHA_TEST);
//     glAlphaFunc( GL_GREATER, 0.3f);
//     glTranslatef( 0,0,0.3f);

    //pOglRenderer->CommitVertexDescriptor( pTexCoords );
    pOglRenderer->DisableClientState( CLIENT_STATE_COLOR_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_VERTEX_ARRAY );
    //pOglRenderer->DisableClientState( CLIENT_STATE_TEX0_ARRAY );
    
    CVector4<unsigned char> vWhite(255,255,255,255);
    pOglRenderer->CommitColor( vWhite );

    //pOglRenderer->CommitTexture( 0, pTexture );
    //pOglRenderer->CommitPrimitive( pIndices );
    pOglRenderer->CommitShader( pShader );
    pOglRenderer->CommitUniformShaderParam( *pShader, "tex0" , 0 );
    pOglRenderer->CommitUniformShaderParam( *pShader, "tex1" , 1 );
    pOglRenderer->CommitModel( model );
    pOglRenderer->CommitShader( NULL );
    glPopMatrix();
    
    pOglRenderer->DisableClientState( CLIENT_STATE_VERTEX_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX0_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX1_ARRAY );
    pOglRenderer->DisableTexture( 0, pTexture );
    pOglRenderer->DisableTexture( 1, pTexture );
    pOglRenderer->Finalize();

    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
