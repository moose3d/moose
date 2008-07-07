#include "PhoenixGameObject.h"
#include "PhoenixOGLRenderer.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Scene;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void 
Phoenix::Scene::CGameObject::UpdateTransforms()
{
  RenderableHandleList::iterator it;
  for( size_t i=0;i< m_LodLevels.size(); i++ )
  {
    it = m_LodLevels[i].begin();
    for( ; it != m_LodLevels[i].end(); it++)
    {  
      
      (*(*it))->SetWorldTransform( GetWorldTransform());
      (*(*it))->SetLocalTransform( GetLocalTransform());
    }					       
  }
}
/////////////////////////////////////////////////////////////////
