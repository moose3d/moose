#include "PhoenixModelLoader.h"
/////////////////////////////////////////////////////////////////
#define DELETE(OBJ) if(OBJ != NULL ) delete OBJ; OBJ=NULL;
/////////////////////////////////////////////////////////////////
Phoenix::Data::CModelLoader::~CModelLoader()
{
  DELETE(m_pPositions);
  DELETE(m_pNormals);
  DELETE(m_pColors);
  DELETE(m_pTexCoords);
  DELETE(m_pIndices);
  // Release group names
  while( !m_mapGroups.empty() )
  {
    DELETE ((*m_mapGroups.begin()).second);
    m_mapGroups.erase( m_mapGroups.begin());
  }
}
/////////////////////////////////////////////////////////////////
