#ifndef __PhoenixDefaultEntities_h__
#define __PhoenixDefaultEntities_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixResourceManager.h>
#include <PhoenixObjectUpdater.h>
#include <PhoenixTexture.h>
#include <PhoenixShader.h>
#include <PhoenixVertexDescriptor.h>
#include <PhoenixIndexArray.h>
#include "PhoenixAPI.h"
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
    /////////////////////////////////////////////////////////////////
    /// Default object updater object.
    class PHOENIX_API CPhoenixObjectUpdater : public Phoenix::Core::CObjectUpdater, 
				  public Phoenix::Core::CSingleton<Phoenix::Default::CPhoenixObjectUpdater>
    {
      friend class Phoenix::Core::CSingleton<Phoenix::Default::CPhoenixObjectUpdater>;
    };
  }; // namespace Default
}; // namespace Phoenix
////////////////////
// Fake global variables for managers.
#define g_DefaultTextureManager (Phoenix::Default::TextureManager::GetInstance())
#define g_DefaultVertexManager  (Phoenix::Default::VertexManager::GetInstance())
#define g_DefaultIndexManager   (Phoenix::Default::IndexManager::GetInstance())
#define g_DefaultShaderManager  (Phoenix::Default::ShaderManager::GetInstance())
#define g_DefaultUpdater        (Phoenix::Default::CPhoenixObjectUpdater::GetInstance())
#define g_UniqueNameStr         ((Phoenix::Core::CUniqueNameCreator::GetInstance())->GetUniqueName())
#define g_UniqueName            ((Phoenix::Core::CUniqueNameCreator::GetInstance())->GetUniqueName().c_str())
#define CreateUniqueNameStr(A)  ((Phoenix::Core::CUniqueNameCreator::GetInstance())->GetUniqueName(A))
#define CreateUniqueName(A)     ((Phoenix::Core::CUniqueNameCreator::GetInstance())->GetUniqueName(A).c_str())
////////////////////
// Constants
#define TEXTURE_HANDLE_COUNT 8
#endif
/////////////////////////////////////////////////////////////////
