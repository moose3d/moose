/*
 *  PhoenixCollisionEvent.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __PhoenixCollisionEvent_h__
#define __PhoenixCollisionEvent_h__
#include "PhoenixMessageSystem.h"
namespace Phoenix
{
    namespace Scene
    {
        class CGameObject;
    }
    namespace AI
    {
        /// Generic Collision event.
        class CCollisionEvent : public Phoenix::AI::CMessage
        {
        protected:
            /// The colliding object.
            Phoenix::Scene::CGameObject *m_pCollider;
            CCollisionEvent( Phoenix::Scene::CGameObject *pCollider ) : m_pCollider(pCollider) {}
        public:
            virtual ~CCollisionEvent() {}
            Phoenix::Scene::CGameObject * GetCollider() const { return m_pCollider;}
        };
        // Upon collision start
        class CCollisionEnter : public Phoenix::AI::CCollisionEvent 
        {
        public:
            CCollisionEnter( Phoenix::Scene::CGameObject *pCollider ) : Phoenix::AI::CCollisionEvent(pCollider) {}
        };
        // Upon collsion continuing
        class CCollisionStay : public Phoenix::AI::CCollisionEvent 
        {
        public:
            CCollisionStay( Phoenix::Scene::CGameObject *pCollider ) : Phoenix::AI::CCollisionEvent(pCollider) {}
        };
        // Upon collision end
        class CCollisionExit : public Phoenix::AI::CCollisionEvent 
        {
        public:
            CCollisionExit( Phoenix::Scene::CGameObject *pCollider ) : Phoenix::AI::CCollisionEvent(pCollider) {}
        };
        
    } // Scene
} // Phoenix
#endif
