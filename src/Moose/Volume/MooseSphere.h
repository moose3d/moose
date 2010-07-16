/*
 * MooseSphere.h
 *
 *  Created on: Sep 18, 2009
 *      Author: entity
 */

#ifndef MOOSESPHERE_H_
#define MOOSESPHERE_H_
/////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MoosePositional.h"
#include "MooseTransform.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
  	/////////////////////////////////////////////////////////////////
    /// The class for generic sphere with center and radius.
    class MOOSE_API CSphere : public Moose::Spatial::CPositional
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
      CSphere( const Moose::Math::CVector3<float> & vPosition, float fRadius ) : Moose::Spatial::CPositional( vPosition )
      {
				m_fRadius = fRadius;
				m_fRadiusSqr = m_fRadius * m_fRadius;
      }
      /////////////////////////////////////////////////////////////////
      /// Assingment operator.
      /// \param ref sphere to be assigned.
      inline Moose::Volume::CSphere & operator=(CSphere ref)
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
#ifndef SWIG
      /////////////////////////////////////////////////////////////////
      /// For debugging.
      friend std::ostream& operator<<( std::ostream &stream, Moose::Volume::CSphere sphere )
      {
				stream << "C = " << sphere.GetPosition() << ", radius " << sphere.GetRadius();
				return stream;
      }

      friend void operator*=( Moose::Volume::CSphere & sphere, const Moose::Math::CTransform & transf );
#endif
      void Transform( const Moose::Math::CTransform & transf );
    }; // class CSphere
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor using fast but not so accurate algorithm.
    /// \param vertexDescriptor Vertices.
    /// \returns Bounding sphere.
    MOOSE_API Moose::Volume::CSphere CalculateBoundingSphere( const Moose::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor using slightly slower, but more accurate algorithm
    /// \param vertexDescriptor Vertices.
    /// \returns Bounding sphere.
    MOOSE_API Moose::Volume::CSphere CalculateBoundingSphereTight( const Moose::Graphics::CVertexDescriptor &vertexDescriptor );
    ////////////////////
    /// Calculates the bounding sphere for vertices in VertexDescriptor by given set of indices using slightly slower, but more accurate algorithm
    /// \param vertexDescriptor Vertices.
    /// \param indices Which vertices are used.
    /// \returns Bounding sphere.
    MOOSE_API Moose::Volume::CSphere CalculateBoundingSphereTight( const Moose::Graphics::CVertexDescriptor &vertexDescriptor,
    		const Moose::Graphics::CIndexArray &indices );
    ////////////////////
    /// Creates a new sphere is the minimum sphere containing two spheres given as arguments.
    /// \param one One sphere.
    /// \param two Another sphere.
    /// \returns Sphere enclosing both spheres.
    MOOSE_API Moose::Volume::CSphere MergeSpheres( const Moose::Volume::CSphere &one, const Moose::Volume::CSphere &two);

  }// Volume
} // Moose
/////////////////////////////////////////////////////////////////
#endif /* MOOSESPHERE_H_ */
