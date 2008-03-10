#ifndef __PhoenixMathGeometry_h__
#define __PhoenixMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector3.h"
#include "PhoenixVector4.h"
#include "PhoenixSpatial.h"
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    // The plane class, always normalized. ( || Normal || == 1 )
    /////////////////////////////////////////////////////////////////
    class CPlane : public Phoenix::Math::CVector4<float>
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
    class CQuad : public Phoenix::Spatial::CPositional 
    {
    protected:
      // The width of the quad
      float m_fWidth;
      // The height of the quad
      float m_fHeight;
      float m_fHalfWidth;
      float m_fHalfHeight;
    
    public:
      // The constructor

      CQuad() : CPositional()
      {
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
      }

      void SetWidth( float fWidth )
      {
	m_fWidth = fWidth;
	m_fHalfWidth = m_fWidth * 0.5;
      }
      void SetHeight( float fHeight )
      {
	m_fHeight = fHeight;
	m_fHalfHeight = m_fHeight * 0.5;
      }
      float GetWidth()
      {
	return m_fWidth;
      }
      float GetHeight()
      {
	return m_fHeight;
      }
      float GetHalfWidth()
      {
	return m_fHalfWidth;
      }
      float GetHalfHeight()
      {
	return m_fHalfHeight;
      }
    }; // class CQuad
    /////////////////////////////////////////////////////////////////
    /// Abstraction for rays. 
    class CRay : public Phoenix::Spatial::COneDirectional, public Phoenix::Spatial::CPositional
    {
    public:
      ////////////////////
      /// Constructor.
      CRay() 
      {
	SetDirection(0,0,-1);
	SetPosition(0,0,0);
      }
    }; // class CRay
    /////////////////////////////////////////////////////////////////
    /// Abstraction for lines.
    class CLine : protected CRay
    {
    public:
      CLine()
      {
	SetStart(0,0,0);
	SetEnd(0,0,0);
      }
      ////////////////////
      /// Assigns starting point.
      /// \param fX X-component of start point.
      /// \param fY Y-component of start point.
      /// \param fZ Z-component of start point.
      inline void SetStart( float fX, float fY, float fZ)
      {
	CRay::SetPosition(fX, fY, fZ);
      }
      ////////////////////
      /// Assigns starting point.
      /// \param vPoint Starting point of line.
      inline void SetStart( const CVector3<float> &vPoint )
      {
	CRay::SetPosition(vPoint);
      }
      ////////////////////
      /// Assigns end point.
      /// \param vPoint End point of line.
      inline void SetEnd( float fX, float fY, float fZ )
      {
	CRay::SetDirection(fX, fY, fZ);
      }
      ////////////////////
      /// Assigns end point.
      /// \param vPoint End point of line.
      inline void SetEnd( const CVector3<float> &vPoint )
      {
	CRay::SetDirection(vPoint);
      }
      ////////////////////
      /// Returns start point.
      /// \returns CVector3<float> representing start point.
      inline const CVector3<float> GetStart() const
      {
	return CRay::GetPosition();
      }
      ////////////////////
      /// Returns end point.
      /// \returns CVector3<float> representing end point.
      inline const CVector3<float> GetEnd() const
      {
	return CRay::GetDirection();
      }
      ////////////////////
      /// Returns direction.
      /// \returns Unnormalized direction vector from start to end point.
      inline CVector3<float> GetDirection() const
      {
	return GetEnd() - GetStart();
      }
    };
    /////////////////////////////////////////////////////////////////
  }; // namespace Math
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
