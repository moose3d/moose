#ifndef __PhoenixModelHelper_h__
#define __PhoenixModelHelper_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixRenderableModel.h>
#include <PhoenixMilkshapeLoader.h>
#include <PhoenixCore.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
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
    class PHOENIX_API CModelHelper : public Phoenix::Core::CSingleton<CModelHelper>
    {
      friend class Phoenix::Core::CSingleton<CModelHelper>;
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
      ////////////////////
      /// Creates CModel object from data that is currently loaded.
      /// \param iFlags Flags that indicate what data should be loaded from renderable - use MS3DDATA_OPTIONS values.
      /// \param szGroupName Which group of indices is used from model data.
      /// \param bInterleaved Use interleaved data on model. By default, false.
      /// \returns On success, pointer to CModel object, on failure NULL ptr.
      Phoenix::Graphics::CModel * CreateModel( int iFlags, const char *szGroupName = NULL, bool bInterleaved = false );
      ////////////////////
      /// Initializes CRenderable object from currently loaded data.
      /// \attention This function does not add Model itself into ModelManager.
      /// \warning Color loading is disabled, see constructor. It is not implemented in CRenderable.
      /// \param szName Renderable name prefix for storing data in resourcemanagers. \attention Actual names are <szName>_vertices, _normals, _indices, _texcoords0, _colors.
      /// \param rModel CRenderable where data handles are attached to.
      /// \param lstGroupNames List of group names that are attached to renderable index handles in that order.
            /// \returns On success, zero.
      /// \returns On failure, non-zero.
      //int CreateRenderable( const char * szName, Phoenix::Graphics::CRenderable & rModel, std::list<std::string> & lstGroupNames);

      ////////////////////
      /// Clears currently loaded data.
      void Clear();
    };
  } // namespace Graphics
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#define g_ModelHelper (Phoenix::Data::CModelHelper::GetInstance())
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
