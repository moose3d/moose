#ifndef __PhoenixModelHelper_h__
#define __PhoenixModelHelper_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixModel.h>
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
    /// Loads milkshape file and creates new CModel object out of it. 
    /// \attention This function does not add Model itself into ModelManager.
    /// \warning Color loading is disabled, see LOAD_COMMON_STUFF macro. It is not implemented int CModel.
    /// \param szFilename Path to milkshape model file.
    /// \param szName Model name prefix for storing data in resourcemanagers. \attention Actual names are <szName>_vertices, _normals, _indices, _texcoords0, _colors. 
    /// \param rModel CModel where data handles are attached to.
    /// \param iFlags Flags that indicate what data should be loaded from model - use MS3DDATA_OPTIONS values.
    /// \returns On success, zero.
    /// \returns On failure, non-zero.
    int LoadMilkshapeModel( const char *szFilename, const char * szName, Phoenix::Graphics::CModel & rModel, int iFlags = 0);
    ////////////////////
    /// Loads milkshape file and creates new CModel object out of it.
    /// \attention This function does not add Model itself into ModelManager.
    /// \warning Color loading is disabled, see LOAD_COMMON_STUFF macro. It is not implemented int CModel.
    /// \param szFilename Path to milkshape model file.
    /// \param szName Model name prefix for storing data in resourcemanagers. \attention Actual names are <szName>_vertices, _normals, _indices, _texcoords0, _colors.
    /// \param rModel CModel where data handles are attached to.
    /// \param lstGroupNames List of group names that are attached to model index handles in that order.
    /// \param iFlags Flags that indicate what data should be loaded from model - use MS3DDATA_OPTIONS values.
    /// \returns On success, zero.
    /// \returns On failure, non-zero.
    int LoadMilkshapeModel( const char *szFilename, const char * szName, Phoenix::Graphics::CModel & rModel, std::list<std::string> & lstGroupNames, int iFlags = 0);
  } // namespace Graphics 
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////