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

#ifndef __MooseThreadedTask_h__
#define __MooseThreadedTask_h__
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <SDL_thread.h>
#include <MooseStateBase.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Core
    {
        enum ThreadState { kInit, kRunning, kDone, kFinalized };
        ////////////////////
        /// Interface for background tasks.
        class IThreadedTask : public Moose::Core::CStateBase<Moose::Core::ThreadState>
        {
        private:
            SDL_Thread *m_pThread; ///< Thread pointer.
        public:
            ////////////////////
            /// Constructor.
            IThreadedTask();
            ////////////////////
            /// Destructor.
            virtual ~IThreadedTask();
            ////////////////////
            /// Invokes thread.
            void Start();
            ////////////////////
            /// Stops thread.
            void Stop();
            ////////////////////
            /// Implement activity into this in child class.
            virtual void Run() = 0;
            ////////////////////
            /// Invoked when thread exits.
            virtual void OnFinish() {}
        private:
            ////////////////////
            /// Wraps thread creation and execution to Run().
            static int Execute( void *data ); 
        };
    } // Core
} // Moose
////////////////////////////////////////////////////////////////////////////////
#endif
