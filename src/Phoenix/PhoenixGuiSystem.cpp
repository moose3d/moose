#include <PhoenixGuiSystem.h>
#include <SDL.h>
#include <CEGUI.h>
#include <CEGUIDefaultResourceProvider.h>
#include <RendererModules/OpenGLGUIRenderer/openglrenderer.h>
#include <libconfig.h++>
#include <iostream>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::string;
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
Phoenix::GUI::CGuiSystem::LoadResources( const char *szPath, const char *szName )
{
  
  libconfig::Config config;
  try 
  {
    config.readFile( szPath );
    if ( ! config.lookup("gui_config").isGroup())
      throw libconfig::SettingNotFoundException();

    CEGUI::DefaultResourceProvider *rp = 
    static_cast<CEGUI::DefaultResourceProvider*>  (CEGUI::System::getSingleton().getResourceProvider());
    string schemes = config.lookup("gui_config.paths.schemes");
    string imagesets = config.lookup("gui_config.paths.imagesets");
    string fonts = config.lookup("gui_config.paths.fonts");
    string layouts = config.lookup("gui_config.paths.layouts");
    string looknfeel = config.lookup("gui_config.paths.looknfeel");
    string scripts = config.lookup("gui_config.paths.lua_scripts");
    // "Resources/Config/CEGUI/..."
    rp->setResourceGroupDirectory("schemes",      schemes);
    rp->setResourceGroupDirectory("imagesets",    imagesets);
    rp->setResourceGroupDirectory("fonts",        fonts);
    rp->setResourceGroupDirectory("layouts",      layouts);
    rp->setResourceGroupDirectory("looknfeels",   looknfeel);
    rp->setResourceGroupDirectory("lua_scripts",  scripts); 
    
    // Set the default resource groups.
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts"); 

    string layout_prefix = "gui_config." + string( szName ? szName : "defaults.");
    
    // load in a font.  The first font loaded automatically becomes the default font.
    string font = config.lookup( layout_prefix + "font");

    CEGUI::FontManager::getSingleton().createFont( font );
    string scheme = config.lookup( layout_prefix + "scheme" );

    CEGUI::SchemeManager::getSingleton().loadScheme( scheme );
    
    string layout = config.lookup( layout_prefix + "layout");
    CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout( layout );

    CEGUI::System::getSingleton().setGUISheet( myRoot );


  }  
  catch( libconfig::ParseException & ex )
  {
    cerr << "parse exception" << ex.getError() << endl;
  }
  catch( libconfig::FileIOException & ex )
  {
    cerr << "Couldn't load file." << endl;
  }
  catch ( libconfig::SettingNotFoundException & ex )
  {
    cerr << "Setting not found!" << endl;
  }
  catch ( CEGUI::InvalidRequestException & ex )
  {
    cerr << ex.getMessage() << endl;
  }
  
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::GUI::CGuiSystem::InjectMouseUp( GuiEvent & e )
{
  switch ( e.button.button )
  {
  case SDL_BUTTON_LEFT:
    return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    break;
  case SDL_BUTTON_MIDDLE:
    return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
    break;
  case SDL_BUTTON_RIGHT:
    return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
    break;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::GUI::CGuiSystem::InjectMouseDown( GuiEvent & e )
{
  switch ( e.button.button )
  {
    // handle real mouse buttons
  case SDL_BUTTON_LEFT:
    return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    break;
  case SDL_BUTTON_MIDDLE:
    return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
    break;
  case SDL_BUTTON_RIGHT:
    return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
    break;
    // handle the mouse wheel
  case SDL_BUTTON_WHEELDOWN:
    return CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
    break;
  case SDL_BUTTON_WHEELUP:
    return CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
    break;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::GUI::CGuiSystem::InjectMouseMotion( GuiEvent & e )
{
  return CEGUI::System::getSingleton().injectMousePosition(  static_cast<float>(e.motion.x), static_cast<float>(e.motion.y) );
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::GUI::CGuiSystem::InjectKeyDown( GuiEvent & e )
{
  
  // to tell CEGUI that a key was pressed, we inject the scancode.
  bool bRetval = CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);
    
  // as for the character it's a litte more complicated. we'll use for translated unicode value.
  // this is described in more detail below.
  if (e.key.keysym.unicode != 0)
  {
    bRetval = bRetval || CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode);
  }
  
  return bRetval;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::GUI::CGuiSystem::InjectKeyUp( GuiEvent & e )
{
  return CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
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
void 
Phoenix::GUI::CGuiSystem::Render()
{
  CEGUI::System::getSingleton().renderGUI();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::GUI::CGuiSystem::SetWindowText( const char *szWinName, const char *szText )
{
  CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Window* pWindow = winMgr.getWindow(szWinName);
  if ( pWindow )
    pWindow->setText(szText);
}
/////////////////////////////////////////////////////////////////
  // WindowManager& winMgr = WindowManager::getSingleton ();
  /*winMgr.getWindow("testButton")->subscribeEvent( PushButton::EventClicked,
    &handleTestButton); */
