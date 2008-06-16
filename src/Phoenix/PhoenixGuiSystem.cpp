#include <PhoenixGuiSystem.h>
#include <SDL.h>
#include <CEGUI.h>
#include <CEGUIDefaultResourceProvider.h>
#include <RendererModules/OpenGLGUIRenderer/openglrenderer.h>
/////////////////////////////////////////////////////////////////
void
Phoenix::GUI::CGuiSystem::Initialize( size_t nWidth, size_t nHeight)
{
  //CSDLScreen::m_ScreenParams.
  // \TODO Fix screen width and height
  CEGUI::OpenGLRenderer* myRenderer = new CEGUI::OpenGLRenderer( 0, nWidth, nHeight ); 
  new CEGUI::System(myRenderer);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::LoadResources( const char *szPath )
{
  CEGUI::DefaultResourceProvider *rp = 
  static_cast<CEGUI::DefaultResourceProvider*>  (CEGUI::System::getSingleton().getResourceProvider());
  
  // "Resources/Config/CEGUI/..."
  rp->setResourceGroupDirectory("schemes",      "datafiles/schemes/");
  rp->setResourceGroupDirectory("imagesets",    "datafiles/imagesets/");
  rp->setResourceGroupDirectory("fonts",        "datafiles/fonts/");
  rp->setResourceGroupDirectory("layouts",      "datafiles/layouts/");
  rp->setResourceGroupDirectory("looknfeels",   "datafiles/looknfeel/");
  rp->setResourceGroupDirectory("lua_scripts",  "datafiles/lua_scripts/"); 

  // Set the default resource groups.
  CEGUI::Imageset::setDefaultResourceGroup("imagesets");
  CEGUI::Font::setDefaultResourceGroup("fonts");
  CEGUI::Scheme::setDefaultResourceGroup("schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
  CEGUI::WindowManager::setDefaultResourceGroup("layouts");
  CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts"); 

  // load in a font.  The first font loaded automatically becomes the default font.
  //if(! CEGUI::FontManager::getSingleton().isFontPresent( "Commonwealth-10" ) )
  CEGUI::FontManager::getSingleton().createFont( "Iconified-12.font" );

  // load in the scheme file, which auto-loads the TaharezLook imageset
  CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );


    
  using namespace CEGUI;
  Window* myRoot = WindowManager::getSingleton().loadWindowLayout( "test.layout" );
  //  Window* myRoot = WindowManager::getSingleton().loadWindowLayout( "" );
  System::getSingleton().setGUISheet( myRoot );

  // we will use of the WindowManager.
  WindowManager& winMgr = WindowManager::getSingleton ();
  /*winMgr.getWindow("testButton")->subscribeEvent( PushButton::EventClicked,
    &handleTestButton); */
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::InjectMouseUp( GuiEvent & e )
{
  switch ( e.button.button )
  {
  case SDL_BUTTON_LEFT:
    CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    break;
  case SDL_BUTTON_MIDDLE:
    CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
    break;
  case SDL_BUTTON_RIGHT:
    CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::InjectMouseDown( GuiEvent & e )
{
  switch ( e.button.button )
  {
    // handle real mouse buttons
  case SDL_BUTTON_LEFT:
    CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    break;
  case SDL_BUTTON_MIDDLE:
    CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
    break;
  case SDL_BUTTON_RIGHT:
    CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
    break;
    // handle the mouse wheel
  case SDL_BUTTON_WHEELDOWN:
    CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
    break;
  case SDL_BUTTON_WHEELUP:
    CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::InjectMouseMotion( GuiEvent & e )
{
  CEGUI::System::getSingleton().injectMousePosition(  static_cast<float>(e.motion.x), static_cast<float>(e.motion.y) );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::InjectKeyDown( GuiEvent & e )
{
  // to tell CEGUI that a key was pressed, we inject the scancode.
  CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);
    
  // as for the character it's a litte more complicated. we'll use for translated unicode value.
  // this is described in more detail below.
  if (e.key.keysym.unicode != 0)
  {
    CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode);
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::InjectKeyUp( GuiEvent & e )
{
  CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::Update( size_t nPassedTime )
{
  CEGUI::System::getSingleton().injectTimePulse( static_cast<float>(nPassedTime*0.001f) );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::GUI::CGuiSystem::SelectGUI( const char *szRootName )
{
  //System::getSingleton().setGUISheet( myRoot );
}
/////////////////////////////////////////////////////////////////
// WindowManager& winMgr = WindowManager::getSingleton ();
  /*winMgr.getWindow("testButton")->subscribeEvent( PushButton::EventClicked,
    &handleTestButton); */
