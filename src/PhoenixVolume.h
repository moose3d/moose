#ifndef __PhoenixVolume_h__
#define __PhoenixVolume_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Spatial;
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
    /// Class for axis-aligned (x,y,z) box.
    class CAxisAlignedBox : public CPositional, public CDimensional
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
                             CDimensional( fWidth, fHeight, fLength) { }
    };
    /////////////////////////////////////////////////////////////////
    // The class for Oriented Box. Forward vector will be the principal axis,
    // right vector the second and up the third. Dimensions: length = forward,
    // width = right, height = up.
    /////////////////////////////////////////////////////////////////

    class COrientedBox : public CAxisAlignedBox, public COrientable
    {
    protected:

      CPlane    m_Planes[6];    // planes oriented along the box walls
      float     m_aCorners[24]; // The corners of the box, 3 floats, 8 corners

    public:
      ////////////////////
      /// Constructor.
      COrientedBox() : CAxisAlignedBox()
      {
	memset(m_aCorners,0,sizeof(float)*24);
      }
      
      inline CPlane &operator[](BBOX_PLANE_TYPE iPlane)
      {
	return m_Planes[iPlane];
      }
      void operator=( const CAxisAlignedBox & box )
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
      void operator=( const COrientedBox & box )
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

      CVector3<float> GetCorner( BBOX_CORNER_TYPE tCorner );
    
      friend std::ostream& operator<<( std::ostream &stream, COrientedBox box );
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
    // Class for a Cone
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
    // A class for discrete oriented polytope, defines a convex bounding 
    // volume by set of planes.
    /////////////////////////////////////////////////////////////////
    class CPolytope
    {
    protected:
      std::list<CPlane> m_lstPlanes;
    public:
      ////////////////////
      CPolytope()      {}
      ////////////////////
      ~CPolytope()      { m_lstPlanes.clear();  }
      ////////////////////
      void AddPlane( CPlane &Plane )
      {
	m_lstPlanes.push_back(Plane);
      }
      ////////////////////
      std::list<CPlane> &Planes()
      {
	return m_lstPlanes;
      }
      ////////////////////
      unsigned int GetNumPlanes()
      {
	return m_lstPlanes.size();
      }
      ////////////////////
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
    }; // CPolytope
  };// namespace Volume
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void 
CPhoenix::Math::CCone:SetDirection( const CVector3<float> &  vDir )
{
  m_vDirection = vDir.GetNormalized();
}
/////////////////////////////////////////////////////////////////
inline CVector3<float> 
CPhoenix::Math::CCone:GetDirection()  const
{
  return m_vDirection;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:GetAngle() const
{
  return m_fAngle;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:SinReciprocal() const
{
  return m_fSinReciprocal;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:SinSqr() const
{
  return m_fSinAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:CosSqr() const
{
  return m_fCosAngleSqr;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:CosAngle() const
{
  return m_fCosAngle;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:SinAngle() const
{
  return m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
inline void 
CPhoenix::Math::CCone:SetLength(float fLength)
{
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
inline float 
CPhoenix::Math::CCone:GetLength() const
{
  return m_fLength;
}
/////////////////////////////////////////////////////////////////
#endif
