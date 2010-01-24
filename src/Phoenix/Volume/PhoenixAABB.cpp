#include "PhoenixAABB.h"
#include "PhoenixGlobals.h"
#include "PhoenixVertexDescriptor.h"
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CAxisAlignedBox::CAxisAlignedBox() :
								  CPositional(CVector3<float>(0.0f,0.0f,0.0f)),
						      CDimensional3D(0.0f,0.0f,0.0f),
						      m_vMin(0.0f,0.0f,0.0f), m_vMax(0.0f,0.0f,0.0f)
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CAxisAlignedBox::CAxisAlignedBox( const CVector3<float> &vCenter,
						   float fWidth, float fHeight, float fLength ) :  CPositional( vCenter ),
												   CDimensional3D( fWidth, fHeight, fLength)
{
  m_vMin[0] = GetPosition()[0] - GetHalfWidth();
  m_vMin[1] = GetPosition()[1] - GetHalfHeight();
  m_vMin[2] = GetPosition()[2] - GetHalfLength();

  m_vMax[0] = GetPosition()[0] + GetHalfWidth();
  m_vMax[1] = GetPosition()[1] + GetHalfHeight();
  m_vMax[2] = GetPosition()[2] + GetHalfLength();
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Phoenix::Volume::CAxisAlignedBox::GetMin() const
{
  return m_vMin;
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Phoenix::Volume::CAxisAlignedBox::GetMax() const
{
  return m_vMax;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CAxisAlignedBox
Phoenix::Volume::CalculateAlignedBox( const Phoenix::Graphics::CVertexDescriptor &vertices )
{
	if ( vertices.GetSize() > 1)
	{
		CVector3<float> vMin(	vertices.GetPointer<float>()[0],
													vertices.GetPointer<float>()[1],
													vertices.GetPointer<float>()[2] );

		CVector3<float> vMax( vertices.GetPointer<float>()[0],
													vertices.GetPointer<float>()[1],
													vertices.GetPointer<float>()[2] );

		for( int v=1; v<vertices.GetSize(); v++)
		{
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[0], vMax[0], vMin[0]);
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[1], vMax[1], vMin[1]);
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[2], vMax[2], vMin[2]);
		}
		CVector3<float> vDimension = (vMax - vMin);
		CVector3<float> vCenter = vMin + vDimension*0.5f;
		return CAxisAlignedBox( vCenter, vDimension[0], vDimension[1], vDimension[2]);
	}
	else
	{
		return CAxisAlignedBox();
	}
}
/////////////////////////////////////////////////////////////////
