#ifndef __MooseOpenAssetImportLoader_h__
#define __MooseOpenAssetImportLoader_h__
////////////////////////////////////////////////////////////////////////////////
#include "MooseModelLoader.h"
#include "MooseAPI.h"
#include <assimp.hpp>
#include <aiScene.h> 
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Data
  {
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
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
