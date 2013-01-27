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

#ifndef __MooseOBB_h__
#define __MooseOBB_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MoosePositional.h"
#include "MooseDimensional3D.h"
#include "MooseOrientable.h"
#include "MoosePlane.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTransform.h"
#include <iostream>
#include "MooseAABB.h"
/////////////////////////////////////////////////////////////////
namespace Moose
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
    class MOOSE_API COrientedBox : public Moose::Spatial::CPositional,
				     public Moose::Spatial::CDimensional3D,
				     public Moose::Spatial::COrientable
    {
    protected:
      /// Planes oriented along the box walls.
      //Moose::Math::CPlane    m_Planes[6];
      /// Corners of box, 3 floats, 8 corners
      //Moose::Math::CVector3<float>     m_vecCorners[8];
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
				SetOrientation( Moose::Math::CVector3<float>(0,1,0),
						Moose::Math::CVector3<float>(0,0,-1),
						Moose::Math::CVector3<float>(1,0,0));
				m_qRotation.Identity();
      }
#ifndef SWIG
      ////////////////////
      /// Index operator for accessing box planes.
      /// \param iPlane Plane id.
      /// \returns One of box planes.
      /*inline Moose::Math::CPlane &operator[](BBOX_PLANE_TYPE iPlane)
      {
      	return GetPlane(iPlane);
      }*/
#endif
      ////////////////////
      /// Index operator for accessing box planes.
      /// \param iPlane Plane id.
      /// \returns One of box planes.
      /*inline Moose::Math::CPlane &GetPlane(BBOX_PLANE_TYPE iPlane)
      {
          return m_Planes[iPlane];
      }*/
      ////////////////////
      /// Assignment operator for creating OBB from AABB.
      /// \param box Axis-aligned bounding box reference.
      void operator=( const CAxisAlignedBox & box )
      {
				SetOrientation( Moose::Math::CVector3<float>(0,1,0),
						Moose::Math::CVector3<float>(0,0,-1),
						Moose::Math::CVector3<float>(1,0,0));
				SetPosition( box.GetPosition());
				SetWidth( box.GetWidth());
				SetHeight( box.GetHeight());
				SetLength( box.GetLength());
				//CalculateCorners();
				//CalculatePlanes();
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
				//CalculateCorners();
				//CalculatePlanes();
      }
      ////////////////////
      /// Sets orientation of this box.
      /// \param vUp Up vector.
      /// \param vForward Forward vector.
      /// \param vRight Right vector.
      void SetOrientation(const Moose::Math::CVector3<float> &vUp,
			  const Moose::Math::CVector3<float> &vForward,
			  const Moose::Math::CVector3<float> &vRight);
      ////////////////////
      /// Calculates corners of this box.
      //void CalculateCorners();
      /// Calculates planes of this box.
      //void CalculatePlanes();
      ////////////////////
      /// Returns specific corner as vector.
      /// \param tCorner Corner index.
      /// \returns CVector3<float>.
      Moose::Math::CVector3<float> GetCorner( BBOX_CORNER_TYPE tCorner ) const;
      ////////////////////
      /// Returns specific corner as vector.
      /// \param tCorner Corner index.
      /// \returns CVector3<float>.
      //const Moose::Math::CVector3<float> & GetCorner( BBOX_CORNER_TYPE tCorner ) const;
#ifndef SWIG
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
				stream << "TLF:" << Moose::Math::CVector3<float>(box.GetCorner(TOP_LEFT_FRONT)) << std::endl;
				stream << "TLB:" << Moose::Math::CVector3<float>(box.GetCorner(TOP_LEFT_BACK)) << std::endl;
				stream << "TRF:" << Moose::Math::CVector3<float>(box.GetCorner(TOP_RIGHT_FRONT)) << std::endl;
				stream << "TRB:" << Moose::Math::CVector3<float>(box.GetCorner(TOP_RIGHT_BACK)) << std::endl;
				stream << "BLF:" << Moose::Math::CVector3<float>(box.GetCorner(BOTTOM_LEFT_FRONT)) << std::endl;
				stream << "BLB:" << Moose::Math::CVector3<float>(box.GetCorner(BOTTOM_LEFT_BACK)) << std::endl;
				stream << "BRF:" << Moose::Math::CVector3<float>(box.GetCorner(BOTTOM_RIGHT_FRONT)) << std::endl;
				stream << "BRB:" << Moose::Math::CVector3<float>(box.GetCorner(BOTTOM_RIGHT_BACK)) << std::endl;
				return stream;
      }

      friend void Moose::Volume::operator*=( Moose::Volume::COrientedBox & box, const Moose::Math::CTransform & transf );
#endif
      void Transform(const Moose::Math::CTransform & transf );
    };
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray
    /// \param vertexDescriptor Vertices.
    /// \returns Oriented bounding box
    MOOSE_API Moose::Volume::COrientedBox    CalculateOrientedBoundingBox( const Moose::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the oriented bounding box for vertices in vertexarray using the given set of indices.
    /// \param vertexDescriptor Vertices.
    /// \param indices Which vertices are used.
    /// \returns Bounding sphere.
    MOOSE_API Moose::Volume::COrientedBox CalculateOrientedBoundingBox( const Moose::Graphics::CVertexDescriptor &vertexDescriptor,
    		const Moose::Graphics::CIndexArray &elementList );
    ////////////////////
    /// Creates a new Oriented Box containing two oriented boxes.
    /// \param obOne One oriented box.
    /// \param obTwo Another oriented box.
    /// \returns Bounding box enclosing both boxes.
    MOOSE_API Moose::Volume::COrientedBox MergeOrientedBoxes( const Moose::Volume::COrientedBox &obOne, const Moose::Volume::COrientedBox &obTwo );
#ifndef SWIG
    void operator*=( Moose::Volume::COrientedBox & box, const Moose::Math::CTransform & transf );
#endif
  } // Volume
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif

