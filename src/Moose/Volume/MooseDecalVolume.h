#ifndef MOOSEDECALVOLUME_H_
#define MOOSEDECALVOLUME_H_
#include "MoosePolytope.h"
#include "MoosePositional.h"
#include "MooseDimensional3D.h"
#include "MooseOrientable.h"
#include "MooseVector3.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
  	/////////////////////////////////////////////////////////////////
    /// Decal volume class. Used for creating scorch marks, for instance.
    class MOOSE_API CDecalVolume : public Moose::Volume::CPolytope,
				     public Moose::Spatial::CPositional,
				     public Moose::Spatial::CDimensional3D,
				     protected Moose::Spatial::COrientable
    {
    public:
      ////////////////////
      /// Constructor. Creates a volume with plane normals pointing towards center.
      /// \param vPosition	Decal center.
      /// \param vNormal	Decal unit normal vector.
      /// \param vNormal	Decal unit tangent vector.
      /// \param fWidth		Width of decal.
      /// \param fHeight	Height of decal.
      /// \param fLength	Length of decal - used for front and back plane clipping.
      CDecalVolume( const Moose::Math::CVector3<float> & vPosition,
		    const Moose::Math::CVector3<float> & vNormal,
		    const Moose::Math::CVector3<float> & vTangent,
		    float fWidth, float fHeight, float fLength) : Moose::Spatial::CPositional(vPosition),
      Moose::Spatial::CDimensional3D( fWidth, fHeight, fLength )
      {
				SetDirectionForwardUp( vNormal, vTangent );

				// T  P
				// left

				Moose::Math::CPlane plane;
				plane.Calculate( -GetTangentVector(), GetPosition() + GetTangentVector()*GetHalfWidth());
				AddPlane( plane );
				// right
				plane.Calculate( GetTangentVector(), GetPosition() - GetTangentVector()*GetHalfWidth());
				AddPlane( plane );

				// B  P
				// Bottom
				plane.Calculate( -GetBitangentVector(), GetPosition() + GetBitangentVector()*GetHalfHeight() );
				AddPlane( plane );
				// top
				plane.Calculate( GetBitangentVector(), GetPosition() - GetBitangentVector()*GetHalfHeight()  );
				AddPlane( plane );

				// N P
				// front
				plane.Calculate( -GetNormalVector(), GetPosition() + GetNormalVector() * GetHalfLength() );
				AddPlane( plane );
				// back
				plane.Calculate(  GetNormalVector(), GetPosition() - GetNormalVector() * GetHalfLength() );
				AddPlane( plane );
      }

      const Moose::Math::CVector3<float> & GetNormalVector() const
      {
      	return GetForwardVector();
      }

      const Moose::Math::CVector3<float> & GetTangentVector() const
      {
      	return GetUpVector();
      }

      const Moose::Math::CVector3<float> & GetBitangentVector() const
      {
      	return GetRightVector();
      }
    }; //class CDecalVolume
  } // Volume
} // Moose
#endif /* MOOSEDECALVOLUME_H_ */
