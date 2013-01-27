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

#ifndef MOOSEPLANE_H_
#define MOOSEPLANE_H_
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseVector3.h>
#include <MooseVector4.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
	  /////////////////////////////////////////////////////////////////
	  // The plane class, always normalized. ( || Normal || == 1 )
	  /////////////////////////////////////////////////////////////////
	  class MOOSE_API CPlane : public Moose::Math::CVector4<float>
	  {
	  public:
		CPlane();
		CPlane( const Moose::Math::CVector3<float> &vNormal, float fD );
		CPlane( float fX, float fY, float fZ, float fD );
		~CPlane();
		void SetNormal( Moose::Math::CVector3<float> vNormal);
		void SetDistance( float fDistance );
		void Normalize();
		void Calculate( Moose::Math::CVector3<float> vNormal, const Moose::Math::CVector3<float> & vPoint );
#ifndef SWIG
		friend std::ostream& Moose::Math::operator<<( std::ostream &stream, const Moose::Math::CPlane & plane );
#endif
	  };
#ifndef SWIG
	  std::ostream& operator<<( std::ostream &stream, const Moose::Math::CPlane & plane );
#endif
  } // Math
} // Moose
/////////////////////////////////////////////////////////////
#endif /* MOOSEPLANE_H_ */
