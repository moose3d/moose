#ifndef __PhoenixMathGeometry_h__
#define __PhoenixMathGeometry_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector4.h"
#include "PhoenixSpatial.h"
#include <list>
#include <iostream>
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    /////////////////////////////////////////////////////////////////
    // The plane class, always normalized. ( || Normal || == 1 )
    /////////////////////////////////////////////////////////////////
    class CPlane : public CVector4<float>
    {
    public:

      CPlane();
      ~CPlane();
      void SetNormal( CVector3<float> vNormal);
      void SetDistance( float fDistance );    
      void Normalize();
      void Calculate( CVector3<float> vNormal, const CVector3<float> & vPoint );

      friend std::ostream& operator<<( std::ostream &stream, const CPlane & plane );
      
    };
    /////////////////////////////////////////////////////////////////
    // The class for a Quad
    class CQuad : public CPositional 
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
  }; // namespace Math
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
