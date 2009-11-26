#include "PhoenixObjectUpdater.h"
////////////////////////////////////////////////////////////////////////////////
Phoenix::Core::CObjectUpdater::~CObjectUpdater()
{
  while( m_lstUpdateables.empty() == false )
  {
  	IHandlerBase * pTmp = m_lstUpdateables.front();
		m_lstUpdateables.pop_front();
  	delete pTmp;
  }

}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Core::CObjectUpdater::Update( float fSecondsPassed  )
{
  UpdateableList::iterator it = m_lstUpdateables.begin();
  while( it != m_lstUpdateables.end() )
  {
	if ( (*it)->IsNull())
    {
      delete (*it);
      it = m_lstUpdateables.erase(it);
    }
    else
    {
      (*it)->Update( fSecondsPassed );
      ++it;
    }

  }
}
////////////////////////////////////////////////////////////////////////////////
size_t 
Phoenix::Core::CObjectUpdater::GetUpdateableCount() const
{
  return m_lstUpdateables.size();
}
////////////////////////////////////////////////////////////////////////////////
