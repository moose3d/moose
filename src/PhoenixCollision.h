#include "PhoenixVolume.h"
namespace Phoenix
{
  namespace Collision
  {
    /// The intersection test functions
    char SphereIntersectsCone ( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CCone &cone );
    char SphereIntersectsSphere( const Phoenix::Volume::CSphere &sphereOne, const Phoenix::Volume::CSphere &sphereTwo);
    int  SphereIntersectsAxisAlignedBox( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CAxisAlignedBox &aaBox);
    //char SphereIntersectsOrientedBox( const CSphere &sphere, const COrientedBox &obBox);

    ////////////////////
    /// The principal method for determining the sphere-polygon intersection.
    /// \param sphere Sphere which is tested.
    /// \param pVertices Vertices of the polygon which sphere is tested on.
    /// \param nNumVertices Number of vertices in polygon.
    /// \param vOffsetMovement Offset vector of collision.
    /// \returns true if intersection ,false otherwise.
    int  SphereIntersectsPolygon( Phoenix::Volume::CSphere &sphere,   
				  float *pVertices,  
				  unsigned int nNumVertices, 
				  CVector3<float> &vOffsetMovement);  
    
    int  SphereIntersectsPolygonEdges( Phoenix::Volume::CSphere &sphere, 
				       float * pVertices, 
				       unsigned int nNumVertices);  
  
    /// Checks whether a ray and a sphere will intersect.
    /// If the ray does not intersect the sphere, the vIntersection is set to 
    /// closest point on the ray.
    /// 
    /// \param vRayDir The direction of the line
    /// \param vRayStart A point of ray origin.
    /// \param vIntersection0 The first point where line intersects the sphere on vRayStart side. If NULL, value is not set.
    /// \param vIntersection1 The second point where line intersects the sphere on vRayEnd side. If NULL, value is not set.
    /// \param Sphere The sphere which is tested.
    /// \returns 0 if the ray and sphere do not intersect,
    ///	       1 if the ray and sphere intersect on one point,
    ///          2 if the ray and sphere intersect on two points.
    int RayIntersectsSphere( const CVector3<float> &vRayStart,  
			     const CVector3<float> &vRayEnd,
			     CVector3<float> *pvIntersection0,
			     CVector3<float> *pvIntersection1,
			     const Phoenix::Volume::CSphere &Sphere);
    
    /// \brief Checks intersection with ray and OBB:
    /// \param vRayDir The direction of the line. || vRayDir || has to be 1.
    /// \param vRayStart A point of ray origin.
    /// \param obBox The oriented box which is checked
    /// \param pfValue pointer to float variable receives distance from 
    ///                ray origin to closest intersection point.
    /// \returns 0 if ray does not intersect OBB. pfValue is undetermined.
    ///          1 if ray intersects OBB. pfValue is set to closest distance.
    int RayIntersectsOBB( const CVector3<float> &vRayStart, 
			  const CVector3<float> &vRayDir,
			  const Phoenix::Volume::COrientedBox &obBox,
			  float *pfValue);
  
    // Generic intersection enum
    enum INTERSECTION_TYPE
    {
      INTERSECTION = 0,
      INSIDE = 1,
      OUTSIDE = 2
    };
    enum LINE_PLANE_INTERSECTION_TYPE
    {
      FRONT, 
      BEHIND,
      INTERSECTS
    };

    LINE_PLANE_INTERSECTION_TYPE LineIntersectsPlane( const CPlane &plane, 
						      const CVector3<float> &vPoint1, 
						      const CVector3<float> &vPoint2,
						      CVector3<float> &vCollisionPoint);

    /*CVector3<float> LinePlaneIntersectionPoint( const CPlane &plane, 
						const CVector3<float> &vPoint1, 
						const CVector3<float> &vPoint2);*/

    INTERSECTION_TYPE  SphereIntersectsPolytone( const Phoenix::Volume::CSphere &Sphere, const Phoenix::Volume::CPolytope &kDop );
    INTERSECTION_TYPE  OrientedBoxIntersectsPolytope( const Phoenix::Volume::COrientedBox &obBox, const Phoenix::Volume::CPolytope &kDop );
    INTERSECTION_TYPE  SphereIntersectsPlane( Phoenix::Math::CPlane &plane, const Phoenix::Volume::CSphere &sphere, float &fCenterDistance);

    
    CVector3<float> ClosestPointOnLine( const CVector3<float> &vPoint,
					const CVector3<float> &vLinePoint1,
					const CVector3<float> &vLinePoint2 );
    /// The closest point on ray to given point.
    /// vDirection must be unit vector.
    CVector3<float> ClosestPointOnRay( const CVector3<float> &vPoint,
				       const CVector3<float> &vStartPoint,
				       const CVector3<float> &vDirection );
    
    int InsidePolygon( CVector3<float> &vPoint, float *pVertices, unsigned int nNumVertices );
    // Returns the floating-point distance from point to a plane
    float PointDistanceFromPlane( CVector3<float> &vPoint, CPlane plane );
  
    int   PointIntersectsOBB( const Phoenix::Volume::COrientedBox &obBox, const CVector3<float> &vPoint );
    // Returns true if point is inside the volume limited by the planes in kDop.
    char	PointWithinKDOP( CVector3<float> &vPoint, Phoenix::Volume::CPolytope &kDop);
    
    

    enum PLANE_INTERSECTION_TYPE
    {
      PLANE_NONE,
      PLANE_POINT,
      PLANE_LINE,
      PLANE_EQUAL
    };

    /////////////////////////////////////////////////////////////////
    /// Calculates the point of intersection between three planes.
    ///  If return value is PLANE_POINT, the Vpoint contains the coordinates.
    /// \returns PLANE_POINT when planes interect in explicit point, 
    ///		 PLANE_LINE when planes intersect along a line,
    ///		 PLANE_NONE when no intersection occurs
    ///
    PLANE_INTERSECTION_TYPE PlaneIntersectionPoint( CPlane &Plane1, 
						    CPlane &Plane2,
						    CPlane &Plane3,
						    CVector3<float> &vPoint );
    /////////////////////////////////////////////////////////////////
    /// Determines whether a plane and axis-aligned box intersect.
    /// \param rPlane The plane which is tested.
    /// \param rAABB The axis-aligned box which is tested.
    /// \returns INTERSECTS if the plane and box intersect,
    ///  FRONT if the box is on the side where plane normal points to,
    ///  BEHIND if the box is on the opposite side where plane normal points to.
    INTERSECTION_TYPE PlaneIntersectsAABB( const CPlane &rPlane,  
					   const Phoenix::Volume::CAxisAlignedBox &rAABB);

    /////////////////////////////////////////////////////////////////
    // 
    // The billboarding utils which can be used to align polygons to face the 
    // camera.
    //
/*     class CCamera; */
/*     namespace Billboard */
/*     { */
    
/*       void AlignToCameraPlane( CVector3<float> *vX,  */
/* 			       CVector3<float> *vY,  */
/* 			       CCamera *pCamera, */
/* 			       CVector3<float> *vQuadPos, */
/* 			       float fQuadWidthDiv2, */
/* 			       float fQuadHeightDiv2 ); */
/*     } */
  }; // namespace Collision
};// namespace Phoenix
