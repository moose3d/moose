#include "PhoenixEventHandler.h"
/////////////////////////////////////////////////////////////////
Phoenix::Core::CEventHandler::~CEventHandler()
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
Phoenix::Core::CEventHandler::HandleEvent( const Phoenix::Core::CEvent *pEvent )
{
  // 
  HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pEvent)));
  if ( it != m_mapHandlers.end())
  {
    it->second->Exec(pEvent);
  }
}
/////////////////////////////////////////////////////////////////
