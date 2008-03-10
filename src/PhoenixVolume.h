#ifndef __PhoenixVolume_h__
#define __PhoenixVolume_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
    ////////////////////
    /// Bounding box plane indices.
    enum BBOX_PLANE_TYPE
    {
      TOP  = 0,
      BOTTOM = 1,
      LEFT = 2,
      RIGHT = 3,
      FRONT = 4,
      BACK = 5
    };
    ////////////////////
    /// Corner indices for bounding boxes.
    enum  BBOX_CORNER_TYPE
    {
      TOP_LEFT_BACK = 0,
      TOP_LEFT_FRONT = 1,
      TOP_RIGHT_BACK = 2,
      TOP_RIGHT_FRONT = 3,
      BOTTOM_LEFT_BACK = 4,
      BOTTOM_LEFT_FRONT = 5,
      BOTTOM_RIGHT_BACK = 6,
      BOTTOM_RIGHT_FRONT = 7
    };
    /////////////////////////////////////////////////////////////////
    // A class for discrete oriented polytope, defines a convex bounding 
    // volume by set of planes.
    class CPolytope
    {
    protected:
      std::list<Phoenix::Math::CPlane> m_lstPlanes;
    public:
      ////////////////////
      CPolytope()      {}
      ////////////////////
      ~CPolytope()      { m_lstPlanes.clear();  }
      ////////////////////
      void AddPlane( Phoenix::Math::CPlane &Plane )
      {
	m_lstPlanes.push_back(Plane);
      }
      ////////////////////
      std::list<Phoenix::Math::CPlane> &Planes()
      {
	return m_lstPlanes;
      }
      ////////////////////
      unsigned int GetNumPlanes()
      {
	return m_lstPlanes.size();
      }
      ////////////////////
      friend std::ostream& operator<<( std::ostream &stream, const Phoenix::Volume::CPolytope & polytope )
      {
	std::list<Phoenix::Math::CPlane>::const_iterator it = polytope.m_lstPlanes.begin();
	stream << "KDOP planes:" << std::endl;
	for( ; it != polytope.m_lstPlanes.end(); it++)
	{
	  stream << *it << std::endl;
	}
	return stream;
      }
    }; // CPolytope
    /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) box.
    class CAxisAlignedBox : public Phoenix::Spatial::CPositional, 
                            public Phoenix::Spatial::CDimensional3D
    {
    public:
      ////////////////////
      /// The constuctor.
      CAxisAlignedBox()    {}
      ////////////////////x
      /// The parametrized constructor.
      CAxisAlignedBox( const CVector3<float> &vCenter, 
		       float fWidth, float fHeight,  float fLength ) : 
                             CPositional( vCenter ), 
                             CDimensional3D( fWidth, fHeight, fLength) {  }
    };
    /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) cube.
    class CAxisAlignedCube : public Phoenix::Spatial::CPositional, 
                             public Phoenix::Spatial::CDimensional1D
    {
    public:
      ////////////////////
      /// The constuctor.
      CAxisAlignedCube()    {}
      ////////////////////x
      /// The parametrized constructor.
      CAxisAlignedCube( const CVector3<float> &vCenter, float fEdgeLength ) : 
      CPositional( vCenter ), CDimensional1D( fEdgeLength ) {  }
    };
    /////////////////////////////////////////////////////////////////
    /// The class for Oriented Box. Forward vector will be the principal axis,
    /// right vector the second and up the third. Dimensions: length = forward,
    /// width = right, height = up.
    class COrientedBox : public Phoenix::Spatial::CPositional, 
                         public Phoenix::Spatial::CDimensional3D, 
			 public Phoenix::Spatial::COrientable
    {
    protected:
      /// Planes oriented along the box walls.
      Phoenix::Math::CPlane    m_Planes[6];    
      /// Corners of box, 3 floats, 8 corners
      float     m_aCorners[24]; 
    public:
      ////////////////////
      /// Constructor.
      COrientedBox() 
      {
	memset(m_aCorners,0,sizeof(float)*24);
	SetOrientation( Phoenix::Math::CVector3<float>(0,1,0),
			Phoenix::Math::CVector3<float>(0,0,-1),
			Phoenix::Math::CVector3<float>(1,0,0));
	m_qRotation.Identity();
      }
      ////////////////////
      /// Index operator for accessing box planes.
      /// \param iPlane Plane id.
      /// \returns One of box planes.
      inline Phoenix::Math::CPlane &operator[](BBOX_PLANE_TYPE iPlane)
      {
	return m_Planes[iPlane];
      }
      ////////////////////
      /// Assignment operator for creating OBB from AABB.
      /// \param box Axis-aligned bounding box reference.
      void operator=( const CAxisAlignedBox & box )
      {
	SetOrientation( Phoenix::Math::CVector3<float>(0,1,0),
			Phoenix::Math::CVector3<float>(0,0,-1),
			Phoenix::Math::CVector3<float>(1,0,0));
	SetPosition( box.GetPosition());
	SetWidth( box.GetWidth());
	SetHeight( box.GetHeight());
	SetLength( box.GetLength());
	CalculateCorners();
	CalculatePlanes();
      }
      ////////////////////
      /// Assignment operator for OBBs.
      /// \param box Oriented bounding box reference.
      void operator=( const COrientedBox & box )
      {
	SetOrientation( box.GetUpVector(),
			box.GetForwardVector(),
			box.GetRightVector());
	SetPosition( box.GetPosition());
	SetWidth( box.GetWidth());
	SetHeight( box.GetHeight());
	SetLength( box.GetLength());
	m_qRotation = box.m_qRotation;
	CalculateCorners();
	CalculatePlanes();
      }
      ////////////////////
      /// Sets orientation of this box.
      /// \param vUp Up vector.
      /// \param vForward Forward vector.
      /// \param vRight Right vector.
      void SetOrientation(const Phoenix::Math::CVector3<float> &vUp, 
			  const Phoenix::Math::CVector3<float> &vForward,
			  const Phoenix::Math::CVector3<float> &vRight);
      ////////////////////
      /// Returns corners of this box as float array.
      /// \returns Pointer to beginning of array.
      const float * GetCorners() const;
      ////////////////////
      /// Calculates corners of this box.
      void CalculateCorners();
      /// Calculates planes of this box.
      void CalculatePlanes();
      ////////////////////
      /// Returns specific corner as vector.
      /// \param tCorner Corner index.
      /// \returns CVector3<float>.
      CVector3<float> GetCorner( BBOX_CORNER_TYPE tCorner );
      ////////////////////
      /// Stream output.
      /// \param stream Output stream.
      /// \param box Box to be printed.
      /// \returns Reference to outputstream.
      friend std::ostream& operator<<( std::ostream &stream, COrientedBox box )
      {
	stream << std::endl
	       << "F = " << box.GetForwardVector() << ", scale " << box.GetLength() << std::endl
	       << "R = " << box.GetRightVector()   << ", scale " << box.GetWidth() << std::endl
	       << "U = " << box.GetUpVector()      << ", scale " << box.GetHeight() << std::endl
	       << "C = " << box.GetPosition() << std::endl;
	stream << "TLF:" << CVector3<float>(box.GetCorner(TOP_LEFT_FRONT)) << std::endl;
	stream << "TLB:" << CVector3<float>(box.GetCorner(TOP_LEFT_BACK)) << std::endl;
	stream << "TRF:" << CVector3<float>(box.GetCorner(TOP_RIGHT_FRONT)) << std::endl;
	stream << "TRB:" << CVector3<float>(box.GetCorner(TOP_RIGHT_BACK)) << std::endl;
	stream << "BLF:" << CVector3<float>(box.GetCorner(BOTTOM_LEFT_FRONT)) << std::endl;
	stream << "BLB:" << CVector3<float>(box.GetCorner(BOTTOM_LEFT_BACK)) << std::endl;
	stream << "BRF:" << CVector3<float>(box.GetCorner(BOTTOM_RIGHT_FRONT)) << std::endl;
	stream << "BRB:" << CVector3<float>(box.GetCorner(BOTTOM_RIGHT_BACK)) << std::endl;
	return stream;
      }
    
    };
    /////////////////////////////////////////////////////////////////
    /// The class for generic sphere with center and radius.
    class CSphere : public Phoenix::Spatial::CPositional
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
      CSphere( const CVector3<float> & vPosition, float fRadius ) : CPositional( vPosition )
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
    // Class for a Cone
    class CCone : public Phoenix::Spatial::CPositional 
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
      ////////////////////
      /// Constructor.
      CCone();
      ////////////////////
      /// Parametrized constructor
      /// \param vPos Cone starting point.
      /// \param vDir Direction where cone is facing.
      /// \param fAngle Angle of cone.
      /// \param fLength How far cone reaches.
      CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength );
      ////////////////////
      /// Assigns view angle of the cone
      /// \param fAngle Angle in degrees.
      void SetAngle( float fAngle );
      ////////////////////
      /// Assigns direction of cone.
      /// \param vDir direction where cone will be facing.
      void SetDirection( const CVector3<float> &  vDir );
      ////////////////////
      /// Returns cone direction.
      /// \returns CVector3 with cone direction.
      CVector3<float> GetDirection()  const;
      ////////////////////
      /// Returns angle of the cone in degrees
      /// \returns Cone angle.
      float GetAngle() const;
      ////////////////////
      /// Returns the 1 / sin(angle)
      /// \returns float.
      float SinReciprocal() const;
      ////////////////////
      /// Returns squared sin(angle)
      /// \returns Squared sin(angle)
      float SinSqr() const;
      ////////////////////
      /// Squared cos(angle)
      /// \returns squared cos(angle)
      float CosSqr() const;
      ////////////////////
      /// Returns cos(angle).
      /// \returns cos(angle)
      float CosAngle() const;
      ////////////////////
      /// Returns sin(angle).
      /// \returns sin(angle)
      float SinAngle() const;
      ////////////////////
      /// Sets cone length.
      /// \param fLength length value.
      void SetLength(float fLength);
      ////////////////////
      /// Get length.
      /// \param Length of cone.
      float GetLength() const;

    }; // class CCone
    /////////////////////////////////////////////////////////////////
    /// Decal volume class. Used for creating scorch marks, for instance.
    class CDecalVolume : public Phoenix::Volume::CPolytope,
			 public Phoenix::Spatial::CPositional,
			 public Phoenix::Spatial::CDimensional3D,
			 protected Phoenix::Spatial::COrientable
    {
    public:
      ////////////////////
      /// Constructor. Creates a volume with plane normals pointing towards center.
      /// \param vPosition	Decal center.
      /// \param vNormal	Decal normal vector.
      /// \param vNormal	Decal tangent vector.
      /// \param fWidth		Width of decal.
      /// \param fHeight	Height of decal.
      /// \param fLength	Length of decal - used for front and back plane clipping.
      CDecalVolume( const Phoenix::Math::CVector3<float> & vPosition, 
		    const Phoenix::Math::CVector3<float> & vNormal,
		    const Phoenix::Math::CVector3<float> & vTangent,
		    float fWidth, float fHeight, float fLength) : Phoenix::Spatial::CPositional(vPosition), 
								  Phoenix::Spatial::CDimensional3D( fWidth, fHeight, fLength )
      {
	SetDirectionForwardUp( vNormal, vTangent );

	// T · P
	// left
	float fTmp = GetTangentVector().Dot(GetPosition());
	Phoenix::Math::CPlane plane( -GetTangentVector(), GetHalfWidth() - fTmp);
	AddPlane( plane );
	// right
	plane.SetNormal( GetTangentVector() );
	plane.SetDistance( GetHalfWidth() + fTmp );
	AddPlane( plane );
	
	// B · P
	// Bottom
	fTmp = GetBitangentVector().Dot( GetPosition());
	plane.SetNormal( -GetBitangentVector());
	plane.SetDistance( GetHalfHeight() - fTmp );
	AddPlane( plane );
	// top
	plane.SetNormal( GetBitangentVector() );
	plane.SetDistance( GetHalfHeight() + fTmp );
	AddPlane( plane );

	// N · P
	// front
	fTmp = GetNormalVector().Dot( GetPosition());
	plane.SetNormal( GetNormalVector());
	plane.SetDistance( GetHalfHeight() + fTmp );
	AddPlane( plane );
	// back
	plane.SetNormal( -GetNormalVector());
	plane.SetDistance( GetHalfHeight() - fTmp );
	AddPlane( plane );
      }

      const CVector3<float> & GetNormalVector() const 
      {
	return GetForwardVector();
      }

      const CVector3<float> & GetTangentVector() const 
      {
	return GetUpVector();
      }
      
      const CVector3<float> & GetBitangentVector() const 
      {
	return GetRightVector();
      }
    }; //class CDecalVolume
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    Phoenix::Volume::COrientedBox    CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the axis-aligned bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    Phoenix::Volume::CAxisAlignedBox CalculateAlignedBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor using fast but not so accurate algorithm.
    /// \param vertexDescriptor Vertices.
    /// \returns Bounding sphere.
    Phoenix::Volume::CSphere CalculateBoundingSphere( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor using slightly slower, but more accurate algorithm
    /// \param vertexDescriptor Vertices.
    /// \returns Bounding sphere.
    Phoenix::Volume::CSphere CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor by given set of indices using slightly slower, but more accurate algorithm
    /// \param vertexDescriptor Vertices.
    /// \param indices Which vertices are used.
    /// \returns Bounding sphere.
    Phoenix::Volume::CSphere CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor, 
							   const Phoenix::Graphics::CIndexArray &indices );
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray using the given set of indices.
    /// \param vertexDescriptor Vertices.
    /// \param indices Which vertices are used.
    /// \returns Bounding sphere.
    Phoenix::Volume::COrientedBox CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor,
								const Phoenix::Graphics::CIndexArray &elementList );
    ////////////////////
    /// Creates a new sphere is the minimum sphere containing two spheres given as arguments.
    /// \param one One sphere.
    /// \param two Another sphere.
    /// \returns Sphere enclosing both spheres.
    Phoenix::Volume::CSphere MergeSpheres( const Phoenix::Volume::CSphere &one, const Phoenix::Volume::CSphere &two);
    ////////////////////
    /// Creates a new Oriented Box containing two oriented boxes.
    /// \param obOne One oriented box.
    /// \param obTwo Another oriented box.
    /// \returns Bounding box enclosing both boxes.
    Phoenix::Volume::COrientedBox MergeOrientedBoxes( const Phoenix::Volume::COrientedBox &obOne, const Phoenix::Volume::COrientedBox &obTwo );
    
    /////////////////////////////////////////////////////////////////
    // Constructs a 4x4 rotation matrix by inserting the Oriented Box axis 
    // as column vectors of the it.
    //
    //     Fx Rx Ux 0
    // M = Fy Ry Uy 0 ,  F = Forward Vector
    //     Fz Rz Uz 0    R = Right Vector
    //     0  0  0  1    U = Up Vector
    /////////////////////////////////////////////////////////////////
    Phoenix::Math::CMatrix4x4<float> OrientedBoxAxisToRotationMatrix(  const Phoenix::Volume::COrientedBox &ob  );
  };// namespace Volume
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Volume::CCone::SetDirection( const CVector3<float> &  vDir )
{
  m_vDirection = vDir.GetNormalized();
}
/////////////////////////////////////////////////////////////////
inline CVector3<float> 
Phoenix::Volume::CCone::GetDirection()  const
{
  return m_vDirection;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::GetAngle() const
{
  return m_fAngle;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::SinReciprocal() const
{
  return m_fSinReciprocal;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::SinSqr() const
{
  return m_fSinAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::CosSqr() const
{
  return m_fCosAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::CosAngle() const
{
  return m_fCosAngle;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::SinAngle() const
{
  return m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Volume::CCone::SetLength(float fLength)
{
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Volume::CCone::GetLength() const
{
  return m_fLength;
}
/////////////////////////////////////////////////////////////////


#endif
