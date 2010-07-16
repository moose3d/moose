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
