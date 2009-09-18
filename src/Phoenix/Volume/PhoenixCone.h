#ifndef PHOENIXCONE_H_
#define PHOENIXCONE_H_
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
    /////////////////////////////////////////////////////////////////
    // Class for a Cone
    class PHOENIX_API CCone : public Phoenix::Spatial::CPositional
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
      CVector3<float> m_vDirection;
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
      CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength );
      ////////////////////
      /// Assigns view angle of the cone
      /// \param fAngle Angle in degrees.
      void SetAngle( float fAngle );
      ////////////////////
      /// Assigns direction of cone.
      /// \param vDir direction where cone will be facing.
      void SetDirection( const CVector3<float> &  vDir );
      ////////////////////
      /// Returns cone direction.
      /// \returns CVector3 with cone direction.
      CVector3<float> GetDirection()  const;
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
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Volume::CCone::SetDirection( const CVector3<float> &  vDir )
{
  m_vDirection = vDir.GetNormalized();
}
/////////////////////////////////////////////////////////////////
inline CVector3<float>
Phoenix::Volume::CCone::GetDirection()  const
{
  return m_vDirection;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::GetAngle() const
{
  return m_fAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::SinReciprocal() const
{
  return m_fSinReciprocal;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::SinSqr() const
{
  return m_fSinAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::CosSqr() const
{
  return m_fCosAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::CosAngle() const
{
  return m_fCosAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::SinAngle() const
{
  return m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Volume::CCone::SetLength(float fLength)
{
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Volume::CCone::GetLength() const
{
  return m_fLength;
}
/////////////////////////////////////////////////////////////////

#endif /* PHOENIXCONE_H_ */
