/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/////////////////////////////////////////////////////////////////
#ifndef __MooseFrustum_h__
#define __MooseFrustum_h__
/////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MoosePolytope.h"
#include "MooseOBB.h"
#include "MoosePlane.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
#define NUM_FRUSTUM_CORNERS 8
#define NUM_FRUSTUM_PLANES  6
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// The frustum class for culling.
    class MOOSE_API CFrustum : public Moose::Volume::CPolytope
    {
    public:
      typedef Moose::Volume::BBOX_PLANE_TYPE FRUSTUM_PLANE ;
      ////////////////////
      /// The corner indices.
      enum FRUSTUM_CORNER
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

	} ;
      
  
    protected:
      /// The corners of the frustum box.
      Moose::Math::CVector3<float> m_aCorners[NUM_FRUSTUM_CORNERS];  
      /////////////////////////////////////////////////////////////////
    public:
      ////////////////////
      /// The constructor
      CFrustum();
      ////////////////////
      /// Assigns a plane.
      /// \param nIndex The plane index which is assigned.
      /// \param vPlane The plane data.
      void		SetPlane( FRUSTUM_PLANE nIndex, Moose::Math::CPlane &vPlane);
      ////////////////////
      /// Return a reference to the plane.
      /// \param nIndex The index of the plane desired.
      /// \returns The requested plane.
      Moose::Math::CPlane	&GetPlane( FRUSTUM_PLANE nIndex );
      ////////////////////
      /// Return a reference to the plane.
      /// \param nIndex The index of the plane desired.
      /// \returns The requested plane.
      const Moose::Math::CPlane	&GetPlane( FRUSTUM_PLANE nIndex ) const;
      ////////////////////
      /// Returns a corner of the frustum.
      /// \param nIndex The index of the corner.
      /// \returns CVector3 with the position of the desired corner.
      Moose::Math::CVector3<float>   GetCorner( FRUSTUM_CORNER nIndex );
      ////////////////////
      /// Sets a corner value.
      /// \param nIndex The index of the corner to be set.
      /// \param vPoint The value which is assigned.
      void		SetCorner( FRUSTUM_CORNER nIndex, const Moose::Math::CVector3<float> &vPoint );
      
    };
  }; // namespace Graphics
};// namespace Moose
#endif
