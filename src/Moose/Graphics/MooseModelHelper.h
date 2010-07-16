#ifndef __MooseModelHelper_h__
#define __MooseModelHelper_h__
/////////////////////////////////////////////////////////////////
#include <MooseRenderableModel.h>
#include <MooseMilkshapeLoader.h>
#include <MooseCore.h>
#include "MooseAPI.h"
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
      CModelLoader *m_pLoader;
      float	    m_fScale;
      CModelHelper() : m_pLoader( NULL ), m_fScale(1.0f) {}
    public:
      void SetScaling( float fScale );
      ////////////////////
      /// Loads milkshape file and stores data into managers.
      /// \param szFilename Path to milkshape model file.
      /// \returns On success, zero.
      /// \returns On failure, non-zero.
      int LoadMilkshapeData( const char *szFilename );
      ////////////////////
      /// Loads OBJ file and stores data into managers.
      /// \param szFilename Path to OBJ model file.
      /// \returns On success, zero.
      /// \returns On failure, non-zero.
      int LoadObjData( const char *szFilename );
      ///////////////////
      /// Automatically determines which file type given file is.
      /// \param szFilename File to be loaded.
      /// \returns On success, zero.
      /// \retursn On failure, non-zero.
      int Load( const char *szFilename );
      ////////////////////
      /// Creates CModel object from data that is currently loaded.
      /// \param iFlags Flags that indicate what data should be loaded from renderable - use MS3DDATA_OPTIONS values.
      /// \param szGroupName Which group of indices is used from model data.
      /// \param bInterleaved Use interleaved data on model. By default, false.
      /// \returns On success, pointer to CModel object, on failure NULL ptr.
      Moose::Graphics::CModel * CreateModel( int iFlags, const char *szGroupName = NULL, bool bInterleaved = false );
      ////////////////////
      /// Accesses current model loader.
      /// \returns Pointer to current model loader.
      const CModelLoader * GetModelLoader();
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
