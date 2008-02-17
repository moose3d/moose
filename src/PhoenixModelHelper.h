#ifndef __PhoenixModelHelper_h__
#define __PhoenixModelHelper_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixModel.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Data
  {
    ////////////////////
    /// Loads milkshape file and creates new CModel object out of it.
    /// \param szFilename Path to milkshape model file.
    /// \param szName Model name prefix for storing data in resourcemanagers.
    /// \returns On success, Pointer to CModel.
    /// \returns On failure, NULL.
    Phoenix::Graphics::CModel * LoadMilkshapeModel( const char *szFilename, const char * szName );
  } // namespace Graphics 
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
