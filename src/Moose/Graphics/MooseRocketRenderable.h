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

#ifndef __MooseRocketRenderable_h__
#define __MooseRocketRenderable_h__
#include <MooseRenderable.h>
////////////////////////////////////////////////////////////////////////////////
namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        class COglRenderer;
        ////////////////////
        /// Renders GUI components using librocket. 
        class MOOSE_API CRocketRenderable : public Moose::Graphics::CRenderable
        {
        protected:
            /// Rocket context. Not freed when renderable is destroyed.
            Rocket::Core::Context * m_pContext; 
        public:
            ////////////////////
            /// Initializes context to this renderable.
            CRocketRenderable( Rocket::Core::Context *pContext );
            ////////////////////
            /// Called by Moose when object is rendered.
            void Render( Moose::Graphics::COglRenderer & r );

        }; // CRocketRenderable
    } // Graphics 
} // Moose
////////////////////////////////////////////////////////////////////////////////
#endif
