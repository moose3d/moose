#ifndef __MOOSEAABB_H__
#define __MOOSEAABB_H__
///////////////////////////////////////////////////////////////////////////////
#include <MooseVector3.h>
#include "MoosePositional.h"
#include "MooseDimensional3D.h"
#include "MooseIndexArray.h"
#include "MooseTransform.h"
#include "MooseVertexDescriptor.h"
#include <iostream>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
    /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) box.
    class MOOSE_API CAxisAlignedBox : public Moose::Spatial::CPositional,
																				public Moose::Spatial::CDimensional3D
    {
    private:
      Moose::Math::CVector3<float> m_vMin;
      Moose::Math::CVector3<float> m_vMax;
    public:
      ////////////////////
      /// Initializes box with zero width, height and length, positioned at origo.
      CAxisAlignedBox();
      ////////////////////
      /// Initializes bounding box.
      /// \param vCenter Center of the box
      /// \param fWidth Width of box (on x-axis)
      /// \param fHeight Height of box (on y-axis)
      /// \param fLength Length of box (on z-axis)
      CAxisAlignedBox( const Moose::Math::CVector3<float> &vCenter,
		       float fWidth, float fHeight,  float fLength );
      ////////////////////
      /// \returns Corner point with smallest coordinate values.
      const Moose::Math::CVector3<float> & GetMin() const;
      /// \returns Corner point with greatest coordinate values.
      const Moose::Math::CVector3<float> & GetMax() const;
    }; // AABB
    ////////////////////
    /// Calculates the axis-aligned bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    MOOSE_API Moose::Volume::CAxisAlignedBox CalculateAlignedBox( const Moose::Graphics::CVertexDescriptor &vertexDescriptor );

  } // Volume
} // Moose
#endif /* MOOSEAABB_H_ */
