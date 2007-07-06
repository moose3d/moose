#include "GSE_IlluminationSet.h"
#include "GSE_GraphNodes.h"
#include <algorithm>
#include <string.h>
/// The light distance sorting function /////////////////////////
bool AscendingByAttenuation( GSE_Light *pLightA, GSE_Light *pLightB ) 
{
  if ( pLightA->IsEnabled() && !pLightB->IsEnabled() )     return 1;
  else if ( !pLightA->IsEnabled() && pLightB->IsEnabled()) return 0;
  else       return pLightA->m_fBrightness < pLightB->m_fBrightness;
}              
/////////////////////////////////////////////////////////////////
void
GSE_IlluminationSet::Sort( const GSE_Vector3 &vPosition )
{
  m_vSorted.clear();
  m_vSorted.assign(m_setLights.begin(), m_setLights.end());
  // Calculate brightness of lights 
  vector<GSE_Light *>::iterator it = m_vSorted.begin();
  
  float fDistance = 0.0f;
  
  for(; it != m_vSorted.end(); it++)
  {
    fDistance = (vPosition - (*it)->GetPosition()).Length();
    // Calculate the attenuation factor (not the brightness)
    (*it)->m_fBrightness = (*it)->m_fConstantAttenuation + 
                          (fDistance * (*it)->m_fLinearAttenuation) +
                          (fDistance*fDistance * (*it)->m_fQuadraticAttenuation );
  }
  
  
  // Sort Lights according to attenuation
  sort( m_vSorted.begin(), m_vSorted.end(), AscendingByAttenuation );
  
}
/////////////////////////////////////////////////////////////////
void
GSE_IlluminationSet::Apply( GSE_LightClusterNode *pLightCluster, 
			    const GSE_Vector3 & vPosition )
{
  Sort(vPosition);
  // Up to GSE_MAX_ACTIVE_LIGHTS lights are added to cluster.
  vector<GSE_Light *>::iterator it = m_vSorted.begin();
  unsigned int nLight=0;
  for(; nLight<GSE_MAX_ACTIVE_LIGHTS && nLight < m_vSorted.size();
      nLight++,it++)
  {
    pLightCluster->SetLight(nLight, *it);
  }
  for( ; nLight<GSE_MAX_ACTIVE_LIGHTS; nLight++)
  {
    pLightCluster->SetLight(nLight, NULL );
  }
}
/////////////////////////////////////////////////////////////////
void
GSE_IlluminationSet::AddLight( GSE_Light *pLight )
{
  m_setLights.insert(pLight);
}
/////////////////////////////////////////////////////////////////
void
GSE_IlluminationSet::RemoveLight( GSE_Light *pLight )
{
  set<GSE_Light *>::iterator it; 
  if ( (it = m_setLights.find(pLight)) != m_setLights.end() )
  {
    m_setLights.erase( it );
  }
}
/////////////////////////////////////////////////////////////////
GSE_IlluminationSet *
GSE_IlluminationSetMgr::CreateIlluminationSet(const char *szName)
{
  GSE_IlluminationSet *pSet  = NULL;
  if ( szName != NULL && strlen(szName)  > 0 )
  {

    // If material with same name is found, return it.
    if ( (pSet = Find( string(szName))) == NULL )
    {
      pSet = new GSE_IlluminationSet();
           
      // Insert material name into the map
      GSE_Mapper<string,GSE_IlluminationSet>::Map( string(string(szName)), pSet);
      // Add material into the container
      GSE_Container<GSE_IlluminationSet>::Add( pSet );
      
    } 
    else 
    {
      GSE_WARN("IlluminationSet '" << szName  << "' already exists, using it..");
    }
  } 
  else
  {
    GSE_WARN("IlluminationSet name empty, cannot create.");
  }
  return pSet;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_IlluminationSet.cpp,v $
// Revision 1.2  2007/03/16 08:12:58  entity
// CreateIlluminationSet added
//
// Revision 1.1  2007/03/15 21:01:59  entity
// Initial import to CVS
//
/////////////////////////////////////////////////////////////////
