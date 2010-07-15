#ifndef __PhoenixClearBuffers_h__
#define __PhoenixClearBuffers_h__
///////////////////////////////////////////////////////////////////////////////
#include <Phoenix.h>
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics 
  {
    ////////////////////
    /// For clearing depth and color buffers.
    class CClearBuffers : public Phoenix::Graphics::CRenderable
    {
    protected:
      Phoenix::Math::CVector4<float> m_Color;
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
      CClearBuffers( const Phoenix::Math::CVector4<float> & vColor, 
                     int iFlags ) : 
        m_Color(vColor), 
        m_iFlags(iFlags)
      {
        
      }
      ////////////////////
      ///
      void Render( Phoenix::Graphics::COglRenderer & renderer )
      {
        renderer.CommitState( Phoenix::Graphics::STATE_DEPTH_WRITE );
        glClearColor( m_Color[0], m_Color[1], m_Color[2], m_Color[3] );
        glClear( m_iFlags );
      }
      ////////////////////
      /// Sets color.
      /// \param color Vector4<float> color to be set.
      void SetColor( const Phoenix::Math::CVector4<float> & vColor )
      {
        m_Color = vColor;
      }
      ////////////////////
      /// \returns clear color.
      Phoenix::Math::CVector4<float> & GetColor()
      {
        return m_Color;
      }
    };
  } // Graphics 
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* CCLEARBUFFERS_H_ */
