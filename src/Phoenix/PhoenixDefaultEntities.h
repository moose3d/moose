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
  namespace Scene
  {
	  class CGameObject;

  }
  namespace Default
  {
    // Typedefs for default handle types.
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::COglTexture>       TEXTURE_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CVertexDescriptor> VERTEX_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CIndexArray>       INDEX_HANDLE;
    typedef Phoenix::Core::CHandle<Phoenix::Graphics::CShader>           SHADER_HANDLE;
#define MODEL_HANDLE Phoenix::Core::CHandle<Phoenix::Graphics::CModel>
    ////////////////////
    // Typedefs for default resource manager types.
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::COglTexture,		TEXTURE_HANDLE>  TextureManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CVertexDescriptor,	VERTEX_HANDLE>   VertexManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CIndexArray,		INDEX_HANDLE>    IndexManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Graphics::CShader,		SHADER_HANDLE>   ShaderManager;
    typedef Phoenix::Core::CResourceManager< Phoenix::Scene::CGameObject, Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> > ObjectMgr;
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
    ObjectMgr * GetObjectMgr();
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
/////////////////////////////////////////////////////////////////
/// Log streams.
#define g_Error ( Phoenix::Core::CLogger::GetInstance()->Err())
#define g_Log   ( Phoenix::Core::CLogger::GetInstance()->Msg())
#define g_Warn  ( Phoenix::Core::CLogger::GetInstance()->Warn())
////////////////////////////////////////////////////////////////////////////////
/// Manager for gameobjects.


//Phoenix::Core::CResourceManager<Phoenix::Scene::CGameObject, Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> > * GetObjectMgr();

#define g_ObjectMgr            (Phoenix::Default::ObjectMgr::GetInstance())
#define g_TextureMgr           (Phoenix::Default::TextureManager::GetInstance())
#define g_VertexMgr            (Phoenix::Default::VertexManager::GetInstance())
#define g_IndexMgr             (Phoenix::Default::IndexManager::GetInstance())
#define g_ShaderMgr            (Phoenix::Default::ShaderManager::GetInstance())
#define g_ModelMgr             (Phoenix::Core::CResourceManager< Phoenix::Graphics::CModel, Phoenix::Core::CHandle<Phoenix::Graphics::CModel> >::GetInstance())
#define g_ObjectUpdater        (Phoenix::Default::CPhoenixObjectUpdater::GetInstance())


#endif
/////////////////////////////////////////////////////////////////
