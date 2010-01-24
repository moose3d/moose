#ifndef PHOENIXQUAD_H_
#define PHOENIXQUAD_H_
/////////////////////////////////////////////////////////////
#include <PhoenixPositional.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
  /////////////////////////////////////////////////////////////////
      // The class for a Quad
      class PHOENIX_API CQuad : public Phoenix::Spatial::CPositional
      {
      protected:
        // The width of the quad
        float m_fWidth;
        // The height of the quad
        float m_fHeight;
        float m_fHalfWidth;
        float m_fHalfHeight;

      public:
        ////////////////////
        /// The constructor
        CQuad() : Phoenix::Spatial::CPositional(), m_fWidth(0.0f), m_fHeight(0.0f) {}
        ////////////////////
        /// Sets width
        /// \param fWidth Width to be assigned.
        void SetWidth( float fWidth )
        {
  	m_fWidth = fWidth;
  	m_fHalfWidth = m_fWidth * 0.5f;
        }
        ////////////////////
        /// Sets height
        /// \param fHeight Height to be assigned.
        void SetHeight( float fHeight )
        {
  	m_fHeight = fHeight;
  	m_fHalfHeight = m_fHeight * 0.5f;
        }
        float GetWidth() const
        {
  	return m_fWidth;
        }
        float GetHeight() const
        {
  	return m_fHeight;
        }
        float GetHalfWidth() const
        {
  	return m_fHalfWidth;
        }
        float GetHalfHeight() const
        {
  	return m_fHalfHeight;
        }
        void CreateFrom2DCoords(int iX1, int iY1, int iX2, int iY2 );
        friend std::ostream& Phoenix::Math::operator<<( std::ostream &stream, const Phoenix::Math::CQuad & quad );
      }; // class CQuad
      std::ostream& operator<<( std::ostream &stream, const Phoenix::Math::CQuad & quad );
  } // Math
} // Phoenix
/////////////////////////////////////////////////////////////
#endif /* PHOENIXQUAD_H_ */
