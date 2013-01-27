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

#ifndef __MooseOpenAssetImportLoader_h__
#define __MooseOpenAssetImportLoader_h__
////////////////////////////////////////////////////////////////////////////////
#include "MooseModelLoader.h"
#include "MooseAPI.h"
#include <assimp.hpp>
#include <aiScene.h> 
#include <vector>
#include <string>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Data
  {
    typedef std::vector<std::string> MeshNameList;
    class COpenAssetImportLoader 
    {
    private:
      Assimp::Importer       m_Importer;
      const aiScene         *m_pScene;
      const aiMesh          *m_pMesh;
    public:
      void Load( const char * szFilename );
      void SelectMesh ( const char *szMeshname );

      Moose::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const;
      Moose::Graphics::CVertexDescriptor * GetColorArray()       const;
      Moose::Graphics::CVertexDescriptor * GetNormalArray()      const;
      Moose::Graphics::CVertexDescriptor * GetVertexArray()      const;
      Moose::Graphics::CVertexDescriptor * GetInterleavedArray() const;
      Moose::Graphics::CIndexArray       * GetIndexArray()       const;      
    
        bool  HasTexCoordArray( size_t nTexUnit = 0) const;
        bool  HasColorArray() const;
        bool  HasNormalArray() const;
        bool  HasVertexArray() const;
        bool  HasIndexArray() const;
      MeshNameList GetMeshes() const;
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
