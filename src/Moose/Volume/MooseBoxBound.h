#ifndef __MOOSEBOXBOUND_H_
#define __MOOSEBOXBOUND_H_
///////////////////////////////////////////////////////////////////////////////
#include "MooseOBB.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Volume
	{
		/////////////////////////////////////////////////////////////////
		class MOOSE_API CBoxBound
		{
		private:
		  /// Model bounding box.
		  Moose::Volume::COrientedBox	m_BoundingBox;
		public:
		  CBoxBound();
		  virtual ~CBoxBound();
		  ////////////////////
		  /// Returns bounding box.
		  /// \returns Reference to bounding box.
		  Moose::Volume::COrientedBox & GetBoundingBox();
		  ////////////////////
		  /// Returns bounding sphere.
		  /// \returns Reference to bounding box.
		  const Moose::Volume::COrientedBox & GetBoundingBox() const;
		};
	} // Volume
}// Moose
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSEBOXBOUND_H_ */
