/*
 *  MoosePhysicsWorld.cpp
 *  Moose
 *
 *  Created by eNtity on 12/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "MoosePhysicsWorld.h"
/////////////////////////////////////////////////////////////////
#define TO_BTVEC3( VEC ) (btVector3( VEC[0], VEC[1], VEC[2]))
//////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Physics;
prefix::CPhysicsWorld::CPhysicsWorld()
{
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pCollisionDispatcher    = new btCollisionDispatcher(m_pCollisionConfiguration);
    m_pSolver		      = new btSequentialImpulseConstraintSolver;
    m_iMaxProxies	      = 1024;
    
    
    m_pOverlappingPairCache = new btAxisSweep3( TO_BTVEC3(m_WorldAABBMin),
                                               TO_BTVEC3(m_WorldAABBMax),
                                               m_iMaxProxies );  
    m_pDynamicsWorld= new btDiscreteDynamicsWorld(m_pCollisionDispatcher,
                                                  m_pOverlappingPairCache,
                                                  m_pSolver,
                                                  m_pCollisionConfiguration);
    
    m_pDynamicsWorld->setGravity(btVector3(0,-10.0,0));
    
}
//////////////////////////////////////////////////////////////////////
prefix::CPhysicsWorld::~CPhysicsWorld()
{

}
//////////////////////////////////////////////////////////////////////
void 
prefix::CPhysicsWorld::SetGravity( const Moose::Math::CVector3<float> & vGravity )
{
    m_pDynamicsWorld->setGravity(btVector3(vGravity[0],vGravity[1],vGravity[2]));
}
//////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float> 
prefix::CPhysicsWorld::GetGravity()
{
    btVector3 vec = m_pDynamicsWorld->getGravity();
    return Moose::Math::CVector3<float>(vec[0], vec[1], vec[2]);
}
//////////////////////////////////////////////////////////////////////
