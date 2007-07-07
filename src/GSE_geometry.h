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
#ifndef __GSE_geometry_h__
#define __GSE_geometry_h__
/////////////////////////////////////////////////////////////////
#include "GSE_structures.h"
#include "GSE_Orientable.h"
#include "GSE_Dimensional.h"
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Geometry
{

  /////////////////////////////////////////////////////////////////
  // The plane class, always normalized. ( || Normal || == 1 )
  /////////////////////////////////////////////////////////////////
  class GSE_Plane : public GSE_Vector<float,4>
  {
  public:

    typedef enum {
      X = 0,
      Y = 1,
      Z = 2,
      D = 3
    } GSE_PlaneComponent;

    GSE_Plane();
    GSE_Plane( GSE_Vector3 vNormal, GSE_Vector3 vPoint);
    ~GSE_Plane();

    GSE_Plane &SetNormal(GSE_Vector3 vNormal);
    GSE_Plane &SetDistance( float fDistance );    

    void Normalize();
    inline float GetDistance() const 
    {
      return m_pValues[3];
    }
    inline float &Distance()
    {
      return m_pValues[3];
    }
    void Calculate( GSE_Vector3 vNormal, const GSE_Vector3 & vPoint );

    inline float &operator[](GSE_PlaneComponent comp){
      return m_pValues[comp];
    }
    friend std::ostream& operator<<( std::ostream &stream, GSE_Plane plane )
    {
      stream << "(" << plane.m_pValues[0] << "," << plane.m_pValues[1] << "," << plane.m_pValues[2] << "," << plane.m_pValues[3] ;
      return stream;
    }

  };

  /////////////////////////////////////////////////////////////////
  /// The class for axis-aligned (x,y,z) box.
  /////////////////////////////////////////////////////////////////
  class GSE_AxisAlignedBox : public GSE_Positional, public GSE_Dimensional
  {
  public:
    ////////////////////
    /// The constuctor.
    GSE_AxisAlignedBox()
    {
      
    }
    ////////////////////
    /// The parametrized constructor.
    GSE_AxisAlignedBox( const GSE_Vector3 &vCenter, float fWidth, float fHeight,  float fLength ) : 
      GSE_Positional( vCenter ), GSE_Dimensional( fWidth, fHeight, fLength)
    {
      
    }
  };
  /////////////////////////////////////////////////////////////////
  // The class for Oriented Box. Forward vector will be the principal axis,
  // right vector the second and up the third. Dimensions: length = forward,
  // width = right, height = up.
  /////////////////////////////////////////////////////////////////
  class GSE_OrientedBox : public GSE_AxisAlignedBox, public GSE_Orientable
  {
  protected:
    GSE_Plane m_Planes[6];    // planes oriented along the box walls
    float     m_aCorners[24]; // The corners of the box, 3 floats, 8 corners

  public:
    typedef enum 
    {
      TOP  = 0,
      BOTTOM = 1,
      LEFT = 2,
      RIGHT = 3,
      FRONT = 4,
      BACK = 5
    } GSE_BoxWallPlane;

    typedef enum 
    {
      TOP_LEFT_BACK = 0,
      TOP_LEFT_FRONT = 1,
      TOP_RIGHT_BACK = 2,
      TOP_RIGHT_FRONT = 3,
      BOTTOM_LEFT_BACK = 4,
      BOTTOM_LEFT_FRONT = 5,
      BOTTOM_RIGHT_BACK = 6,
      BOTTOM_RIGHT_FRONT = 7
    } BoxCorner_t;

    GSE_OrientedBox() : GSE_AxisAlignedBox()
    {
      memset(m_aCorners,0,sizeof(float)*24);
    }
    
    inline GSE_Plane &operator[](GSE_BoxWallPlane iPlane)
    {
      return m_Planes[iPlane];
    }
    inline void operator=( const GSE_AxisAlignedBox & box )
    {
      SetOrientation( GSE_Vector3::GetWorldY(),
		      GSE_Vector3::GetWorldZ(),
		      GSE_Vector3::GetWorldX());
      SetPosition( box.GetPosition());
      SetWidth( box.GetWidth());
      SetHeight( box.GetHeight());
      SetLength( box.GetLength());
      CalculateCorners();
      CalculatePlanes();
    }
    inline void operator=( const GSE_OrientedBox & box )
    {
      SetOrientation( box.GetUpVector(),
		      box.GetForwardVector(),
		      box.GetRightVector());
      SetPosition( box.GetPosition());
      SetWidth( box.GetWidth());
      SetHeight( box.GetHeight());
      SetLength( box.GetLength());
      CalculateCorners();
      CalculatePlanes();
    }
    GSE_OrientedBox &SetOrientation(const GSE_Vector3 &vUp, 
				    const GSE_Vector3 &vForward,
				    const GSE_Vector3 &vRight);

    const float * GetCorners() const;
    void CalculateCorners();
    void CalculatePlanes();

    VertexDescriptor GetCorner( BoxCorner_t corner );
    
    friend std::ostream& operator<<( std::ostream &stream, GSE_OrientedBox box )
    {
      stream << endl
	     << "R = " << box.GetForwardVector() << ", scale " << box.GetLength() << endl
 	     << "S = " << box.GetRightVector()   << ", scale " << box.GetWidth() << endl
	     << "T = " << box.GetUpVector()      << ", scale " << box.GetHeight() << endl
	     << "C = " << box.GetPosition() << endl;
      stream << "TLF:" << GSE_Vector3(box.GetCorner(TOP_LEFT_FRONT)) << endl;
      stream << "TLB:" << GSE_Vector3(box.GetCorner(TOP_LEFT_BACK)) << endl;
      stream << "TRF:" << GSE_Vector3(box.GetCorner(TOP_RIGHT_FRONT)) << endl;
      stream << "TRB:" << GSE_Vector3(box.GetCorner(TOP_RIGHT_BACK)) << endl;
      stream << "BLF:" << GSE_Vector3(box.GetCorner(BOTTOM_LEFT_FRONT)) << endl;
      stream << "BLB:" << GSE_Vector3(box.GetCorner(BOTTOM_LEFT_BACK)) << endl;
      stream << "BRF:" << GSE_Vector3(box.GetCorner(BOTTOM_RIGHT_FRONT)) << endl;
      stream << "BRB:" << GSE_Vector3(box.GetCorner(BOTTOM_RIGHT_BACK)) << endl;
      return stream;
    }
    
  };
  /////////////////////////////////////////////////////////////////
  /// The class for generic sphere with center and radius.
  /////////////////////////////////////////////////////////////////
  class GSE_Sphere : public GSE_Positional
  {
  protected:
    /// Sphere radius.
    float m_fRadius;
    /// Squared radius.
    float m_fRadiusSqr;
  public:
    /////////////////////////////////////////////////////////////////
    /// The default constructor.
    GSE_Sphere() : GSE_Positional()
    {
      m_fRadius    = 0.0f;
      m_fRadiusSqr = 0.0f;
    }
    /////////////////////////////////////////////////////////////////
    /// The parametrized constructor.
    GSE_Sphere( GSE_Vector3 vPosition, float fRadius ) : GSE_Positional( vPosition )
    {
      m_fRadius = fRadius;
      m_fRadiusSqr = m_fRadius * m_fRadius;
    }
    /////////////////////////////////////////////////////////////////
    /// Assingment operator.
    /// \param ref sphere to be assigned.
    inline GSE_Sphere & operator=(GSE_Sphere ref)
    {
      SetPosition(ref.GetPosition());
      SetRadius( ref.m_fRadius );
      return *this;
    }
    /////////////////////////////////////////////////////////////////
    /// Sets the radius.
    /// \param fRadius the radius value.
    inline void SetRadius(float fRadius)
    {
      m_fRadius = fRadius;
      m_fRadiusSqr = m_fRadius * m_fRadius;
    }
    /////////////////////////////////////////////////////////////////
    /// Return the radius.
    /// \return float radius.
    inline float GetRadius() const
    {
      return m_fRadius;
    }
    /////////////////////////////////////////////////////////////////
    /// Returns the squared radius. Useful in some situations.
    /// \returns squared radius.
    inline float GetRadiusSqr() const
    {
      return m_fRadiusSqr;
    }
    /////////////////////////////////////////////////////////////////
    /// For debugging.
    friend std::ostream& operator<<( std::ostream &stream, GSE_Sphere sphere )
    {
      stream << "C = " << sphere.GetPosition() << ", radius " << sphere.GetRadius(); 
      
      return stream;
    }
    
  }; // class GSE_Sphere
  /////////////////////////////////////////////////////////////////
  // The class for a Quad
  /////////////////////////////////////////////////////////////////
  class GSE_Quad : public GSE_Positional 
  {
  protected:
    // The width of the quad
    float m_fWidth;
    // The height of the quad
    float m_fHeight;
    float m_fHalfWidth;
    float m_fHalfHeight;
    
  public:
    // The constructor

    GSE_Quad() : GSE_Positional()
    {
      m_fWidth = 0.0f;
      m_fHeight = 0.0f;
    }

    void SetWidth( float fWidth )
    {
      m_fWidth = fWidth;
      m_fHalfWidth = m_fWidth * 0.5;
    }
    void SetHeight( float fHeight )
    {
      m_fHeight = fHeight;
      m_fHalfHeight = m_fHeight * 0.5;
    }
    float GetWidth()
    {
      return m_fWidth;
    }
    float GetHeight()
    {
      return m_fHeight;
    }
    float GetHalfWidth()
    {
      return m_fHalfWidth;
    }
    float GetHalfHeight()
    {
      return m_fHalfHeight;
    }
    
  }; // class GSE_Quad
  /////////////////////////////////////////////////////////////////
  // The class for a Cone
  /////////////////////////////////////////////////////////////////
  class GSE_Cone : public GSE_Positional 
  {
  protected:

    // *******************************************
    // *  direction
    // *  \   ^   /
    // *   \a | a/   a= angle
    // *    \^|^/
    // *     \|/
    // *   position
    // *******************************************

    // The angle of the cone, in radians.
    float m_fAngle;
    // The direction of the cone
    GSE_Vector3 m_vDirection;
    // The length of the cone
    float m_fLength;
    // 1 / sin(a); speeds up the calculations
    float m_fSinReciprocal;
    float m_fSinAngle; // sin (angle)
    float m_fCosAngle; // cos ( angle )
    float m_fCosAngleSqr; // (cos(angle))²
    float m_fSinAngleSqr; // (sin(angle))²

  public:
    GSE_Cone() : GSE_Positional(), m_vDirection(-GSE_Vector3::GetWorldZ())
    {
      SetAngle(0.0f);
      m_fLength = 1.0f;
    }
    // The constructor
    GSE_Cone( const GSE_Vector3 &vPos, const GSE_Vector3 &vDir, float fAngle, float fLength ) : GSE_Positional(vPos),m_vDirection(vDir)
    {
      SetAngle(fAngle);
      m_fLength = fLength;
    }
    // Assigns the angle of the cone
    void SetAngle( float fAngle )
    {

      m_fAngle = fAngle;
      m_fSinAngle = sinf(m_fAngle);
      m_fCosAngle = cosf(m_fAngle);
      m_fSinReciprocal = (1.0f / m_fSinAngle);
      m_fCosAngleSqr = m_fCosAngle * m_fCosAngle;
      m_fSinAngleSqr = m_fSinAngle * m_fSinAngle;

    }
    // Assigns the direction of the cone to vDir
    void SetDirection( GSE_Vector3 vDir )
    {
      m_vDirection = vDir.GetUnit();
    }
    // Returns the direction which the Cone is pointing at
    GSE_Vector3 GetDirection()  const
    {
      return m_vDirection;
    }
    // Returns the angle of the cone, in degrees
    float GetAngle() const
    {
      return m_fAngle;
    }
    // Returns the 1 / sin(angle);
    float SinReciprocal() const
    {
      return m_fSinReciprocal;
    }
    float SinSqr() const
    {
      return m_fSinAngleSqr;
    }
    float CosSqr() const
    {
      return m_fCosAngleSqr;
    }
    float CosAngle() const
    {
      return m_fCosAngle;
    }
    float SinAngle() const
    {
      return m_fSinAngle;
    }
    void SetLength(float fLength)
    {
      m_fLength = fLength;
    }
    float GetLength() const
    {
      return m_fLength;
    }
  }; // class GSE_Cone

  /////////////////////////////////////////////////////////////////
  // A class for discrete oriented polytope, defines a convex bounding 
  // volume by set of planes.
  /////////////////////////////////////////////////////////////////
  class GSE_KDOP
  {
  protected:
    std::list<GSE_Plane> m_lstPlanes;
  public:
    /////////////////////////////////////////////////////////////////
    GSE_KDOP()
    {
      
    }
    /////////////////////////////////////////////////////////////////
    ~GSE_KDOP()
    {
      m_lstPlanes.clear();
    }
    /////////////////////////////////////////////////////////////////
    void AddPlane( GSE_Plane &Plane )
    {
      m_lstPlanes.push_back(Plane);
    }
    /////////////////////////////////////////////////////////////////
    std::list<GSE_Plane> &Planes()
    {
      return m_lstPlanes;
    }
    /////////////////////////////////////////////////////////////////
    unsigned int GetNumPlanes()
    {
      return m_lstPlanes.size();
    }
    friend std::ostream& operator<<( std::ostream &stream, GSE_KDOP kDop )
    {
      std::list<GSE_Plane>::iterator it = kDop.m_lstPlanes.begin();
      stream << "KDOP planes:" << endl;
      for( ; it != kDop.m_lstPlanes.end(); it++)
      {
	stream << *it << endl;
      }
      return stream;
    }
  }; // GSE_KDOP
  /////////////////////////////////////////////////////////////////
    // Generic intersection enum
  typedef enum 
  {
    INTERSECTION = 0,
    INSIDE = 1,
    OUTSIDE = 2
  } GSE_IntersectionType;
    typedef enum {
    INTERSECTS,
    FRONT,
    BEHIND,
  } GSE_PlaneIntersection;
  // Calculates the oriented bounding box for vertices in vertexarray
  GSE_OrientedBox CalculateOrientedBoundingBox( VertexDescriptor pVertices, 
						unsigned int nNumVertices );
  
  // Calculates the axis-aligned bounding box for vertices in vertexarray
  GSE_AxisAlignedBox CalculateAlignedBox( VertexDescriptor pVertices, int nNumVertices ) ;
  
  // Calculates the bounding sphere for vertices in VertexDescriptor using fast
  // but not so accurate algorithm.
  GSE_Sphere CalculateBoundingSphere( VertexDescriptor pVertices, 
				      unsigned int nNumVertices );
  
  // Calculates the bounding sphere for vertices in VertexDescriptor using slightly 
  // slower, but more accurate algorithm
  GSE_Sphere CalculateBoundingSphereTight( VertexDescriptor pVertices, 
					   unsigned int nNumVertices );
  // Calculates the bounding sphere for vertices in VertexDescriptor by given set of indices
  // using slightly slower, but more accurate algorithm
  GSE_Sphere CalculateBoundingSphereTight( VertexDescriptor pVertices, 
					   const GSE_IndexBuffer &elementList );
  // Calculates the oriented bounding box for vertices in vertexarray using 
  // the given set of indices.
  GSE_OrientedBox CalculateOrientedBoundingBox( VertexDescriptor pVertices,
					       const GSE_IndexBuffer &elementList );
  // The intersection test functions
  char SphereIntersectsCone ( const GSE_Sphere &sphere, const GSE_Cone &cone );
  char SphereIntersectsSphere( const GSE_Sphere &sphereOne, const GSE_Sphere &sphereTwo);

  int SphereIntersectsAxisAlignedBox( const GSE_Sphere &sphere, const GSE_AxisAlignedBox &aaBox);
  //char SphereIntersectsOrientedBox( const GSE_Sphere &sphere, const GSE_OrientedBox &obBox);
  
  // Creates a new sphere is the minimum sphere containing two spheres given as arguments.
  GSE_Sphere MergeSpheres( const GSE_Sphere &one, const GSE_Sphere &two);
  // Creates a new Oriented Box containing two oriented boxes
  GSE_OrientedBox MergeOrientedBoxes( const GSE_OrientedBox &obOne, const GSE_OrientedBox &obTwo );
  //
  // The principal method for determining the sphere-polygon intersection 
  //
  char  SphereIntersectsPolygon( GSE_Sphere &sphere,   // The sphere which is tested
				 VertexDescriptor pVertices,  // The vertices of the polygon which sphere is tested on
				 unsigned int nNumVertices, // The number of vertices in the polygon
				 GSE_Vector3 &vOffsetMovement);  // The offset vector of collision

  char  SphereIntersectsPolygonEdges( GSE_Sphere &sphere, 
				      VertexDescriptor pVertices, 
				      unsigned int nNumVertices);  
  
  float AngleBetweenVectors( GSE_Vector3 &vVect1, GSE_Vector3 &vVect2);

  GSE_PlaneIntersection	LineIntersectsPlane( const GSE_Plane &plane, 
					     const GSE_Vector3 &vPoint1, 
					     const GSE_Vector3 &vPoint2);
  
  GSE_Vector3 LinePlaneIntersectionPoint( const GSE_Plane &plane, 
					  const GSE_Vector3 &vPoint1, 
					  const GSE_Vector3 &vPoint2);

  GSE_Vector3 ClosestPointOnLine( const GSE_Vector3 &vPoint,
				  const GSE_Vector3 &vLinePoint1,
				  const GSE_Vector3 &vLinePoint2 );
  /// The closest point on ray to given point.
  /// vDirection must be unit vector.
  GSE_Vector3 ClosestPointOnRay( const GSE_Vector3 &vPoint,
				 const GSE_Vector3 &vStartPoint,
				 const GSE_Vector3 &vDirection );
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
  int RayIntersectsSphere( GSE_Vector3 &vRayStart,  GSE_Vector3 &vRayEnd,
			   GSE_Vector3 *pvIntersection0,
			   GSE_Vector3 *pvIntersection1,
			   GSE_Sphere &Sphere);

  /// \brief Checks intersection with ray and OBB:
  /// \param vRayDir The direction of the line. || vRayDir || has to be 1.
  /// \param vRayStart A point of ray origin.
  /// \param obBox The oriented box which is checked
  /// \param pfValue pointer to float variable receives distance from 
  ///                ray origin to closest intersection point.
  /// \returns 0 if ray does not intersect OBB. pfValue is undetermined.
  ///          1 if ray intersects OBB. pfValue is set to closest distance.
  int RayIntersectsOBB( const GSE_Vector3 &vRayStart, 
			const GSE_Vector3 &vRayDir,
			const GSE_OrientedBox &obBox,
			float *pfValue);

  char InsidePolygon( GSE_Vector3 &vPoint, 
		      VertexDescriptor pVertices, 
		      unsigned int nNumVertices );
  // Returns the floating-point distance from point to a plane
  float PointDistanceFromPlane( GSE_Vector3 &vPoint, GSE_Plane plane );
  
  int   PointIntersectsOBB( const GSE_OrientedBox &obBox, const GSE_Vector3 &vPoint );
  // Returns true if point is inside the volume limited by the planes in kDop.
  char	PointWithinKDOP( GSE_Vector3 &vPoint, GSE_KDOP &kDop);
  GSE_IntersectionType  SphereIntersectsKDOP( GSE_Sphere &Sphere, GSE_KDOP &kDop );
  GSE_IntersectionType  OrientedBoxIntersectsKDOP( GSE_OrientedBox &obBox, GSE_KDOP &kDop );

  /////////////////////////////////////////////////////////////////
  // Constructs a 4x4 rotation matrix by inserting the Oriented Box axis 
  // as column vectors of the it.
  //
  //     Fx Rx Ux 0
  // M = Fy Ry Uy 0 ,  F = Forward Vector
  //     Fz Rz Uz 0    R = Right Vector
  //     0  0  0  1    U = Up Vector
  /////////////////////////////////////////////////////////////////
  GSE_Matrix4x4f OrientedBoxAxisToRotationMatrix(  const GSE_OrientedBox &ob  );
  



  
  GSE_PlaneIntersection SphereIntersectsPlane( GSE_Plane &plane,
						 const GSE_Sphere &sphere,
						 float &fCenterDistance);

  typedef enum {
    POINT,
    LINE,
    NO_INTERSECTION
  } GSE_PlaneIntersectionType;
  /////////////////////////////////////////////////////////////////
  // Calculates the point of intersection between three planes.
  // Returns:	POINT when planes interect in explicit point, 
  //		LINE when planes intersect along a line,
  //		NO_INTERSECTION when no intersection occurs
  // Then return value is POINT, the Vpoint contains the coordinates.
  /////////////////////////////////////////////////////////////////
  GSE_PlaneIntersectionType PlaneIntersectionPoint( GSE_Plane &Plane1, 
						    GSE_Plane &Plane2,
						    GSE_Plane &Plane3,
						    GSE_Vector3 &vPoint );
  
  /////////////////////////////////////////////////////////////////
  /// Determines whether a plane and axis-aligned box intersect.
  /// \param rPlane The plane which is tested.
  /// \param rAABB The axis-aligned box which is tested.
  /// \returns INTERSECTS if the plane and box intersect,
  ///  FRONT if the box is on the side where plane normal points to,
  ///  BEHIND if the box is on the opposite side where plane normal points to.
  GSE_PlaneIntersection PlaneIntersectsAABB( const GSE_Plane &rPlane,  
					     const GSE_AxisAlignedBox &rAABB);
  
  
  
  /////////////////////////////////////////////////////////////////
  // 
  // The billboarding utils which can be used to align polygons to face the 
  // camera.
  //
  /////////////////////////////////////////////////////////////////
  class GSE_Camera;
  namespace Billboard
  {
    
    void AlignToCameraPlane( GSE_Vector3 *vX, 
			     GSE_Vector3 *vY, 
			     GSE_Camera *pCamera,
			     GSE_Vector3 *vQuadPos,
			     float fQuadWidthDiv2,
			     float fQuadHeightDiv2 );
    

  }
  
  
  
}

