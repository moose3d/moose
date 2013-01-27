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

#include "MooseEnableable.h"

namespace Moose
{
    namespace Graphics
    {
        ///////////////////////
        /// Polygon offset for solving z-fighting effects on overlaid objects.
        class CPolygonOffset : public Moose::Core::CEnableable
        {
        protected:
            float m_fFactor; ///!< Variable depth offset factor 
            float m_fUnits;  ///!< Constant depth offset factor
        public:
            CPolygonOffset() : m_fFactor(0.0), m_fUnits(0.0) { }
            virtual ~CPolygonOffset() {}
            void Set( float fFactor, float fUnits ) { m_fFactor = fFactor; m_fUnits = fUnits; }
            void SetFactor( float fValue ) { m_fFactor = fValue; }
            void SetUnits( float fValue ) { m_fUnits = fValue; }
            float & GetFactor() { return m_fFactor; }
            float & GetUnits()  { return m_fUnits; }
        };
    }
}

