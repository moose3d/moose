/*
 *  MooseEventQueue.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 7/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __MooseEventQueue_h__
#define __MooseEventQueue_h__
#include <MooseAPI.h>
#if defined(MOOSE_APPLE_IPHONE)
#include <dispatch/dispatch.h>
#endif
#include <queue>
namespace Moose
{
    namespace Scene
    {
        
#ifdef MOOSE_APPLE_IPHONE
        enum EventType {
            kTouchBegan,
            kTouchMoved,
            kTouchEnded
        };
        
        struct Event {
            int   type;
            float x;
            float y;
            int   flags;
        };
#else
#include <SDL.h>
        typedef Uint8 EventType;
        typedef SDL_Event Event;
        typedef int dispatch_semaphore_t;
#endif 
            // Abstracted eventqueue.
        class CEventQueue : protected std::queue<Event>
        {
#if !defined(MOOSE_APPLE_IPHONE)
        protected:
            dispatch_semaphore_t m_Lock;
        public:
            CEventQueue();
            virtual ~CEventQueue();
            void Push( const Event & e );
            void Pop();
            bool Empty() const;
            Event Front() const;
#endif
        };
    } // Scene
} // Moose
#endif
