#ifndef __PhoenixScene_h__
#define __PhoenixScene_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSpatialGraph.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixDefaultEntities.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Scene
	{
		///////////////////////////////////////////////////////////////////////////////
		/// Scene where everything is.
		class CScene
		{
		public:
			CSpatialGraph	*							 m_pSpatialGraph;   ///<! Spatial graph used for collision and culling.
			CTransformGraph *  					 m_pTransformGraph; ///<! Transforms. By default, every object is child of root.
			GameObjectList               m_lstGameObjects;  ///<! list of objects in this scene.

			CScene(unsigned int nNumLevels, float fWorldSize);
			virtual ~CScene();

			CSpatialGraph & GetSpatialGraph();
			CTransformGraph &  GetTransformGraph();


			// Create object, put it into transform graph and spatial graph.
			template<class T> T * CreateGameObject( const char *szName, const CVector3<float> & vPosition = CVector3<float>(0,0,0) )
			{
				T *pObj = new T;
				pObj->Init();

				// By default, emerge at 0,0,0
				pObj->GetLocalTransform().SetTranslation(vPosition);
				pObj->SetName(szName);
				g_ObjectMgr->Create(pObj, szName, pObj->GetObjectHandle());

				// so it is affected by transforms.
				CObjectTransform *pObjTr = GetTransformGraph().Insert(pObj);

				// So it can be culled
				GetSpatialGraph().InsertObject(pObj);

				// Call Update periodically
				g_DefaultUpdater->Manage( pObj->GetObjectHandle() );
				return pObj;
			}

			void RemoveGameObject( Phoenix::Scene::CGameObject *pObj );

		};
	} // Scene
} // Phoenix
///////////////////////////////////////////////////////////////////////////////////
#endif /* CSCENE_H_ */
