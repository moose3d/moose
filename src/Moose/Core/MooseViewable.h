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


#ifndef MOOSEVIEWABLE_H_
#define MOOSEVIEWABLE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    class MOOSE_API CViewable
    {
    private:
      bool m_bVisible;
    public:
      CViewable() : m_bVisible(0){ }
      virtual ~CViewable() {}
      ////////////////////
      /// Returns is object visible or invisible.
      /// \returns true for visible, false for invisible.
      bool IsVisible() const {   return m_bVisible; }
      ////////////////////
      /// Sets object visible or invisible.
      /// \param bFlag Boolean flag - true for visible, false for invisible.
      void SetVisible( bool bFlag ) { m_bVisible = bFlag; }

    };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEVIEWABLE_H_ */
