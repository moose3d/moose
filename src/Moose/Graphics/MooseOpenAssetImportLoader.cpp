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

#include "MooseOpenAssetImportLoader.h"
#include <aiPostProcess.h>
#include <stdexcept>
#include <MooseVertexDescriptor.h>
#include <MooseIndexArray.h>
#include <MooseExceptions.h>
#include <cstring>
////////////////////////////////////////////////////////////////////////////////
namespace prefix = Moose::Data;
using namespace std;
using namespace Moose::Graphics;
using namespace Moose::Exceptions;
////////////////////////////////////////////////////////////////////////////////
void
prefix::COpenAssetImportLoader::Load( const char *szFilename )
{
  m_pScene = m_Importer.ReadFile( szFilename, 
                                  aiProcess_CalcTangentSpace       | 
                                  aiProcess_Triangulate            |
                                  aiProcess_JoinIdenticalVertices  |
                                  aiProcess_SortByPType );

  if ( m_pScene == NULL ) throw CMooseRuntimeError(m_Importer.GetErrorString());
  
  if ( m_pScene->mNumMeshes == 0 ) throw CMooseRuntimeError(std::string("No meshes!"));
  

  m_pMesh = NULL;

}
////////////////////////////////////////////////////////////////////////////////
void
prefix::COpenAssetImportLoader::SelectMesh( const char *szMeshname )
{
  // With NULL, we select first mesh
  // At this point, there is supposed to be some meshes
  if ( szMeshname == NULL || strlen(szMeshname) == 0 ) 
  {
    m_pMesh = m_pScene->mMeshes[0];
    return;
  }
  
  for ( size_t i=0; i< m_pScene->mNumMeshes; i++)
  {
    if ( std::string(m_pScene->mMeshes[i]->mName.data) == szMeshname )
    {
      m_pMesh = m_pScene->mMeshes[i];
      break;
    }
  }

  // Throw exception with available mesh names
  if ( m_pMesh == NULL )
  {
    CMooseRuntimeError err("No such mesh '");
    err << szMeshname << "' found!";
    err << "\nAvailable meshes:\n";
    for ( size_t i=0; i< m_pScene->mNumMeshes; i++) 
    {
      err << m_pScene->mMeshes[i]->mName.data << "\n";
    }
    throw err;
  }
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
prefix::COpenAssetImportLoader::GetTexCoordArray( size_t nTexUnit ) const
{
  CVertexDescriptor *pTexc = new CVertexDescriptor( ELEMENT_TYPE_TEX_2F, 
                                                    m_pMesh->mNumVertices );
  
  for( size_t i=0;i<m_pMesh->mNumVertices; i++)
  {
    pTexc->GetPointer<float>(i)[0] = m_pMesh->mTextureCoords[0][i].x;
    pTexc->GetPointer<float>(i)[1] = m_pMesh->mTextureCoords[0][i].y;
    //pTexc->GetPointer<float>(i)[2] = m_pMesh->mTextureCoords[0][i].z;
  }  
  return pTexc;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
prefix::COpenAssetImportLoader::GetColorArray() const
{
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
prefix::COpenAssetImportLoader::GetNormalArray() const
{
  CVertexDescriptor * pVertices = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, 
                                                        m_pMesh->mNumVertices);
  for( size_t i=0;i<m_pMesh->mNumVertices; i++)
  {
    pVertices->GetPointer<float>(i)[0] = m_pMesh->mNormals[i].x;
    pVertices->GetPointer<float>(i)[1] = m_pMesh->mNormals[i].y;
    pVertices->GetPointer<float>(i)[2] = m_pMesh->mNormals[i].z;
  }
  return pVertices;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
prefix::COpenAssetImportLoader::GetVertexArray(  ) const
{
  CVertexDescriptor * pVertices = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, m_pMesh->mNumVertices);
  for( size_t i=0;i<m_pMesh->mNumVertices; i++)
  {
    pVertices->GetPointer<float>(i)[0] = m_pMesh->mVertices[i].x;
    pVertices->GetPointer<float>(i)[1] = m_pMesh->mVertices[i].y;
    pVertices->GetPointer<float>(i)[2] = m_pMesh->mVertices[i].z;
  }
  return pVertices;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CVertexDescriptor * 
prefix::COpenAssetImportLoader::GetInterleavedArray(  ) const
{
  
  CVertexDescriptor *pInter = new CVertexDescriptor( ELEMENT_TYPE_V3F_N3F_T2F, m_pMesh->mNumVertices );

  for( size_t i=0;i<m_pMesh->mNumVertices; i++)
  {
    pInter->GetPointer<float>(i)[0] = m_pMesh->mVertices[i].x;
    pInter->GetPointer<float>(i)[1] = m_pMesh->mVertices[i].y;
    pInter->GetPointer<float>(i)[2] = m_pMesh->mVertices[i].z;

    pInter->GetPointer<float>(i)[3] = m_pMesh->mNormals[i].x;
    pInter->GetPointer<float>(i)[4] = m_pMesh->mNormals[i].y;
    pInter->GetPointer<float>(i)[5] = m_pMesh->mNormals[i].z;
    
    pInter->GetPointer<float>(i)[6] = m_pMesh->mTextureCoords[0][i].x;
    pInter->GetPointer<float>(i)[7] = m_pMesh->mTextureCoords[0][i].y;
  }
    return pInter;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CIndexArray * 
prefix::COpenAssetImportLoader::GetIndexArray( ) const
{
  CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, m_pMesh->mNumFaces*3);
  if ( pIndices->IsShortIndices() )
  {
    for( size_t i=0; i<m_pMesh->mNumFaces; i++)
    {
      pIndices->GetPointer<unsigned short>()[i*3]   = m_pMesh->mFaces[i].mIndices[0];
      pIndices->GetPointer<unsigned short>()[i*3+1] = m_pMesh->mFaces[i].mIndices[1];
      pIndices->GetPointer<unsigned short>()[i*3+2] = m_pMesh->mFaces[i].mIndices[2];
    }
  }
  else
  {
    for( size_t i=0; i<m_pMesh->mNumFaces; i++)
    {
      pIndices->GetPointer<unsigned int>()[i*3]   = m_pMesh->mFaces[i].mIndices[0];
      pIndices->GetPointer<unsigned int>()[i*3+1] = m_pMesh->mFaces[i].mIndices[1];
      pIndices->GetPointer<unsigned int>()[i*3+2] = m_pMesh->mFaces[i].mIndices[2];
    }
  }

  return pIndices;
}
////////////////////////////////////////////////////////////////////////////////
bool  prefix::COpenAssetImportLoader::HasTexCoordArray( size_t nTexUnit ) const
{
  return  (m_pMesh->HasTextureCoords(nTexUnit));
}
////////////////////////////////////////////////////////////////////////////////
bool  prefix::COpenAssetImportLoader::HasColorArray() const
{
    return (m_pMesh->mColors);
}
////////////////////////////////////////////////////////////////////////////////
bool  prefix::COpenAssetImportLoader::HasNormalArray() const
{
  return (m_pMesh->HasNormals());
}
////////////////////////////////////////////////////////////////////////////////
bool  prefix::COpenAssetImportLoader::HasVertexArray() const
{
    return (m_pMesh->mVertices);
}
////////////////////////////////////////////////////////////////////////////////
bool  prefix::COpenAssetImportLoader::HasIndexArray() const
{
  return (m_pMesh->HasFaces());
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > 
prefix::COpenAssetImportLoader::GetMeshes() const
{
  MeshNameList meshNames(m_pScene->mNumMeshes);
  
  for ( size_t i=0; i< m_pScene->mNumMeshes; i++)
  {
    meshNames.push_back(std::string(m_pScene->mMeshes[i]->mName.data));
  } 
  return meshNames;
}
////////////////////////////////////////////////////////////////////////////////
