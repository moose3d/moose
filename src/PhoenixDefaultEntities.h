#ifndef __PhoenixDefaultEntities_h__
#define __PhoenixDefaultEntities_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixResourceManager.h"
#include "PhoenixTexture.h"
#include "PhoenixShader.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Default
  {
    // Typedefs for default handle types.
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::COglTexture>       TEXTURE_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CVertexDescriptor> VERTEX_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CIndexArray>       INDEX_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CShader>           SHADER_HANDLE;
    ////////////////////
    // Typedefs for default resource manager types.
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::COglTexture,		TEXTURE_HANDLE>  TextureManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CVertexDescriptor,	VERTEX_HANDLE>   VertexManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CIndexArray,		INDEX_HANDLE>    IndexManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CShader,		SHADER_HANDLE>   ShaderManager;
    ////////////////////
#ifdef WIN32
#ifndef PHOENIX_DLL_EXPORT
    //  extern template class __declspec(dllimport) Phoenix::Core::CResourceManager<Phoenix::Graphics::COglTexture,        Phoenix::Default::TEXTURE_HANDLE>;
    //  extern template class __declspec(dllimport) Phoenix::Core::CResourceManager<Phoenix::Graphics::CVertexDescriptor,  Phoenix::Default::TEXTURE_HANDLE>;
    //  extern template class __declspec(dllimport) Phoenix::Core::CResourceManager<Phoenix::Graphics::CIndexArray,	    Phoenix::Default::INDEX_HANDLE>;
    //  extern template class __declspec(dllimport) Phoenix::Core::CResourceManager<Phoenix::Graphics::CShader,	    Phoenix::Default::SHADER_HANDLE>;
    
#endif
#endif
  }; // namespace Default
}; // namespace Phoenix
////////////////////
// Fake global variables for managers.
#define g_DefaultTextureManager (Phoenix::Default::TextureManager::GetInstance())
#define g_DefaultVertexManager  (Phoenix::Default::VertexManager::GetInstance())
#define g_DefaultIndexManager   (Phoenix::Default::IndexManager::GetInstance())
#define g_DefaultShaderManager   (Phoenix::Default::ShaderManager::GetInstance())
////////////////////
// Constants
#define TEXTURE_HANDLE_COUNT 8
#endif
/////////////////////////////////////////////////////////////////
