#include <MooseGuiSystem.h>
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
/// Translate a SDLKey to the proper CEGUI::Key
/////////////////////////////////////////////////////////////////
CEGUI::uint SDLKeyToCEGUIKey(SDLKey key)
{
  using namespace CEGUI;
  switch (key)
  {
  case SDLK_BACKSPACE:    return Key::Backspace;
  case SDLK_TAB:          return Key::Tab;
  case SDLK_RETURN:       return Key::Return;
  case SDLK_PAUSE:        return Key::Pause;
  case SDLK_ESCAPE:       return Key::Escape;
  case SDLK_SPACE:        return Key::Space;
  case SDLK_COMMA:        return Key::Comma;
  case SDLK_MINUS:        return Key::Minus;
  case SDLK_PERIOD:       return Key::Period;
  case SDLK_SLASH:        return Key::Slash;
  case SDLK_0:            return Key::Zero;
  case SDLK_1:            return Key::One;
  case SDLK_2:            return Key::Two;
  case SDLK_3:            return Key::Three;
  case SDLK_4:            return Key::Four;
  case SDLK_5:            return Key::Five;
  case SDLK_6:            return Key::Six;
  case SDLK_7:            return Key::Seven;
  case SDLK_8:            return Key::Eight;
  case SDLK_9:            return Key::Nine;
  case SDLK_COLON:        return Key::Colon;
  case SDLK_SEMICOLON:    return Key::Semicolon;
  case SDLK_EQUALS:       return Key::Equals;
  case SDLK_LEFTBRACKET:  return Key::LeftBracket;
  case SDLK_BACKSLASH:    return Key::Backslash;
  case SDLK_RIGHTBRACKET: return Key::RightBracket;
  case SDLK_a:            return Key::A;
  case SDLK_b:            return Key::B;
  case SDLK_c:            return Key::C;
  case SDLK_d:            return Key::D;
  case SDLK_e:            return Key::E;
  case SDLK_f:            return Key::F;
  case SDLK_g:            return Key::G;
  case SDLK_h:            return Key::H;
  case SDLK_i:            return Key::I;
  case SDLK_j:            return Key::J;
  case SDLK_k:            return Key::K;
  case SDLK_l:            return Key::L;
  case SDLK_m:            return Key::M;
  case SDLK_n:            return Key::N;
  case SDLK_o:            return Key::O;
  case SDLK_p:            return Key::P;
  case SDLK_q:            return Key::Q;
  case SDLK_r:            return Key::R;
  case SDLK_s:            return Key::S;
  case SDLK_t:            return Key::T;
  case SDLK_u:            return Key::U;
  case SDLK_v:            return Key::V;
  case SDLK_w:            return Key::W;
  case SDLK_x:            return Key::X;
  case SDLK_y:            return Key::Y;
  case SDLK_z:            return Key::Z;
  case SDLK_DELETE:       return Key::Delete;
  case SDLK_KP0:          return Key::Numpad0;
  case SDLK_KP1:          return Key::Numpad1;
  case SDLK_KP2:          return Key::Numpad2;
  case SDLK_KP3:          return Key::Numpad3;
  case SDLK_KP4:          return Key::Numpad4;
  case SDLK_KP5:          return Key::Numpad5;
  case SDLK_KP6:          return Key::Numpad6;
  case SDLK_KP7:          return Key::Numpad7;
  case SDLK_KP8:          return Key::Numpad8;
  case SDLK_KP9:          return Key::Numpad9;
  case SDLK_KP_PERIOD:    return Key::Decimal;
  case SDLK_KP_DIVIDE:    return Key::Divide;
  case SDLK_KP_MULTIPLY:  return Key::Multiply;
  case SDLK_KP_MINUS:     return Key::Subtract;
  case SDLK_KP_PLUS:      return Key::Add;
  case SDLK_KP_ENTER:     return Key::NumpadEnter;
  case SDLK_KP_EQUALS:    return Key::NumpadEquals;
  case SDLK_UP:           return Key::ArrowUp;
  case SDLK_DOWN:         return Key::ArrowDown;
  case SDLK_RIGHT:        return Key::ArrowRight;
  case SDLK_LEFT:         return Key::ArrowLeft;
  case SDLK_INSERT:       return Key::Insert;
  case SDLK_HOME:         return Key::Home;
  case SDLK_END:          return Key::End;
  case SDLK_PAGEUP:       return Key::PageUp;
  case SDLK_PAGEDOWN:     return Key::PageDown;
  case SDLK_F1:           return Key::F1;
  case SDLK_F2:           return Key::F2;
  case SDLK_F3:           return Key::F3;
  case SDLK_F4:           return Key::F4;
  case SDLK_F5:           return Key::F5;
  case SDLK_F6:           return Key::F6;
  case SDLK_F7:           return Key::F7;
  case SDLK_F8:           return Key::F8;
  case SDLK_F9:           return Key::F9;
  case SDLK_F10:          return Key::F10;
  case SDLK_F11:          return Key::F11;
  case SDLK_F12:          return Key::F12;
  case SDLK_F13:          return Key::F13;
  case SDLK_F14:          return Key::F14;
  case SDLK_F15:          return Key::F15;
  case SDLK_NUMLOCK:      return Key::NumLock;
  case SDLK_SCROLLOCK:    return Key::ScrollLock;
  case SDLK_RSHIFT:       return Key::RightShift;
  case SDLK_LSHIFT:       return Key::LeftShift;
  case SDLK_RCTRL:        return Key::RightControl;
  case SDLK_LCTRL:        return Key::LeftControl;
  case SDLK_RALT:         return Key::RightAlt;
  case SDLK_LALT:         return Key::LeftAlt;
  case SDLK_LSUPER:       return Key::LeftWindows;
  case SDLK_RSUPER:       return Key::RightWindows;
  case SDLK_SYSREQ:       return Key::SysRq;
  case SDLK_MENU:         return Key::AppMenu;
  case SDLK_POWER:        return Key::Power;
  default:                return 0;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
void
Moose::GUI::CGuiSystem::Initialize( size_t nWidth, size_t nHeight)
{
  //CSDLScreen::m_ScreenParams.
  // \TODO Fix screen width and height

  // Ok, this is pretty crappy workaround, but it will have to do.
  if ( CEGUI::System::getSingletonPtr() == NULL )
  {
    CEGUI::OpenGLRenderer* myRenderer = new CEGUI::OpenGLRenderer( 0, nWidth, nHeight ); 
    new CEGUI::System(myRenderer);
    /// enables unicode representation
    SDL_EnableUNICODE(1);
  }

}
/////////////////////////////////////////////////////////////////
void 
Moose::GUI::CGuiSystem::LoadResources( const char *szPath )
{
  libconfig::Config config;
  try 
  {
    config.readFile( szPath );
    if ( ! config.lookup("gui_config").isGroup())
      throw libconfig::ConfigException();

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
    

  }  
  catch( libconfig::ParseException & ex )
  {
    cerr << "parse exception" << ex.getError() << endl;
  }
  catch( libconfig::FileIOException & ex )
  {
    cerr << "Couldn't load file." << endl;
  }
  catch ( libconfig::ConfigException & ex )
  {
    cerr << "Setting not found!" << endl;
  }
  catch ( CEGUI::InvalidRequestException & ex )
  {
    cerr << ex.getMessage() << endl;
  }
  
}
/////////////////////////////////////////////////////////////////
void 
Moose::GUI::CGuiSystem::LoadGUI( const char *szPath, const char *szName )
{
  libconfig::Config config;
  try 
  {
    config.readFile( szPath );  
    string layout_prefix = "gui_config." + string( szName ? szName : "defaults");
    layout_prefix += ".";
    // load in a font.  The first font loaded automatically becomes the default font.
    string font = config.lookup( layout_prefix + "font");

    try 
    {
      CEGUI::FontManager::getSingleton().createFont( font );
    } 
    catch ( CEGUI::AlreadyExistsException & ex )
    {
      // NOP, we got the proper font
    }
    
    string scheme = config.lookup( layout_prefix + "scheme" );
    try 
    {
      CEGUI::SchemeManager::getSingleton().loadScheme( scheme );
    }
    catch ( CEGUI::AlreadyExistsException & ex )
    {
      // NOP, we got the proper scheme
    }
    string layout = config.lookup( layout_prefix + "layout");
    CEGUI::Window* newRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout( layout );
    
    // assign new name to map and set it visible
    m_mapNameToRoot[string(szName)] = newRoot;
    CEGUI::System::getSingleton().setGUISheet( newRoot );
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
Moose::GUI::CGuiSystem::InjectMouseUp( GuiEvent & e )
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
Moose::GUI::CGuiSystem::InjectMouseDown( GuiEvent & e )
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
Moose::GUI::CGuiSystem::InjectMouseMotion( GuiEvent & e )
{
  return CEGUI::System::getSingleton().injectMousePosition(  static_cast<float>(e.motion.x), static_cast<float>(e.motion.y) );
}
/////////////////////////////////////////////////////////////////
bool
Moose::GUI::CGuiSystem::InjectKeyDown( GuiEvent & e )
{
  
  // to tell CEGUI that a key was pressed, we inject the scancode.
  bool bRetval = CEGUI::System::getSingleton().injectKeyDown( SDLKeyToCEGUIKey(e.key.keysym.sym) );

  // as for the character it's a litte more complicated. we'll use for translated unicode value.
  // this is described in more detail below.
  bRetval = CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode) || bRetval;
  
  
  return bRetval;
}
/////////////////////////////////////////////////////////////////
bool
Moose::GUI::CGuiSystem::InjectKeyUp( GuiEvent & e )
{
  return CEGUI::System::getSingleton().injectKeyUp( SDLKeyToCEGUIKey(e.key.keysym.sym));
}
/////////////////////////////////////////////////////////////////
void 
Moose::GUI::CGuiSystem::Update( size_t nPassedTime )
{
  CEGUI::System::getSingleton().injectTimePulse( static_cast<float>(nPassedTime*0.001f) );
}
/////////////////////////////////////////////////////////////////
void
Moose::GUI::CGuiSystem::SelectGUI( const char *szRootName )
{
  std::map<std::string, CEGUI::Window *>::iterator  it;
  it = m_mapNameToRoot.find(string(szRootName));

  if ( it != m_mapNameToRoot.end())
  {
    CEGUI::System::getSingleton().setGUISheet( it->second );
  }
}
/////////////////////////////////////////////////////////////////
void 
Moose::GUI::CGuiSystem::Render()
{
  CEGUI::System::getSingleton().renderGUI();
}
/////////////////////////////////////////////////////////////////
void 
Moose::GUI::CGuiSystem::SetWindowText( const char *szWinName, const char *szText )
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
