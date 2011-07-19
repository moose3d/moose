#ifndef __MooseDefaultEntities_h__
#define __MooseDefaultEntities_h__
/////////////////////////////////////////////////////////////////
#include <MooseResourceManager.h>
#include <MooseUpdateable.h>
#include <MooseTexture.h>
#include <MooseShader.h>
#include <MooseVertexDescriptor.h>
#include <MooseIndexArray.h>
#include "MooseAPI.h"
#include <MooseAssetBundle.h>
#include <MooseSDLScreen.h>

/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
	  class CGameObject;
    class CApplication;
  }
  namespace Data
  {
    class CModelHelper;
  }
  namespace Graphics
  {
	  class CModel;
	  class CCamera;
  }
  namespace Collision 
  {
    class ICollider;
  }
  namespace Sound
  {
      class CALSample;
      class CALStreamSample;
      class CAudioSystem;
  }
  namespace Volume 
  {
    class CSphere;
  }
  namespace Core
  {
    class CUniqueNameCreator;
  }
  namespace Default
  {
    // Typedefs for default handle types.
    typedef Moose::Core::CHandle<Moose::Graphics::COglTexture>       TEXTURE_HANDLE;
    typedef Moose::Core::CHandle<Moose::Graphics::CVertexDescriptor> VERTEX_HANDLE;
    typedef Moose::Core::CHandle<Moose::Graphics::CIndexArray>       INDEX_HANDLE;
    typedef Moose::Core::CHandle<Moose::Graphics::CShader>           SHADER_HANDLE;
    typedef Moose::Core::CHandle<Moose::Graphics::CModel>			 MODEL_HANDLE;
    typedef Moose::Core::CHandle<Moose::Scene::CGameObject>			 OBJECT_HANDLE;
    typedef Moose::Core::CHandle<Moose::Graphics::CCamera>			 CAMERA_HANDLE;
    typedef Moose::Core::CHandle<Moose::Collision::ICollider>		 COLLIDER_HANDLE;
    typedef Moose::Core::CHandle<Moose::Sound::CALSample>            SOUND_SAMPLE_HANDLE;
    typedef Moose::Core::CHandle<Moose::Sound::CALStreamSample>      SOUND_STREAM_HANDLE;
    typedef Moose::Core::CHandle<Moose::Volume::CSphere>             SPHERE_HANDLE;
    ////////////////////
    // Typedefs for default resource manager types.
    typedef Moose::Core::CResourceManager< Moose::Graphics::COglTexture,		TEXTURE_HANDLE > TextureManager;
    typedef Moose::Core::CResourceManager< Moose::Graphics::CVertexDescriptor,	VERTEX_HANDLE  > VertexManager;
    typedef Moose::Core::CResourceManager< Moose::Graphics::CIndexArray,		INDEX_HANDLE   > IndexManager;
    typedef Moose::Core::CResourceManager< Moose::Graphics::CShader,			SHADER_HANDLE  > ShaderManager;
    typedef Moose::Core::CResourceManager< Moose::Graphics::CCamera,			CAMERA_HANDLE  > CameraManager;
    typedef Moose::Core::CResourceManager< Moose::Scene::CGameObject, 			OBJECT_HANDLE  > ObjectMgr;
    typedef Moose::Core::CResourceManager< Moose::Graphics::CModel, 			MODEL_HANDLE   > ModelManager;
    typedef Moose::Core::CResourceManager< Moose::Collision::ICollider, 		COLLIDER_HANDLE > ColliderManager;
    typedef Moose::Core::CResourceManager< Moose::Sound::CALSample,             SOUND_SAMPLE_HANDLE>   SoundSampleManager;
    typedef Moose::Core::CResourceManager< Moose::Sound::CALStreamSample,       SOUND_STREAM_HANDLE>   SoundStreamManager;
    typedef Moose::Core::CResourceManager< Moose::Volume::CSphere,              SPHERE_HANDLE>         SphereManager;
    ////////////////////
#ifdef WIN32
#ifndef MOOSE_DLL_EXPORT
    //  extern template class __declspec(dllimport) Moose::Core::CResourceManager<Moose::Graphics::COglTexture,        Moose::Default::TEXTURE_HANDLE>;
    //  extern template class __declspec(dllimport) Moose::Core::CResourceManager<Moose::Graphics::CVertexDescriptor,  Moose::Default::TEXTURE_HANDLE>;
    //  extern template class __declspec(dllimport) Moose::Core::CResourceManager<Moose::Graphics::CIndexArray,	    Moose::Default::INDEX_HANDLE>;
    //  extern template class __declspec(dllimport) Moose::Core::CResourceManager<Moose::Graphics::CShader,	    Moose::Default::SHADER_HANDLE>;

#endif
#endif

    ObjectMgr * 		GetObjectMgr();
    TextureManager * 	GetTextureMgr();
    VertexManager * 	GetVertexMgr();
    IndexManager  * 	GetIndexMgr();
    ShaderManager * 	GetShaderMgr();
    ModelManager * 		GetModelMgr();
    CameraManager * 	GetCameraMgr();
    ColliderManager *   GetColliderMgr();
    Moose::Sound::CAudioSystem * GetAudioSystem();
    SoundSampleManager * GetSoundSampleMgr();
    SoundStreamManager * GetSoundStreamMgr();
    SphereManager *      GetSphereMgr();
    Moose::Core::CUniqueNameCreator * GetNameCreator();
    Moose::Core::CAssetBundle *       GetAssetBundle();
    Moose::Data::CModelHelper *       GetModelHelper();
    Moose::Window::CSDLScreen *       GetScreen();
    Moose::Scene::CApplication *      GetApplication();
  }; // namespace Default
}; // namespace Moose
////////////////////
// Fake global variables for managers.
#define g_DefaultTextureManager (Moose::Default::TextureManager::GetInstance())
#define g_DefaultVertexManager  (Moose::Default::VertexManager::GetInstance())
#define g_DefaultIndexManager   (Moose::Default::IndexManager::GetInstance())
#define g_DefaultShaderManager  (Moose::Default::ShaderManager::GetInstance())
#define g_UniqueNameStr         ((Moose::Core::CUniqueNameCreator::GetInstance())->GetUniqueName())
#define g_UniqueName            ((Moose::Core::CUniqueNameCreator::GetInstance())->GetUniqueName().c_str())
#define CreateUniqueNameStr(A)  ((Moose::Core::CUniqueNameCreator::GetInstance())->GetUniqueName(A))
#define CreateUniqueName(A)     ((Moose::Core::CUniqueNameCreator::GetInstance())->GetUniqueName(A).c_str())
/////////////////////////////////////////////////////////////////
/// Log streams.
#define g_Error ( Moose::Core::CLogger::GetInstance()->Err())
#define g_Log   ( Moose::Core::CLogger::GetInstance()->Msg())
#define g_Warn  ( Moose::Core::CLogger::GetInstance()->Warn())
////////////////////////////////////////////////////////////////////////////////
/// Manager for gameobjects.


