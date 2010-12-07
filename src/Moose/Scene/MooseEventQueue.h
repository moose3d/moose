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
            kTouchEnded,
            kAccelerate
        };
#define MAX_TOUCH_PER_EVENT 5
      struct Event 
      {
        char   type;
        char  touches; ///!< Number of touches in this event.
        float x[MAX_TOUCH_PER_EVENT];
        float y[MAX_TOUCH_PER_EVENT];
        float z;
        int   flags;
        Event() {}
        Event( const Event & e )
        {
          *this = e;
        }
        void operator=( const Event & e )
        {
          type = e.type;
          touches = e.touches;
          // kAccelerate might leave touches as zero, while still using first two values.
          x[0] = e.x[0];
          y[0] = e.y[0];
          
          for(int i=1;i<e.touches;i++)
          {
            x[i] = e.x[i];
            y[i] = e.y[i];
          }
          z = e.z;
          flags = e.flags;
        }
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
#if defined(MOOSE_APPLE_IPHONE)
        protected:
            dispatch_semaphore_t m_Lock;
        public:
            CEventQueue();
            virtual ~CEventQueue();
            void Push( const Event & e );
            void Pop();
            bool Empty() const;
            Event Front() const;
            void Clear();
#endif
        };
    } // Scene
} // Moose
#endif
