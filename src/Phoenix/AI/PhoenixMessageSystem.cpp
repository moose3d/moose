#include "PhoenixMessageSystem.h"
/////////////////////////////////////////////////////////////////
Phoenix::AI::CMessageQueue::~CMessageQueue()
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
Phoenix::AI::CMessageQueue::EnqueueMessage( Phoenix::AI::CMessage *pMessage, const Phoenix::Core::CTimeStamp & tTimeStamp )
{
  pMessage->SetTimeStamp( GetTime()+tTimeStamp );
  m_priqMessages.push( pMessage );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::AI::CMessageQueue::Update(  )
{
  Phoenix::Core::CTimer::Reset();
  Phoenix::AI::CMessage *pMessage = NULL;

  while ( (pMessage = GetNextMessage()) != NULL )
  {
    HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pMessage)));
    if ( it != m_mapHandlers.end())
    {
      (*it).second->SendMessage( pMessage );
    }
    delete pMessage;
  }
  
  std::pair< Phoenix::AI::CHandlerFunctionBase *, Phoenix::AI::CMessage *> *pPair;
  while ( (pPair = GetNextSoleMessage())  != NULL )
  {
    pPair->first->Call(pPair->second);
    // delete message
    delete pPair->second;
    // delete pair
    delete pPair;
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CMessage * 
Phoenix::AI::CMessageQueue::GetNextMessage()
{
  // Check are there any messages left
  if ( m_priqMessages.empty()) return NULL;
  
  Phoenix::AI::CMessage * pMessage = m_priqMessages.top();
  //std::cerr << "Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetTime() )
  {
    m_priqMessages.pop();
    return pMessage;
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////
std::pair< Phoenix::AI::CHandlerFunctionBase *, Phoenix::AI::CMessage *> *
Phoenix::AI::CMessageQueue::GetNextSoleMessage()
{
  // Check are there any messages left
  if ( m_priqSoleMessages.empty()) return NULL;
  

  std::pair< Phoenix::AI::CHandlerFunctionBase *, Phoenix::AI::CMessage *> *pPair = m_priqSoleMessages.top();
  Phoenix::AI::CMessage * pMessage = pPair->second;

  //std::cerr << "(SoleMessage) : Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetTime() )
  {
    m_priqSoleMessages.pop();
    return pPair;
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CMessageQueue::Prepare()
{
  Phoenix::Core::CTimer::Reset();
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CHandlerList::~CHandlerList()
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
Phoenix::AI::CHandlerList::PushReceiver( const Phoenix::AI::CHandlerFunctionBase *pHandler  )
{
  m_lstHandlers.push_back( const_cast<Phoenix::AI::CHandlerFunctionBase *>(pHandler) );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CHandlerList::RemoveReceiver( const Phoenix::AI::CHandlerFunctionBase *pHandler )
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
Phoenix::AI::CHandlerList::SendMessage( const Phoenix::AI::CMessage *pMessage )
{
  Handlers::iterator it = m_lstHandlers.begin();
  // send message to each object listening
  for( ; it != m_lstHandlers.end(); it++)
  {
    (*it)->Call( pMessage );
  }

}
/////////////////////////////////////////////////////////////////
