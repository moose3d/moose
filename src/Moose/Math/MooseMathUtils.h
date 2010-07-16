#ifndef __MooseMathUtils_h__
#define __MooseMathUtils_h__
/////////////////////////////////////////////////////////////////
#include <math.h>
/////////////////////////////////////////////////////////////////
#define EPSILON 0.000001f
#define TOO_CLOSE_TO_ZERO(FLOAT_VALUE) ( fabs(FLOAT_VALUE)< EPSILON )
#define QUITE_CLOSE_TO( VALUE, ANOTHER_VALUE ) ( fabsf((VALUE - ANOTHER_VALUE)) < EPSILON )
/////////////////////////////////////////////////////////////////
namespace Moose 
{
  namespace Math 
  {
    /// The handy PI constants
    const float PI     = 3.14159265358979323f;
    const float PImul2 = 6.28318530717958647f;
    const float PIdiv2 = 1.57079632679489661f;
    /// 1.0 / logf(2)
    const float OneDivLogN2 = 1.44269504088896340737f;
    ////////////////////
    /// Convertes degrees to radians.
    /// \param fDegrees degree which will be converted.
    /// \return Radian equiavlent for degrees.
    inline float Deg2Rad( float fDegrees ) 
    {
      return fDegrees*0.01745329251994329572f;
    }
    ////////////////////
    /// Convertes radians to degrees.
    /// \param fRadians radians which will be converted.
    /// \return Degree equiavlent for radian.
    inline float Rad2Deg( float fRadians )
    {
      return fRadians*57.29577951308232103113f;
    }    
    ////////////////////
    /// Calculates logarithm in two base.
    inline float Log2( float fValue )
    {
      return logf( fValue ) * OneDivLogN2;
    }
  }; // namespace Math
}; //namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
