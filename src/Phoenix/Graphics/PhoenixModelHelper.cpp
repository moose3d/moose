#include "PhoenixModelHelper.h"
#include "PhoenixMilkshapeLoader.h"
#include "PhoenixObjLoader.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixSpatial.h"
#include <sstream>
#include <string>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Data;
using namespace Phoenix::Spatial;
using std::ostringstream;
using std::string;
/////////////////////////////////////////////////////////////////
const CModelLoader *
Phoenix::Data::CModelHelper::GetModelLoader()
{
	return m_pLoader;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::Load( const char *szFilename  )
{
	if ( Phoenix::Data::CMilkshapeLoader::IsMilkshapeFile(szFilename ) )
	{
		return g_ModelHelper->LoadMilkshapeData(szFilename);
	}

	int len = strlen(szFilename);
	if ( (len > 4) && (strcasestr( ".obj", szFilename-4 ) != 0) )
	{
		return g_ModelHelper->LoadObjData( szFilename );
	}
	else return 1;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::LoadMilkshapeData( const char *szFilename  )
{
  // delete previous loader.

  delete m_pLoader;
  m_pLoader = NULL;

  if ( szFilename == NULL )	return 1;

  m_pLoader = new CMilkshapeLoader() ;

  /* If loading fails */
  if ( m_pLoader->Load( szFilename ) )
  {
    delete m_pLoader;
    m_pLoader = NULL;
    return 1;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Data::CModelHelper::LoadObjData( const char *szFilename )
{
  // delete previous loader.
  delete m_pLoader;
  m_pLoader = NULL;

  if ( szFilename == NULL )	return 1;

  m_pLoader = new CObjLoader();

  /* If loading fails */
  if ( m_pLoader->Load( szFilename ) )
  {
    delete m_pLoader;
    m_pLoader = NULL;
    return 1;
  }
  // all ok.
  return 0;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel *
Phoenix::Data::CModelHelper::CreateModel( int iFlags, const char *szGroupName, bool bInterleaved )
{

	CModel *pModel = new CModel();

	  /* determine which data is used in comparison */
	  // int iCompFlags = VERTEX_COMP_POSITION;
	//   if ( iFlags &  OPT_VERTEX_NORMALS  ) iCompFlags |= VERTEX_COMP_NORMAL;
	//   if ( iFlags &  OPT_VERTEX_COLORS )   iCompFlags |= VERTEX_COMP_COLOR;
	//   if ( iFlags &  OPT_VERTEX_TEXCOORDS) iCompFlags |= VERTEX_COMP_TEXCOORD;



	  /* Resource allocation is one-way, either everything succeeds or nothing goes.*/

	  if ( bInterleaved )
	  {
	  	CVertexDescriptor *pInterleaved = m_pLoader->GetInterleavedArray();
	    // manage array
	  	assert( g_DefaultVertexManager->Create( pInterleaved,  g_UniqueName, pModel->GetVertexHandle() ) == 0);
	  }
	  else
	  {
	    // Create vertex handle
	    assert( g_DefaultVertexManager->Create(m_pLoader->GetVertexArray(m_fScale),  g_UniqueName, pModel->GetVertexHandle() ) == 0);

	    /* load vertex normals */
	    if ( iFlags & OPT_VERTEX_NORMALS && m_pLoader->HasNormalArray() )
	    {
	      /* Create normal handle */
	      assert ( g_DefaultVertexManager->Create(m_pLoader->GetNormalArray(), g_UniqueName, pModel->GetNormalHandle()) == 0);
	    }
	    /* load texture coordinates */
	    if ( iFlags & OPT_VERTEX_TEXCOORDS && m_pLoader->HasTexCoordArray())
	    {
	      /* Create texcoord handle */
	      assert ( g_DefaultVertexManager->Create(m_pLoader->GetTexCoordArray(), g_UniqueName, pModel->GetTextureCoordinateHandle(0)) == 0);
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
	  if ( szGroupName == NULL || szGroupName[0] == '\n')
	  {
	  	CIndexArray *pArray = m_pLoader->GetIndexArray();
	  	assert( pArray->GetNumIndices() > 0 );
	    assert( g_DefaultIndexManager->Create( pArray, g_UniqueName, pModel->GetIndices()) == 0 );
	  }
	  else
	  {
	      CIndexArray *pIndices = m_pLoader->GetIndexArray( szGroupName );
	      assert ( pIndices != NULL && "Group is NULL" );
	      assert (pIndices->GetNumIndices() > 0 && "Not enough indices ");
	      assert( g_DefaultIndexManager->Create( pIndices, g_UniqueName, pModel->GetIndices() ) == 0 );

	  }

  return pModel;
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
