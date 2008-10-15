#ifndef __PhoenixMathGeometry_h__
#define __PhoenixMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector3.h"
#include "PhoenixVector4.h"
#include "PhoenixSpatial.h"
#include <list>
#include <iostream>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    // The plane class, always normalized. ( || Normal || == 1 )
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CPlane : public Phoenix::Math::CVector4<float>
    {
    public:
      CPlane();
      CPlane( const CVector3<float> &vNormal, float fD );
      CPlane( float fX, float fY, float fZ, float fD );
      ~CPlane();
      void SetNormal( Phoenix::Math::CVector3<float> vNormal);
      void SetDistance( float fDistance );    
      void Normalize();
      void Calculate( Phoenix::Math::CVector3<float> vNormal, const Phoenix::Math::CVector3<float> & vPoint );

      friend std::ostream& operator<<( std::ostream &stream, const CPlane & plane );
      
    };
    /////////////////////////////////////////////////////////////////
    // The class for a Quad
    class PHOENIX_API CQuad : public Phoenix::Spatial::CPositional 
    {
    protected:
      // The width of the quad
      float m_fWidth;
      // The height of the quad
      float m_fHeight;
      float m_fHalfWidth;
      float m_fHalfHeight;
    
    public:
      ////////////////////
      /// The constructor
      CQuad() : CPositional(), m_fWidth(0.0f), m_fHeight(0.0f) {}
      ////////////////////
      /// Sets width
      /// \param fWidth Width to be assigned.
      void SetWidth( float fWidth )
      {
	m_fWidth = fWidth;
	m_fHalfWidth = m_fWidth * 0.5f;
      }
      ////////////////////
      /// Sets height
      /// \param fHeight Height to be assigned.
      void SetHeight( float fHeight )
      {
	m_fHeight = fHeight;
	m_fHalfHeight = m_fHeight * 0.5f;
      }
      float GetWidth() const
      {
	return m_fWidth;
      }
      float GetHeight() const
      {
	return m_fHeight;
      }
      float GetHalfWidth() const
      {
	return m_fHalfWidth;
      }
      float GetHalfHeight() const
      {
	return m_fHalfHeight;
      }
      friend std::ostream& operator<<( std::ostream &stream, const CQuad & quad );
    }; // class CQuad
    /////////////////////////////////////////////////////////////////
    /// Abstraction for 3d lines. Infinitely thin and long, extending both directions
    /// from reference point.
    class PHOENIX_API CLine : public Phoenix::Spatial::COneDirectional, 
		  public Phoenix::Spatial::CPositional
    {
    public:
      ////////////////////
      /// Default constructor. 
      /// \param vPos Reference point through which line goes.
      /// \param vDir Line direction - should .
      CLine( const Phoenix::Math::CVector3<float> & vPos = CVector3<float>(0,0,0), 
	     const Phoenix::Math::CVector3<float> & vDir = CVector3<float>(0,0,-1) ) : COneDirectional( vDir ),
										       CPositional(vPos){ }

    }; // class CLine
    /////////////////////////////////////////////////////////////////
    /// Abstraction for rays ( like lines, but extend only to one direction. ).
    class PHOENIX_API CRay : public CLine
    {
      
    };
    /////////////////////////////////////////////////////////////////
    /// Abstraction for lines.
    class PHOENIX_API CLineSegment : public CLine
    {
    protected:
      float m_fDistanceStart;
      float m_fDistanceEnd;
    public:
      ////////////////////
      /// Constructs line segment using two points in 3d space.
      CLineSegment( )
      {
	CLine::SetPosition( 0.0f,0.0f,0.0f );
	COneDirectional::SetDirection( 0.0f,0.0f,-1.0f);
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
	CVector3<float> vDir = (vEnd - vStart);
	float fLength = vDir.Length();
	vDir.Normalize();
	SetDirection(vDir);
	// start is reference point
	CLine::SetPosition( vStart );
	
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
      inline CVector3<float> GetStart() const
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
