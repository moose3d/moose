/*
 * PhoenixAABB.h
 *
 *  Created on: Sep 18, 2009
 *      Author: entity
 */

#ifndef PHOENIXAABB_H_
#define PHOENIXAABB_H_
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTransform.h"
#include "PhoenixVertexDescriptor.h"
#include <iostream>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
    /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) box.
    class PHOENIX_API CAxisAlignedBox : public Phoenix::Spatial::CPositional,
																				public Phoenix::Spatial::CDimensional3D
    {
    private:
      CVector3<float> m_vMin;
      CVector3<float> m_vMax;
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
      CAxisAlignedBox( const CVector3<float> &vCenter,
		       float fWidth, float fHeight,  float fLength );
      ////////////////////
      /// \returns Corner point with smallest coordinate values.
      const CVector3<float> & GetMin() const;
      /// \returns Corner point with greatest coordinate values.
      const CVector3<float> & GetMax() const;
    }; // AABB
    ////////////////////
    /// Calculates the axis-aligned bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    PHOENIX_API Phoenix::Volume::CAxisAlignedBox CalculateAlignedBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );

  } // Volume
} // Phoenix
#endif /* PHOENIXAABB_H_ */
