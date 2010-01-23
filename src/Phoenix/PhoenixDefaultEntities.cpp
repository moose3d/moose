#include "PhoenixDefaultEntities.h"
///////////////////////////////////////////////////////////////////////////////
Phoenix::Default::ObjectMgr *
Phoenix::Default::GetObjectMgr()
{
	return g_ObjectMgr;
}
///////////////////////////////////////////////////////////////////////////////
//#define g_TextureMgr           (Phoenix::Default::TextureManager::GetInstance())
//#define g_VertexMgr            (Phoenix::Default::VertexManager::GetInstance())
//#define g_IndexMgr             (Phoenix::Default::IndexManager::GetInstance())
//#define g_ShaderMgr            (Phoenix::Default::ShaderManager::GetInstance())
//#define g_ModelMgr             (Phoenix::Core::CResourceManager< Phoenix::Graphics::CModel, Phoenix::Core::CHandle<Phoenix::Graphics::CModel> >::GetInstance())
//#define g_ObjectUpdater        (Phoenix::Default::CPhoenixObjectUpdater::GetInstance())
