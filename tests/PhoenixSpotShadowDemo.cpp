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
#define SPOT_CUTOFF 45.0f
float g_fSpotAngleFactor = 1.0f;
/////////////////////////////////////////////////////////////////
int main()
{
  CSDLScreen::m_SDLScreenParams.m_iWidth = 1024;
  CSDLScreen::m_SDLScreenParams.m_iHeight = 512;
  if ( CSDLScreen::GetInstance() == NULL )
  {
    cerr << "Can't open screen." << endl;
  }
  ////////////////////
  COglRenderer *pRenderer = new COglRenderer();

  CCamera lightCamera;  
  lightCamera.SetViewport(0,0,TEXWIDTH,TEXHEIGHT);
  lightCamera.SetNearClipping( 1 );
  lightCamera.SetFarClipping( 351 );
  //lightCamera.SetViewOrtho(-50, 150, -50,150);
  lightCamera.SetFieldOfView(SPOT_CUTOFF*2.0f);
  lightCamera.SetPosition(0,15,0);
  lightCamera.RotateAroundX( -90 );
  
  CLight directionalLight;
  directionalLight.SetPosition(0, 15, 0);
  //directionalLight.SetType( DIRECTIONAL );
  directionalLight.SetType( SPOTLIGHT );
  directionalLight.SetSpotAngle(SPOT_CUTOFF);
  directionalLight.SetDirection( CVector3<float>( 0,-1,0).GetNormalized());
  directionalLight.SetDiffuseColor( CVector4<unsigned char>(225,225,225,255));
  directionalLight.SetAmbientColor( CVector4<unsigned char>(225,225,225,255));
  directionalLight.SetSpecularColor( CVector4<unsigned char>(255,255,255,255));
  directionalLight.SetQuadraticAttenuation(0.001f);
  //lightCamera.SetRotation( RotationArc( directionalLight.GetDirection(), lightCamera.GetForwardVector() ));

  CCamera camera;  
  camera.SetPosition(0,10,10);
  camera.SetViewport( 0,0, 512, 512 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 1000.0f );
  camera.SetFieldOfView( 45.0f);
  camera.RotateAroundUp( 90.0 );




  CModel *pModel    = new CModel();
  Phoenix::Data::LoadMilkshapeModel( "Resources/Models/shadowscene.ms3d",    "shadowmodel", *pModel, OPT_VERTEX_NORMALS | OPT_VERTEX_INDICES );

  COglTexture *pTextureWall = pRenderer->CreateTexture( "./Resources/Textures/wall.tga");
  assert( pTextureWall  != NULL );

  CModel *pBackdrop = new CModel();
  Phoenix::Data::LoadMilkshapeModel( "Resources/Models/shadowbackdrop.ms3d", "shadowbackdrop", *pBackdrop, OPT_VERTEX_NORMALS | OPT_VERTEX_TEXCOORDS | OPT_VERTEX_INDICES );
  g_DefaultTextureManager->Create( pTextureWall, "texture_wall", pBackdrop->GetTextureHandle());
  pBackdrop->SetShaderParameter( "base", 0 );
  
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

  CShader *pShadowShader  = pRenderer->CreateShader("Resources/Shaders/shadow.point.vertex.glsl",
						    "Resources/Shaders/shadow.spot.frag.glsl");
  

  

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
	else if ( event.key.keysym.sym == SDLK_w )
	{
	  lightCamera.RotateAroundX( 2.0f);
	  CQuaternion q = RotationArc(directionalLight.GetDirection(), lightCamera.GetForwardVector());
	  RotateVector(q, directionalLight.GetDirection() );
	}
	else if ( event.key.keysym.sym == SDLK_s )
	{
	  lightCamera.RotateAroundX( -2.0f);
	  CQuaternion q = RotationArc(directionalLight.GetDirection(), lightCamera.GetForwardVector());
	  RotateVector(q, directionalLight.GetDirection() );
	}
	else if ( event.key.keysym.sym == SDLK_a )
	{
	  lightCamera.RotateAroundY( -2.0f);
	  CQuaternion q = RotationArc(directionalLight.GetDirection(), lightCamera.GetForwardVector());
	  RotateVector(q, directionalLight.GetDirection() );
	}
	else if ( event.key.keysym.sym == SDLK_d )
	{
	  lightCamera.RotateAroundY( 2.0f);
	  CQuaternion q = RotationArc(directionalLight.GetDirection(), lightCamera.GetForwardVector());
	  RotateVector(q, directionalLight.GetDirection() );
	}
	else if ( event.key.keysym.sym == SDLK_r )
	{
	  lightCamera.Move( CVector3<float>(0,2.0f,0.0) );
	  directionalLight.Move( CVector3<float>(0,2.0f,0) );
	}
	else if ( event.key.keysym.sym == SDLK_f )
	{
	  lightCamera.Move( CVector3<float>(0,-2.0f,0));
	  directionalLight.Move( CVector3<float>(0,-2.0f,0) );
	}
	else if ( event.key.keysym.sym == SDLK_g )
	{
	  g_fSpotAngleFactor += 0.2f;
	  lightCamera.SetFieldOfView(SPOT_CUTOFF*g_fSpotAngleFactor);
	  cerr << "factor now: " << g_fSpotAngleFactor << endl;
	}
	else if ( event.key.keysym.sym == SDLK_t )
	{
	  g_fSpotAngleFactor -= 0.2f;
	  lightCamera.SetFieldOfView(SPOT_CUTOFF*g_fSpotAngleFactor);
	  cerr << "factor now: " << g_fSpotAngleFactor << endl;
	}
	break;
      }
    }

    if ( camera.IsViewChanged())            camera.UpdateView();
    if ( camera.IsProjectionChanged())      camera.UpdateProjection();
    if ( lightCamera.IsViewChanged())       lightCamera.UpdateView();
    if ( lightCamera.IsProjectionChanged()) lightCamera.UpdateProjection();



    pRenderer->DisableState( STATE_LIGHTING );
    pRenderer->CommitState( STATE_DEPTH_TEST );
    pRenderer->CommitState( STATE_FACECULLING );
    // Eliminate artifacts caused by shadow mapping
    glPolygonOffset(18.0f, 1.0f);
    glEnable( GL_POLYGON_OFFSET_FILL );
    
    //pRenderer->CommitFrameBufferSingle( *pFBO, 0);
      glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
      pRenderer->CommitCamera( lightCamera );
      pRenderer->ClearBuffer(COLOR_BUFFER );
      pRenderer->ClearBuffer(DEPTH_BUFFER );

      pRenderer->CommitMaterial( material );
      pRenderer->CommitLight( directionalLight );
      pRenderer->CommitModel( *pModel  );

    pRenderer->CommitTexture( 0, pTexture );
    glDisable( GL_POLYGON_OFFSET_FILL );
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);
    //pRenderer->RollbackFrameBuffer( *pFBO );      

    //mResult = camera.GetInvView() * lightCamera.GetViewMatrix() * lightCamera.GetProjectionMatrix() * mBias;
    mResult = mBias * lightCamera.GetProjectionMatrix() * lightCamera.GetViewMatrix() *camera.GetInvView() ;
    mResult.Transpose();

    glMatrixMode(GL_TEXTURE);
           glLoadMatrixf( mResult.GetArray() );
    //       glLoadMatrixf(mBias.GetTransposition().GetArray());		// The bias matrix to convert to a 0 to 1 ratio
    //   glMultMatrixf(lightCamera.GetProjectionMatrix().GetTransposition().GetArray());	// The light's projection matrix
    //  glMultMatrixf(lightCamera.GetViewMatrix().GetTransposition().GetArray());	// The light's modelview matrix
    //  glMultMatrixf(camera.GetInvView().GetTransposition().GetArray());
    glMatrixMode(GL_MODELVIEW);    

    // Here is where we set the mode and function for shadow mapping with shadow2DProj().
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    pRenderer->ClearBuffer(COLOR_BUFFER );
    pRenderer->ClearBuffer(DEPTH_BUFFER );

    pRenderer->CommitCamera( camera );    
    pRenderer->CommitShader ( pShadowShader );
       
       pRenderer->CommitTexture( 7, pTexture );
       pRenderer->CommitFilter( MIN_LINEAR, pTexture->GetType());
       pRenderer->CommitFilter( MAG_LINEAR, pTexture->GetType());
       pRenderer->CommitLight( directionalLight );
       pRenderer->CommitUniformShaderParam( *pShadowShader, "shadowMap", 7 );
       pRenderer->CommitModel( *pModel );
       pRenderer->CommitModel( *pBackdrop );
       pRenderer->DisableTexture(0, pTextureWall );

    pRenderer->DisableTexture( 7, pTexture );
    pRenderer->CommitShader( NULL );

    lightCamera.SetViewport(0,0,TEXWIDTH,TEXHEIGHT);

    // Draw light view into right part of screen
    lightCamera.SetViewport(512,0,TEXWIDTH, TEXHEIGHT);
    pRenderer->CommitCamera( lightCamera );
    
         pRenderer->ClearBuffer( DEPTH_BUFFER );
         pRenderer->CommitState( STATE_LIGHTING );
         pRenderer->CommitMaterial( material );
         pRenderer->CommitLight( directionalLight );
         pRenderer->CommitModel( *pModel  );
       
    pRenderer->Finalize();
    lightCamera.SetViewport(0,0,TEXWIDTH, TEXHEIGHT);
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
