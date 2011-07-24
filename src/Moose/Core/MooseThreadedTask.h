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
