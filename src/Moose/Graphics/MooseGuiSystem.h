/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __CGuiSystem_h__
#define __CGuiSystem_h__
#include <CEGUI.h>
#include <cstdlib>
#include "MooseAPI.h"
#include <map>
#include <string>
/////////////////////////////////////////////////////////////////
union SDL_Event;

/////////////////////////////////////////////////////////////////
typedef bool (* GuiCallbackFunc)( const CEGUI::EventArgs & args );
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace GUI
  {
    typedef SDL_Event GuiEvent;
    ////////////////////
    /// Adapter for CEGUI.
    class MOOSE_API CGuiSystem
    {
    private:
      std::map<std::string, CEGUI::Window *> m_mapNameToRoot;
    public:
      ////////////////////
      /// Init necessary systems.
      void Initialize( size_t nWidth, size_t nHeight );
      ////////////////////
      /// Setup data paths from config file.
      /// \param szPath Path to config file.
      /// \param szName Name of layout to load.
      void LoadResources( const char *szPath );
      ////////////////////
      /// Load GUI layout from config file.
      /// \param szPath Path to config file.
      /// \param szName Name of GUI layout to load.
      void LoadGUI( const char *szPath, const char *szName );
      ////////////////////
      ///  Injects mouse up event into GUI system.
      bool InjectMouseUp( GuiEvent & e );
      ////////////////////
      ///  Injects mouse down event into GUI system.
      bool InjectMouseDown( GuiEvent & e );
      ////////////////////
      ///  Injects mouse motion event into GUI system.
      bool InjectMouseMotion( GuiEvent & e);
      ////////////////////
      ///  Injects key down event into GUI system.
      bool InjectKeyDown( GuiEvent & e );
      ////////////////////
      ///  Injects key down event into GUI system.
      bool InjectKeyUp( GuiEvent & e );
      ////////////////////
      /// Renders gui.
      void Render();
      ////////////////////
      /// Updates system.
      void Update( size_t nPassedTime );
      ////////////////////
      /// Selects which GUI layout is shown.
      void SelectGUI( const char *szRootName );
      ////////////////////
      /// Sets window text.
      void SetWindowText( const char *szWinName, const char *szText );
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
Moose::GUI::CGuiSystem::SetEventHandler( const char *szElementName, 
					   const CEGUI::String & event,
					   bool (CLASS_TYPE::*pCallback)(const CEGUI::EventArgs &args), 
					   CLASS_TYPE *pObj)
{
  CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton ();
  winMgr.getWindow(szElementName)->subscribeEvent( event, CEGUI::Event::Subscriber(pCallback, pObj) ); 
}
/////////////////////////////////////////////////////////////////
#endif

