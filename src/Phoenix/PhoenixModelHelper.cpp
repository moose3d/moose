#include <PhoenixModelHelper.h>
#include <PhoenixMilkshapeLoader.h>
#include <PhoenixDefaultEntities.h>
#include <PhoenixSpatial.h>
#include <sstream>
#include <string>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Data;
using namespace Phoenix::Spatial;
using std::ostringstream;
using std::string;
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::LoadMilkshapeModel( const char *szFilename, const char *szName, int iFlags )
{
  // delete previous loader.
  if ( m_pLoader ) 
  {
    delete m_pLoader;
    m_pLoader = NULL;
  }

  if ( szFilename == NULL )	return 1;				
  if ( szName == NULL )		return 1;				

  m_pLoader = new CMilkshapeLoader() ;			
  assert( m_pLoader != NULL && "Out of memory, can't create Milkshapeloader"); 

  /* If loading fails */						
  if ( m_pLoader->Load( szFilename ) )					
  {									
    delete m_pLoader;							
    m_pLoader = NULL;
    return 1;								
  }									 
  
  string name = szName;							
  
  /* determine which data is used in comparison */			
  int iCompFlags = VERTEX_COMP_POSITION;				
  if ( iFlags &  OPT_VERTEX_NORMALS  ) iCompFlags |= VERTEX_COMP_NORMAL; 
  if ( iFlags &  OPT_VERTEX_COLORS )   iCompFlags |= VERTEX_COMP_COLOR;	
  if ( iFlags &  OPT_VERTEX_TEXCOORDS) iCompFlags |= VERTEX_COMP_TEXCOORD; 
									
  m_pLoader->GenerateModelData( iCompFlags );				
									
  /* Resource allocation is one-way, either everything succeeds or nothing goes.*/ 
									
  /* Create vertex handle */						
  assert( g_DefaultVertexManager->Create(m_pLoader->GetVertices(),  name + "_vertices") == 0); 
  /* Mark resourcemanager-tracked data so pointers will not be free'd.*/ 
  m_pLoader->ResetVertices();						
									
  /* load vertex normals */						
  if ( iFlags & OPT_VERTEX_NORMALS )					
  {									
    /* Create normal handle */						
    assert ( g_DefaultVertexManager->Create(m_pLoader->GetNormals(), name + "_normals") == 0); 
    m_pLoader->ResetNormals();						
  }									
  /* load texture coordinates */					
  if ( iFlags & OPT_VERTEX_TEXCOORDS )					
  {									
    /* Create texcoord handle */					
    assert ( g_DefaultVertexManager->Create(m_pLoader->GetTexCoords(), name + "_texcoords0") == 0);	
    m_pLoader->ResetTexCoords();						
  }									
  /* load colors */							
  if ( iFlags & OPT_VERTEX_COLORS )					
  {									
    /* Create texcoord handle */					
    /*assert ( g_DefaultVertexManager->Create(m_pLoader->GetVertexColors(),*/ 
    /*name + "_colors",*/						
    /*  rModel.GetTextureCoordinateHandle()) == 0);		     */ 
 									
  }									
  // All ok
  return 0;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::CreateRenderable( const char *szName, CRenderable & rModel, const char *szGroupName )
{
  string name = szName;
  assert( g_DefaultVertexManager->AttachHandle( name + "_vertices",	rModel.GetVertexHandle()) == 0);
  assert(  g_DefaultVertexManager->AttachHandle( name + "_normals",	rModel.GetNormalHandle()) == 0);
  assert(  g_DefaultVertexManager->AttachHandle( name + "_texcoords0",	rModel.GetTextureCoordinateHandle()) == 0);
  
  /// Create index handles
  assert ( m_pLoader->GetIndices().size() == 1 && "Too many triangle indices");

  if ( szGroupName == NULL )
  {
    ostringstream stream;  
    stream << name << "_list_indices";
    assert( g_DefaultIndexManager->Create( m_pLoader->GetIndices()[0], 
					   stream.str().c_str(), 
					   rModel.GetListIndices()) == 0 );     
    m_pLoader->ResetIndices();
  }
  else
  {
    CIndexArray *pIndices = m_pLoader->GetGroupIndices( szGroupName );
    assert ( pIndices != NULL && "Group is NULL");
    ostringstream stream;  
    stream << name << "_" << szGroupName << "_indices";
    assert( g_DefaultIndexManager->Create( pIndices, 
					   stream.str().c_str(), 
					   rModel.GetListIndices()) == 0 );
    m_pLoader->ResetGroup( szGroupName );
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
  // Release loader memory
  if ( m_pLoader ) 
  {
    delete m_pLoader;
    m_pLoader = NULL;
  }
}
/////////////////////////////////////////////////////////////////
