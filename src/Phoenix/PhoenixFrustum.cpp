#include "PhoenixFrustum.h"
#include "PhoenixCollision.h"
#include <list>
/////////////////////////////////////////////////////////////////
using std::list;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrustum::CFrustum()
{
  // Add planes to the list 
  CPlane top,bottom,left,right,near,far;

  AddPlane( top );
  AddPlane( bottom );
  AddPlane( left );
  AddPlane( right );
  AddPlane( near );
  AddPlane( far );

  // Make pointers to the planes
  list<CPlane>::iterator it = GetPlanes().begin();
  
  m_aPlanes[TOP]    = &(*it); it++;
  m_aPlanes[BOTTOM] = &(*it); it++;  
  m_aPlanes[LEFT]   = &(*it); it++;  
  m_aPlanes[RIGHT]  = &(*it); it++;  
  m_aPlanes[BACK]   = &(*it); it++;  
  m_aPlanes[FRONT]    = &(*it); 

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CFrustum::SetPlane(FRUSTUM_PLANE nIndex, CPlane &vPlane)
{
  *(m_aPlanes[nIndex]) = vPlane;
}
/////////////////////////////////////////////////////////////////
CPlane &
Phoenix::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex )
{
  return *(m_aPlanes[nIndex]);
}
/////////////////////////////////////////////////////////////////
const CPlane &
Phoenix::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex ) const
{
  return *(m_aPlanes[nIndex]);
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
