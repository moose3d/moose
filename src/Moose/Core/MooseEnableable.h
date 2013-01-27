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


#ifndef MOOSEENABLEABLE_H_
#define MOOSEENABLEABLE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  	  class MOOSE_API CEnableable
        {
        private:
          /// Is object enabled.
          bool		m_bEnabled;
        public:
          ////////////////////
          /// Default constructor. Disables object.
          CEnableable() : m_bEnabled(0) { }

          virtual ~CEnableable() {}
          ////////////////////
          /// Returns is object enabled or disabled.
          /// \returns true for enabled, false for disabled.
          bool IsEnabled() const {   return m_bEnabled; }
          ////////////////////
          /// Sets object enabled or disabled.
          /// \param bFlag Boolean flag - true for enabling, false for disabling.
          void SetEnabled(bool bFlag ) { m_bEnabled = bFlag; }

          inline void operator=( bool bFlag ) { SetEnabled(bFlag); }
        };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEENABLEABLE_H_ */
