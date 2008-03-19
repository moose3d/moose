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
const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

float fOffset = 0.0f;

enum GUI_COMPONENT_TYPE 
{
  GUI_COMP_BUTTON = 0
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
  CButton() {
    SetType(GUI_COMP_BUTTON );
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
    if ( rMessage.GetType() == GUI_MSG_MOUSE_CLICK )
    {
      const CMouseClickEvent<CComponentBase> & event = static_cast<const CMouseClickEvent<CComponentBase> &>(rMessage);
      if ( rElement.GetType() == GUI_COMP_BUTTON )
      {
	CButton & btn = static_cast< CButton &>(rElement);
	if ( btn.MouseCoordinatesInside( event.GetCoords()) )
	{
	  cerr << "CLICK!" << endl;
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
    m_pFontset = pRenderer->CreateFontset("Resources/Fonts/trebuc.ttf", 12);
  }
  
  void Process( CComponentBase & rObject )
  {
    if ( rObject.GetType()  == GUI_COMP_BUTTON )
    {
      CButton &btn = static_cast<CButton &>(rObject);
      const CVector3<float> & vPos = rObject.GetWorldTransform().GetTranslation();
      m_pRenderer->CommitTransform( btn.GetWorldTransform());
      if ( btn.HasFocus())
	glColor3f(0,1,0);
      else 
	glColor3f(1,1,1);	
      glPolygonMode( GL_FRONT, GL_LINE);
      glBegin( GL_QUADS );
        glVertex2f(vPos[0], vPos[1]);
	glVertex2f(vPos[0]+rObject.GetWidth(), vPos[1]);
	glVertex2f(vPos[0]+rObject.GetWidth(), vPos[1]+rObject.GetHeight());
	glVertex2f(vPos[0], vPos[1]+rObject.GetHeight());
      glEnd();
      glPolygonMode( GL_FRONT, GL_FILL);
      m_pRenderer->CommitString( *m_pFontset, vPos[0], vPos[1], btn.GetText().c_str());
      m_pRenderer->RollbackTransform();
    }
  }
};
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

  CGuiSystem<CComponentBase> *pGuiSystem = new CGuiSystem<CComponentBase>();
  CButton *pButton = pGuiSystem->Create<CButton>( "main.button");
  pButton->SetWidth( 120 );
  pButton->SetHeight( 25 );
  pButton->SetText( std::string("Hello world!"));
  pButton->GetLocalTransform().SetTranslation( 10,10,0);
  pButton->GetLocalTransform().SetScaling(1.0f);
  pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_CLICK, *pButton );
  pGuiSystem->RegisterReceiver( GUI_MSG_MOUSE_MOTION, *pButton );

  //pGuiSystem->GetRoot().GetTransformNode()->AddEdge( pButton->GetTransformNode());
  pGuiSystem->SetRoot( pButton );
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
	pGuiSystem->EnqueueMouseClick( CVector2<int>(event.button.x,SCREEN_HEIGHT-event.button.y));
	break;
      case SDL_MOUSEBUTTONUP:
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
    
    
    pGuiSystem->Render<CGuiSystemOGLAdapter>(guiOglAdapter);
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
