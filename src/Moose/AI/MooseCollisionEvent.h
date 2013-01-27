/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
