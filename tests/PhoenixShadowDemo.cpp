#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::Data;
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
#define TEXWIDTH  512
#define TEXHEIGHT 512
/////////////////////////////////////////////////////////////////
int main()
{
  if ( CSDLScreen::GetInstance() == NULL )
  {
    cerr << "Can't open screen." << endl;
  }
  ////////////////////
  COglRenderer *pRenderer = new COglRenderer();

  CCamera lightCamera;  
  lightCamera.SetViewport(0,0,TEXWIDTH,TEXHEIGHT);
  lightCamera.SetNearClipping( -12 );
  lightCamera.SetFarClipping( 5 );
  lightCamera.SetViewOrtho(-50, 50, -50,50);
  lightCamera.SetPosition(0,0,0);
  lightCamera.RotateAroundRight( -90 );
  
  CLight directionalLight;
  directionalLight.SetPosition( 0, 0, 0);
  directionalLight.SetType( DIRECTIONAL );
  directionalLight.SetDirection( CVector3<float>( 0,-1,0).GetNormalized());
  directionalLight.SetDiffuseColor( CVector4<unsigned char>(225,225,225,255));
  directionalLight.SetAmbientColor( CVector4<unsigned char>(125,15,105,255));
  directionalLight.SetSpecularColor( CVector4<unsigned char>(255,255,255,255));

  //lightCamera.SetRotation( RotationArc( directionalLight.GetDirection(), lightCamera.GetForwardVector() ));

  CCamera camera;  
  camera.SetPosition(0,10,10);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 1000.0f );
  camera.SetFieldOfView( 45.0f);
  camera.RotateAroundUp( 90.0 );
  CMilkshapeLoader *pLoader = new CMilkshapeLoader() ;
  pLoader->Load( "Resources/Models/shadowscene.ms3d");
  pLoader->GenerateModelData();
  
  CModel model;
  g_DefaultVertexManager->Create(pLoader->GetVertices(), "shadowmodel_vertices", model.GetVertexHandle());
  g_DefaultVertexManager->Create(pLoader->GetNormals(), "shadowmodel_normals", model.GetNormalHandle());
  
  model.AddIndexHandle(INDEX_HANDLE());
  g_DefaultIndexManager->Create( pLoader->GetIndices().front(), "shadowmodel_indices", model.GetIndexHandles().front());
  
  pLoader->ResetVertices();
  pLoader->ResetNormals();
  pLoader->ResetIndices();
  
  delete pLoader;
  SDL_Event event;  

  CMaterial material;
  material.SetDiffuse( CVector4<float>(0.86,0.86,0.86,1.0f));
  material.SetAmbient( CVector4<float>(0.26,0.26,0.26,1.0f));
  material.SetSpecular( CVector4<float>(1,1,1,1));
  material.SetShininess( 0.28000000f);
  
  // create empty texture for depth data
  COglTexture *pTexture  = pRenderer->CreateTexture( TEXWIDTH, TEXHEIGHT, TEXTURE_2D, TEX_FORMAT_DEPTH_COMPONENT);
  // create a handle pointing to it
  TEXTURE_HANDLE hTexture;
  g_DefaultTextureManager->Create( pTexture, "depth_texture", hTexture);
  // create framebuffer object
  CFrameBufferObject *pFBO = pRenderer->CreateFramebuffer( TEXWIDTH, TEXHEIGHT, FBO_COLOR_BUFFER | FBO_DEPTH_BUFFER );
  // attach previously creatd texture to it
  pRenderer->AttachTextureToFramebuffer( *pFBO, hTexture, 0 );

  const CMatrix4x4<float> mBias( 0.5f, 0.0f, 0.0f, 0.5f, 
				 0.0f, 0.5f, 0.0f, 0.5f,
				 0.0f, 0.0f, 0.5f, 0.5f,
				 0.0f, 0.0f, 0.0f, 1.0f );
  
  CMatrix4x4<float> mResult;

  CShader *pShadowShader  = pRenderer->CreateShader("Resources/Shaders/shadow.vert.glsl",
						    "Resources/Shaders/shadow.frag.glsl");
  
  COglTexture *pTextureTest = pRenderer->CreateTexture( "./Resources/Textures/wall.tga");
  assert( pTextureTest  != NULL );
  

  SDL_EnableKeyRepeat(   SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  while ( g_bLoop )
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
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  camera.Move( -camera.GetForwardVector()*10.0f);
	}
	else if ( event.key.keysym.sym == SDLK_UP )
	{
	  camera.Move( camera.GetForwardVector()*10.0f);
	}
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 3.0f);
	}
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -3.0f);
	}

	break;
      }
    }
    if ( camera.IsViewChanged())            camera.UpdateView();
    if ( camera.IsProjectionChanged())      camera.UpdateProjection();
    if ( lightCamera.IsViewChanged())       lightCamera.UpdateView();
    if ( lightCamera.IsProjectionChanged()) lightCamera.UpdateProjection();



    pRenderer->CommitState( STATE_LIGHTING );
    pRenderer->CommitState( STATE_DEPTH_TEST );
    // Eliminate artifacts caused by shadow mapping
    glPolygonOffset(7.0f, 18.0f);
    glEnable( GL_POLYGON_OFFSET_FILL );
    
    //pRenderer->CommitFrameBufferSingle( *pFBO, 0);
      pRenderer->CommitCamera( lightCamera );
      pRenderer->ClearBuffer(COLOR_BUFFER );
      pRenderer->ClearBuffer(DEPTH_BUFFER );

      pRenderer->CommitMaterial( material );
      pRenderer->CommitLight( directionalLight );
      pRenderer->CommitModel( model  );


      pRenderer->CommitTexture( 0, pTexture );
      glDisable( GL_POLYGON_OFFSET_FILL );
      //pRenderer->RollbackFrameBuffer( *pFBO );
      glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);
      
      //mResult = camera.GetInvView() * lightCamera.GetViewMatrix() * lightCamera.GetProjectionMatrix() * mBias;
      //mResult = mBias * lightCamera.GetProjectionMatrix() * lightCamera.GetViewMatrix() *camera.GetInvView() ;
      //mResult.Transpose();

    glMatrixMode(GL_TEXTURE);
      glLoadMatrixf( mResult.GetArray() );
       glLoadMatrixf(mBias.GetTransposition().GetArray());		// The bias matrix to convert to a 0 to 1 ratio
       glMultMatrixf(lightCamera.GetProjectionMatrix().GetTransposition().GetArray());	// The light's projection matrix
       glMultMatrixf(lightCamera.GetViewMatrix().GetTransposition().GetArray());	// The light's modelview matrix
       glMultMatrixf(camera.GetInvView().GetTransposition().GetArray());
    glMatrixMode(GL_MODELVIEW);    

    // Here is where we set the mode and function for shadow mapping with shadow2DProj().
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    pRenderer->ClearBuffer(COLOR_BUFFER );
    pRenderer->ClearBuffer(DEPTH_BUFFER );

    pRenderer->CommitCamera( camera );    
    pRenderer->CommitShader ( pShadowShader );

       pRenderer->CommitTexture( 0, pTexture );
       pRenderer->CommitFilter( MIN_LINEAR, pTexture->GetType());
       pRenderer->CommitFilter( MAG_LINEAR, pTexture->GetType());
       pRenderer->CommitLight( directionalLight );
       pRenderer->CommitUniformShaderParam( *pShadowShader, "shadowMap", 0 );
       pRenderer->CommitModel( model );

    pRenderer->DisableTexture( 0, pTexture );
    pRenderer->CommitShader( NULL );

    pRenderer->Finalize();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);    

    CSDLScreen::GetInstance()->SwapBuffers();
  }
  ////////////////////
  // Cleanup
  delete pRenderer;
  return 0;
}
/////////////////////////////////////////////////////////////////
