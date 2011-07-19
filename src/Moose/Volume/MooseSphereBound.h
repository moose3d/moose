#ifndef MOOSESPHEREBOUND_H_
#define MOOSESPHEREBOUND_H_
/////////////////////////////////////////////////////////////////
#include "MooseSphere.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Volume
	{
		class MOOSE_API CSphereBound
		{
		private:
		  /// Model bounding sphere
		  Moose::Volume::CSphere		m_BoundingSphere;
		public:
		  CSphereBound();
		  virtual ~CSphereBound();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding sphere.
		  Moose::Volume::CSphere & GetBoundingSphere();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding sphere.
		  const Moose::Volume::CSphere & GetBoundingSphere() const;
          ////////////////////
          /// Sets bounding sphere.
          /// \param s Sphere to be used.
          void SetBoundingSphere( const Moose::Volume::CSphere & s );
		};
	} // Volume
} // Moose
#endif /* MOOSESPHEREBOUND_H_ */
