/*
 * PhoenixSphere.h
 *
 *  Created on: Sep 18, 2009
 *      Author: entity
 */

#ifndef PHOENIXSPHERE_H_
#define PHOENIXSPHERE_H_
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixTransform.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
  	/////////////////////////////////////////////////////////////////
    /// The class for generic sphere with center and radius.
    class PHOENIX_API CSphere : public Phoenix::Spatial::CPositional
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

      friend void operator*=( Phoenix::Volume::CSphere & sphere, const Phoenix::Math::CTransform & transf );

    }; // class CSphere
    ////////////////////
		/// Calculates the bounding sphere for vertices in VertexDescriptor using fast but not so accurate algorithm.
		/// \param vertexDescriptor Vertices.
		/// \returns Bounding sphere.
		PHOENIX_API Phoenix::Volume::CSphere CalculateBoundingSphere( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
		////////////////////
		/// Calculates the bounding sphere for vertices in VertexDescriptor using slightly slower, but more accurate algorithm
		/// \param vertexDescriptor Vertices.
		/// \returns Bounding sphere.
		PHOENIX_API Phoenix::Volume::CSphere CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor );
		////////////////////
		/// Calculates the bounding sphere for vertices in VertexDescriptor by given set of indices using slightly slower, but more accurate algorithm
		/// \param vertexDescriptor Vertices.
		/// \param indices Which vertices are used.
		/// \returns Bounding sphere.
		PHOENIX_API Phoenix::Volume::CSphere CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor,
											 const Phoenix::Graphics::CIndexArray &indices );
		 ////////////////////
		/// Creates a new sphere is the minimum sphere containing two spheres given as arguments.
		/// \param one One sphere.
		/// \param two Another sphere.
		/// \returns Sphere enclosing both spheres.
		PHOENIX_API Phoenix::Volume::CSphere MergeSpheres( const Phoenix::Volume::CSphere &one, const Phoenix::Volume::CSphere &two);

  }// Volume
} // Phoenix
/////////////////////////////////////////////////////////////////
#endif /* PHOENIXSPHERE_H_ */
