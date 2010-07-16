/*
 *  CSphereRenderable.h
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MooseSphereRenderable_h__
#define __MooseSphereRenderable_h__
////////////////////////////////////////////////////////////////
#include "MooseVertexDescriptor.h"
#include "MooseRenderable.h"
#include "MooseSphere.h"
#include "MooseOGLRenderer.h"
////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        class CSphereRenderable : public Moose::Graphics::CRenderable
        {
        protected:
            Moose::Graphics::CVertexDescriptor m_Vertices;
            Moose::Graphics::CVertexDescriptor m_Color;
        public:
            CSphereRenderable();
            void Render( Moose::Graphics::COglRenderer & r );
            void UseSphere( const Moose::Volume::CSphere & sphere );
            void SetColor( float fR, float fG, float fB, float fA);
        };
    } // Graphics 
} // Moose
///////////////////////////////////////////////////////////////
#endif
