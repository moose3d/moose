#ifndef PHOENIXCAPSULE_H_
#define PHOENIXCAPSULE_H_
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSphere.h"
#include "PhoenixLineSegment.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Volume
  {
    /////////////////////////////////////////////////////////////////
    /// Capsule class (sphere swept line)
    class PHOENIX_API CCapsule : public Phoenix::Volume::CSphere,
																 public Phoenix::Math::CLineSegment
    {
    public:
      ////////////////////
      /// Constructor.
      /// \param vStart Start point of capsule.
      /// \param vEnd End point of capsule
      /// \param fRadius Radius of the capsule sphere.
      CCapsule( const Phoenix::Math::CVector3<float> & vStart,
							  const Phoenix::Math::CVector3<float> & vEnd,
								float fRadius )
      {
				SetRadius( fRadius );
				Set( vStart, vEnd );
      }
    }; // class CCapsule
  } // Volume
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXCAPSULE_H_ */
