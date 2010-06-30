/*
 *  PhoenixCollisionEvent.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "PhoenixMessageSystem.h"
namespace Phoenix
{
    namespace Scene
    {
        class CGameObject;
    }
    namespace AI
    {
        /// Collision event for entering, colliding, and not colliding.
        /// Separated by function call (OnCollisionEnter, OnCollision, OnCollisionExit).
        class CCollisionEvent : public Phoenix::AI::CMessage
        {
        protected:
            /// The colliding object.
            Phoenix::Scene::CGameObject *m_pCollider;
        public:
            CCollisionEvent( Phoenix::Scene::CGameObject *pCollider ) : m_pCollider(pCollider) {}
            virtual ~CCollisionEvent() {}
        };
    } // Scene
} // Phoenix