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


#ifndef MOOSEFOCUSABLE_H_
#define MOOSEFOCUSABLE_H_
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  class MOOSE_API CFocusable
  {
  private:
    /// Does this element have current focus.
    bool	m_bHasFocus;
  public:
    ////////////////////
    /// Constructor. Unfocuses object.
    CFocusable() : m_bHasFocus(0) {}
    virtual ~CFocusable() {}
    ////////////////////
    /// Checks is this element focused.
    /// \returns Non-zero on focus, zero otherwise.
    bool HasFocus() const { return m_bHasFocus;  }
    ////////////////////
    /// Sets focus flag.
    /// \param bFlag Non-zero for focus on, 0 for unfocus.
    void SetFocus( bool bFlag ) { m_bHasFocus = bFlag; }
  };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEFOCUSABLE_H_ */
