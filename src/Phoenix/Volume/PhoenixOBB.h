#ifndef __PhoenixOBB_h__
#define __PhoenixOBB_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixVector3.h"
#include "PhoenixPositional.h"
#include "PhoenixDimensional3D.h"
#include "PhoenixOrientable.h"
#include "PhoenixPlane.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixTransform.h"
#include <iostream>
#include "PhoenixAABB.h"
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
    /// The class for Oriented Box. Forward vector will be the principal axis,
    /// right vector the second and up the third. Dimensions: length = forward,
    /// width = right, height = up.
    class PHOENIX_API COrientedBox : public Phoenix::Spatial::CPositional,
				     public Phoenix::Spatial::CDimensional3D,
				     public Phoenix::Spatial::COrientable
    {
    protected:
      /// Planes oriented along the box walls.
      Phoenix::Math::CPlane    m_Planes[6];
      /// Corners of box, 3 floats, 8 corners
      Phoenix::Math::CVector3<float>     m_vecCorners[8];
    public:
      ////////////////////
      /// Constructor.
      COrientedBox()
      {
				//memset(m_aCorners,0,sizeof(float)*24);
				// reset coords. (Is it really necessary?)
				// m_vecCorners[0][0] = m_vecCorners[0][1] =  m_vecCorners[0][2] = 0.0f;
				// 	m_vecCorners[1][0] = m_vecCorners[1][1] =  m_vecCorners[1][2] = 0.0f;
				//  	m_vecCorners[2][0] = m_vecCorners[2][1] =  m_vecCorners[2][2] = 0.0f;
				//  	m_vecCorners[3][0] = m_vecCorners[3][1] =  m_vecCorners[3][2] = 0.0f;
				//  	m_vecCorners[4][0] = m_vecCorners[4][1] =  m_vecCorners[4][2] = 0.0f;
				//  	m_vecCorners[5][0] = m_vecCorners[5][1] =  m_vecCorners[5][2] = 0.0f;
				//  	m_vecCorners[6][0] = m_vecCorners[6][1] =  m_vecCorners[6][2] = 0.0f;
				//  	m_vecCorners[7][0] = m_vecCorners[7][1] =  m_vecCorners[7][2] = 0.0f;
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
      /// Calculates corners of this box.
      void CalculateCorners();
      /// Calculates planes of this box.
      void CalculatePlanes();
      ////////////////////
      /// Returns specific corner as vector.
      /// \param tCorner Corner index.
      /// \returns CVector3<float>.
      Phoenix::Math::CVector3<float> & GetCorner( BBOX_CORNER_TYPE tCorner );
      ////////////////////
      /// Returns specific corner as vector.
      /// \param tCorner Corner index.
      /// \returns CVector3<float>.
      const Phoenix::Math::CVector3<float> & GetCorner( BBOX_CORNER_TYPE tCorner ) const;
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
				stream << "TLF:" << Phoenix::Math::CVector3<float>(box.GetCorner(TOP_LEFT_FRONT)) << std::endl;
				stream << "TLB:" << Phoenix::Math::CVector3<float>(box.GetCorner(TOP_LEFT_BACK)) << std::endl;
				stream << "TRF:" << Phoenix::Math::CVector3<float>(box.GetCorner(TOP_RIGHT_FRONT)) << std::endl;
				stream << "TRB:" << Phoenix::Math::CVector3<float>(box.GetCorner(TOP_RIGHT_BACK)) << std::endl;
				stream << "BLF:" << Phoenix::Math::CVector3<float>(box.GetCorner(BOTTOM_LEFT_FRONT)) << std::endl;
				stream << "BLB:" << Phoenix::Math::CVector3<float>(box.GetCorner(BOTTOM_LEFT_BACK)) << std::endl;
				stream << "BRF:" << Phoenix::Math::CVector3<float>(box.GetCorner(BOTTOM_RIGHT_FRONT)) << std::endl;
				stream << "BRB:" << Phoenix::Math::CVector3<float>(box.GetCorner(BOTTOM_RIGHT_BACK)) << std::endl;
				return stream;
      }
      friend void Phoenix::Volume::operator*=( Phoenix::Volume::COrientedBox & box, const Phoenix::Math::CTransform & transf );
    };
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    PHOENIX_API Phoenix::Volume::COrientedBox    CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray using the given set of indices.
    /// \param vertexDescriptor Vertices.
    /// \param indices Which vertices are used.
    /// \returns Bounding sphere.
    PHOENIX_API Phoenix::Volume::COrientedBox CalculateOrientedBoundingBox( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor,
    		const Phoenix::Graphics::CIndexArray &elementList );
    ////////////////////
    /// Creates a new Oriented Box containing two oriented boxes.
    /// \param obOne One oriented box.
    /// \param obTwo Another oriented box.
    /// \returns Bounding box enclosing both boxes.
    PHOENIX_API Phoenix::Volume::COrientedBox MergeOrientedBoxes( const Phoenix::Volume::COrientedBox &obOne, const Phoenix::Volume::COrientedBox &obTwo );
    void operator*=( Phoenix::Volume::COrientedBox & box, const Phoenix::Math::CTransform & transf );
  } // Volume
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif

