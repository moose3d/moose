#ifndef PHOENIXONEDIRECTIONAL_H_
#define PHOENIXONEDIRECTIONAL_H_
#include <PhoenixVector3.h>
#include <PhoenixAPI.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
      /// The class for single-directional object.
      class PHOENIX_API COneDirectional
      {
      protected:
        /// Direction vector.
        Phoenix::Math::CVector3<float> m_vDirection;
      public:
        ////////////////////
        /// Constructor.
        COneDirectional() {}
        ////////////////////
        /// Constructor.
        COneDirectional( const Phoenix::Math::CVector3<float> & vDir ) { SetDirection(vDir); }
        ////////////////////
        /// Assigns direction.
        /// \param vDirection direction vector to be assigned.
        inline void SetDirection( const Phoenix::Math::CVector3<float> & vDirection )
        {
  	m_vDirection = vDirection;
  	m_vDirection.Normalize();
        }
        ////////////////////
        /// Assigns direction.
        /// \param fX X-component of direction vector.
        /// \param fY Y-component of direction vector.
        /// \param fZ Z-component of direction vector.
        inline void SetDirection( float fX, float fY, float fZ )
        {
  	m_vDirection[0] = fX;
  	m_vDirection[1] = fY;
  	m_vDirection[2] = fZ;
  	m_vDirection.Normalize();
        }
        ////////////////////
        /// Returns direction.
        /// \returns Vector with current direction.
        // inline Phoenix::Math::CVector3<float> & GetDirection()
  //       {
  // 	return m_vDirection;
  //       }
        ////////////////////
        /// Returns direction.
        /// \returns Vector with current direction.
        inline const Phoenix::Math::CVector3<float> & GetDirection() const
        {
  	return m_vDirection;
        }
      };
      /////////////////////////////////////////////////////////////////
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXONEDIRECTIONAL_H_ */
