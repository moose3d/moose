#include "PhoenixModelLoader.h"
/////////////////////////////////////////////////////////////////
Phoenix::Data::CModelLoader::~CModelLoader()
{
  delete m_pPositions;
  delete m_pNormals;
  delete m_pColors;
  delete m_pTexCoords;
  delete m_pIndices;
  // Release group names
  while( !m_mapGroups.empty() )
  {
    delete (*m_mapGroups.begin()).second;
    m_mapGroups.erase( m_mapGroups.begin());
  }
}
/////////////////////////////////////////////////////////////////
