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
Phoenix::Data::CModelHelper::LoadMilkshapeModel( const char *szFilename, const char *szName, int iFlags, const char **aszGroupNames, bool bInterleaved )
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
  
  if ( bInterleaved )
  {
    size_t nNumElements =  m_pLoader->GetVertices()->GetSize();
    CVertexDescriptor *pInterleaved = new CVertexDescriptor( ELEMENT_TYPE_V3F_N3F_T2F, nNumElements);

    // Copy data into interleaved array
    for( size_t i=0;i<nNumElements;i++)
    {
      pInterleaved->GetPointer<float>(i)[0] = m_pLoader->GetVertices()->GetPointer<float>(i)[0];
      pInterleaved->GetPointer<float>(i)[1] = m_pLoader->GetVertices()->GetPointer<float>(i)[1];
      pInterleaved->GetPointer<float>(i)[2] = m_pLoader->GetVertices()->GetPointer<float>(i)[2];
      
      pInterleaved->GetPointer<float>(i)[3] = m_pLoader->GetNormals()->GetPointer<float>(i)[0];
      pInterleaved->GetPointer<float>(i)[4] = m_pLoader->GetNormals()->GetPointer<float>(i)[1];
      pInterleaved->GetPointer<float>(i)[5] = m_pLoader->GetNormals()->GetPointer<float>(i)[2];

      pInterleaved->GetPointer<float>(i)[6] = m_pLoader->GetTexCoords()->GetPointer<float>(i)[0];
      pInterleaved->GetPointer<float>(i)[7] = m_pLoader->GetTexCoords()->GetPointer<float>(i)[1];
    }
    // manage array
    assert( g_DefaultVertexManager->Create( pInterleaved,  name + "_interleaved") == 0);
  }
  else
  {
    // Create vertex handle 
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
  }
  if ( aszGroupNames == NULL || aszGroupNames[0] == NULL )
  {
    ostringstream stream;  
    stream << name << "_list_indices";
    assert( g_DefaultIndexManager->Create( m_pLoader->GetIndices()[0], 
					   stream.str().c_str()) == 0 );     
    m_pLoader->ResetIndices();
  }
  else
  {
    size_t nGroupIndex = 0;
    
    for( ; aszGroupNames[nGroupIndex]; ++nGroupIndex)
    {
      CIndexArray *pIndices = m_pLoader->GetGroupIndices( aszGroupNames[nGroupIndex] );
      assert ( pIndices != NULL && "Group is NULL" );

      ostringstream stream;  
      stream << name << "_" << aszGroupNames[nGroupIndex] << "_indices";
      assert( g_DefaultIndexManager->Create( pIndices, 
					     stream.str().c_str()) == 0 );
      m_pLoader->ResetGroup( aszGroupNames[nGroupIndex] );
    }
  }
  
  
  // All ok
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
  // Release loader memory
  if ( m_pLoader ) 
  {
    delete m_pLoader;
    m_pLoader = NULL;
  }
}
  /////////////////////////////////////////////////////////////////
