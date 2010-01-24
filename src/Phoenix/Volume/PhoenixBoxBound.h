#ifndef __PHOENIXBOXBOUND_H_
#define __PHOENIXBOXBOUND_H_
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixOBB.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Volume
	{
		/////////////////////////////////////////////////////////////////
		class PHOENIX_API CBoxBound
		{
		private:
		  /// Model bounding box.
		  Phoenix::Volume::COrientedBox	m_BoundingBox;
		public:
		  CBoxBound();
		  virtual ~CBoxBound();
		  ////////////////////
		  /// Returns bounding box.
		  /// \returns Reference to bounding box.
		  Phoenix::Volume::COrientedBox & GetBoundingBox();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding box.
		  const Phoenix::Volume::COrientedBox & GetBoundingBox() const;
		};
	} // Volume
}// Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXBOXBOUND_H_ */
