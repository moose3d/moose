#ifndef __MOOSEPOSITIONAL_H__
#define __MOOSEPOSITIONAL_H__
/////////////////////////////////////////////////////////////

#include <MooseVector3.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
      /// The base class for Objects with position.
      class MOOSE_API CPositional
      {
      protected:
        /// Vector where position is stored.
        Moose::Math::CVector3<float> m_vPosition;
        /// has the position changed.
        int	      m_bPositionChanged;
      public:
        ////////////////////
        /// Default constructor.
        CPositional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
        {

        }
        ////////////////////
        /// Parametrized constructor.
        /// \param vPosition vector for location-
        CPositional( const Moose::Math::CVector3<float> & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
        {
        }
        ////////////////////
        /// Destructor.
        virtual ~CPositional() {}
        ////////////////////
        /// Assigns position.
        /// \param vPosition vector of which values are used.
        inline void SetPosition( const Moose::Math::CVector3<float> &vPosition )
        {
  	m_vPosition = vPosition;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Assigns the position.
        /// \param fX position on X-axis.
        /// \param fY position on Y-axis.
        /// \param fZ position on Z-axis.
        inline void SetPosition( float fX, float fY, float fZ )
        {
  	m_vPosition[0] = fX;
  	m_vPosition[1] = fY;
  	m_vPosition[2] = fZ;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Returns position.
        /// \param current position.
        inline const Moose::Math::CVector3<float> & GetPosition() const
        {
  	return m_vPosition;
        }

        ////////////////////
        /// Adds vector to current position.
        /// \param vPosition the movement applied to current position.
        inline void Move( const Moose::Math::CVector3<float> & vPosition )
        {
  	m_vPosition = m_vPosition + vPosition;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Returns true if position has changed.
        /// \returns boolean
        int IsPositionChanged() const
        {
  	return m_bPositionChanged;
        }
        ////////////////////
        /// Sets the position change flag.
        /// \param bFlag true for enable, false for disable.
        inline void SetPositionChanged(int bFlag )
        {
  	m_bPositionChanged = bFlag;
        }
      };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEPOSITIONAL_H_ */
