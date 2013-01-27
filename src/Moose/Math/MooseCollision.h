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

#ifndef __MooseCollision_h__
#define __MooseCollision_h__
/////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MooseOBB.h"
#include "MooseAABB.h"
#include "MooseAACube.h"
#include "MooseCapsule.h"
#include "MooseSphere.h"
#include "MooseDecalVolume.h"
#include "MoosePolytope.h"
#include "MooseCone.h"
#include "MooseAPI.h"
#include "MoosePlane.h"
#include "MooseRay.h"
#include "MooseVertex.h"
#include "MooseLineSegment.h"
/////////////////////////////////////////////////////////////////
namespace Moose
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
    ///////////////////
    /// Checks does capsule intersect a plane.
    /// \param plane Plane used in comparison.
    /// \param capsule Capsule used in comparison.
    /// \return -1 if capsule is behind the plane.
    /// \return 0 if capsule intersects plane.
    /// \return 1 if capsule is in front of the plane.
    MOOSE_API int CapsuleIntersectsPlane( const Moose::Math::CPlane & plane,
                                            const Moose::Volume::CCapsule & capsule );

    ////////////////////
    /// Checks does line segment intersects a plane.
    /// \param plane Plane operand.
    /// \param line Line segment operand.
    /// \param vCollisionPoint In case of LINE_INTERSECTS_PLANE, this contains the collision point - otherwise it's value is undetermined.
    /// \returns LINE_PLANE_INTERSECTION.
    MOOSE_API LINESEGM_PLANE_INTERSECTION LineSegmentIntersectsPlane( const Moose::Math::CPlane &plane,
									const Moose::Math::CLineSegment &line,
									Moose::Math::CVector3<float> &vCollisionPoint);
    ////////////////////
    /// Checks does ray intersect a plane.
    /// \param plane Plane operand.
    /// \param ray Ray operand.
    /// \param vCollisionPoint In case of intersection, this contains the collision point - otherwise it's value is undetermined.
    /// \returns Non-zero if there is an intersection, zero otherwise.
    MOOSE_API int RayIntersectsPlane( const Moose::Math::CPlane &plane,
					const Moose::Math::CRay &ray,
					Moose::Math::CVector3<float> &vCollisionPoint );
    ////////////////////
    /// Checks does ray intersect a plane.
    /// \param plane Plane operand.
    /// \param ray Ray operand.
    /// \param pfDistance In case of intersection, this contains the collision point distance from ray starting point - otherwise it's value is undetermined.
    /// \returns Non-zero if there is an intersection, zero otherwise.
    MOOSE_API int RayIntersectsPlane( const Moose::Math::CPlane &plane,
    		const Moose::Math::CRay &ray,
    		float *pfDistance = NULL );
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
    MOOSE_API int RayIntersectsSphere( const Moose::Math::CRay &ray,
					 Moose::Math::CVector3<float> *pvIntersection0,
					 Moose::Math::CVector3<float> *pvIntersection1,
					 const Moose::Volume::CSphere &Sphere);
    ////////////////////
    /// Checks whether a ray and a sphere will intersect.
    /// If the ray does not intersect the sphere, the vIntersection is set to
    /// closest point on the ray.
    ///
    /// \param ray Ray reference.
    /// \param pfIntersection0 distance to first point where ray intersects the sphere on starting side. If NULL, value is not set.
    /// \param pfIntersection1 distance to second point where ray intersects the sphere on ending side. If NULL, value is not set.
    /// \param Sphere The sphere which is tested.
    /// \returns 0 if the ray and sphere do not intersect,
    ///	         1 if the ray and sphere intersect on one point,
    ///          2 if the ray and sphere intersect on two points.
    MOOSE_API int RayIntersectsSphere( const Moose::Math::CRay &ray,
    		float *pfIntersection0,
    		float *pfIntersection1,
    		const Moose::Volume::CSphere &Sphere);
    ////////////////////
    /// Does given ray intersect static triangle.
    /// \param ray			Ray to be checked.
    /// \param vVertex0			First vertex of triangle.
    /// \param vVertex1			Second vertex of triangle.
    /// \param vVertex2			Third vertex of triangle.
    /// \param vPointOfIntersection     If ray intersects, the point of intersection inside triangle. Otherwise undefined.
    /// \returns true on intersection, false otherwise.
    MOOSE_API bool RayIntersectsTriangle( const Moose::Math::CRay & ray,
    		const Moose::Math::CVector3<float> & vVertex0,
    		const Moose::Math::CVector3<float> & vVertex1,
    		const Moose::Math::CVector3<float> & vVertex2,
    		Moose::Math::CVector3<float> &vPointOfIntersection );
    ////////////////////
    /// Does given ray intersect static triangle.
    /// \param ray			Ray to be checked.
    /// \param vVertex0			First vertex of triangle.
    /// \param vVertex1			Second vertex of triangle.
    /// \param vVertex2			Third vertex of triangle.
    /// \param pfPointOfIntersection     If ray intersects, distance to the point of intersection inside triangle. Otherwise undefined.
    /// \returns true on intersection, false otherwise.
    MOOSE_API bool RayIntersectsTriangle( const Moose::Math::CRay & ray,
    		const Moose::Math::CVector3<float> & vVertex0,
    		const Moose::Math::CVector3<float> & vVertex1,
    		const Moose::Math::CVector3<float> & vVertex2,
    		float *pfPointOfIntersection = NULL);
    ////////////////////
    /// Is given point inside triangle.
    /// \param vPoint Point to be checked.
    /// \param aVertices Corner vertices of triangle.
    /// \return true if is, false otherwise.
    MOOSE_API bool PointInsideTriangle( const Moose::Math::CVector3<float> &vPoint,
					  const Moose::Math::CVector3<float> & vVertex0,
					  const Moose::Math::CVector3<float> & vVertex1,
					  const Moose::Math::CVector3<float> & vVertex2 );

    ////////////////////
    /// Does given line segment intersect static triangle.
    /// \param line			Line segment to be checked.
    /// \param vVertex0			First vertex of triangle.
    /// \param vVertex1			Second vertex of triangle.
    /// \param vVertex2			Third vertex of triangle.
    /// \param vPointOfIntersection     If line segment intersects, the point of intersection inside triangle. Otherwise undefined.
    /// \returns non-zero on intersection, zero otherwise.
    MOOSE_API int LineSegmentIntersectsTriangle( const Moose::Math::CLineSegment & line,
						   const Moose::Math::CVector3<float> & vVertex0,
						   const Moose::Math::CVector3<float> & vVertex1,
						   const Moose::Math::CVector3<float> & vVertex2,
						   Moose::Math::CVector3<float> &vPointOfIntersection );


    MOOSE_API  float LineSegmentToLineSegmentDistanceSquared( const Moose::Math::CLineSegment & line0, const Moose::Math::CLineSegment & line1, float * pfS = NULL, float * pfT = NULL );

    MOOSE_API float LineToLineDistanceSquared( const Moose::Math::CLine & line0, const Moose::Math::CLine & line1, float * pfS = NULL, float * pfT = NULL );
    ////////////////////
    /// Returns the floating-point distance from point to a plane.
    /// \param vPoint Point of origin.
    /// \param plane Plane object.
    /// \returns Distance to plane - value 0.0f is in plane.
    MOOSE_API float PointDistanceFromPlane( const Moose::Math::CVector3<float> &vPoint,
					      const Moose::Math::CPlane &plane );

    ////////////////////
    /// Find closest point on ray to given point.
    /// \param vPoint Point in space.
    /// \param ray Ray which is compared to point.
    /// \param Closest position on ray to given point.
    MOOSE_API  void ClosestPointOnRay( const Moose::Math::CVector3<float> &vPoint,
					 const Moose::Math::CRay &ray,
					 Moose::Math::CVector3<float> & vClosestPoint);

    ////////////////////
    /// Find closest point on line to given point.
    /// \param vPoint Point in space.
    /// \param line Line which is compared to point.
    /// \param Closest position on line to given point.
    MOOSE_API void ClosestPointOnLine( const Moose::Math::CVector3<float> &vPoint,
					 const Moose::Math::CLine &line,
					 Moose::Math::CVector3<float> & vClosestPoint);


    ////////////////////
    /// Find closest point on line to given point.
    /// \param vPoint Point in space.
    /// \param line Line which is compared to point.
    /// \param vClosestPoint Closest position on line to given point.
    MOOSE_API void ClosestPointOnLineSegment( const Moose::Math::CVector3<float> &vPoint,
						const Moose::Math::CLineSegment & line,
						Moose::Math::CVector3<float> & vClosestPoint );



    ////////////////////
    /// Checks whether a plane and box intersect.
    /// \param plane A plane.
    /// \param box An oriented box.
    /// \returns Non-zero on intersect, zero otherwise.
    MOOSE_API int PlaneIntersectsBox( const Moose::Math::CPlane &plane,
					const Moose::Volume::COrientedBox &box );

    ////////////////////
    /// Checks whether a plane and box intersect.
    /// \param plane A plane.
    /// \param box An axis-aligned box.
    /// \returns Non-zero on intersect, zero otherwise.
    MOOSE_API int PlaneIntersectsBox( const Moose::Math::CPlane &plane,
					const Moose::Volume::CAxisAlignedBox &box );
    ////////////////////
    /// Checks whether a triangle and oriented box intersect.
    /// \param vVertex0 First corner of triangle.
    /// \param vVertex1 Second corner of triangle.
    /// \param vVertex2 Third corner of triangle.
    /// \param box Oriented box.
    /// \returns non-zero on intersect, zero otherwise.
    MOOSE_API int TriangleIntersectsOBB( Moose::Math::CVector3<float> vVertex0,
					   Moose::Math::CVector3<float> vVertex1,
					   Moose::Math::CVector3<float> vVertex2,
					   const Moose::Volume::COrientedBox &box );

    ////////////////////
    /// Checks whether point is inside oriented box.
    /// \param vPoint The point to be checked.
    /// \param obBox An oriented box.
    /// \returns non-zero on intersect, zero otherwise.
    MOOSE_API int   PointIntersectsOBB( const Moose::Math::CVector3<float> &vPoint,
					  const Moose::Volume::COrientedBox &obBox );

    ////////////////////
    /// Calculates collision point between three planes.
    /// \param plane1 First plane.
    /// \param plane2 Second plane.
    /// \param plane3 Third plane.
    /// \param vCollisionPoint Point of collision.
    /// \returns Non-zero if planes do not intersect in a single point. Value of vCollisionPoint is undetermined.
    /// \returns Zero if planes intersect in a single point. Value of vCollisionPoint is set to that point.
    MOOSE_API int  CollisionPoint3Planes( const Moose::Math::CPlane & plane1,
					    const Moose::Math::CPlane & plane2,
					    const Moose::Math::CPlane & plane3,
					    Moose::Math::CVector3<float> &vCollisionPoint);
    ////////////////////
    /// Checks does a sphere and a plane intersect.
    /// \param plane Plane object.
    /// \param sphere Sphere object.
    /// \param fCenterDistance Distance of sphere center into plane.
    /// \returns <0 If sphere is completely on negative half-side of plane.
    /// \returns 0 If sphere intersects plane
    /// \returns >1 If sphere is completely on positive half-side of plane.
    MOOSE_API int SphereIntersectsPlane( const Moose::Math::CPlane &plane,
					   const Moose::Volume::CSphere &sphere,
					   float &fCenterDistance);
    ////////////////////
    /// Checks does sphere intersect axis-aligned cube.
    /// \param sphere A sphere
    /// \param aaCube A cube
    /// \returns Zero if sphere and cube do not intersect.
    /// \returns Non-zero if sphere and cube intersect.
    MOOSE_API int SphereIntersectsAACube( const Moose::Volume::CSphere &sphere,
					    const Moose::Volume::CAxisAlignedCube &aaCube);

    ////////////////////
    /// Checks does sphere intersect axis-aligned bounding box.
    /// \param sphere A sphere
    /// \param aabb Axis-aligned bounding box
    /// \returns false if sphere and box do not intersect.
    /// \returns true if sphere and box intersect.
    MOOSE_API bool SphereIntersectsAABB( const Moose::Volume::CSphere &sphere,
					   const Moose::Volume::CAxisAlignedBox &aabb);

    ///////////////////
    /// Checks intersection between oriented box and sphere.
    /// \param sphere Sphere
    /// \param box OBB
    /// \return true if two objects intersect,
    ///	\return	false otherwise.
    MOOSE_API bool SphereIntersectsOBB( const Moose::Volume::CSphere &sphere, const Moose::Volume::COrientedBox & box );
    ////////////////////
    /// Checks do two spheres intersect.
    /// \param sphereOne First sphere
    /// \param sphereTwo Second sphere
    /// \returns Zero if spheres do not intersect.
    /// \returns Non-zero if spheres intersect.
    MOOSE_API int SphereIntersectsSphere( const Moose::Volume::CSphere &sphereOne,
					    const Moose::Volume::CSphere &sphereTwo );

    ////////////////////
    /// Checks does sphere intersect a capsule.
    /// \param sphere Sphere object.
    /// \param capsule Capsule object.
    /// \returns false on non-intersection.
    /// \returns true on intersection.
    MOOSE_API bool SphereIntersectsCapsule( const Moose::Volume::CSphere & sphere,
					      const Moose::Volume::CCapsule & capsule );
    ////////////////////
    /// Checks does oriented box intersect a capsule.
    /// \param box Oriented box object.
    /// \param capsule Capsule object.
    /// \returns false on non-intersection.
    /// \returns true on intersection.
    MOOSE_API bool OBBIntersectsCapsule( const Moose::Volume::COrientedBox & box,
                                         const Moose::Volume::CCapsule & capsule );
    ////////////////////
    /// Checks does a capsule intersect another capsule.
    /// \param a First capsule object.
    /// \param b Second capsule object.
    /// \returns false on non-intersection.
    /// \returns true on intersection.
    MOOSE_API bool CapsuleIntersectsCapsule( const Moose::Volume::CCapsule & a,
                                             const Moose::Volume::CCapsule & b );
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
    MOOSE_API  S2S_COLLISION_TYPE  SphereIntersectsSphere( const Moose::Volume::CSphere &sphereOne,
							     const Moose::Math::CVector3<float> &vVelocityOne,
							     const Moose::Volume::CSphere &sphereTwo,
							     const Moose::Math::CVector3<float> &vVelocityTwo,
							     Moose::Math::CVector3<float> & vIntersectionPoint,
							     float &fRelativeTime );

    ////////////////////
    /// Checks is point within sphere.
    /// \param sphere sphere to be checked.
    /// \param vPoint 3-Vector representing position in space.
    /// \returns non-zero on intersectsion.
    /// \returns zero on non-intersection.
    MOOSE_API int PointInsideSphere( const Moose::Volume::CSphere & sphere,
				       const Moose::Math::CVector3<float> & vPoint );


    ////////////////////
    /// Calculates which triangles are completely or partially inside decal volume.
    /// \param decalVolume Volume used in evaluation
    /// \param vertices VertexDescriptor of vertex data.
    /// \param indices Triangle indices
    /// \param lstTriangleFans List, where lists of vertex coordinates are stored for final mesh.
    MOOSE_API void CalculateDecalMesh( const Moose::Volume::CDecalVolume & decalVolume,
					 const Moose::Graphics::CVertexDescriptor & vertices,
					 const Moose::Graphics::CIndexArray & indices,
					 std::list< std::list< Moose::Math::CVector3<float> > > & lstTriangleFans );
    ////////////////////
    /// Calculates which triangles are completely or partially inside decal volume.
    /// \param decalVolume Volume used in evaluation
    /// \param vertices VertexDescriptor of vertex coordinate data.
    /// \param normals VertexDescriptor of vertex normal data.
    /// \param indices Triangle indices
    /// \param lstTriangleFans List, where lists of vertices are stored for final mesh.
    MOOSE_API void CalculateDecalMesh( const Moose::Volume::CDecalVolume & decalVolume,
					 const Moose::Graphics::CVertexDescriptor & vertices,
					 const Moose::Graphics::CVertexDescriptor & normals,
					 const Moose::Graphics::CIndexArray & indices,
					 std::list< std::list< Moose::Spatial::CVertex > > & lstTriangleFans );

    ////////////////////
    /// Clips given polygon with plane. New vertices replace previous ones.
    /// \param plane Clipping plane.
    /// \param lstVertices List of vertices (vector3s).
    MOOSE_API void ClipPolygon( const Moose::Math::CPlane & plane, std::list< Moose::Math::CVector3<float> > & lstVertices );

    ////////////////////
    /// Clips given polygon with plane. New vertices replace previous ones.
    /// \param plane Clipping plane.
    /// \param lstVertices List of vertices (CVertex).
    MOOSE_API void ClipPolygon( const Moose::Math::CPlane & plane, std::list< Moose::Spatial::CVertex > & lstVertices );

    ////////////////////
    /// Does sphere intersect a polytope.
    /// \param obBox Oriented bounding box to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    MOOSE_API VOLUME_INTERSECTION  OBBIntersectsPolytope( const Moose::Volume::COrientedBox & obBox, const Moose::Volume::CPolytope & poly );
    ////////////////////
    /// Does sphere intersect a polytope.
    /// \param sphere Sphere to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    MOOSE_API VOLUME_INTERSECTION  SphereIntersectsPolytope( const Moose::Volume::CSphere &sphere, const Moose::Volume::CPolytope & poly );
    ////////////////////
    /// Does AABB intersect a polytope.
    /// \param aabb Axisaligned box to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    MOOSE_API VOLUME_INTERSECTION  AABBIntersectsPolytope( const Moose::Volume::CAxisAlignedBox &aabb, const Moose::Volume::CPolytope & poly );
    ////////////////////
    /// Does AABB cube intersect a polytope.
    /// \param aabb AxisAlignedCube to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    MOOSE_API VOLUME_INTERSECTION  AABBIntersectsPolytope( const Moose::Volume::CAxisAlignedCube &aabb, const Moose::Volume::CPolytope & poly );
    ////////////////////
    /// Does capsuled intersect a polytope.
    /// \param capsule Capsule to be checked.
    /// \param poly Polytope to be checked against.
    /// \returns INTERSECTS if the intersect
    /// \return OUTSIDE if the do not intersect.
    MOOSE_API VOLUME_INTERSECTION  CapsuleIntersectsPolytope( const Moose::Volume::CCapsule & capsule, const Moose::Volume::CPolytope & poly );


    
    ////////////////////
    /// Checks intersection with ray and OBB.
    /// \param ray Ray object.
    /// \param obBox The oriented box which is checked
    /// \param pfValue optional pointer to float variable that receives distance from
    ///                ray origin to closest intersection point. By default, value is NULL.
    /// \returns false if ray does not intersect OBB. pfValue is undetermined.
    ///          true if ray intersects OBB. pfValue is set to closest distance.
    MOOSE_API bool RayIntersectsOBB( const Moose::Math::CRay &ray, const Moose::Volume::COrientedBox &obBox,  float *pfValue = NULL );

    ////////////////////
    /// Checks intersection with ray and AABB.
    /// \param ray Ray object.
    /// \param aaBox an axis-aligned box which is checked
    /// \param pfValue optional pointer to float variable that receives distance from
    ///                ray origin to closest intersection point. By default, value is NULL.
    /// \returns false if ray does not intersect AABB. pfValue is undetermined.
    ///          true if ray intersects AABB. pfValue is set to closest distance.
    MOOSE_API bool RayIntersectsAABB( const Moose::Math::CRay &ray, const Moose::Volume::CAxisAlignedBox &aaBox,  float *pfValue = NULL );
    ////////////////////
		/// Checks intersection with ray and AABB.
		/// \param ray Ray object.
 	  /// \param aaBox Axis-aligned cube which is checked
	  /// \param pfValue optional pointer to float variable that receives distance from
		///                ray origin to closest intersection point. By default, value is NULL.
		/// \returns false if ray does not intersect AABB. pfValue is undetermined.
		///          true if ray intersects AABB. pfValue is set to closest distance.
		MOOSE_API bool RayIntersectsAACube( const Moose::Math::CRay &ray, const Moose::Volume::CAxisAlignedCube &aaBox,  float *pfValue = NULL );
		////////////////////
    /// Checks intersection between sphere and cone.
    /// \param sphere Sphere object
    /// \param cone Cone object.
    /// \returns true if two objects intersect
    ///          false otherwise.
    MOOSE_API bool SphereIntersectsCone ( const Moose::Volume::CSphere &sphere, const Moose::Volume::CCone &cone );
    ///////////////////
    /// Checks intersection between oriented boxes.
    /// \param box1 First OBB
    /// \param box2 Second OBB
    /// \return true if two objects intersect,
    ///	\return	false otherwise.
    MOOSE_API bool OBBIntersectsOBB( const Moose::Volume::COrientedBox & box1, const Moose::Volume::COrientedBox & box2 );

    


    
    /*     int  SphereIntersectsAxisAlignedBox( const Moose::Volume::CSphere &sphere, const Moose::Volume::CAxisAlignedBox &aaBox); */
    /*     //char SphereIntersectsOrientedBox( const CSphere &sphere, const COrientedBox &obBox); */

    /*     //////////////////// */
    /*     /// The principal method for determining the sphere-polygon intersection. */
    /*     /// \param sphere Sphere which is tested. */
    /*     /// \param pVertices Vertices of the polygon which sphere is tested on. */
    /*     /// \param nNumVertices Number of vertices in polygon. */
    /*     /// \param vOffsetMovement Offset vector of collision. */
    /*     /// \returns true if intersection ,false otherwise. */
    /*     int  SphereIntersectsPolygon( Moose::Volume::CSphere &sphere,    */
    /* 				  float *pVertices,   */
    /* 				  unsigned int nNumVertices,  */
    /* 				  CVector3<float> &vOffsetMovement);   */

    /*     int  SphereIntersectsPolygonEdges( Moose::Volume::CSphere &sphere,  */
    /* 				       float * pVertices,  */
    /* 				       unsigned int nNumVertices);   */


    /*     int RayIntersectsOBB( const CVector3<float> &vRayStart,  */
    /* 			  const CVector3<float> &vRayDir, */
    /* 			  const Moose::Volume::COrientedBox &obBox, */
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

    /*     //INTERSECTION_TYPE  SphereIntersectsPolytone( const Moose::Volume::CSphere &Sphere, const Moose::Volume::CPolytope &kDop ); */
    /*     //INTERSECTION_TYPE  OrientedBoxIntersectsPolytope( const Moose::Volume::COrientedBox &obBox, const Moose::Volume::CPolytope &kDop ); */



    /*     //CVector3<float> ClosestPointOnLine( const CVector3<float> &vPoint, */
    /*     //const CVector3<float> &vLinePoint1, */
    /*     //const CVector3<float> &vLinePoint2 ); */






    /*     // Returns true if point is inside the volume limited by the planes in kDop. */
    /*     //  char	PointWithinKDOP( CVector3<float> &vPoint, Moose::Volume::CPolytope &kDop); */



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
    /* /\* 					   const Moose::Volume::CAxisAlignedBox &rAABB); *\/ */

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
} // namespace Moose
/////////////////////////////////////////////////////////////////
inline int
Moose::Collision::PointInsideSphere( const Moose::Volume::CSphere & sphere,
				       const Moose::Math::CVector3<float> & vPoint )
{
  return ( (sphere.GetPosition() - vPoint).LengthSqr() < sphere.GetRadiusSqr());
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////


