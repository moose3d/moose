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