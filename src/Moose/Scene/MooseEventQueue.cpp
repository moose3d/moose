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
 *  MooseEventQueue.cpp
 *  iPhoneMoose
 *
 *  Created by eNtity on 7/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
// is not used without iPhoneMoose

#include "MooseEventQueue.h"
using namespace std;
namespace prefix = Moose::Scene;
#define NUM_RESOURCES 1

#if defined(MOOSE_APPLE_IPHONE)
prefix::CEventQueue::CEventQueue()
{
    m_Lock = dispatch_semaphore_create(NUM_RESOURCES);
}

prefix::CEventQueue::~CEventQueue()
{
    dispatch_release(m_Lock);
}

void
prefix::CEventQueue::Push( const Event & e )
{
    dispatch_semaphore_wait(m_Lock,DISPATCH_TIME_FOREVER);
    push( e );
    dispatch_semaphore_signal(m_Lock);
}

void
prefix::CEventQueue::Pop()
{
    dispatch_semaphore_wait(m_Lock,DISPATCH_TIME_FOREVER);
    pop();
    dispatch_semaphore_signal(m_Lock);
}

prefix::Event 
prefix::CEventQueue::Front() const 
{
    dispatch_semaphore_wait(m_Lock,DISPATCH_TIME_FOREVER);
    Event e = front();
    dispatch_semaphore_signal(m_Lock);    
    return e;
}

bool
prefix::CEventQueue::Empty() const
{
    dispatch_semaphore_wait(m_Lock,DISPATCH_TIME_FOREVER);
    bool isEmpty = empty();
    dispatch_semaphore_signal(m_Lock);    
    return isEmpty;
}

void
prefix::CEventQueue::Clear() 
{
    dispatch_semaphore_wait(m_Lock,DISPATCH_TIME_FOREVER);
    while( !empty()) pop();
    dispatch_semaphore_signal(m_Lock);
}
#endif
