#include "PhoenixFrustum.h"
#include "PhoenixCollision.h"
#include <vector>
/////////////////////////////////////////////////////////////////
using std::vector;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrustum::CFrustum()
{
  // Add planes to the vector
  CPlane plane;
  AddPlane( plane);
  AddPlane( plane);
  AddPlane( plane );
  AddPlane( plane );
  AddPlane( plane );
  AddPlane( plane );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CFrustum::SetPlane(FRUSTUM_PLANE nIndex, CPlane &vPlane)
{
  m_Planes[nIndex] = vPlane;
}
/////////////////////////////////////////////////////////////////
CPlane &
Phoenix::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex )
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
const CPlane &
Phoenix::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex ) const
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
CVector3<float>
Phoenix::Graphics::CFrustum::GetCorner( FRUSTUM_CORNER nIndex )
{
  return m_aCorners[nIndex];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CFrustum::SetCorner( FRUSTUM_CORNER nIndex, const CVector3<float> & vPoint )
{
  m_aCorners[nIndex] = vPoint;  
}
/////////////////////////////////////////////////////////////////
