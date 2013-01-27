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


#ifndef MOOSESELECTABLE_H_
#define MOOSESELECTABLE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
   class MOOSE_API CSelectable
   {
   private:
     /// Is object selected.
     bool		m_bSelected;
   public:
     ////////////////////
     /// Default constructor. Unselects object.
     CSelectable() : m_bSelected(0){ }
     virtual ~CSelectable() {}
     ////////////////////
     /// Returns is object selected or unselected.
     /// \returns true for selected, false for unselected.
     bool IsSelected() const {   return m_bSelected; }
     ////////////////////
     /// Sets object selected or unselected.
     /// \param bFlag Boolean flag - true for selecting, false for unselecting.
     void SetSelected( bool bFlag ) { m_bSelected = bFlag; }
   };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSESELECTABLE_H_ */
