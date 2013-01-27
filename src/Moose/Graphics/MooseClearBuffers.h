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

#ifndef __MooseClearBuffers_h__
#define __MooseClearBuffers_h__
///////////////////////////////////////////////////////////////////////////////
#include <Moose.h>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics 
  {
    ////////////////////
    /// For clearing depth and color buffers.
    class CClearBuffers : public Moose::Graphics::CRenderable
    {
    protected:
      Moose::Math::CVector4<float> m_Color;
      int                            m_iFlags;
    public:
      ////////////////////
      /// With default values black, both color and depth are cleared.
      CClearBuffers( ) : 
        m_Color(0.0f, 0.0f, 0.0f, 1.0f),
        m_iFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
      {
          GetRenderState().SetRenderLayer(0); 
      }
      ////////////////////
      /// Parametrized for special case.
      CClearBuffers( const Moose::Math::CVector4<float> & vColor, 
                     int iFlags ) : 
        m_Color(vColor), 
        m_iFlags(iFlags)
      {
        
      }
      ////////////////////
      ///
      void Render( Moose::Graphics::COglRenderer & renderer )
      {
        renderer.CommitState( Moose::Graphics::STATE_DEPTH_WRITE );
        glClearColor( m_Color[0], m_Color[1], m_Color[2], m_Color[3] );
        glClear( m_iFlags );
      }
      ////////////////////
      /// Sets color.
      /// \param color Vector4<float> color to be set.
      void SetColor( const Moose::Math::CVector4<float> & vColor )
      {
        m_Color = vColor;
      }
      ////////////////////
      /// \returns clear color.
      Moose::Math::CVector4<float> & GetColor()
      {
        return m_Color;
      }
    };
  } // Graphics 
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif /* CCLEARBUFFERS_H_ */
