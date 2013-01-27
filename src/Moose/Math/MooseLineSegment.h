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

#ifndef __MooseMathGeometry_h__
#define __MooseMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#include "MooseVector3.h"
#include "MooseLine.h"
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    /// Abstraction for lines.
    class MOOSE_API CLineSegment : public Moose::Math::CLine
    {
    protected:
      float m_fDistanceStart;
      float m_fDistanceEnd;
    public:
      ////////////////////
      /// Constructs line segment using two points in 3d space.
      CLineSegment( )
      {
    	Moose::Math::CLine::SetPosition( 0.0f,0.0f,0.0f );
		Moose::Spatial::COneDirectional::SetDirection( 0.0f,0.0f,-1.0f);
		m_fDistanceStart = 0.0f;
		m_fDistanceEnd   = 0.0f;
      }
      ////////////////////
      /// Constructs line segment using two points in 3d space.
      CLineSegment( const Moose::Math::CVector3<float> & vStart, 
		    const Moose::Math::CVector3<float> & vEnd )
      {
	Set(vStart, vEnd);
      }
      ////////////////////
      /// 
      void Set( const Moose::Math::CVector3<float> & vStart, 
		const Moose::Math::CVector3<float> & vEnd )
      {
	// Set direction from start to end.
	Moose::Math::CVector3<float> vDir = (vEnd - vStart);
	float fLength = vDir.Length();
	vDir.Normalize();
	SetDirection(vDir);
	// start is reference point
	Moose::Math::CLine::SetPosition( vStart );
	
 	m_fDistanceStart = 0.0f; 
	m_fDistanceEnd = fLength;
      }
      inline void SetDistanceStart( float fValue ) { m_fDistanceStart = fValue; }
      inline void SetDistanceEnd( float fValue ) { m_fDistanceEnd = fValue; }
      float GetDistanceStart() const { return m_fDistanceStart; }
      float GetDistanceEnd() const { return m_fDistanceEnd; }
      ////////////////////
      /// Returns start point.
      /// \returns CVector3<float> representing start point.
      inline Moose::Math::CVector3<float> GetStart() const
      {
	return GetPosition()+GetDirection()*GetDistanceStart();
      }
      ////////////////////
      /// Returns end point.
      /// \returns CVector3<float> representing end point.
      inline CVector3<float> GetEnd() const
      {
	return GetPosition()+GetDirection()*GetDistanceEnd();
      }
      
    };
    /////////////////////////////////////////////////////////////////
  }; // namespace Math
}; // namespace Moose
#endif
/////////////////////////////////////////////////////////////////
