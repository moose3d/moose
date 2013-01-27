/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
////////////////////////////////////////////////////////////////////////////////
Moose::Data::MeshNameList
Moose::Data::CModelHelper::GetAvailableMeshes() const
{
  // If we have stuff, then return name list (vector)
  if ( m_pLoader ) 
  {
    return m_pLoader->GetMeshes();
  }
  else return MeshNameList(); // otherwise return empty list
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

  CIndexArray *pArray = m_pLoader->GetIndexArray();
  assert( pArray->GetNumIndices() > 0 );
  int iRetval = g_IndexMgr->Create( pArray, g_UniqueName, pModel->GetIndices());
  
  if ( iRetval != 0 )
  {
    throw CMooseRuntimeError("Cannot create index array");
  }
 
  return pModel;
}
/////////////////////////////////////////////////////////////////
CIndexArray * 
Moose::Data::CModelHelper::CreateIndices( const char *szGroupName, const char *szResourceName )
{
  // Sanity check, no data, no indices.
  if ( !m_pLoader ) return NULL;

  m_pLoader->SelectMesh(szGroupName);  
  CIndexArray *pIndices = NULL;

  pIndices = m_pLoader->GetIndexArray();

  // Create resource either by unique name or resource name
  int iRetval;
  if ( szResourceName == NULL ) 
  {
    iRetval = g_IndexMgr->Create( pIndices, g_UniqueName);
  } 
  else 
  {
    iRetval = g_IndexMgr->Create( pIndices, szResourceName);
  }
    
  if ( iRetval != 0 )
  {
    throw CMooseRuntimeError("Cannot create index array");
  }
    

 
  return pIndices;
}
/////////////////////////////////////////////////////////////////
void
Moose::Data::CModelHelper::Clear()
{
  delete m_pLoader;
  m_pLoader = NULL;
}
/////////////////////////////////////////////////////////////////
