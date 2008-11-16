#include "PhoenixObjectUpdater.h"
/////////////////////////////////////////////////////////////////
Phoenix::Core::CObjectUpdater::~CObjectUpdater()
{
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    delete m_vecUpdateables[n];
  }
  m_vecUpdateables.clear();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CObjectUpdater::Update( float fSecondsPassed  )
{
  UpdateableVector::iterator it = m_vecUpdateables.begin();
  for( ; it != m_vecUpdateables.end();it++)
  {
    if ( (*it)->IsNull())
    {
      delete (*it);
      m_vecUpdateables.erase(it);
    } 
    else
    {
      (*it)->Update( fSecondsPassed );
    }
  } 
}
/////////////////////////////////////////////////////////////////
