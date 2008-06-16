#ifndef __CGuiSystem_h__
#define __CGuiSystem_h__
#include <CEGUI.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////
union SDL_Event;

/////////////////////////////////////////////////////////////////
typedef bool (* GuiCallbackFunc)( const CEGUI::EventArgs & args );
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace GUI
  {
    typedef SDL_Event GuiEvent;
    ////////////////////
    /// Adapter for CEGUI.
    class CGuiSystem
    {
    public:
      ////////////////////
      /// Init necessary systems.
      void Initialize( size_t nWidth, size_t nHeight );
      ////////////////////
      /// Setup data files from config file.
      void LoadResources( const char *szPath );
      ////////////////////
      ///  Injects mouse up event into GUI system.
      void InjectMouseUp( GuiEvent & e );
      ////////////////////
      ///  Injects mouse down event into GUI system.
      void InjectMouseDown( GuiEvent & e );
      ////////////////////
      ///  Injects mouse motion event into GUI system.
      void InjectMouseMotion( GuiEvent & e);
      ////////////////////
      ///  Injects key down event into GUI system.
      void InjectKeyDown( GuiEvent & e );
      ////////////////////
      ///  Injects key down event into GUI system.
      void InjectKeyUp( GuiEvent & e );
      ////////////////////
      /// Updates system.
      void Update( size_t nPassedTime );
      ////////////////////
      /// Selects which GUI layout is shown.
      void SelectGUI( const char *szRootName );
      ////////////////////
      /// Sets handler for element with static member function / normal function.
      /// \param szElementName Name of element in GUI config files.
      /// \param event CEGUI Event identifier (ie.PushButton::EventClicked)
      /// \param pCallback Pointer to function to be called on event.
      void SetEventHandler( const char *szElementName, const CEGUI::String &event, GuiCallbackFunc *pCallback );
      ////////////////////
      /// Sets handler for element with member function.
      /// \param szElementName Name of element in GUI config files.
      /// \param event CEGUI Event identifier (ie.PushButton::EventClicked)
      /// \param pCallback Pointer to member function.
      /// \param pObj Pointer to object, which member function will be called on event.
      template<class CLASS_TYPE>
      void SetEventHandler( const char *szElementName, 
			    const CEGUI::String & event,
			    bool (CLASS_TYPE::* pCallback)(const CEGUI::EventArgs &args), 
			    CLASS_TYPE *pObj);
    };  
  };
};
/////////////////////////////////////////////////////////////////
template<class CLASS_TYPE>
void 
Phoenix::GUI::CGuiSystem::SetEventHandler( const char *szElementName, 
					   const CEGUI::String & event,
					   bool (CLASS_TYPE::*pCallback)(const CEGUI::EventArgs &args), 
					   CLASS_TYPE *pObj)
{
  CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton ();
  winMgr.getWindow(szElementName)->subscribeEvent( event, CEGUI::Event::Subscriber(pCallback, pObj) ); 
}
/////////////////////////////////////////////////////////////////
#endif

