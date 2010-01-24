#ifndef PHOENIXSPHEREBOUND_H_
#define PHOENIXSPHEREBOUND_H_
/////////////////////////////////////////////////////////////////
#include "PhoenixSphere.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Volume
	{
		class PHOENIX_API CSphereBound
		{
		private:
		  /// Model bounding sphere
		  Phoenix::Volume::CSphere		m_BoundingSphere;
		public:
		  CSphereBound();
		  virtual ~CSphereBound();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding sphere.
		  Phoenix::Volume::CSphere & GetBoundingSphere();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding sphere.
		  const Phoenix::Volume::CSphere & GetBoundingSphere() const;
		};
	} // Volume
} // Phoenix
#endif /* PHOENIXSPHEREBOUND_H_ */
