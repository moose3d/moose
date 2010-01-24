#ifndef __PHOENIXLINE_H__
#define __PHOENIXLINE_H__
/////////////////////////////////////////////////////////////
#include <PhoenixVector3.h>
#include <PhoenixOneDirectional.h>
#include <PhoenixPositional.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
  /////////////////////////////////////////////////////////////////
      /// Abstraction for 3d lines. Infinitely thin and long, extending both directions
      /// from reference point.
      class PHOENIX_API CLine : public Phoenix::Spatial::COneDirectional,
  		  public Phoenix::Spatial::CPositional
      {
      public:
        ////////////////////
        /// Default constructor.
        /// \param vPos Reference point through which line goes.
        /// \param vDir Line direction - should .
        CLine( const Phoenix::Math::CVector3<float> & vPos = Phoenix::Math::CVector3<float>(0,0,0),
  	     const Phoenix::Math::CVector3<float> & vDir = Phoenix::Math::CVector3<float>(0,0,-1) ) : Phoenix::Spatial::COneDirectional( vDir ),
  										       Phoenix::Spatial::CPositional(vPos){ }

      }; // class CLine
  } // Math
} // Phoenix
/////////////////////////////////////////////////////////////
#endif /* PHOENIXLINE_H_ */
