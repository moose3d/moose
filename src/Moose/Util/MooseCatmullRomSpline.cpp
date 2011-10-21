#include "MooseCatmullRomSpline.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Math;
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float> 
Moose::Util::CCatmullRomSpline::GetPosition( float t ) const
{
  
  int seg = std::max(0.0f,ceilf(t - 1.0f));
  float fT = t - (float)((int)t);
  CVector3<float> vPos[4] = { this->at(seg), this->at(1+seg), this->at(2+seg), this->at(3+seg) };
  
  return 0.5f * (  (2.0f * vPos[1] ) +
                 ( -vPos[0] + vPos[2]  ) * fT +
                 ( 2.0f*vPos[0] - 5.0f * vPos[1] + 4.0f* vPos[2] -  vPos[3]) * fT*fT +
                 (     -vPos[0] + 3.0f * vPos[1] - 3.0f* vPos[2] +  vPos[3]) * fT*fT*fT);
}
////////////////////////////////////////////////////////////////////////////////