//Moose::Core::CResourceManager<Moose::Scene::CGameObject, Moose::Core::CHandle<Moose::Scene::CGameObject> > * GetObjectMgr();

#define g_ObjectMgr            (Moose::Default::ObjectMgr::GetInstance())
#define g_TextureMgr           (Moose::Default::TextureManager::GetInstance())
#define g_VertexMgr            (Moose::Default::VertexManager::GetInstance())
#define g_IndexMgr             (Moose::Default::IndexManager::GetInstance())
#define g_ShaderMgr            (Moose::Default::ShaderManager::GetInstance())
#define g_ModelMgr             (Moose::Core::CResourceManager< Moose::Graphics::CModel, Moose::Core::CHandle<Moose::Graphics::CModel> >::GetInstance())
#define g_CameraMgr            (Moose::Core::CResourceManager< Moose::Graphics::CCamera, Moose::Core::CHandle<Moose::Graphics::CCamera> >::GetInstance())
#define g_ColliderMgr          (Moose::Core::CResourceManager< Moose::Collision::ICollider, Moose::Core::CHandle<Moose::Collision::ICollider> >::GetInstance())
// TODO should these be combined?
#define g_StreamMgr            (SoundStreamManager::GetInstance())
#define g_SampleMgr            (SoundSampleManager::GetInstance())
#define g_SphereMgr            (SphereManager::GetInstance())
// For even easier access to resources
#define g_Objects            (*Moose::Default::ObjectMgr::GetInstance())
#define g_Textures           (*Moose::Default::TextureManager::GetInstance())
#define g_Vertices            (*Moose::Default::VertexManager::GetInstance())
#define g_Indices             (*Moose::Default::IndexManager::GetInstance())
#define g_Shaders            (*Moose::Default::ShaderManager::GetInstance())
#define g_Models             (*Moose::Core::CResourceManager< Moose::Graphics::CModel, Moose::Core::CHandle<Moose::Graphics::CModel> >::GetInstance())
#define g_Cameras            (*Moose::Core::CResourceManager< Moose::Graphics::CCamera, Moose::Core::CHandle<Moose::Graphics::CCamera> >::GetInstance())
#define g_Colliders          (*Moose::Core::CResourceManager< Moose::Collision::ICollider, Moose::Core::CHandle<Moose::Collision::ICollider> >::GetInstance())
// TODO should these be combined?
#define g_Streams            (*SoundStreamManager::GetInstance())
#define g_Samples            (*SoundSampleManager::GetInstance())
// Audio system
#define g_AudioSystem          (Moose::Sound::CAudioSystem::GetInstance())
#define g_AssetBundle        (Moose::Core::CAssetBundle::GetInstance())
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
