/////////////////////////////////////////////////////////////////
#ifndef __MooseSDLScreen_h__
#define __MooseSDLScreen_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
#include "MooseScreen.h"
#include <string>
/////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
namespace Moose
{
  namespace Window
  {
    
    class MOOSE_API CSDLScreenImpl
    {
    private:
#ifdef MOOSE_USE_OPENGL3
      SDL_Window *  m_pMainWindow; ///< Our window.
      SDL_GLContext m_pGLContext;  ///< GL context for 3.0+
#endif
      ////////////////////
      /// The constructor
      CSDLScreenImpl();
      ////////////////////
      /// The deconstructor
      ~CSDLScreenImpl();
    };
    /////////////////////////////////////////////////////////////////
  }; // namespace Window
}; // namespace Moose
#endif
#endif
/////////////////////////////////////////////////////////////////
