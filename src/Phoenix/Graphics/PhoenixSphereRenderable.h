/*
 *  CSphereRenderable.h
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __PhoenixSphereRenderable_h__
#define __PhoenixSphereRenderable_h__
////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixRenderable.h"
#include "PhoenixSphere.h"
#include "PhoenixOGLRenderer.h"
////////////////////////////////////////////////////////////////
namespace Phoenix
{
    namespace Graphics
    {
        class CSphereRenderable : public Phoenix::Graphics::CRenderable
        {
        protected:
            Phoenix::Graphics::CVertexDescriptor m_Vertices;
            Phoenix::Graphics::CVertexDescriptor m_Color;
        public:
            CSphereRenderable();
            void Render( Phoenix::Graphics::COglRenderer & r );
            void UseSphere( const Phoenix::Volume::CSphere & sphere );
            void SetColor( float fR, float fG, float fB, float fA);
        };
    } // Graphics 
} // Phoenix
///////////////////////////////////////////////////////////////
#endif
