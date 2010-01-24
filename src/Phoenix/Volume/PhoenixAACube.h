#ifndef PHOENIXAACUBE_H_
#define PHOENIXAACUBE_H_
#include "PhoenixVector3.h"
#include "PhoenixPositional.h"
#include "PhoenixDimensional1D.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTransform.h"
#include <iostream>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
  /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) cube.
    class PHOENIX_API CAxisAlignedCube : public Phoenix::Spatial::CPositional,
					 public Phoenix::Spatial::CDimensional1D
    {
    public:
      ////////////////////
      /// The constructor.
      CAxisAlignedCube()    {}
      ////////////////////
      /// The parameterized constructor.
      CAxisAlignedCube( const Phoenix::Math::CVector3<float> &vCenter, float fEdgeLength ) :
    		  Phoenix::Spatial::CPositional( vCenter ), Phoenix::Spatial::CDimensional1D( fEdgeLength ) {  }
    }; // AACube
  } // Volume
} // Phoenix
#endif /* PHOENIXAACUBE_H_ */
