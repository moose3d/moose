#include "MooseModelHelper.h"
#include "MooseMilkshapeLoader.h"
#include "MooseObjLoader.h"
#include "MooseDefaultEntities.h"
#include <sstream>
#include <string>
#include <cassert>
#include <aiPostProcess.h>
#include <stdexcept>
#include <MooseVertexDescriptor.h>
#include <MooseIndexArray.h>
#include <MooseExceptions.h>
////////////////////////////////////////////////////////////////////////////////
namespace prefix = Moose::Data;
using namespace std;
using namespace Moose::Graphics;
using namespace Moose::Data;
using namespace Moose::Spatial;
using namespace Moose::Exceptions;
/////////////////////////////////////////////////////////////////
const COpenAssetImportLoader *
Moose::Data::CModelHelper::GetModelLoader()
{
	return m_pLoader;
}
/////////////////////////////////////////////////////////////////
void
Moose::Data::CModelHelper::Load( const char *szFilename  )
{
  if ( m_pLoader ) delete m_pLoader;
  m_pLoader = new COpenAssetImportLoader();
  m_pLoader->Load(szFilename);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CModel *
Moose::Data::CModelHelper::CreateModel( int iFlags, const char *szGroupName, bool bInterleaved )
{

  CModel *pModel = new CModel();

  m_pLoader->SelectMesh(szGroupName);

  /* Resource allocation is one-way, either everything succeeds or nothing goes.*/

  if ( bInterleaved )
  {
    CVertexDescriptor *pInterleaved = m_pLoader->GetInterleavedArray();
    // manage array
    int iRetval = g_VertexMgr->Create( pInterleaved, 
                                                  g_UniqueName, 
                                                  pModel->GetVertexHandle() );
    if ( iRetval != 0 )
    {
      throw CMooseRuntimeError("Cannot create vertex array");
    }
  }
  else
  {
    // Create vertex handle
    int iRetval = g_VertexMgr->Create( m_pLoader->GetVertexArray(),  
                                                  g_UniqueName, 
                                                  pModel->GetVertexHandle() );
    if ( iRetval != 0 )
    {
      throw CMooseRuntimeError("Cannot create vertex array");
    }

    /* load vertex normals */
    if ( iFlags & OPT_VERTEX_NORMALS && m_pLoader->HasNormalArray() )
    {
      /* Create normal handle */
      iRetval = g_VertexMgr->Create(m_pLoader->GetNormalArray(), 
                                               g_UniqueName, 
                                               pModel->GetNormalHandle());
      if ( iRetval != 0 )
      {
        throw CMooseRuntimeError("Cannot create normal array");
      }
    }
    /* load texture coordinates */
    if ( iFlags & OPT_VERTEX_TEXCOORDS && m_pLoader->HasTexCoordArray())
    {
      /* Create texcoord handle */
      iRetval = g_VertexMgr->Create(m_pLoader->GetTexCoordArray(), g_UniqueName, pModel->GetTextureCoordinateHandle(0));

      if ( iRetval != 0 )
      {
        throw CMooseRuntimeError("Cannot create texture coordinate array");
      }

    }
    /* load colors */
    if ( iFlags & OPT_VERTEX_COLORS )
    {


    }
  }
  
  if ( szGroupName == NULL || szGroupName[0] == '\n')
  {
    CIndexArray *pArray = m_pLoader->GetIndexArray();
    assert( pArray->GetNumIndices() > 0 );
    int iRetval = g_IndexMgr->Create( pArray, g_UniqueName, pModel->GetIndices());

    if ( iRetval != 0 )
    {
      throw CMooseRuntimeError("Cannot create index array");
    }
    
  }
  else
  {
    CIndexArray *pIndices = m_pLoader->GetIndexArray();
    int iRetval = g_IndexMgr->Create( pIndices, g_UniqueName, pModel->GetIndices() );

    if ( iRetval != 0 )
    {
      throw CMooseRuntimeError("Cannot create index array");
    }
  }
  return pModel;
}
/////////////////////////////////////////////////////////////////
void
Moose::Data::CModelHelper::Clear()
{
  delete m_pLoader;
  m_pLoader = NULL;
}
/////////////////////////////////////////////////////////////////
