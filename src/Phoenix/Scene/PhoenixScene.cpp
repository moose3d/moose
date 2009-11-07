#include "PhoenixScene.h"
#include "PhoenixSpatialGraph.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixDefaultEntities.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace std;
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CScene::CScene(unsigned int nNumLevels, float fWorldSize)
{
	m_pSpatialGraph   = new CSpatialGraph(nNumLevels, fWorldSize);
	m_pTransformGraph = new CTransformGraph();

}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CScene::~CScene()
{
	delete m_pSpatialGraph;
	delete m_pTransformGraph;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject *
Phoenix::Scene::CScene::AddGameObject( Phoenix::Scene::CGameObject *pObj )
{
	pObj->Init();
	// Create handle to object
	g_ObjectMgr->Create(pObj, pObj->GetName(), pObj->GetObjectHandle());

	// so it is affected by transforms.
	CObjectTransform *pObjTr = GetTransformGraph().Insert(pObj);

	// So it can be culled
	GetSpatialGraph().InsertObject(pObj);

	// Call Update periodically
	g_DefaultUpdater->Manage( pObj->GetObjectHandle() );
	return pObj;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemoveGameObject( Phoenix::Scene::CGameObject *pObj )
{
	if ( pObj == NULL ) return;



	if ( pObj->GetTransformNode() != NULL ) // remove from transform (with children)
	{
		TransformableList tmpList;
		GetTransformGraph().CollectSubtree( pObj, tmpList );
		while( tmpList.empty() == false)
		{
			CTransformable *pTmp = tmpList.front();
			tmpList.pop_front();
			// remove from spatial
			GetSpatialGraph().DeleteObject( dynamic_cast<Phoenix::Scene::CGameObject *>(pTmp) );
			// (remove from transform graph and) delete object.
			delete pTmp;
		}
	}
	delete pObj;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CSpatialGraph &
Phoenix::Scene::CScene::GetSpatialGraph()
{
	return *m_pSpatialGraph;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CTransformGraph &
Phoenix::Scene::CScene::GetTransformGraph()
{
	return *m_pTransformGraph;
}
///////////////////////////////////////////////////////////////////////////////