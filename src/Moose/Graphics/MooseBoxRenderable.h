/*
 *  CBoxRenderable.h
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __MooseBoxRenderable_h__
#define __MooseBoxRenderable_h__
////////////////////////////////////////////////////////////////////////
#include "MooseVertexDescriptor.h"
#include "MooseRenderable.h"
#include "MooseOBB.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        class CBoxRenderable : public Moose::Graphics::CRenderable
        {
        protected:
            Moose::Graphics::CVertexDescriptor m_Vertices;
            Moose::Graphics::CVertexDescriptor m_Color;
        public:
            CBoxRenderable();
            void Render( Moose::Graphics::COglRenderer & r );
            void UseBox( const Moose::Volume::COrientedBox & box );
        };
    } // Graphics
} // Moose
#endif
