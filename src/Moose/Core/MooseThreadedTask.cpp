#include "MooseThreadedTask.h"
#include "MooseExceptions.h"
using namespace Moose::Exceptions;
using namespace Moose::Core;
////////////////////////////////////////////////////////////////////////////////
Moose::Core::IThreadedTask::IThreadedTask() : m_pThread(NULL)
{
    SetState(kInit);
}
////////////////////////////////////////////////////////////////////////////////
Moose::Core::IThreadedTask::~IThreadedTask() 
{
    SDL_WaitThread(m_pThread, NULL);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Core::IThreadedTask::Start() 
{
    SetState(kRunning);
    m_pThread = SDL_CreateThread( IThreadedTask::Execute, this);
    if ( m_pThread == NULL ) 
        throw CMooseRuntimeError("Cannot create threaded task!");
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Core::IThreadedTask::Stop()
{
    SetState(kDone);
}
////////////////////////////////////////////////////////////////////////////////
int
Moose::Core::IThreadedTask::Execute( void *data ) 
{
    reinterpret_cast<IThreadedTask *>(data)->Run();
    reinterpret_cast<IThreadedTask *>(data)->SetState(kDone);
    reinterpret_cast<IThreadedTask *>(data)->OnFinish();
    reinterpret_cast<IThreadedTask *>(data)->SetState(kFinalized);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
