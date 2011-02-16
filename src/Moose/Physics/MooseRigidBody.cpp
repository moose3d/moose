/*
 *  MooseRigidBody.cpp
 *  Moose
 *
 *  Created by eNtity on 12/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


void SetRigidBody( btRigidBody *pBody )
{
    m_pRigidBody = pBody;
}



btRigidBody * GetRigidBody()
{
    return m_pRigidBody;
}
