#include "PhoenixObjectUpdater.h"
////////////////////////////////////////////////////////////////////////////////
Phoenix::Core::CObjectUpdater::~CObjectUpdater()
{
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    delete m_vecUpdateables[n];
  }
  m_vecUpdateables.clear();
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Core::CObjectUpdater::Update( float fSecondsPassed  )
{
  UpdateableVector::iterator it = m_vecUpdateables.begin();
  while( it != m_vecUpdateables.end() )
  {
	if ( (*it)->IsNull())
    {
      delete (*it);
      it = m_vecUpdateables.erase(it);
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
  return m_vecUpdateables.size();
}
////////////////////////////////////////////////////////////////////////////////
