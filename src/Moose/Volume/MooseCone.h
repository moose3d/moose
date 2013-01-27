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

#ifndef MOOSECONE_H_
#define MOOSECONE_H_
#include <MoosePositional.h>
#include <MooseVector3.h>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
    /////////////////////////////////////////////////////////////////
    // Class for a Cone
    class MOOSE_API CCone : public Moose::Spatial::CPositional
    {
    protected:

      // *******************************************
      // *  direction
      // *  \   ^   /
      // *   \a | a/   a= angle
      // *    \^|^/
      // *     \|/
      // *   position
      // *******************************************

      // The angle of the cone, in radians.
      float m_fAngle;
      // The direction of the cone
      Moose::Math::CVector3<float> m_vDirection;
      // The length of the cone
      float m_fLength;
      // 1 / sin(a); speeds up the calculations
      float m_fSinReciprocal;
      float m_fSinAngle; // sin (angle)
      float m_fCosAngle; // cos ( angle )
      float m_fCosAngleSqr; // (cos(angle))
      float m_fSinAngleSqr; // (sin(angle))

    public:
      ////////////////////
      /// Constructor.
      CCone();
      ////////////////////
      /// Parametrized constructor
      /// \param vPos Cone starting point.
      /// \param vDir Direction where cone is facing.
      /// \param fAngle Angle of cone.
      /// \param fLength How far cone reaches.
      CCone( const Moose::Math::CVector3<float> &vPos, const Moose::Math::CVector3<float> &vDir, float fAngle, float fLength );
      ////////////////////
      /// Assigns view angle of the cone
      /// \param fAngle Angle in degrees.
      void SetAngle( float fAngle );
      ////////////////////
      /// Assigns direction of cone.
      /// \param vDir direction where cone will be facing.
      void SetDirection( const Moose::Math::CVector3<float> &  vDir );
      ////////////////////
      /// Returns cone direction.
      /// \returns CVector3 with cone direction.
      Moose::Math::CVector3<float> GetDirection()  const;
      ////////////////////
      /// Returns angle of the cone in degrees
      /// \returns Cone angle.
      float GetAngle() const;
      ////////////////////
      /// Returns the 1 / sin(angle)
      /// \returns float.
      float SinReciprocal() const;
      ////////////////////
      /// Returns squared sin(angle)
      /// \returns Squared sin(angle)
      float SinSqr() const;
      ////////////////////
      /// Squared cos(angle)
      /// \returns squared cos(angle)
      float CosSqr() const;
      ////////////////////
      /// Returns cos(angle).
      /// \returns cos(angle)
      float CosAngle() const;
      ////////////////////
      /// Returns sin(angle).
      /// \returns sin(angle)
      float SinAngle() const;
      ////////////////////
      /// Sets cone length.
      /// \param fLength length value.
      void SetLength(float fLength);
      ////////////////////
      /// Get length.
      /// \param Length of cone.
      float GetLength() const;

    }; // class CCone


  };// namespace Volume
};// namespace Moose
/////////////////////////////////////////////////////////////////
inline void
Moose::Volume::CCone::SetDirection( const Moose::Math::CVector3<float> &  vDir )
{
  m_vDirection = vDir.GetNormalized();
}
/////////////////////////////////////////////////////////////////
inline Moose::Math::CVector3<float>
Moose::Volume::CCone::GetDirection()  const
{
  return m_vDirection;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::GetAngle() const
{
  return m_fAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::SinReciprocal() const
{
  return m_fSinReciprocal;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::SinSqr() const
{
  return m_fSinAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::CosSqr() const
{
  return m_fCosAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::CosAngle() const
{
  return m_fCosAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::SinAngle() const
{
  return m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Volume::CCone::SetLength(float fLength)
{
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Volume::CCone::GetLength() const
{
  return m_fLength;
}
/////////////////////////////////////////////////////////////////

#endif /* MOOSECONE_H_ */
