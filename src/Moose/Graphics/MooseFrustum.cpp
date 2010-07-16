#include "MooseFrustum.h"
#include "MooseCollision.h"
#include <vector>
/////////////////////////////////////////////////////////////////
using std::vector;
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Volume;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFrustum::CFrustum()
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
Moose::Graphics::CFrustum::SetPlane(FRUSTUM_PLANE nIndex, CPlane &vPlane)
{
  m_Planes[nIndex] = vPlane;
}
/////////////////////////////////////////////////////////////////
CPlane &
Moose::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex )
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
const CPlane &
Moose::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex ) const
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
CVector3<float>
Moose::Graphics::CFrustum::GetCorner( FRUSTUM_CORNER nIndex )
{
  return m_aCorners[nIndex];
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CFrustum::SetCorner( FRUSTUM_CORNER nIndex, const CVector3<float> & vPoint )
{
  m_aCorners[nIndex] = vPoint;  
}
/////////////////////////////////////////////////////////////////
