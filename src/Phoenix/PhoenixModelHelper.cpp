#include "PhoenixModelHelper.h"
#include "PhoenixMilkshapeLoader.h"
#include "PhoenixObjLoader.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixSpatial.h"
#include <sstream>
#include <string>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Data;
using namespace Phoenix::Spatial;
using std::ostringstream;
using std::string;
/////////////////////////////////////////////////////////////////
void
CreateModelUsingLoader( CModelLoader * pLoader, const char *szName, int iFlags, const char **aszGroupNames, bool bInterleaved, float fScaling )
{
  string name = szName;							
  /* determine which data is used in comparison */			
  // int iCompFlags = VERTEX_COMP_POSITION;				
//   if ( iFlags &  OPT_VERTEX_NORMALS  ) iCompFlags |= VERTEX_COMP_NORMAL; 
//   if ( iFlags &  OPT_VERTEX_COLORS )   iCompFlags |= VERTEX_COMP_COLOR;	
//   if ( iFlags &  OPT_VERTEX_TEXCOORDS) iCompFlags |= VERTEX_COMP_TEXCOORD; 
									

									
  /* Resource allocation is one-way, either everything succeeds or nothing goes.*/ 
  
  if ( bInterleaved )
  {
    CVertexDescriptor *pInterleaved = pLoader->GetInterleavedArray();
    // manage array
    assert( g_DefaultVertexManager->Create( pInterleaved,  name + "_interleaved") == 0);
  }
  else
  {
    // Create vertex handle 
    assert( g_DefaultVertexManager->Create(pLoader->GetVertexArray(fScaling),  name + "_vertices") == 0); 
    
    /* load vertex normals */						
    if ( iFlags & OPT_VERTEX_NORMALS )					
    {									
      /* Create normal handle */						
      assert ( g_DefaultVertexManager->Create(pLoader->GetNormalArray(), name + "_normals") == 0); 
    }									
    /* load texture coordinates */					
    if ( iFlags & OPT_VERTEX_TEXCOORDS )					
    {									
      /* Create texcoord handle */					
      assert ( g_DefaultVertexManager->Create(pLoader->GetTexCoordArray(), name + "_texcoords0") == 0);	
    }									
    /* load colors */							
    if ( iFlags & OPT_VERTEX_COLORS )					
    {									
      /* Create texcoord handle */					
      /*assert ( g_DefaultVertexManager->Create(pLoader->GetVertexColors(),*/ 
      /*name + "_colors",*/						
      /*  rModel.GetTextureCoordinateHandle()) == 0);		     */ 
 									
    }									
  }
  if ( aszGroupNames == NULL || aszGroupNames[0] == NULL )
  {
    ostringstream stream;  
    stream << name << "_list_indices";
    assert( g_DefaultIndexManager->Create( pLoader->GetIndexArray(), stream.str().c_str()) == 0 );     
  }
  else
  {
    size_t nGroupIndex = 0;
    
    for( ; aszGroupNames[nGroupIndex]; ++nGroupIndex)
    {
      CIndexArray *pIndices = pLoader->GetIndexArray( aszGroupNames[nGroupIndex] );
      assert ( pIndices != NULL && "Group is NULL" );

      ostringstream stream;  
      stream << name << "_" << aszGroupNames[nGroupIndex] << "_indices";
      assert( g_DefaultIndexManager->Create( pIndices, stream.str().c_str()) == 0 );
    }
  }
  

}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::LoadMilkshapeModel( const char *szFilename, const char *szName, int iFlags, const char **aszGroupNames, bool bInterleaved )
{
  // delete previous loader.

  delete m_pLoader;
  m_pLoader = NULL;
  
  if ( szFilename == NULL )	return 1;				
  if ( szName == NULL )		return 1;				

  m_pLoader = new CMilkshapeLoader() ;			
  
  /* If loading fails */						
  if ( m_pLoader->Load( szFilename ) )					
  {									
    delete m_pLoader;							
    m_pLoader = NULL;
    return 1;								
  }									 
  CreateModelUsingLoader( m_pLoader, szName, iFlags, aszGroupNames, bInterleaved, m_fScale );
  // all ok.
  delete m_pLoader;
  m_pLoader = NULL;
  return 0;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::LoadObjModel( const char *szFilename, const char *szName, int iFlags, const char **aszGroupNames, bool bInterleaved )
{
  // delete previous loader.
  delete m_pLoader;
  m_pLoader = NULL;
  
  if ( szFilename == NULL )	return 1;				
  if ( szName == NULL )		return 1;				

  m_pLoader = new CObjLoader();			

  /* If loading fails */						
  if ( m_pLoader->Load( szFilename ) )					
  {									
    delete m_pLoader;							
    m_pLoader = NULL;
    return 1;								
  }									 
  CreateModelUsingLoader( m_pLoader, szName, iFlags, aszGroupNames, bInterleaved, m_fScale );
  delete m_pLoader;
  m_pLoader = NULL;
  // all ok.
  return 0;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::CreateRenderable( const char *szName, CRenderable & rModel, const char *szGroupName, bool bInterleaved )
{
  string name = szName;

  if ( bInterleaved )
  {
    g_DefaultVertexManager->AttachHandle( name + "_interleaved", rModel.GetVertexHandle());
  }
  else
  {
    g_DefaultVertexManager->AttachHandle( name + "_vertices",	rModel.GetVertexHandle());
    g_DefaultVertexManager->AttachHandle( name + "_normals",	rModel.GetNormalHandle());
    g_DefaultVertexManager->AttachHandle( name + "_texcoords0",	rModel.GetTextureCoordinateHandle());
  }
 
  if ( szGroupName == NULL )
  {
    assert( g_DefaultIndexManager->AttachHandle( name + "_list_indices",  rModel.GetIndices()) == 0 );     
  }
  else
  {
    ostringstream stream;  
    stream << name << "_" << szGroupName << "_indices";
    assert( g_DefaultIndexManager->AttachHandle( stream.str().c_str(), rModel.GetIndices()) == 0 );
  }
  
  return 0;
}
/////////////////////////////////////////////////////////////////
// int
// Phoenix::Data::CreateRenderable( const char * szName, Phoenix::Graphics::CRenderable & rModel, std::list<std::string> & lstGroupNames )
// {

//   // Load indices
//   if ( iFlags & OPT_VERTEX_INDICES )
  //   {
  //     std::list<std::string>::iterator it = lstGroupNames.begin();
  //     // Create index handles 
  //     for( ; it != lstGroupNames.end(); it++)
  //     {
  //       CIndexArray *pIndices = pLoader->GetGroupIndices( (*it).c_str() );
  //       assert ( pIndices != NULL && "Group is NULL");

  //       ostringstream stream;  
  //       stream << name << "_" << *it << "_indices";
  //       rModel.AddIndexHandle(new INDEX_HANDLE());
  //       assert( g_DefaultIndexManager->Create( pIndices, 
  // 					     stream.str().c_str(), 
  // 					     *rModel.GetIndexHandles().back()) == 0 );
  //       pLoader->ResetGroup( (*it).c_str() );

  //     }
    
  //   }

  
  //   return 0;
  // }
  /////////////////////////////////////////////////////////////////
void
Phoenix::Data::CModelHelper::Clear()
{
  delete m_pLoader;
  m_pLoader = NULL;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Data::CModelHelper::SetScaling( float fScale )
{
  m_fScale = fScale;
}
/////////////////////////////////////////////////////////////////
