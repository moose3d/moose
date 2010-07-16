#include "MooseFlock.h"
#include <algorithm>
#include "MooseCollision.h"
namespace libname = Moose::AI;
using namespace Moose::Collision;
using namespace Moose::AI;
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
libname::CFlock::CFlock()
{
  
}
/////////////////////////////////////////////////////////////////
libname::CFlock::~CFlock()
{
  
}
/////////////////////////////////////////////////////////////////
void
libname::CFlock::AddToFlock( CBoid *pBoid )
{
  m_vecBoids.push_back(pBoid);
}
/////////////////////////////////////////////////////////////////
void
libname::CFlock::RemoveFromFlock( CBoid *pBoid )
{
  BoidPtrVector::iterator result;
  result = std::find( m_vecBoids.begin(), m_vecBoids.end(), pBoid );
  if ( result != m_vecBoids.end())
    m_vecBoids.erase( result );
}
/////////////////////////////////////////////////////////////////
void
libname::CFlock::Update()
{
  BoidPtrVector::iterator it;
  CBoid *boid;
  BoidPtrList local;
  CVector3<float> vTmp;
  CVector3<float> vAcc;
  CVector3<float> vMin;
  CVector3<float> vMax;
  bool first = true;
  for( it = m_vecBoids.begin(); it != m_vecBoids.end(); it++)
  {
    boid = *it;
    LocateNearestBoids( boid, local);
    boid->CalculateSeparation(local, vTmp);

    vAcc = vTmp;

    boid->CalculateCohesion(local, vTmp);
    vAcc += vTmp;
    boid->CalculateAlignment(local, vTmp);
    vAcc += vTmp;
    vAcc += GetTarget()-boid->GetPosition();
    
    boid->SetSpeed(vAcc.Length());
    boid->SetDirection( vAcc );
    if ( first )
    {
      vMin = boid->GetPosition();
      vMax = boid->GetPosition();
    }
    else 
    {
      if ( boid->GetPosition()[0] < vMin[0] ) vMin[0] = boid->GetPosition()[0];
      if ( boid->GetPosition()[1] < vMin[1] ) vMin[1] = boid->GetPosition()[1];
      if ( boid->GetPosition()[2] < vMin[2] ) vMin[2] = boid->GetPosition()[2];
      if ( boid->GetPosition()[0] > vMax[0] ) vMax[0] = boid->GetPosition()[0];
      if ( boid->GetPosition()[1] > vMax[1] ) vMax[1] = boid->GetPosition()[1];
      if ( boid->GetPosition()[2] > vMax[2] ) vMax[2] = boid->GetPosition()[2];
    }
  }
  m_BoundingSphere.SetRadius( (vMax-vMin).Length()*0.5f);
  m_BoundingSphere.SetPosition( (vMin+vMax)*0.5f);
}
/////////////////////////////////////////////////////////////////
Moose::Volume::CSphere &
libname::CFlock::GetBoundingSphere() 
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
void
libname::CFlock::LocateNearestBoids( const CBoid *pBoid, BoidPtrList & lstClosest )
{
  // Clear previous list
  lstClosest.clear();

  BoidPtrVector::iterator it;
  for( it = m_vecBoids.begin(); it != m_vecBoids.end(); it++)
  {
    if (  pBoid != *it && SphereIntersectsSphere( (*it)->GetNeighborSphere(), pBoid->GetNeighborSphere()) )
    {
      lstClosest.push_back( (*it) );
    }
  }
}
/////////////////////////////////////////////////////////////////
void 
libname::CFlock::SetTarget( const Moose::Math::CVector3<float> & vTargetPos )
{
  m_vTarget = vTargetPos;
}
/////////////////////////////////////////////////////////////////
const CVector3<float> & 
libname::CFlock::GetTarget() const
{
  return m_vTarget;
}
/////////////////////////////////////////////////////////////////
BoidPtrVector & 
libname::CFlock::GetBoids()
{
  return m_vecBoids;
}
/////////////////////////////////////////////////////////////////
libname::CBoid::CBoid() : m_fSpeed(0.0), m_fMaxSpeed(1.0f)
{
  
}
/////////////////////////////////////////////////////////////////
libname::CBoid::~CBoid()
{
  
}
/////////////////////////////////////////////////////////////////
void
libname::CBoid::SetSpeed( float fSpeed)
{
  m_fSpeed = fSpeed > GetMaxSpeed() ? GetMaxSpeed() : fSpeed;

}
/////////////////////////////////////////////////////////////////
float
libname::CBoid::GetSpeed()
{
  return m_fSpeed;
}
/////////////////////////////////////////////////////////////////
void
libname::CBoid::SetMaxSpeed( float fSpeed)
{
  m_fMaxSpeed = fSpeed;
}
/////////////////////////////////////////////////////////////////
float
libname::CBoid::GetMaxSpeed()
{
  return m_fMaxSpeed;
}
/////////////////////////////////////////////////////////////////
void
libname::CBoid::SetIndex( int iIndex )
{
  m_iIndex = iIndex;
}
/////////////////////////////////////////////////////////////////
int
libname::CBoid::GetIndex() const
{
  return m_iIndex;
}
/////////////////////////////////////////////////////////////////
bool
libname::CBoid::BelongsToFlock() const
{
  return (m_iIndex > -1 );
}
/////////////////////////////////////////////////////////////////
bool
libname::CBoid::CalculateSeparation( const BoidPtrList & lstClosest, CVector3<float> & vec ) 
{
  bool retval = false;
  if ( lstClosest.size() == 0)  return retval;
  
  CVector3<float> vLocalAvgToNeighbors(0.0f,0.0f,0.0f);
  BoidPtrList::const_iterator it = lstClosest.begin();
  CVector3<float> vTmp;
  for( ;it != lstClosest.end();it++)
  {
    vTmp =  (*it)->GetPosition() - GetPosition();
    if ( vTmp.LengthSqr() < 5.0f )
    {
      vLocalAvgToNeighbors += vTmp;
      retval = true;
    }
  }
  vLocalAvgToNeighbors /= lstClosest.size();
  vec = -vLocalAvgToNeighbors;
  return retval;
}
/////////////////////////////////////////////////////////////////
void
libname::CBoid::CalculateAlignment( const BoidPtrList & lstClosest, CVector3<float> & vec ) 
{
  if ( lstClosest.size() == 0)   return;
  
  CVector3<float> vLocalAvgDir(0.0f,0.0f,0.0f);
  BoidPtrList::const_iterator it = lstClosest.begin();
  for( ;it != lstClosest.end();it++)
  {
    vLocalAvgDir+= (*it)->GetDirection();
  }
  vLocalAvgDir /= lstClosest.size();
  vec = vLocalAvgDir;
}
/////////////////////////////////////////////////////////////////
void
libname::CBoid::CalculateCohesion( const BoidPtrList & lstClosest, CVector3<float> & vec ) 
{
  if ( lstClosest.size() == 0)   return;
  
  CVector3<float> vLocalAvgPos(0.0f,0.0f,0.0f);
  BoidPtrList::const_iterator it = lstClosest.begin();
  for( ;it != lstClosest.end();it++)
  {
    vLocalAvgPos += (*it)->GetPosition();
  }

  vLocalAvgPos /= lstClosest.size();
  vec = vLocalAvgPos - GetPosition();
  
}
/////////////////////////////////////////////////////////////////
const Moose::Volume::CSphere & 
libname::CBoid::GetNeighborSphere() const
{
  return m_NeighborSphere; 
}
/////////////////////////////////////////////////////////////////