#endif
/////////////////////////////////////////////////////////////////
// $Id: GSE_geometry.h,v 1.25 2007/05/31 14:00:45 entity Exp $
// desc : Geometry utility classes.
// author : AG/eNtity
// history : 
//		$Log: GSE_geometry.h,v $
//		Revision 1.25  2007/05/31 14:00:45  entity
//		missing comments added.
//
//		Revision 1.24  2007/05/20 15:12:16  entity
//		closest point on ray added
//
//		Revision 1.23  2007/05/19 11:29:02  entity
//		const stuff
//
//		Revision 1.22  2007/05/18 08:08:09  entity
//		Vector3 constructor check & fixes
//
//		Revision 1.21  2007/05/10 21:02:32  entity
//		PointIntersectsOBB
//
//		Revision 1.20  2007/05/10 19:12:18  entity
//		RayIntersectsOBB added
//
//		Revision 1.19  2007/05/10 12:28:26  entity
//		comments added
//
//		Revision 1.18  2007/04/03 12:52:32  entity
//		orientedbox is now inherited from axisalignedbox
//
//		Revision 1.17  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.16  2007/03/31 15:56:35  entity
//		ElementList->IndexBuffer rename
//
//		Revision 1.15  2007/03/29 13:12:00  entity
//		dimensional AxisAlignedBox
//
//		Revision 1.14  2007/03/29 06:54:37  entity
//		added param constructo for AxisAlignedBox
//
//		Revision 1.13  2007/03/28 10:36:08  entity
//		plane w->d change
//
//		Revision 1.12  2007/03/28 08:27:21  entity
//		PlaneIntersectsAABB added,
//		some type renaming
//
//		Revision 1.11  2007/03/21 11:48:09  entity
//		compiles now
//
//		Revision 1.10  2007/03/21 11:11:41  entity
//		CalculateBoundingSphereTight() and CalculateOrientedBoundingBox()
//		with vertex array and indices
//
//		Revision 1.9  2007/03/21 08:22:15  entity
//		const & stuff added
//
//		Revision 1.8  2007/03/20 20:11:43  entity
//		removed initial* stuff
//
//		Revision 1.7  2007/03/20 17:41:01  entity
//		Code cleanups and initial position added for GSE_Sphere
//
//		Revision 1.6  2007/03/20 13:26:55  entity
//		const & for plane Calculate(...)
//		initial directions for oriented box
//
//		Revision 1.5  2007/02/27 14:04:40  entity
//		RayIntersectsSphere fixes + added assignment operator for Sphere
//
//		Revision 1.4  2007/02/26 14:03:03  entity
//		added RayIntersectsSphere
//
//		Revision 1.3  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.2  2006/10/16 07:10:48  entity
//		Created new GSE_Vector template and corresponding changes
//
/////////////////////////////////////////////////////////////////
