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

#include "MooseMessageSystem.h"
/////////////////////////////////////////////////////////////////
Moose::AI::CMessageQueue::~CMessageQueue()
{
  HandlerMap::iterator it = m_mapHandlers.begin();
  while( it != m_mapHandlers.end())
  {
    delete it->second;
    ++it;
  }
  m_mapHandlers.clear();
}
/////////////////////////////////////////////////////////////////
void 
Moose::AI::CMessageQueue::EnqueueMessage( Moose::AI::CMessage *pMessage, const Moose::Core::CTimeStamp & tTimeStamp )
{
  pMessage->SetTimeStamp( GetTime()+tTimeStamp );
  m_priqMessages.push( pMessage );
}
/////////////////////////////////////////////////////////////////
void 
Moose::AI::CMessageQueue::Update(  )
{
  Moose::Core::CTimer::Reset();
  Moose::AI::CMessage *pMessage = NULL;

  while ( (pMessage = GetNextMessage()) != NULL )
  {
    HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pMessage)));
    if ( it != m_mapHandlers.end())
    {
      (*it).second->SendMessage( pMessage );
    }
    delete pMessage;
  }
  
  /*std::pair< Moose::AI::CHandlerFunctionBase *, Moose::AI::CMessage *> *pPair;
  while ( (pPair = GetNextSoleMessage())  != NULL )
  {
    pPair->first->Call(pPair->second);
    // delete message
    delete pPair->second;
    // delete pair
    delete pPair;
    }*/
}
/////////////////////////////////////////////////////////////////
Moose::AI::CMessage * 
Moose::AI::CMessageQueue::GetNextMessage()
{
  // Check are there any messages left
  if ( m_priqMessages.empty()) return NULL;
  
  Moose::AI::CMessage * pMessage = m_priqMessages.top();
  //std::cerr << "Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetTime() )
  {
    m_priqMessages.pop();
    return pMessage;
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////
/*std::pair< Moose::AI::CHandlerFunctionBase *, Moose::AI::CMessage *> *
Moose::AI::CMessageQueue::GetNextSoleMessage()
{
  // Check are there any messages left
  if ( m_priqSoleMessages.empty()) return NULL;
  

  std::pair< Moose::AI::CHandlerFunctionBase *, Moose::AI::CMessage *> *pPair = m_priqSoleMessages.top();
  Moose::AI::CMessage * pMessage = pPair->second;

  //std::cerr << "(SoleMessage) : Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetTime() )
  {
    m_priqSoleMessages.pop();
    return pPair;
  }
  return NULL;
  }*/
/////////////////////////////////////////////////////////////////
void
Moose::AI::CMessageQueue::Prepare()
{
  Moose::Core::CTimer::Reset();
}
/////////////////////////////////////////////////////////////////
Moose::AI::CHandlerList::~CHandlerList()
{
  Handlers::iterator it = m_lstHandlers.begin();

  // Release handles 
  while( it != m_lstHandlers.end() )
  {
    // CHECK THAT THIS WORKS!!! 
    delete *it;
    ++it;
  }
  // cleanup
  m_lstHandlers.clear();
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CHandlerList::PushReceiver( const Moose::AI::CHandlerFunctionBase *pHandler  )
{
  m_lstHandlers.push_back( const_cast<Moose::AI::CHandlerFunctionBase *>(pHandler) );
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CHandlerList::RemoveReceiver( const Moose::AI::CHandlerFunctionBase *pHandler )
{
  Handlers::iterator it = m_lstHandlers.begin();
  
  // Release handle if found.
  for( ; it != m_lstHandlers.end(); it++)
  {
    if ( pHandler == *it )
    {
      delete *it;
      m_lstHandlers.erase( it );
      break;
    }
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CHandlerList::SendMessage( const Moose::AI::CMessage *pMessage )
{
  Handlers::iterator it = m_lstHandlers.begin();
  // send message to each object listening
  for( ; it != m_lstHandlers.end(); it++)
  {
    (*it)->Call( pMessage );
  }

}
/////////////////////////////////////////////////////////////////
