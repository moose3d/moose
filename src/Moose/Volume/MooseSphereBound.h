/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
