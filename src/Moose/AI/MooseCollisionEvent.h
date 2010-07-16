/*
 *  MooseCollisionEvent.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MooseCollisionEvent_h__
#define __MooseCollisionEvent_h__
#include "MooseMessageSystem.h"
namespace Moose
{
    namespace Scene
    {
        class CGameObject;
    }
    namespace AI
    {
        /// Generic Collision event.
        class CCollisionEvent : public Moose::AI::CMessage
        {
        protected:
            /// The colliding object.
            Moose::Scene::CGameObject *m_pCollider;
            CCollisionEvent( Moose::Scene::CGameObject *pCollider ) : m_pCollider(pCollider) {}
        public:
            virtual ~CCollisionEvent() {}
            Moose::Scene::CGameObject * GetCollider() const { return m_pCollider;}
        };
        // Upon collision start
        class CCollisionEnter : public Moose::AI::CCollisionEvent 
        {
        public:
            CCollisionEnter( Moose::Scene::CGameObject *pCollider ) : Moose::AI::CCollisionEvent(pCollider) {}
        };
        // Upon collsion continuing
        class CCollisionStay : public Moose::AI::CCollisionEvent 
        {
        public:
            CCollisionStay( Moose::Scene::CGameObject *pCollider ) : Moose::AI::CCollisionEvent(pCollider) {}
        };
        // Upon collision end
        class CCollisionExit : public Moose::AI::CCollisionEvent 
        {
        public:
            CCollisionExit( Moose::Scene::CGameObject *pCollider ) : Moose::AI::CCollisionEvent(pCollider) {}
        };
        
    } // Scene
} // Moose
#endif
