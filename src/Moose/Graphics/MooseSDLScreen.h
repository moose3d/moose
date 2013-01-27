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
