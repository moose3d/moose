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

/*
 *  MooseLineRenderable.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __MooseLineRenderable_h__
#define __MooseLineRenderable_h__
/////////////////////////////////////////////////////////////////
#include "MooseVertexDescriptor.h"
#include "MooseRenderable.h"
#include "MooseOGLRenderer.h"
#include "MooseLineSegment.h"
#include "MooseVector3.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        class CLineRenderable : public Moose::Graphics::CRenderable
        {
        protected:
            Moose::Graphics::CVertexDescriptor m_Vertices;
            Moose::Graphics::CVertexDescriptor m_EndposThickness;
            Moose::Graphics::CVertexDescriptor m_Color;
            float m_fThickness;
        public:
            CLineRenderable();
            void Render( Moose::Graphics::COglRenderer & renderer );
            void SetLine( const Moose::Math::CVector3<float> & vStartPos,
                          const Moose::Math::CVector3<float> & vEndPos);
            void SetColor( float fR, float fG, float fB, float fA);
            void SetThickness ( float fValue);
            float GetThickness() const { return m_fThickness;}
        };
    }
}
#endif
/////////////////////////////////////////////////////////////////