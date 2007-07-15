/////////////////////////////////////////////////////////////////
#ifndef __PhoenixFrustum_h__
#define __PhoenixFrustum_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixVolume.h"
/////////////////////////////////////////////////////////////////
#define NUM_FRUSTUM_CORNERS 8
#define NUM_FRUSTUM_PLANES  6
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// The frustum class for culling.
    class CFrustum : public CPolytope
    {
    public:
      typedef BBOX_PLANE_TYPE FRUSTUM_PLANE ;
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
      ////////////////////
      /// The collision types of frustum and other objects.
      enum FRUSTUM_INTERSECTION
      {
	/// frustum does not contain object even partially.
	OUTSIDE,
	/// frustum contains object only partially.
	INTERSECTION,
	/// frustum contains object completely.
	INSIDE
      };
  
    protected:
      ////////////////////
      /// The pointers to the planes.
      CPlane * m_aPlanes[NUM_FRUSTUM_PLANES];   
      /// The corners of the frustum box.
      CVector3<float> m_aCorners[NUM_FRUSTUM_CORNERS];  
      /////////////////////////////////////////////////////////////////
    public:
      ////////////////////
      /// The constructor
      CFrustum();
      ////////////////////
      /// Assigns a plane.
      /// \param nIndex The plane index which is assigned.
      /// \param vPlane The plane data.
      void		SetPlane( FRUSTUM_PLANE nIndex, CPlane &vPlane);
      ////////////////////
      /// Return a reference to the plane.
      /// \param nIndex The index of the plane desired.
      /// \returns The requested plane.
      CPlane	&GetPlane( FRUSTUM_PLANE nIndex );
      ////////////////////
      /// Returns a corner of the frustum.
      /// \param nIndex The index of the corner.
      /// \returns CVector3 with the position of the desired corner.
      CVector3<float>   GetCorner( FRUSTUM_CORNER nIndex );
      ////////////////////
      /// Sets a corner value.
      /// \param nIndex The index of the corner to be set.
      /// \param vPoint The value which is assigned.
      void		SetCorner( FRUSTUM_CORNER nIndex, const CVector3<float> &vPoint );
      ////////////////////
      /// Determines whether frustum and sphere intersect.
      /// \param sphere The sphere to be checked against.
      /// \returns INSIDE if frustum surrounds the sphere completely.
      ///          OUTSIDE if the sphere is outside all frustum planes.
      ///          INTERSECTION if the sphere intersects the any of the frustum planes.
      // CFrustumIntersection_t	IntersectsSphere( const CSphere &sphere );
      ////////////////////
      /// Determines whether frustum and oriented box  intersect.
      /// \param obBox The oriented box to be checked against.
      /// \returns INSIDE if frustum surrounds the box completely.
      ///          OUTSIDE if the box is outside all frustum planes.
      ///          INTERSECTION if the box intersects the any of the frustum planes.
      ///CFrustumIntersection_t	IntersectsOrientedBox( const COrientedBox &obBox );
      ////////////////////
      /// Determines whether frustum and axis-aligned box intersect.
      /// \param aaBox The axis-aligned box to be checked against.
      /// \returns INSIDE if frustum surrounds the box completely.
      ///          OUTSIDE if the box is outside all frustum planes.
      ///          INTERSECTION if the box intersects the any of the frustum planes.
      ///CFrustumIntersection_t     IntersectsAABB( const CAxisAlignedBox &aaBox );
    };
  }; // namespace Graphics
};// namespace Phoenix
#endif
