#ifndef __PhoenixDefaultEntities_h__
#define __PhoenixDefaultEntities_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixResourceManager.h"
#include "PhoenixTexture.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Default
  {
    // Typedefs for default handle types.
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::COglTexture>       TEXTURE_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CVertexDescriptor> VERTEX_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CIndexArray>       INDEX_HANDLE;
    ////////////////////
    // Typedefs for default resource manager types.
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::COglTexture,		TEXTURE_HANDLE>  TextureManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CVertexDescriptor,	VERTEX_HANDLE>   VertexManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CIndexArray,		INDEX_HANDLE>    IndexManager;
    ////////////////////

  }; // namespace Default
}; // namespace Phoenix
////////////////////
// Fake global variables for managers.
#define g_DefaultTextureManager (Phoenix::Default::TextureManager::GetInstance())
#define g_DefaultVertexManager  (Phoenix::Default::VertexManager::GetInstance())
#define g_DefaultIndexManager   (Phoenix::Default::IndexManager::GetInstance())
////////////////////
#endif
/////////////////////////////////////////////////////////////////
