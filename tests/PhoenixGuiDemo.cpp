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
using namespace Phoenix::Gui;
using std::cerr;
using std::endl; 
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int g_bGlow = 0;
//const float g_fRotationSpeedPerSec = 56.15f;
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 800;

float fOffset = 0.0f;

enum GUI_COMPONENT_TYPE 
{
  GUI_COMP_BUTTON = 0,
  GUI_COMP_PANEL  = 1
};
class CComponentBase : public CGuiElement<CComponentBase>,
		       public CTypeBase<GUI_COMPONENT_TYPE>
{
  
};

class CButton : public CComponentBase
{
  friend class Phoenix::Gui::CGuiSystem<CComponentBase>;
  
protected:
  std::string m_strText;
  CButton() 
  {
    SetType(GUI_COMP_BUTTON );
    SetWidth( 28 );
    SetHeight( 10 );
  }
public:  
  void SetText( const std::string & text ){ m_strText = text; }
  const std::string &GetText() const { return m_strText; }
  
  TEXTURE_HANDLE m_hNormal;
};
/////////////////////////////////////////////////////////////////
class CPanel : public CComponentBase
{
  friend class Phoenix::Gui::CGuiSystem<CComponentBase>;
  
protected:
  std::string m_strText;
  CPanel() {
    SetType(GUI_COMP_PANEL );
  }
public:  
  void SetText( const std::string & text ){ m_strText = text; }
  const std::string &GetText() const { return m_strText; }
};
/////////////////////////////////////////////////////////////////
class CMessageAdapter 
{
public:
  void Process( const Phoenix::AI::CMessage<CComponentBase, GUI_MESSAGE_TYPES> &rMessage, CComponentBase &rElement )
  {
    if ( rMessage.GetType() == GUI_MSG_MOUSE_UP )
    {
      const CMouseClickEvent<CComponentBase> & event = static_cast<const CMouseClickEvent<CComponentBase> &>(rMessage);
      if ( rElement.GetType() == GUI_COMP_BUTTON )
      {
	CButton & btn = static_cast< CButton &>(rElement);
	if ( btn.MouseCoordinatesInside( event.GetCoords()) )
	{
	  btn.SetPressed(0);
	}
      }
      
    } 
    else if ( rMessage.GetType() == GUI_MSG_MOUSE_DOWN )
    {
      const CMouseClickEvent<CComponentBase> & event = static_cast<const CMouseClickEvent<CComponentBase> &>(rMessage);
      if ( rElement.GetType() == GUI_COMP_BUTTON )
      {
	CButton & btn = static_cast< CButton &>(rElement);
	if ( btn.MouseCoordinatesInside( event.GetCoords()) )
	{
	  btn.SetPressed(1);
	}
      }
      
    } 
    else if ( rMessage.GetType() == GUI_MSG_MOUSE_MOTION )
    {
      const CMouseMotionEvent<CComponentBase> & event = static_cast<const CMouseMotionEvent<CComponentBase> &>(rMessage);
      if ( rElement.GetType() == GUI_COMP_BUTTON )
      {
	CButton & btn = static_cast< CButton &>(rElement);
	btn.SetFocus ( btn.MouseCoordinatesInside( event.GetCoords()));
	if ( !btn.HasFocus() ) btn.SetPressed(0);
      }
    }

  }
};
/////////////////////////////////////////////////////////////////
class CGuiSystemOGLAdapter
{
private:

  COglRenderer *m_pRenderer;
  CFontset *m_pFontset;
public:

  CGuiSystemOGLAdapter() : m_pRenderer(NULL) {}

  void SetRenderer( COglRenderer *pRenderer )
  {
    m_pRenderer = pRenderer;
    m_pFontset = pRenderer->CreateFontset("Resources/Fonts/verdanab.ttf", 14);
  }
  
