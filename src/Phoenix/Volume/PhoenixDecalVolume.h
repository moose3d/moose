#ifndef PHOENIXDECALVOLUME_H_
#define PHOENIXDECALVOLUME_H_
#include "PhoenixPolytope.h"
#include "PhoenixSpatial.h"
#include "PhoenixVector3.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
  	/////////////////////////////////////////////////////////////////
    /// Decal volume class. Used for creating scorch marks, for instance.
    class PHOENIX_API CDecalVolume : public Phoenix::Volume::CPolytope,
				     public Phoenix::Spatial::CPositional,
				     public Phoenix::Spatial::CDimensional3D,
				     protected Phoenix::Spatial::COrientable
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
      CDecalVolume( const Phoenix::Math::CVector3<float> & vPosition,
		    const Phoenix::Math::CVector3<float> & vNormal,
		    const Phoenix::Math::CVector3<float> & vTangent,
		    float fWidth, float fHeight, float fLength) : Phoenix::Spatial::CPositional(vPosition),
      Phoenix::Spatial::CDimensional3D( fWidth, fHeight, fLength )
      {
				SetDirectionForwardUp( vNormal, vTangent );

				// T  P
				// left

				Phoenix::Math::CPlane plane;
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
  } // Volume
} // Phoenix
#endif /* PHOENIXDECALVOLUME_H_ */
