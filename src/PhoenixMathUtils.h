#ifndef __PhoenixMathUtils_h__
#define __PhoenixMathUtils_h__
/////////////////////////////////////////////////////////////////
#define EPSILON 0.000001f
#define TOO_CLOSE_TO_ZERO(FLOAT_VALUE) ( fabs(FLOAT_VALUE)< EPSILON )
#define QUITE_CLOSE_TO( VALUE, ANOTHER_VALUE ) ( (VALUE - ANOTHER_VALUE) < EPSILON )
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Math 
  {
    /// The handy PI constants
    const float PI     = 3.14159265358979323f;
    const float PIdiv2 = 1.57079632679489661f;

    ////////////////////
    /// Convertes degrees to radians.
    /// \param fDegrees degree which will be converted.
    /// \return Radian equiavlent for degrees.
    inline float Deg2Rad( float fDegrees ) 
    {
      return fDegrees*0.0174532925f;
    }
    ////////////////////
    /// Convertes radians to degrees.
    /// \param fRadians radians which will be converted.
    /// \return Degree equiavlent for radian.
    inline float Rad2Deg( float fRadians )
    {
      return fRadians*57.2957795201f;
    }    
  }; // namespace Math
}; //namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
