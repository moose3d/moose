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
			Phoenix::Scene::CSpatialGraph	*							 m_pSpatialGraph;   ///<! Spatial graph used for collision and culling.
			Phoenix::Scene::CTransformGraph *  					 m_pTransformGraph; ///<! Transforms. By default, every object is child of root.
			GameObjectList               m_lstGameObjects;  ///<! list of objects in this scene.

			CScene(unsigned int nNumLevels, float fWorldSize);
			virtual ~CScene();

			Phoenix::Scene::CSpatialGraph & GetSpatialGraph();
			Phoenix::Scene::CTransformGraph &  GetTransformGraph();


			// Create object, put it into transform graph and spatial graph.
			Phoenix::Scene::CGameObject * AddGameObject( Phoenix::Scene::CGameObject *pObj );


			void RemoveGameObject( Phoenix::Scene::CGameObject *pObj );

		};
	} // Scene
} // Phoenix
///////////////////////////////////////////////////////////////////////////////////
#endif /* CSCENE_H_ */
