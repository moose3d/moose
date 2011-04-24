#ifndef __MooseModelHelper_h__
#define __MooseModelHelper_h__
/////////////////////////////////////////////////////////////////
#include <MooseRenderableModel.h>
#include <MooseOpenAssetImportLoader.h>
#include <MooseCore.h>
#include "MooseAPI.h"
#include <assimp.hpp>
#include <aiScene.h> 
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Data
  {
    enum MS3DDATA_OPTIONS {
      OPT_VERTEX_NORMALS   = 1 << 0,
      OPT_VERTEX_COLORS    = 1 << 1,
      OPT_VERTEX_TEXCOORDS = 1 << 2,
      OPT_VERTEX_INDICES   = 1 << 3
    };
    ////////////////////
    ///
    class MOOSE_API CModelHelper : public Moose::Core::CSingleton<CModelHelper>
    {
      friend class Moose::Core::CSingleton<CModelHelper>;
    private:
      Moose::Data::COpenAssetImportLoader * m_pLoader;
      CModelHelper() : m_pLoader( NULL ) {}
    public:
      ///////////////////
      /// Automatically determines which file type given file is.
      /// \param szFilename File to be loaded.
      void Load( const char *szFilename );
      ////////////////////
      /// Creates CModel object from data that is currently loaded.
      /// \param iFlags Flags that indicate what data should be loaded from renderable - use MS3DDATA_OPTIONS values.
      /// \param szGroupName Which mesh will be loaded as model. If NULL, then the first.
      /// \param bInterleaved Use interleaved data on model. By default, false.
      /// \returns On success, pointer to CModel object, on failure NULL ptr.
      Moose::Graphics::CModel * CreateModel( int iFlags, const char *szGroupName = NULL, bool bInterleaved = false );
      ////////////////////
      /// Accesses current model loader.
      /// \returns Pointer to current model loader.
      const COpenAssetImportLoader * GetModelLoader();
      ////////////////////
      /// Clears currently loaded data.
      void Clear();
    };
  } // namespace Graphics
} // namespace Moose
/////////////////////////////////////////////////////////////////
#define g_ModelHelper (Moose::Data::CModelHelper::GetInstance())
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