  void Process( CComponentBase & rObject )
  {
    if ( rObject.GetType()  == GUI_COMP_BUTTON )
    {
      CButton &btn = static_cast<CButton &>(rObject);

      m_pRenderer->CommitTransform( btn.GetWorldTransform());
      
      
      if ( btn.HasFocus())	glColor4f(1,1,1,1.0f);
      else			glColor4f(1,1,1,0.725f);	

      m_pRenderer->CommitTexture( 0, g_DefaultTextureManager->GetResource( btn.m_hNormal) );

      if ( btn.IsPressed() )  
	glTranslatef(2.0,-2.0,0.0);       

      glBegin( GL_QUADS );
      glTexCoord2f(0,0);      glVertex2f(0, 0);
      glTexCoord2f(1,0);      glVertex2f(rObject.GetWidth(), 0);
      glTexCoord2f(1,1);      glVertex2f(rObject.GetWidth(), rObject.GetHeight());
      glTexCoord2f(0,1);      glVertex2f(0, rObject.GetHeight());
      glEnd();
      
      // glColor4f(1,1,1,1);
//       m_pRenderer->CommitString( *m_pFontset, btn.GetWidth(), 5, btn.GetText().c_str());
      m_pRenderer->RollbackTransform();
      
    }
    else if ( rObject.GetType() == GUI_COMP_PANEL )
    {
      CPanel &pnl = static_cast<CPanel &>(rObject);
      const CVector3<float> & vPos = rObject.GetWorldTransform().GetTranslation();

      m_pRenderer->CommitTransform( pnl.GetWorldTransform());
      m_pRenderer->CommitState( STATE_BLENDING );
      m_pRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA );
      glColor4f(0.625,0.9825f,1,0.25f);	
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glBegin( GL_QUADS );

        glVertex2f(0,			0);
	glVertex2f(rObject.GetWidth(),  0);
	glVertex2f(rObject.GetWidth(),  rObject.GetHeight());
	glVertex2f(0,			rObject.GetHeight());

      glEnd();
      glPolygonMode( GL_FRONT, GL_FILL );
      glColor4f(1,1,1,1);
      m_pRenderer->CommitString( *m_pFontset, 10, pnl.GetHeight(), pnl.GetText().c_str());

      
      m_pRenderer->RollbackTransform();
    }
    
  }
};
/////////////////////////////////////////////////////////////////
int main()
{
  
  CSDLScreen::m_SDLScreenParams.m_iWidth = SCREEN_WIDTH;
  CSDLScreen::m_SDLScreenParams.m_iHeight= SCREEN_HEIGHT;
  CSDLScreen::m_SDLScreenParams.m_iVideoModeFlags |= SDL_FULLSCREEN;
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,43.0f);
  camera.SetViewport( 0,0, SCREEN_WIDTH, SCREEN_HEIGHT );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 500.0f );
  camera.SetFieldOfView( 43.0f);

  
  CCamera camera2D;
  camera2D.SetPosition( 0, 0.0f,0.0f);
  camera2D.SetViewport( 0,0, SCREEN_WIDTH,SCREEN_HEIGHT );
  camera2D.SetNearClipping( -2);
  camera2D.SetFarClipping( 20 );
  camera2D.SetViewOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

  
  
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  
  CTimer timer;
  timer.Reset();
  

  CFontset *pFontset = pOglRenderer->CreateFontset( "Resources/Fonts/trebuc.ttf", 54);
  assert( pFontset != NULL );
  
  CVector4<unsigned char> vWhite(255,255,255,255);
  CVector4<unsigned char> vPurple(255,55,255,255);
  CVector4<unsigned char> vYellow(255,255,0,255);
  CFpsCounter fps;
  fps.Reset();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef( 0,380,0);
  glScalef(0.75,0.75,0.75);
  
  CMatrix4x4<float> matrix;
  glGetFloatv( GL_MODELVIEW_MATRIX, matrix.GetArray());
  
  CGuiSystem<CComponentBase> *pGuiSystem = new CGuiSystem<CComponentBase>();
  CPanel *pPanel = pGuiSystem->Create<CPanel>( "main.panel" );
  pPanel->SetText("P-Fleet Ships");
  pPanel->SetWidth(230);
  pPanel->SetHeight(200);
  pPanel->GetLocalTransform().SetScaling(2.5f);
  pGuiSystem->SetRoot(pPanel);
  pPanel->GetLocalTransform().SetTranslation( 0, 380,0);
  
  
  
  
  
  CButton *pButton = pGuiSystem->Create<CButton>( "main.button");
  COglTexture *pTex = pOglRenderer->CreateTexture( "Resources/Textures/omega_icon.tga" );
  assert( g_DefaultTextureManager->Create( pTex, "omega_icon", pButton->m_hNormal) == 0);
  
  pButton->SetText( std::string("Backgammon"));
  pButton->GetLocalTransform().SetTranslation( 10,0,0);
  pButton->GetLocalTransform().SetScaling(1.0f);
  
  pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_DOWN, *pButton );
  pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_UP, *pButton );
  pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_MOTION, *pButton );

  pGuiSystem->GetRoot().GetTransformNode()->AddEdge( pButton->GetTransformNode());


  pTex = pOglRenderer->CreateTexture( "Resources/Textures/enterprise_icon.tga" );
  assert( g_DefaultTextureManager->Create( pTex, "enterprise_icon") == 0);
  size_t i=0;

  while( i<40 )
  {
    std::ostringstream name;
    name << "main.button" << i;

    pButton = pGuiSystem->Create<CButton>( name.str().c_str() );
    pButton->SetText( std::string("CPP Potkustart"));
    pButton->GetLocalTransform().SetTranslation( 10+35*(i%6),(int)(i/6)*20,0);
    
    assert( g_DefaultTextureManager->AttachHandle( "enterprise_icon", pButton->m_hNormal ) == 0);
    pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_DOWN, *pButton );
    pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_UP, *pButton );
    pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_MOTION, *pButton );
    pGuiSystem->GetRoot().GetTransformNode()->AddEdge( pButton->GetTransformNode());
    
    ++i;
  }


  pGuiSystem->EvaluateLayout();
  pGuiSystem->Prepare();
  
  CMessageAdapter msgAdapter;
  
  CGuiRenderAdapter<CComponentBase, CGuiSystemOGLAdapter> guiOglAdapter;
  guiOglAdapter.GetAdapter().SetRenderer( pOglRenderer );
  
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
	else if ( event.key.keysym.sym == SDLK_UP )
	{

	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{

	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{

	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{

	} 
	else if ( event.key.keysym.sym == SDLK_SPACE )
	{

	} 
	break;
      case SDL_MOUSEBUTTONDOWN:
	pGuiSystem->EnqueueMouseDown( CVector2<int>(event.button.x,SCREEN_HEIGHT-event.button.y));
	break;
      case SDL_MOUSEBUTTONUP:
	pGuiSystem->EnqueueMouseUp( CVector2<int>(event.button.x,SCREEN_HEIGHT-event.button.y));
	break;
      case SDL_MOUSEMOTION:
	pGuiSystem->EnqueueMouseMotion( CVector2<int>(event.button.x,SCREEN_HEIGHT-event.button.y));
	break;
      default:
	break;
      }
    }
    pGuiSystem->Update(msgAdapter);

   
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glClearColor(0,0,0,0);


    pOglRenderer->CommitCamera( camera2D );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_DEPTH_TEST);
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->DisableState( STATE_FACECULLING );
    pOglRenderer->CommitState( STATE_BLENDING );
    pOglRenderer->CommitBlending( BLEND_SRC_ONE, BLEND_DST_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GREATER, 0.0f);
    pOglRenderer->CommitColor( vWhite );
    pOglRenderer->CommitString( *pFontset, 300, 100+fOffset, "Hello world!");
    pOglRenderer->CommitColor( vPurple );
    pOglRenderer->CommitString( *pFontset, 300, 50+fOffset, "This is a font demo...");
    pOglRenderer->CommitColor( vYellow );
    pOglRenderer->CommitString( *pFontset, 300, 0+fOffset, "... with Phoenix!!!");
    
    pOglRenderer->CommitState( STATE_BLENDING );
    pOglRenderer->CommitBlending( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA );    

    //pOglRenderer->CommitCamera( camera2D );
    pGuiSystem->Render<CGuiSystemOGLAdapter>(guiOglAdapter);
    pOglRenderer->DisableState( STATE_BLENDING );
    

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
  delete pGuiSystem;

  CSDLScreen::DestroyInstance();
  return 0;
}
