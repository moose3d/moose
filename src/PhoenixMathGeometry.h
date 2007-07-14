#ifndef __PhoenixMathGeometry_h__
#define __PhoenixMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector4.h"
#include <list>
#include <iostream>
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    // The plane class, always normalized. ( || Normal || == 1 )
    /////////////////////////////////////////////////////////////////
    class CPlane : public CVector4<float>
    {
    public:

      CPlane();
      CPlane( CVector3<float> vNormal, CVector3<float> vPoint);
      ~CPlane();

      CPlane &SetNormal(CVector3<float> vNormal);
      CPlane &SetDistance( float fDistance );    

      void Normalize();
      inline float GetDistance() const 
      {
	return m_pValues[3];
      }
      inline float &Distance()
      {
	return m_pValues[3];
      }
      void Calculate( CVector3<float> vNormal, const CVector3<float> & vPoint );

      inline float &operator[](CPlaneComponent comp){
	return m_pValues[comp];
      }
      friend std::ostream& operator<<( std::ostream &stream, CPlane plane )
      {
	stream << "(" << plane.m_pValues[0] << "," << plane.m_pValues[1] << "," << plane.m_pValues[2] << "," << plane.m_pValues[3] ;
	return stream;
      }

    };

    /////////////////////////////////////////////////////////////////
    /// The class for axis-aligned (x,y,z) box.
    /////////////////////////////////////////////////////////////////
    class CAxisAlignedBox : public CPositional, public CDimensional
    {
    public:
      ////////////////////
      /// The constuctor.
      CAxisAlignedBox()
      {
      
      }
      ////////////////////
      /// The parametrized constructor.
      CAxisAlignedBox( const CVector3<float> &vCenter, float fWidth, float fHeight,  float fLength ) : 
      CPositional( vCenter ), CDimensional( fWidth, fHeight, fLength)
      {
      
      }
    };
    /////////////////////////////////////////////////////////////////
    // The class for Oriented Box. Forward vector will be the principal axis,
    // right vector the second and up the third. Dimensions: length = forward,
    // width = right, height = up.
    /////////////////////////////////////////////////////////////////
    class COrientedBox : public CAxisAlignedBox, public COrientable
    {
    protected:
      CPlane m_Planes[6];    // planes oriented along the box walls
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
      } CBoxWallPlane;

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

      COrientedBox() : CAxisAlignedBox()
      {
	memset(m_aCorners,0,sizeof(float)*24);
      }
    
      inline CPlane &operator[](CBoxWallPlane iPlane)
      {
	return m_Planes[iPlane];
      }
      inline void operator=( const CAxisAlignedBox & box )
      {
	SetOrientation( CVector3<float>::GetWorldY(),
			CVector3<float>::GetWorldZ(),
			CVector3<float>::GetWorldX());
	SetPosition( box.GetPosition());
	SetWidth( box.GetWidth());
	SetHeight( box.GetHeight());
	SetLength( box.GetLength());
	CalculateCorners();
	CalculatePlanes();
      }
      inline void operator=( const COrientedBox & box )
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
      COrientedBox &SetOrientation(const CVector3<float> &vUp, 
				      const CVector3<float> &vForward,
				      const CVector3<float> &vRight);

      const float * GetCorners() const;
      void CalculateCorners();
      void CalculatePlanes();

      VertexDescriptor GetCorner( BoxCorner_t corner );
    
      friend std::ostream& operator<<( std::ostream &stream, COrientedBox box )
      {
	stream << endl
	       << "R = " << box.GetForwardVector() << ", scale " << box.GetLength() << endl
	       << "S = " << box.GetRightVector()   << ", scale " << box.GetWidth() << endl
	       << "T = " << box.GetUpVector()      << ", scale " << box.GetHeight() << endl
	       << "C = " << box.GetPosition() << endl;
	stream << "TLF:" << CVector3<float>(box.GetCorner(TOP_LEFT_FRONT)) << endl;
	stream << "TLB:" << CVector3<float>(box.GetCorner(TOP_LEFT_BACK)) << endl;
	stream << "TRF:" << CVector3<float>(box.GetCorner(TOP_RIGHT_FRONT)) << endl;
	stream << "TRB:" << CVector3<float>(box.GetCorner(TOP_RIGHT_BACK)) << endl;
	stream << "BLF:" << CVector3<float>(box.GetCorner(BOTTOM_LEFT_FRONT)) << endl;
	stream << "BLB:" << CVector3<float>(box.GetCorner(BOTTOM_LEFT_BACK)) << endl;
	stream << "BRF:" << CVector3<float>(box.GetCorner(BOTTOM_RIGHT_FRONT)) << endl;
	stream << "BRB:" << CVector3<float>(box.GetCorner(BOTTOM_RIGHT_BACK)) << endl;
	return stream;
      }
    
    };
    /////////////////////////////////////////////////////////////////
    /// The class for generic sphere with center and radius.
    /////////////////////////////////////////////////////////////////
    class CSphere : public CPositional
    {
    protected:
      /// Sphere radius.
      float m_fRadius;
      /// Squared radius.
      float m_fRadiusSqr;
    public:
      /////////////////////////////////////////////////////////////////
      /// The default constructor.
      CSphere() : CPositional()
      {
	m_fRadius    = 0.0f;
	m_fRadiusSqr = 0.0f;
      }
      /////////////////////////////////////////////////////////////////
      /// The parametrized constructor.
      CSphere( CVector3<float> vPosition, float fRadius ) : CPositional( vPosition )
      {
	m_fRadius = fRadius;
	m_fRadiusSqr = m_fRadius * m_fRadius;
      }
      /////////////////////////////////////////////////////////////////
      /// Assingment operator.
      /// \param ref sphere to be assigned.
      inline CSphere & operator=(CSphere ref)
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
      friend std::ostream& operator<<( std::ostream &stream, CSphere sphere )
      {
	stream << "C = " << sphere.GetPosition() << ", radius " << sphere.GetRadius(); 
      
	return stream;
      }
    
    }; // class CSphere
    /////////////////////////////////////////////////////////////////
    // The class for a Quad
    /////////////////////////////////////////////////////////////////
    class CQuad : public CPositional 
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

      CQuad() : CPositional()
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
    
    }; // class CQuad
    /////////////////////////////////////////////////////////////////
    // The class for a Cone
    /////////////////////////////////////////////////////////////////
    class CCone : public CPositional 
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
      CVector3<float> m_vDirection;
      // The length of the cone
      float m_fLength;
      // 1 / sin(a); speeds up the calculations
      float m_fSinReciprocal;
      float m_fSinAngle; // sin (angle)
      float m_fCosAngle; // cos ( angle )
      float m_fCosAngleSqr; // (cos(angle))²
      float m_fSinAngleSqr; // (sin(angle))²

    public:
      CCone() : CPositional(), m_vDirection(-CVector3<float>::GetWorldZ())
      {
	SetAngle(0.0f);
	m_fLength = 1.0f;
      }
      // The constructor
      CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength ) : CPositional(vPos),m_vDirection(vDir)
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
      void SetDirection( CVector3<float> vDir )
      {
	m_vDirection = vDir.GetUnit();
      }
      // Returns the direction which the Cone is pointing at
      CVector3<float> GetDirection()  const
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
    }; // class CCone

    /////////////////////////////////////////////////////////////////
    // A class for discrete oriented polytope, defines a convex bounding 
    // volume by set of planes.
    /////////////////////////////////////////////////////////////////
    class CKDOP
    {
    protected:
      std::list<CPlane> m_lstPlanes;
    public:
      /////////////////////////////////////////////////////////////////
      CKDOP()
      {
      
      }
      /////////////////////////////////////////////////////////////////
      ~CKDOP()
      {
	m_lstPlanes.clear();
      }
      /////////////////////////////////////////////////////////////////
      void AddPlane( CPlane &Plane )
      {
	m_lstPlanes.push_back(Plane);
      }
      /////////////////////////////////////////////////////////////////
      std::list<CPlane> &Planes()
      {
	return m_lstPlanes;
      }
      /////////////////////////////////////////////////////////////////
      unsigned int GetNumPlanes()
      {
	return m_lstPlanes.size();
      }
      friend std::ostream& operator<<( std::ostream &stream, CKDOP kDop )
      {
	std::list<CPlane>::iterator it = kDop.m_lstPlanes.begin();
	stream << "KDOP planes:" << endl;
	for( ; it != kDop.m_lstPlanes.end(); it++)
	{
	  stream << *it << endl;
	}
	return stream;
      }
    }; // CKDOP
    /////////////////////////////////////////////////////////////////
    // Generic intersection enum
    typedef enum 
    {
      INTERSECTION = 0,
      INSIDE = 1,
      OUTSIDE = 2
    } CIntersectionType;
    typedef enum {
      INTERSECTS,
      FRONT,
      BEHIND,
    } CPlaneIntersection;
    // Calculates the oriented bounding box for vertices in vertexarray
    COrientedBox CalculateOrientedBoundingBox( VertexDescriptor pVertices, 
						  unsigned int nNumVertices );
  
    // Calculates the axis-aligned bounding box for vertices in vertexarray
    CAxisAlignedBox CalculateAlignedBox( VertexDescriptor pVertices, int nNumVertices ) ;
  
    // Calculates the bounding sphere for vertices in VertexDescriptor using fast
    // but not so accurate algorithm.
    CSphere CalculateBoundingSphere( VertexDescriptor pVertices, 
					unsigned int nNumVertices );
  
    // Calculates the bounding sphere for vertices in VertexDescriptor using slightly 
    // slower, but more accurate algorithm
    CSphere CalculateBoundingSphereTight( VertexDescriptor pVertices, 
					     unsigned int nNumVertices );
    // Calculates the bounding sphere for vertices in VertexDescriptor by given set of indices
    // using slightly slower, but more accurate algorithm
    CSphere CalculateBoundingSphereTight( VertexDescriptor pVertices, 
					     const CIndexBuffer &elementList );
    // Calculates the oriented bounding box for vertices in vertexarray using 
    // the given set of indices.
    COrientedBox CalculateOrientedBoundingBox( VertexDescriptor pVertices,
						  const CIndexBuffer &elementList );
    // The intersection test functions
    char SphereIntersectsCone ( const CSphere &sphere, const CCone &cone );
    char SphereIntersectsSphere( const CSphere &sphereOne, const CSphere &sphereTwo);

    int SphereIntersectsAxisAlignedBox( const CSphere &sphere, const CAxisAlignedBox &aaBox);
    //char SphereIntersectsOrientedBox( const CSphere &sphere, const COrientedBox &obBox);
  
    // Creates a new sphere is the minimum sphere containing two spheres given as arguments.
    CSphere MergeSpheres( const CSphere &one, const CSphere &two);
    // Creates a new Oriented Box containing two oriented boxes
    COrientedBox MergeOrientedBoxes( const COrientedBox &obOne, const COrientedBox &obTwo );
    //
    // The principal method for determining the sphere-polygon intersection 
    //
    char  SphereIntersectsPolygon( CSphere &sphere,   // The sphere which is tested
				   VertexDescriptor pVertices,  // The vertices of the polygon which sphere is tested on
				   unsigned int nNumVertices, // The number of vertices in the polygon
				   CVector3<float> &vOffsetMovement);  // The offset vector of collision

    char  SphereIntersectsPolygonEdges( CSphere &sphere, 
					VertexDescriptor pVertices, 
					unsigned int nNumVertices);  
  
    float AngleBetweenVectors( CVector3<float> &vVect1, CVector3<float> &vVect2);

    CPlaneIntersection	LineIntersectsPlane( const CPlane &plane, 
						     const CVector3<float> &vPoint1, 
						     const CVector3<float> &vPoint2);
  
    CVector3<float> LinePlaneIntersectionPoint( const CPlane &plane, 
					    const CVector3<float> &vPoint1, 
					    const CVector3<float> &vPoint2);

    CVector3<float> ClosestPointOnLine( const CVector3<float> &vPoint,
				    const CVector3<float> &vLinePoint1,
				    const CVector3<float> &vLinePoint2 );
    /// The closest point on ray to given point.
    /// vDirection must be unit vector.
    CVector3<float> ClosestPointOnRay( const CVector3<float> &vPoint,
				   const CVector3<float> &vStartPoint,
				   const CVector3<float> &vDirection );
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
    int RayIntersectsSphere( CVector3<float> &vRayStart,  CVector3<float> &vRayEnd,
			     CVector3<float> *pvIntersection0,
			     CVector3<float> *pvIntersection1,
			     CSphere &Sphere);

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
			  const COrientedBox &obBox,
			  float *pfValue);

    char InsidePolygon( CVector3<float> &vPoint, 
			VertexDescriptor pVertices, 
			unsigned int nNumVertices );
    // Returns the floating-point distance from point to a plane
    float PointDistanceFromPlane( CVector3<float> &vPoint, CPlane plane );
  
    int   PointIntersectsOBB( const COrientedBox &obBox, const CVector3<float> &vPoint );
    // Returns true if point is inside the volume limited by the planes in kDop.
    char	PointWithinKDOP( CVector3<float> &vPoint, CKDOP &kDop);
    CIntersectionType  SphereIntersectsKDOP( CSphere &Sphere, CKDOP &kDop );
    CIntersectionType  OrientedBoxIntersectsKDOP( COrientedBox &obBox, CKDOP &kDop );

    /////////////////////////////////////////////////////////////////
    // Constructs a 4x4 rotation matrix by inserting the Oriented Box axis 
    // as column vectors of the it.
    //
    //     Fx Rx Ux 0
    // M = Fy Ry Uy 0 ,  F = Forward Vector
    //     Fz Rz Uz 0    R = Right Vector
    //     0  0  0  1    U = Up Vector
    /////////////////////////////////////////////////////////////////
    CMatrix4x4f OrientedBoxAxisToRotationMatrix(  const COrientedBox &ob  );
  



  
    CPlaneIntersection SphereIntersectsPlane( CPlane &plane,
						 const CSphere &sphere,
						 float &fCenterDistance);

    typedef enum {
      POINT,
      LINE,
      NO_INTERSECTION
    } CPlaneIntersectionType;
    /////////////////////////////////////////////////////////////////
    // Calculates the point of intersection between three planes.
    // Returns:	POINT when planes interect in explicit point, 
    //		LINE when planes intersect along a line,
    //		NO_INTERSECTION when no intersection occurs
    // Then return value is POINT, the Vpoint contains the coordinates.
    /////////////////////////////////////////////////////////////////
    CPlaneIntersectionType PlaneIntersectionPoint( CPlane &Plane1, 
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
    CPlaneIntersection PlaneIntersectsAABB( const CPlane &rPlane,  
					       const CAxisAlignedBox &rAABB);
  
  
  
    /////////////////////////////////////////////////////////////////
    // 
    // The billboarding utils which can be used to align polygons to face the 
    // camera.
    //
    /////////////////////////////////////////////////////////////////
    class CCamera;
    namespace Billboard
    {
    
      void AlignToCameraPlane( CVector3<float> *vX, 
			       CVector3<float> *vY, 
			       CCamera *pCamera,
			       CVector3<float> *vQuadPos,
			       float fQuadWidthDiv2,
			       float fQuadHeightDiv2 );
    

    }
  
  
  
  }; // namespace Math
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
