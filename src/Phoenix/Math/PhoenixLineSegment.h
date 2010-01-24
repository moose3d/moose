#ifndef __PhoenixMathGeometry_h__
#define __PhoenixMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixAPI.h"
#include "PhoenixVector3.h"
#include "PhoenixLine.h"
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    /// Abstraction for lines.
    class PHOENIX_API CLineSegment : public Phoenix::Math::CLine
    {
    protected:
      float m_fDistanceStart;
      float m_fDistanceEnd;
    public:
      ////////////////////
      /// Constructs line segment using two points in 3d space.
      CLineSegment( )
      {
    	Phoenix::Math::CLine::SetPosition( 0.0f,0.0f,0.0f );
		Phoenix::Spatial::COneDirectional::SetDirection( 0.0f,0.0f,-1.0f);
		m_fDistanceStart = 0.0f;
		m_fDistanceEnd   = 0.0f;
      }
      ////////////////////
      /// Constructs line segment using two points in 3d space.
      CLineSegment( const Phoenix::Math::CVector3<float> & vStart, 
		    const Phoenix::Math::CVector3<float> & vEnd )
      {
	Set(vStart, vEnd);
      }
      ////////////////////
      /// 
      void Set( const Phoenix::Math::CVector3<float> & vStart, 
		const Phoenix::Math::CVector3<float> & vEnd )
      {
	// Set direction from start to end.
	Phoenix::Math::CVector3<float> vDir = (vEnd - vStart);
	float fLength = vDir.Length();
	vDir.Normalize();
	SetDirection(vDir);
	// start is reference point
	Phoenix::Math::CLine::SetPosition( vStart );
	
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
      inline Phoenix::Math::CVector3<float> GetStart() const
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
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
