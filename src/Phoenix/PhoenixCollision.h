#ifndef __PhoenixCollision_h__
#define __PhoenixCollision_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVolume.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Collision
  {
    ////////////////////
    /// Return values for line segment and plane intersections.
    enum LINESEGM_PLANE_INTERSECTION
      {
	LINE_FRONT_OF_PLANE,
	LINE_BEHIND_PLANE,
	LINE_IN_PLANE,
	POINT_IN_PLANE
      };
    
    ////////////////////
    /// Returns values for SphereIntersectsSphere( ... ) with velocities.
    enum S2S_COLLISION_TYPE 
      {
	S2S_NEVER = 0,
	S2S_SOON = 1,
	S2S_ALREADY = 2
      };

    ////////////////////
    /// The collision types for volume-volume intersection tests.
    enum VOLUME_INTERSECTION
      {
	/// volume does not contain another even partially.
	OUTSIDE,
	/// volume contains another only partially.
	INTERSECTION,
	/// volume contains another completely.
	INSIDE
      };
    ////////////////////
    /// Checks does line segment intersects a plane.
    /// \param plane Plane operand.
    /// \param line Line segment operand.
    /// \param vCollisionPoint In case of LINE_INTERSECTS_PLANE, this contains the collision point - otherwise it's value is undetermined.
    /// \returns LINE_PLANE_INTERSECTION.
    LINESEGM_PLANE_INTERSECTION LineSegmentIntersectsPlane( const Phoenix::Math::CPlane &plane,
							    const Phoenix::Math::CLineSegment &line,
							    Phoenix::Math::CVector3<float> &vCollisionPoint);
    ////////////////////
    /// Checks does ray intersect a plane.
    /// \param plane Plane operand.
    /// \param ray Ray operand.
    /// \param vCollisionPoint In case of intersection, this contains the collision point - otherwise it's value is undetermined.
    /// \returns Non-zero if there is an intersection, zero otherwise.
    int RayIntersectsPlane( const Phoenix::Math::CPlane &plane,
			    const Phoenix::Math::CRay &ray,
			    Phoenix::Math::CVector3<float> &vCollisionPoint );
    ////////////////////
    /// Checks whether a ray and a sphere will intersect.
    /// If the ray does not intersect the sphere, the vIntersection is set to
    /// closest point on the ray.
    ///
    /// \param ray Ray reference.
    /// \param vIntersection0 The first point where ray intersects the sphere on starting side. If NULL, value is not set.
    /// \param vIntersection1 The second point where ray intersects the sphere on ending side. If NULL, value is not set.
    /// \param Sphere The sphere which is tested.
    /// \returns 0 if the ray and sphere do not intersect,
    ///	         1 if the ray and sphere intersect on one point,
    ///          2 if the ray and sphere intersect on two points.
    int RayIntersectsSphere( const Phoenix::Math::CRay &ray,
			     Phoenix::Math::CVector3<float> *pvIntersection0,
			     Phoenix::Math::CVector3<float> *pvIntersection1,
			     const Phoenix::Volume::CSphere &Sphere);
    
    ////////////////////
    /// Is given point inside triangle.
    /// \param vPoint Point to be checked.
    /// \param aVertices Corner vertices of triangle.
    /// \return true if is, false otherwise.
    bool PointInsideTriangle( const Phoenix::Math::CVector3<float> &vPoint, 
			      const Phoenix::Math::CVector3<float> & vVertex0,
			      const Phoenix::Math::CVector3<float> & vVertex1,
			      const Phoenix::Math::CVector3<float> & vVertex2 );

    ////////////////////
    /// Does given line segment intersect static triangle.
    /// \param line			Line segment to be checked.
    /// \param vVertex0			First vertex of triangle.
    /// \param vVertex1			Second vertex of triangle.
    /// \param vVertex2			Third vertex of triangle.
    /// \param vPointOfIntersection     If line segment intersects, the point of intersection inside triangle. Otherwise undefined.
    /// \returns non-zero on intersection, zero otherwise.
    int LineSegmentIntersectsTriangle( const Phoenix::Math::CLineSegment & line,
				       const Phoenix::Math::CVector3<float> & vVertex0,
				       const Phoenix::Math::CVector3<float> & vVertex1,
				       const Phoenix::Math::CVector3<float> & vVertex2,
				       Phoenix::Math::CVector3<float> &vPointOfIntersection );
    ////////////////////
    /// Does given ray intersect static triangle.
    /// \param ray			Ray to be checked.
    /// \param vVertex0			First vertex of triangle.
    /// \param vVertex1			Second vertex of triangle.
    /// \param vVertex2			Third vertex of triangle.
    /// \param vPointOfIntersection     If ray intersects, the point of intersection inside triangle. Otherwise undefined.
    /// \returns true on intersection, false otherwise.
    bool RayIntersectsTriangle( const Phoenix::Math::CRay & ray,
				const Phoenix::Math::CVector3<float> & vVertex0,
				const Phoenix::Math::CVector3<float> & vVertex1,
				const Phoenix::Math::CVector3<float> & vVertex2,
				Phoenix::Math::CVector3<float> &vPointOfIntersection );

    float LineSegmentToLineSegmentDistanceSquared( const Phoenix::Math::CLineSegment & line0, const Phoenix::Math::CLineSegment & line1 );
    

    ////////////////////
    /// Returns the floating-point distance from point to a plane.
    /// \param vPoint Point of origin.
    /// \param plane Plane object.
    /// \returns Distance to plane - value 0.0f is in plane.
    float PointDistanceFromPlane( const Phoenix::Math::CVector3<float> &vPoint, 
				  const Phoenix::Math::CPlane &plane );
    
    ////////////////////
    /// Find closest point on ray to given point.
    /// \param vPoint Point in space.
    /// \param ray Ray which is compared to point.
    /// \param Closest position on ray to given point.
    void ClosestPointOnRay( const CVector3<float> &vPoint,
			    const Phoenix::Math::CRay &ray,
			    Phoenix::Math::CVector3<float> & vClosestPoint);

    ////////////////////
    /// Find closest point on line to given point.
    /// \param vPoint Point in space.
    /// \param line Line which is compared to point.
    /// \param Closest position on line to given point.
    void ClosestPointOnLine( const CVector3<float> &vPoint,
			     const Phoenix::Math::CLine &line,
			     Phoenix::Math::CVector3<float> & vClosestPoint);

    
    ////////////////////
    /// Find closest point on line to given point.
    /// \param vPoint Point in space.
    /// \param line Line which is compared to point.
    /// \param vClosestPoint Closest position on line to given point.
    void ClosestPointOnLineSegment( const CVector3<float> &vPoint,
				    const Phoenix::Math::CLineSegment & line,
				    Phoenix::Math::CVector3<float> & vClosestPoint );



    ////////////////////
    /// Checks whether a plane and box intersect.
    /// \param plane A plane.
    /// \param box An oriented box.
    /// \returns Non-zero on intersect, zero otherwise.
    int PlaneIntersectsBox( const Phoenix::Math::CPlane &plane,
			    const Phoenix::Volume::COrientedBox &box );

    ////////////////////
    /// Checks whether a plane and box intersect.
    /// \param plane A plane.
    /// \param box An axis-aligned box.
    /// \returns Non-zero on intersect, zero otherwise.
    int PlaneIntersectsBox( const Phoenix::Math::CPlane &plane,
			    const Phoenix::Volume::CAxisAlignedBox &box );
    ////////////////////
    /// Checks whether a triangle and oriented box intersect.
    /// \param vVertex0 First corner of triangle.
    /// \param vVertex1 Second corner of triangle.
    /// \param vVertex2 Third corner of triangle.
    /// \param box Oriented box.
    /// \returns non-zero on intersect, zero otherwise.
    int TriangleIntersectsOBB( Phoenix::Math::CVector3<float> vVertex0,
			       Phoenix::Math::CVector3<float> vVertex1,
			       Phoenix::Math::CVector3<float> vVertex2,
			       const Phoenix::Volume::COrientedBox &box );

    ////////////////////
    /// Checks whether point is inside oriented box.
    /// \param vPoint The point to be checked.
    /// \param obBox An oriented box.
    /// \returns non-zero on intersect, zero otherwise.
    int   PointIntersectsOBB( const CVector3<float> &vPoint,
			      const Phoenix::Volume::COrientedBox &obBox );

    ////////////////////
    /// Calculates collision point between three planes.
    /// \param plane1 First plane.
    /// \param plane2 Second plane.
    /// \param plane3 Third plane.
    /// \param vCollisionPoint Point of collision.
    /// \returns Non-zero if planes do not intersect in a single point. Value of vCollisionPoint is undetermined.
    /// \returns Zero if planes intersect in a single point. Value of vCollisionPoint is set to that point.
    int  CollisionPoint3Planes( const Phoenix::Math::CPlane & plane1,
				const Phoenix::Math::CPlane & plane2,
				const Phoenix::Math::CPlane & plane3,
				Phoenix::Math::CVector3<float> &vCollisionPoint);
    ////////////////////
    /// Checks does a sphere and a plane intersect.
    /// \param plane Plane object.
    /// \param sphere Sphere object.
    /// \param fCenterDistance Distance of sphere center into plane.
    /// \returns <0 If sphere is completely on negative half-side of plane.
    /// \returns 0 If sphere intersects plane
    /// \returns >1 If sphere is completely on positive half-side of plane.
    int SphereIntersectsPlane( const Phoenix::Math::CPlane &plane, 
			       const Phoenix::Volume::CSphere &sphere, 
			       float &fCenterDistance); 
    ////////////////////
    /// Checks does sphere intersect axis-aligned cube.
    /// \param sphere A sphere
    /// \param aaCube A cube
    /// \returns Zero if sphere and cube do not intersect.
    /// \returns Non-zero if sphere and cube intersect.
    int SphereIntersectsAACube( const Phoenix::Volume::CSphere &sphere,  
				const Phoenix::Volume::CAxisAlignedCube &aaCube);
    
    ////////////////////
    /// Checks do two spheres intersect.
    /// \param sphereOne First sphere
    /// \param sphereTwo Second sphere
    /// \returns Zero if spheres do not intersect.
    /// \returns Non-zero if spheres intersect.
    int SphereIntersectsSphere( const Phoenix::Volume::CSphere &sphereOne, 
				const Phoenix::Volume::CSphere &sphereTwo );

    ////////////////////
    /// Checks do two moving spheres intersect.
    /// \param sphereOne First sphere
    /// \param sphereTwo Second sphere
    /// \param vVelocityOne Velocity of first sphere
    /// \param vVelocityTwo Velocity of second sphere
    /// \param vIntersectionPoint Point of intersection, if spheres will intersect.
    /// \param fRelativeTime Relative time [0,1[ when spheres intersect (if they will intersect).
    /// \returns S2S_NEVER   if spheres do not intersect during movement. Values of vIntersectionPoint and fRelativeTime are undefined.
    /// \returns S2S_ALREADY if spheres intersect before moving. Values of vIntersectionPoint and fRelativeTime are undefined.
    /// \returns S2S_SOON  if spheres will intersect during movement.Values of vIntersectionPoint and fRelativeTime are set to corresponding values.
    S2S_COLLISION_TYPE  SphereIntersectsSphere( const Phoenix::Volume::CSphere &sphereOne,
						const Phoenix::Math::CVector3<float> &vVelocityOne,
						const Phoenix::Volume::CSphere &sphereTwo,
						const Phoenix::Math::CVector3<float> &vVelocityTwo,
						Phoenix::Math::CVector3<float> & vIntersectionPoint,
						float &fRelativeTime );
    
    ////////////////////
    /// Checks is point within sphere.
    /// \param sphere sphere to be checked.
    /// \param vPoint 3-Vector representing position in space.
    /// \returns non-zero on intersectsion.
    /// \returns zero on non-intersection.
    int PointInsideSphere( const Phoenix::Volume::CSphere & sphere,  
			   const CVector3<float> & vPoint );

    
    ////////////////////
    /// Calculates which triangles are completely or partially inside decal volume.
    /// \param decalVolume Volume used in evaluation
    /// \param vertices VertexDescriptor of vertex data.
    /// \param indices Triangle indices
    /// \param lstTriangleFans List, where lists of vertex coordinates are stored for final mesh.
    void CalculateDecalMesh( const Phoenix::Volume::CDecalVolume & decalVolume, 
			     const Phoenix::Graphics::CVertexDescriptor & vertices, 
			     const Phoenix::Graphics::CIndexArray & indices,
			     std::list< std::list< Phoenix::Math::CVector3<float> > > & lstTriangleFans );
    ////////////////////
    /// Calculates which triangles are completely or partially inside decal volume.
    /// \param decalVolume Volume used in evaluation
    /// \param vertices VertexDescriptor of vertex coordinate data.
    /// \param normals VertexDescriptor of vertex normal data.
    /// \param indices Triangle indices
    /// \param lstTriangleFans List, where lists of vertices are stored for final mesh.
    void CalculateDecalMesh( const Phoenix::Volume::CDecalVolume & decalVolume, 
			     const Phoenix::Graphics::CVertexDescriptor & vertices, 
			     const Phoenix::Graphics::CVertexDescriptor & normals, 
			     const Phoenix::Graphics::CIndexArray & indices,
			     std::list< std::list< Phoenix::Spatial::CVertex > > & lstTriangleFans );

    ////////////////////
    /// Clips given polygon with plane. New vertices replace previous ones.
    /// \param plane Clipping plane.
    /// \param lstVertices List of vertices (vector3s).
    void ClipPolygon( const Phoenix::Math::CPlane & plane, std::list< Phoenix::Math::CVector3<float> > & lstVertices );
    
    ////////////////////
    /// Clips given polygon with plane. New vertices replace previous ones.
    /// \param plane Clipping plane.
    /// \param lstVertices List of vertices (CVertex).
    void ClipPolygon( const Phoenix::Math::CPlane & plane, std::list< Phoenix::Spatial::CVertex > & lstVertices );

    ////////////////////
    /// Does sphere intersect a polytope.
    /// \param obBox Oriented bounding box to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    VOLUME_INTERSECTION  OBBIntersectsPolytope( const Phoenix::Volume::COrientedBox & obBox, const Phoenix::Volume::CPolytope & poly );
    ////////////////////
    /// Does sphere intersect a polytope.
    /// \param sphere Sphere to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    VOLUME_INTERSECTION  SphereIntersectsPolytope( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CPolytope & poly );
    ////////////////////
    /// Does AABB intersect a polytope.
    /// \param aabb Axisaligned box to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    VOLUME_INTERSECTION  AABBIntersectsPolytope( const Phoenix::Volume::CAxisAlignedBox &aabb, const Phoenix::Volume::CPolytope & poly );
    ////////////////////
    /// Does AABB cube intersect a polytope.
    /// \param aabb AxisAlignedCube to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    VOLUME_INTERSECTION  AABBIntersectsPolytope( const Phoenix::Volume::CAxisAlignedCube &aabb, const Phoenix::Volume::CPolytope & poly );
    ////////////////////
    /// Checks intersection with ray and OBB.
    /// \param ray Ray object.
    /// \param obBox The oriented box which is checked 
    /// \param pfValue optional pointer to float variable that receives distance from  
    ///                ray origin to closest intersection point. By default, value is NULL.
    /// \returns false if ray does not intersect OBB. pfValue is undetermined. 
    ///          true if ray intersects OBB. pfValue is set to closest distance. 
    bool RayIntersectsOBB( const Phoenix::Math::CRay &ray, const Phoenix::Volume::COrientedBox &obBox,  float *pfValue = NULL );

    ////////////////////
    /// Checks intersection with ray and AABB.
    /// \param ray Ray object.
    /// \param aaBox The oriented box which is checked 
    /// \param pfValue optional pointer to float variable that receives distance from  
    ///                ray origin to closest intersection point. By default, value is NULL.
    /// \returns false if ray does not intersect AABB. pfValue is undetermined. 
    ///          true if ray intersects AABB. pfValue is set to closest distance. 
    bool RayIntersectsAABB( const Phoenix::Math::CRay &ray, const Phoenix::Volume::CAxisAlignedBox &aaBox,  float *pfValue = NULL );
      
    /* char SphereIntersectsCone ( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CCone &cone ); */

    /*     int  SphereIntersectsAxisAlignedBox( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CAxisAlignedBox &aaBox); */
    /*     //char SphereIntersectsOrientedBox( const CSphere &sphere, const COrientedBox &obBox); */
    
    /*     //////////////////// */
    /*     /// The principal method for determining the sphere-polygon intersection. */
    /*     /// \param sphere Sphere which is tested. */
    /*     /// \param pVertices Vertices of the polygon which sphere is tested on. */
    /*     /// \param nNumVertices Number of vertices in polygon. */
    /*     /// \param vOffsetMovement Offset vector of collision. */
    /*     /// \returns true if intersection ,false otherwise. */
    /*     int  SphereIntersectsPolygon( Phoenix::Volume::CSphere &sphere,    */
    /* 				  float *pVertices,   */
    /* 				  unsigned int nNumVertices,  */
    /* 				  CVector3<float> &vOffsetMovement);   */
    
    /*     int  SphereIntersectsPolygonEdges( Phoenix::Volume::CSphere &sphere,  */
    /* 				       float * pVertices,  */
    /* 				       unsigned int nNumVertices);   */
    

    /*     int RayIntersectsOBB( const CVector3<float> &vRayStart,  */
    /* 			  const CVector3<float> &vRayDir, */
    /* 			  const Phoenix::Volume::COrientedBox &obBox, */
    /* 			  float *pfValue); */
    
    /*     // Generic intersection enum */
    /*     /\* enum INTERSECTION_TYPE *\/ */
    /*     /\*     { *\/ */
    /* /\*       INTERSECTION = 0, *\/ */
    /* /\*       INSIDE = 1, *\/ */
    /* /\*       OUTSIDE = 2 *\/ */
    /* /\*     }; *\/ */
    

    /*     /\*CVector3<float> LinePlaneIntersectionPoint( const CPlane &plane,  */
    /*       const CVector3<float> &vPoint1,  */
    /*       const CVector3<float> &vPoint2);*\/ */
    
    /*     //INTERSECTION_TYPE  SphereIntersectsPolytone( const Phoenix::Volume::CSphere &Sphere, const Phoenix::Volume::CPolytope &kDop ); */
    /*     //INTERSECTION_TYPE  OrientedBoxIntersectsPolytope( const Phoenix::Volume::COrientedBox &obBox, const Phoenix::Volume::CPolytope &kDop ); */

    
    
    /*     //CVector3<float> ClosestPointOnLine( const CVector3<float> &vPoint, */
    /*     //const CVector3<float> &vLinePoint1, */
    /*     //const CVector3<float> &vLinePoint2 ); */

    

    
    

    /*     // Returns true if point is inside the volume limited by the planes in kDop. */
    /*     //  char	PointWithinKDOP( CVector3<float> &vPoint, Phoenix::Volume::CPolytope &kDop); */
    
    

    /*     enum PLANE_INTERSECTION_TYPE */
    /*     { */
    /*       PLANE_NONE, */
    /*       PLANE_POINT, */
    /*       PLANE_LINE, */
    /*       PLANE_EQUAL */
    /*     }; */

    /*     ///////////////////////////////////////////////////////////////// */
    /*     /// Calculates the point of intersection between three planes. */
    /*     ///  If return value is PLANE_POINT, the Vpoint contains the coordinates. */
    /*     /// \returns PLANE_POINT when planes interect in explicit point,  */
    /*     ///		 PLANE_LINE when planes intersect along a line, */
    /*     ///		 PLANE_NONE when no intersection occurs */
    /*     /// */
    /* /\*   PLANE_INTERSECTION_TYPE PlaneIntersectionPoint( CPlane &Plane1,  *\/ */
    /* /\* 						    CPlane &Plane2, *\/ */
    /* /\* 						    CPlane &Plane3, *\/ */
    /* /\* 						    CVector3<float> &vPoint ); *\/ */
    /*     ///////////////////////////////////////////////////////////////// */
    /*     /// Determines whether a plane and axis-aligned box intersect. */
    /*     /// \param rPlane The plane which is tested. */
    /*     /// \param rAABB The axis-aligned box which is tested. */
    /*     /// \returns INTERSECTS if the plane and box intersect, */
    /*     ///  FRONT if the box is on the side where plane normal points to, */
    /*     ///  BEHIND if the box is on the opposite side where plane normal points to. */
    /* /\*     INTERSECTION_TYPE PlaneIntersectsAABB( const CPlane &rPlane,   *\/ */
    /* /\* 					   const Phoenix::Volume::CAxisAlignedBox &rAABB); *\/ */

    /*     ///////////////////////////////////////////////////////////////// */
    /*     //  */
    /*     // The billboarding utils which can be used to align polygons to face the  */
    /*     // camera. */
    /*     // */
    /* /\*     class CCamera; *\/ */
    /* /\*     namespace Billboard *\/ */
    /* /\*     { *\/ */
    
    /* /\*       void AlignToCameraPlane( CVector3<float> *vX,  *\/ */
    /* /\* 			       CVector3<float> *vY,  *\/ */
    /* /\* 			       CCamera *pCamera, *\/ */
    /* /\* 			       CVector3<float> *vQuadPos, *\/ */
    /* /\* 			       float fQuadWidthDiv2, *\/ */
    /* /\* 			       float fQuadHeightDiv2 ); *\/ */
    /* /\*     } *\/ */

      } // namespace Collision
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline int 
Phoenix::Collision::PointInsideSphere( const Phoenix::Volume::CSphere & sphere, 
				       const Phoenix::Math::CVector3<float> & vPoint )
{
  return ( (sphere.GetPosition() - vPoint).LengthSqr() < sphere.GetRadiusSqr());
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////


