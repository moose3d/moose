#ifndef PHOENIXCAPSULE_H_
#define PHOENIXCAPSULE_H_
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixLineSegment.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
    namespace Volume
    {
        /////////////////////////////////////////////////////////////////
        /// Capsule class (sphere swept line)
        class PHOENIX_API CCapsule : public Phoenix::Math::CLineSegment
        {
        protected:
            /// Sphere radius.
            float m_fRadius;
            /// Squared radius.
            float m_fRadiusSqr;
        public:
            CCapsule()
            {

            }
            ////////////////////
            /// Constructor.
            /// \param vStart Start point of capsule.
            /// \param vEnd End point of capsule
            /// \param fRadius Radius of the capsule sphere.
            CCapsule( const Phoenix::Math::CVector3<float> & vStart,
                    const Phoenix::Math::CVector3<float> & vEnd,
                    float fRadius )
            {
                SetRadius( fRadius );
                Set( vStart, vEnd );
            }
            /////////////////////////////////////////////////////////////////
            /// Sets the radius.
            /// \param fRadius the radius value.
            inline void SetRadius(float fRadius)
            {
                m_fRadius = fRadius;
                m_fRadiusSqr = m_fRadius * m_fRadius;
            }
            /////////////////////////////////////////////////////////////////
            /// Return the radius.
            /// \return float radius.
            inline float GetRadius() const
            {
                return m_fRadius;
            }
            /////////////////////////////////////////////////////////////////
            /// Returns the squared radius. Useful in some situations.
            /// \returns squared radius.
            inline float GetRadiusSqr() const
            {
                return m_fRadiusSqr;
            }
        }; // class CCapsule
    } // Volume
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXCAPSULE_H_ */
