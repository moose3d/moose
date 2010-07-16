#ifndef __MOOSELINE_H__
#define __MOOSELINE_H__
/////////////////////////////////////////////////////////////
#include <MooseVector3.h>
#include <MooseOneDirectional.h>
#include <MoosePositional.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
  /////////////////////////////////////////////////////////////////
      /// Abstraction for 3d lines. Infinitely thin and long, extending both directions
      /// from reference point.
      class MOOSE_API CLine : public Moose::Spatial::COneDirectional,
  		  public Moose::Spatial::CPositional
      {
      public:
        ////////////////////
        /// Default constructor.
        /// \param vPos Reference point through which line goes.
        /// \param vDir Line direction - should .
        CLine( const Moose::Math::CVector3<float> & vPos = Moose::Math::CVector3<float>(0,0,0),
  	     const Moose::Math::CVector3<float> & vDir = Moose::Math::CVector3<float>(0,0,-1) ) : Moose::Spatial::COneDirectional( vDir ),
  										       Moose::Spatial::CPositional(vPos){ }

      }; // class CLine
  } // Math
} // Moose
/////////////////////////////////////////////////////////////
#endif /* MOOSELINE_H_ */
