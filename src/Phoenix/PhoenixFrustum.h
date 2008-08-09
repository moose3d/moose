/////////////////////////////////////////////////////////////////
#ifndef __PhoenixFrustum_h__
#define __PhoenixFrustum_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixVolume.h"
/////////////////////////////////////////////////////////////////
#define NUM_FRUSTUM_CORNERS 8
#define NUM_FRUSTUM_PLANES  6
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// The frustum class for culling.
    class CFrustum : public Phoenix::Volume::CPolytope
    {
    public:
      typedef Phoenix::Volume::BBOX_PLANE_TYPE FRUSTUM_PLANE ;
      ////////////////////
      /// The corner indices.
      enum FRUSTUM_CORNER
	{
	  /// A rather desciptive corner index.
	  TOP_FAR_RIGHT = 0,
	  /// A rather desciptive corner index.
	  TOP_FAR_LEFT = 1,
	  /// A rather desciptive corner index.
	  TOP_NEAR_RIGHT = 2,
	  /// A rather desciptive corner index.
	  TOP_NEAR_LEFT = 3,
	  /// A rather desciptive corner index.
	  BOTTOM_FAR_RIGHT = 4,
	  /// A rather desciptive corner index.
	  BOTTOM_FAR_LEFT = 5,
	  /// A rather desciptive corner index.
	  BOTTOM_NEAR_RIGHT = 6,
	  /// A rather desciptive corner index.
	  BOTTOM_NEAR_LEFT = 7

	} ;
      
  
    protected:
      ////////////////////
      /// The pointers to the planes.
      Phoenix::Math::CPlane * m_aPlanes[NUM_FRUSTUM_PLANES];   
      /// The corners of the frustum box.
      Phoenix::Math::CVector3<float> m_aCorners[NUM_FRUSTUM_CORNERS];  
      /////////////////////////////////////////////////////////////////
    public:
      ////////////////////
      /// The constructor
      CFrustum();
      ////////////////////
      /// Assigns a plane.
      /// \param nIndex The plane index which is assigned.
      /// \param vPlane The plane data.
      void		SetPlane( FRUSTUM_PLANE nIndex, Phoenix::Math::CPlane &vPlane);
      ////////////////////
      /// Return a reference to the plane.
      /// \param nIndex The index of the plane desired.
      /// \returns The requested plane.
      Phoenix::Math::CPlane	&GetPlane( FRUSTUM_PLANE nIndex );
      ////////////////////
      /// Return a reference to the plane.
      /// \param nIndex The index of the plane desired.
      /// \returns The requested plane.
      const Phoenix::Math::CPlane	&GetPlane( FRUSTUM_PLANE nIndex ) const;
      ////////////////////
      /// Returns a corner of the frustum.
      /// \param nIndex The index of the corner.
      /// \returns CVector3 with the position of the desired corner.
      CVector3<float>   GetCorner( FRUSTUM_CORNER nIndex );
      ////////////////////
      /// Sets a corner value.
      /// \param nIndex The index of the corner to be set.
      /// \param vPoint The value which is assigned.
      void		SetCorner( FRUSTUM_CORNER nIndex, const Phoenix::Math::CVector3<float> &vPoint );
      
    };
  }; // namespace Graphics
};// namespace Phoenix
#endif
