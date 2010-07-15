/*
 *  PhoenixEventQueue.cpp
 *  iPhoneMoose
 *
 *  Created by eNtity on 7/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
// is not used without iPhoneMoose
#if defined(PHOENIX_APPLE_IPHONE)
#include "PhoenixEventQueue.h"
using namespace std;
namespace prefix = Phoenix::Scene;
#define NUM_RESOURCES 1
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
#endif
