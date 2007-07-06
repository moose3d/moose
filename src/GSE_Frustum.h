/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_Frustum__
#define __GSE_Frustum__
/////////////////////////////////////////////////////////////////
#include "GSE_geometry.h"
/////////////////////////////////////////////////////////////////
using Geometry::GSE_Plane;
using Geometry::GSE_Sphere;
using Geometry::GSE_OrientedBox;
using Geometry::GSE_AxisAlignedBox;
using Geometry::GSE_KDOP;
/////////////////////////////////////////////////////////////////
#define NUM_FRUSTUM_CORNERS 8
#define NUM_FRUSTUM_PLANES  6
/////////////////////////////////////////////////////////////////
/// The frustum class for culling.
class GSE_Frustum : public GSE_KDOP
{
 public:
  ////////////////////
  /// The plane indices.
  typedef enum {
    /// A plane index.
    TOP = 0,
    /// A plane index.
    BOTTOM = 1,
    /// A plane index.
    LEFT = 2,
    /// A plane index.
    RIGHT = 3,
    /// A plane index.
    NEAR = 4,
    /// A plane index.
    FAR = 5
  } GSE_FrustumPlane;
  ////////////////////
  /// The corner indices.
  typedef enum 
  {
    /// A rather desciptive corner index.
    TOP_FAR_RIGHT = 0,
    /// A rather desciptive corner index.
    TOP_FAR_LEFT = 1,
    /// A rather desciptive corner index.
    TOP_NEAR_RIGHT = 2,
    /// A rather desciptive corner index.
    TOP_NEAR_LEFT = 3,
    /// A rather desciptive corner index.
    BOTTOM_FAR_RIGHT = 4,
    /// A rather desciptive corner index.
    BOTTOM_FAR_LEFT = 5,
    /// A rather desciptive corner index.
    BOTTOM_NEAR_RIGHT = 6,
    /// A rather desciptive corner index.
    BOTTOM_NEAR_LEFT = 7

  } GSE_FrustumCorner;
  ////////////////////
  /// The collision types of frustum and other objects.
  typedef enum 
  {
    /// frustum does not contain object even partially.
    OUTSIDE,
    /// frustum contains object only partially.
    INTERSECTION,
    /// frustum contains object completely.
    INSIDE
  } GSE_FrustumIntersection_t;
  
 protected:
  ////////////////////
  /// The pointers to the planes.
  GSE_Plane *			m_aPlanes[NUM_FRUSTUM_PLANES];   
  /// The corners of the frustum box.
  GSE_Vector3			m_aCorners[NUM_FRUSTUM_CORNERS];  
  /////////////////////////////////////////////////////////////////
 public:
  ////////////////////
  /// The constructor
  GSE_Frustum();
  ////////////////////
  /// Assigns a plane.
  /// \param nIndex The plane index which is assigned.
  /// \param vPlane The plane data.
  void		SetPlane( GSE_FrustumPlane nIndex, GSE_Plane &vPlane);
  ////////////////////
  /// Return a reference to the plane.
  /// \param nIndex The index of the plane desired.
  /// \returns The requested plane.
  GSE_Plane	&GetPlane( GSE_FrustumPlane nIndex );
  ////////////////////
  /// Returns a corner of the frustum.
  /// \param nIndex The index of the corner.
  /// \returns GSE_Vector3 with the position of the desired corner.
  GSE_Vector3   GetCorner( GSE_FrustumCorner nIndex );
  ////////////////////
  /// Sets a corner value.
  /// \param nIndex The index of the corner to be set.
  /// \param vPoint The value which is assigned.
  void		SetCorner( GSE_FrustumCorner nIndex, GSE_Vector3 vPoint );
  ////////////////////
  /// Determines whether frustum and sphere intersect.
  /// \param sphere The sphere to be checked against.
  /// \returns INSIDE if frustum surrounds the sphere completely.
  ///          OUTSIDE if the sphere is outside all frustum planes.
  ///          INTERSECTION if the sphere intersects the any of the frustum planes.
  GSE_FrustumIntersection_t	IntersectsSphere( const GSE_Sphere &sphere );
  ////////////////////
  /// Determines whether frustum and oriented box  intersect.
  /// \param obBox The oriented box to be checked against.
  /// \returns INSIDE if frustum surrounds the box completely.
  ///          OUTSIDE if the box is outside all frustum planes.
  ///          INTERSECTION if the box intersects the any of the frustum planes.
  GSE_FrustumIntersection_t	IntersectsOrientedBox( const GSE_OrientedBox &obBox );
  ////////////////////
  /// Determines whether frustum and axis-aligned box intersect.
  /// \param aaBox The axis-aligned box to be checked against.
  /// \returns INSIDE if frustum surrounds the box completely.
  ///          OUTSIDE if the box is outside all frustum planes.
  ///          INTERSECTION if the box intersects the any of the frustum planes.
  GSE_FrustumIntersection_t     IntersectsAABB( const GSE_AxisAlignedBox &aaBox );
};

#endif
