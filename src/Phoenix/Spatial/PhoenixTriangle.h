#ifndef PHOENIXTRIANGLE_H_
#define PHOENIXTRIANGLE_H_
#include <PhoenixAPI.h>
#include <PhoenixVector3.h>
#include <PhoenixVertex.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Spatial
  {
	  /////////////////////////////////////////////////////////////////
      /// Triangle class for editors.
      class PHOENIX_API CTriangle
      {
      protected:
        /// Vertices in a triangle
        CVertex			m_vVertices[3];
        /// Is this triangle selected.
        int			m_bSelected;
      public:
        ////////////////////
        /// Constructor.
        CTriangle() : m_bSelected(0)
        {

        }
        ////////////////////
        /// Is this triangle selected.
        /// \returns Selection status.
        int IsSelected() const
        {
  	return m_bSelected;
        }
        ////////////////////
        /// Sets selection status.
        /// \param bFlag Boolean selection status, true for selection, false for unselection.
        void SetSelected( int bFlag )
        {
  	m_bSelected = bFlag;
        }
        ////////////////////
        /// Returns vertex.
        /// \param nCorner Which corner vertex of triangle (0,1 or 2)
        /// \returns Reference to vertex.
        CVertex & GetVertex( unsigned int nCorner )
        {
  	return m_vVertices[ nCorner % 3];
        }
        ////////////////////
        /// Returns vertex.
        /// \param nCorner Which corner vertex of triangle (0,1 or 2)
        /// \returns Reference to vertex.
        const CVertex & GetVertex( unsigned int nCorner ) const
        {
  	return m_vVertices[ nCorner % 3];
        }
        ////////////////////
        /// Calculates triangle face normal.
        void CalculateFaceNormal( Phoenix::Math::CVector3<float> & vResult )
        {
  	vResult = (m_vVertices[2].GetPosition() - m_vVertices[0].GetPosition()).Cross(m_vVertices[1].GetPosition() - m_vVertices[0].GetPosition());
  	vResult.Normalize();
        }

      };
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXTRIANGLE_H_ */
