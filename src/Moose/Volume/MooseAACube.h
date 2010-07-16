#ifndef MOOSEAACUBE_H_
#define MOOSEAACUBE_H_
#include "MooseVector3.h"
#include "MoosePositional.h"
#include "MooseDimensional1D.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTransform.h"
#include <iostream>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
  /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) cube.
    class MOOSE_API CAxisAlignedCube : public Moose::Spatial::CPositional,
					 public Moose::Spatial::CDimensional1D
    {
    public:
      ////////////////////
      /// The constructor.
      CAxisAlignedCube()    {}
      ////////////////////
      /// The parameterized constructor.
      CAxisAlignedCube( const Moose::Math::CVector3<float> &vCenter, float fEdgeLength ) :
    		  Moose::Spatial::CPositional( vCenter ), Moose::Spatial::CDimensional1D( fEdgeLength ) {  }
    }; // AACube
  } // Volume
} // Moose
#endif /* MOOSEAACUBE_H_ */
