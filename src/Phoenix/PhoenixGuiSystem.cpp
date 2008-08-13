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
Phoenix::GUI::CGuiSystem::LoadResources( const char *szPath )
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

    //     rp->setResourceGroupDirectory("schemes",      "datafiles/schemes/");
    //     rp->setResourceGroupDirectory("imagesets",    "datafiles/imagesets/");
    //     rp->setResourceGroupDirectory("fonts",        "datafiles/fonts/");
    //     rp->setResourceGroupDirectory("layouts",      "datafiles/layouts/");
    //     rp->setResourceGroupDirectory("looknfeels",   "datafiles/looknfeel/");
    //     rp->setResourceGroupDirectory("lua_scripts",  "datafiles/lua_scripts/"); 

    // Set the default resource groups.
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts"); 

    // load in a font.  The first font loaded automatically becomes the default font.
    //if(! CEGUI::FontManager::getSingleton().isFontPresent( "Commonwealth-10" ) )
    string font = config.lookup("gui_config.defaults.font");
    cerr << "font is: " <<  font << endl;
    //CEGUI::FontManager::getSingleton().createFont( "Iconified-12.font" );
    CEGUI::FontManager::getSingleton().createFont( font );
    string scheme = config.lookup("gui_config.defaults.scheme" );
    // load in the scheme file, which auto-loads the TaharezLook imageset
    //CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );
    CEGUI::SchemeManager::getSingleton().loadScheme( scheme );
    
    //CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout( "test.layout" );
    string layout = config.lookup("gui_config.defaults.layout");
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
