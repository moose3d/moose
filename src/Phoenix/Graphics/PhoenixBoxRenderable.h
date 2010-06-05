/*
 *  CBoxRenderable.h
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __PhoenixBoxRenderable_h__
#define __PhoenixBoxRenderable_h__
////////////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixRenderable.h"
#include "PhoenixOBB.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
    namespace Graphics
    {
        class CBoxRenderable : public Phoenix::Graphics::CRenderable
        {
        protected:
            Phoenix::Graphics::CVertexDescriptor m_Vertices;
            Phoenix::Graphics::CVertexDescriptor m_Color;
        public:
            CBoxRenderable();
            void Render( Phoenix::Graphics::COglRenderer & r );
            void UseBox( const Phoenix::Volume::COrientedBox & box );
        };
    } // Graphics
} // Phoenix
#endif
