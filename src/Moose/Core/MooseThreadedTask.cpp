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
