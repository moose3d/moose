/*
 *  PhoenixLineRenderable.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __PhoenixLineRenderable_h__
#define __PhoenixLineRenderable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixRenderable.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixLineSegment.h"
#include "PhoenixVector3.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
    namespace Graphics
    {
        class CLineRenderable : public Phoenix::Graphics::CRenderable
        {
        protected:
            Phoenix::Graphics::CVertexDescriptor m_Vertices;
            Phoenix::Graphics::CVertexDescriptor m_EndposThickness;
            Phoenix::Graphics::CVertexDescriptor m_Color;
            float m_fThickness;
        public:
            CLineRenderable();
            void Render( Phoenix::Graphics::COglRenderer & renderer );
            void SetLine( const Phoenix::Math::CVector3<float> & vStartPos,
                          const Phoenix::Math::CVector3<float> & vEndPos);
            void SetColor( float fR, float fG, float fB, float fA);
            void SetThickness ( float fValue);
            float GetThickness() const { return m_fThickness;}
        };
    }
}
#endif
/////////////////////////////////////////////////////////////////