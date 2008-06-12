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
Phoenix::AI::CMessageQueue::EnqueueMessage( Phoenix::AI::CMessageBase *pMessage, const Phoenix::Core::CTimeStamp & tTimeStamp )
{
  pMessage->SetTimeStamp( GetCurrentTime()+tTimeStamp );
  m_priqMessages.push( pMessage );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::AI::CMessageQueue::Update(  )
{
  Phoenix::Core::CTimer::Reset();
  Phoenix::AI::CMessageBase *pMessage = NULL;

  while ( (pMessage = GetNextMessage()) != NULL )
  {
    HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pMessage)));
    if ( it != m_mapHandlers.end())
    {
      (*it).second->SendMessage( pMessage );
    }
    delete pMessage;
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CMessageBase * 
Phoenix::AI::CMessageQueue::GetNextMessage()
{
  // Check are there any messages left
  if ( m_priqMessages.empty()) return NULL;
  
  Phoenix::AI::CMessageBase * pMessage = m_priqMessages.top();
  //std::cerr << "Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetCurrentTime() )
  {
    m_priqMessages.pop();
    return pMessage;
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
