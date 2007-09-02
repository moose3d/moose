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
  string strTexFighterBump="/home/entity/omega_normal.tga";
#ifdef OMEGA 
  string strTexFighter="/home/entity/omega.tga";
  std::string name("/home/entity/omega.ms3d");
#else
  string strTexFighter="Resources/Textures/wilko.tga";
  std::string name("Resources/Models/fighter1.ms3d");
#endif    
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
 
  COglTexture *pTextureFighter = pOglRenderer->CreateTexture( strTexFighter );
  COglTexture *pTextureFighterBump = pOglRenderer->CreateTexture( strTexFighterBump );
  
  assert( pTexture2 != NULL);
  VERTEX_HANDLE hVertexHandle;
  VERTEX_HANDLE hTexCoordHandle;
  TEXTURE_HANDLE hTextureHandle;
  TEXTURE_HANDLE hTextureHandle2;
  TEXTURE_HANDLE hFighterTexture;
  TEXTURE_HANDLE hFighterTextureBump;
  INDEX_HANDLE hIndexHandle;

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

  string strFighterTextureName = "fighter_texture";
  assert( g_DefaultTextureManager->Create( pTextureFighter, strFighterTextureName, hFighterTexture ) == 0);
  assert( g_DefaultTextureManager->Create( pTextureFighterBump, (tmpName="fighter_texture_bump"),   hFighterTextureBump ) == 0);
  
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
  
  CModel model;
  model.SetVertexHandle( hVertexHandle );
  model.AddIndexHandle( hIndexHandle );
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

  CShader *pShader = pOglRenderer->CreateShader( std::string("Resources/Shaders/vertex.glsl"), 
						 std::string("Resources/Shaders/fragment.glsl") );
  

  CShader *pBumpShader = pOglRenderer->CreateShader( std::string("Resources/Shaders/bump_vertex.glsl"), 
						     std::string("Resources/Shaders/bump_frag.glsl") );
  CVertexDescriptor *pTangents = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F, 4);
  CalculateTangentArray( *pVertices, *pNormals, *pTexCoords, *pIndices, *pTangents );
  assert(pShader != NULL );
  assert(pBumpShader != NULL );
  SHADER_HANDLE hShaderHandle;
  assert(g_DefaultShaderManager->Create( pBumpShader, (tmpName="BumpShader"), hShaderHandle) == 0 );
  
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


  Phoenix::Data::CMilkshapeLoader loader;

  assert ( loader.Load( name ) == 0 && "Could not open model file!");
  loader.GenerateModelData();

  CVertexDescriptor *pOmegaTangents = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F, loader.GetVertices()->GetSize());
  CalculateTangentArray( *loader.GetVertices(), 
			 *loader.GetNormals(), 
			 *loader.GetTexCoords(), 
			 *(loader.GetIndices()[0]), 
			 *pOmegaTangents);
  loader.Stripify();
  
  CModel fighterModel;
  VERTEX_HANDLE hFighterVertices;
  VERTEX_HANDLE hFighterTexCoords;
  INDEX_HANDLE  hFighterIndices;
  VERTEX_HANDLE hFighterTangents;
  VERTEX_HANDLE hFighterNormals;

  assert(g_DefaultVertexManager->Create( loader.GetVertices(), (tmpName="fighter_vertices"), hFighterVertices ) == 0);  
  assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), (tmpName="fighter_texcoords"), hFighterTexCoords )== 0);
  assert(g_DefaultVertexManager->Create( loader.GetNormals(), (tmpName="fighter_normals"), hFighterNormals )== 0);
  assert(g_DefaultVertexManager->Create( pOmegaTangents, (tmpName="fighter_tangents"), hFighterTangents )== 0);
  
  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "fighter_indices" << i;
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], stream.str(), hFighterIndices ) == 0);
    fighterModel.AddIndexHandle( hFighterIndices );
  }

  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetIndices();

  fighterModel.SetVertexHandle( hFighterVertices );
  fighterModel.SetNormalHandle( hFighterNormals );
  fighterModel.SetTextureCoordinateHandle( hFighterTexCoords );
  fighterModel.SetTextureHandle( hFighterTexture, 0 );
  fighterModel.SetTextureHandle( hFighterTextureBump, 1);
  fighterModel.AddTextureFilter( ENV_REPLACE, 0 );
  fighterModel.AddTextureFilter( MIN_MIP_LINEAR, 0 );
  fighterModel.AddTextureFilter( MAG_LINEAR, 0 );
  fighterModel.AddTextureFilter( ENV_MODULATE, 1 );
  fighterModel.AddTextureFilter( MIN_MIP_LINEAR, 1 );
  fighterModel.AddTextureFilter( MAG_LINEAR, 1 );
  CVertexDescriptor *pShaderTextureParam0 = new CVertexDescriptor(ELEMENT_TYPE_UNIFORM_1I,1);
  CVertexDescriptor *pShaderTextureParam1 = new CVertexDescriptor(ELEMENT_TYPE_UNIFORM_1I,1);
  CVertexDescriptor *pShaderInvRadius = new CVertexDescriptor(ELEMENT_TYPE_UNIFORM_1F,1);
  pShaderTextureParam0->GetPointer<int>()[0] = 0;
  pShaderTextureParam1->GetPointer<int>()[0] = 1;
  pShaderInvRadius->GetPointer<float>()[0] = 0.01f;
  
  VERTEX_HANDLE hTexParam0, hTexParam1, hInvRadius;
  assert(g_DefaultVertexManager->Create( pShaderTextureParam0, (tmpName="fighter_shader_tex0"), hTexParam0 ) == 0);  
  assert(g_DefaultVertexManager->Create( pShaderTextureParam1, (tmpName="fighter_shader_tex1"), hTexParam1 ) == 0);  
  assert(g_DefaultVertexManager->Create( pShaderTextureParam1, (tmpName="fighter_shader_invRadius"), hInvRadius ) == 0);  
  fighterModel.SetShaderParameter( "colorMap", hTexParam0 );
  fighterModel.SetShaderParameter( "normalMap", hTexParam1 );
  fighterModel.SetShaderParameter( "vTangent",  hFighterTangents );
  fighterModel.SetShaderParameter( "invRadius",  hInvRadius );
  fighterModel.SetShaderHandle( hShaderHandle );
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
    pOglRenderer->CommitShader( pBumpShader );
    pOglRenderer->CommitUniformShaderParam( *pBumpShader, "colorMap" , 0 );
    pOglRenderer->CommitUniformShaderParam( *pBumpShader, "normalMap" , 1 );
    pOglRenderer->CommitShaderParam( *pBumpShader, "vTangent", *pTangents);
    pOglRenderer->CommitUniformShaderParam( *pBumpShader, "invRadius", 0.11f);
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
    // Draw textured / transparent triangle
    //pOglRenderer->CommitTexture( 0, pTexture );
    glPushMatrix();
    glTranslatef(2.5f,0,0);
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
    pOglRenderer->DisableClientState( CLIENT_STATE_COLOR_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX0_ARRAY );
    pOglRenderer->DisableClientState( CLIENT_STATE_TEX1_ARRAY );
    pOglRenderer->DisableTexture( 0, pTexture );
    pOglRenderer->DisableTexture( 1, pTexture );
    pOglRenderer->CommitState( STATE_DEPTH_TEST );
    pOglRenderer->CommitState( STATE_ALPHA_TEST );

    glPushMatrix();
    glRotatef( fAngle*4.0f, 0, 1, 0 );
    glEnable(GL_CULL_FACE);
    pOglRenderer->CommitModel( fighterModel );
    glDisable(GL_CULL_FACE);
    glPopMatrix();
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
