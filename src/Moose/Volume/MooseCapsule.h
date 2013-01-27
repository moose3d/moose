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

#ifndef MOOSECAPSULE_H_
#define MOOSECAPSULE_H_
///////////////////////////////////////////////////////////////////////////////
#include "MooseLineSegment.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Volume
    {
        /////////////////////////////////////////////////////////////////
        /// Capsule class (sphere swept line)
        class MOOSE_API CCapsule : public Moose::Math::CLineSegment
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
            CCapsule( const Moose::Math::CVector3<float> & vStart,
                    const Moose::Math::CVector3<float> & vEnd,
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
        ////////////////////
        /// Computes bounding capsule from given vertex set.
        CCapsule  CalculateBoundingCapsule( const Moose::Graphics::CVertexDescriptor &rVertices );
        ////////////////////
        /// Computes bounding capsule from given vertex set defined by indexBuffer and vVertices.
        CCapsule  CalculateBoundingCapsule( const Moose::Graphics::CVertexDescriptor &rVertices,
                                          const Moose::Graphics::CIndexArray &indexBuffer);
    } // Volume
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSECAPSULE_H_ */
