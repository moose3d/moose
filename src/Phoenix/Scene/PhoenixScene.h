#ifndef __PhoenixScene_h__
#define __PhoenixScene_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSpatialGraph.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixAIScript.h"
#include <PhoenixGameObject.h>
#include <PhoenixRenderQueue.h>
#include <PhoenixCameraObject.h>
#include <PhoenixRenderable.h>
///////////////////////////////////////////////////////////////////////////////
struct Tcl_Interp;
namespace Phoenix
{
	namespace Scene
	{
		class CRTSCamera;
		class CDirectionalLightObject;
		typedef Phoenix::Graphics::CRenderQueue< Phoenix::Graphics::CRenderable *> RenderQueue;
		///////////////////////////////////////////////////////////////////////////////
		/// Camera with its properties related to rendering.
		class CCameraProperty
		{
		protected:

			Phoenix::Scene::CCameraObject * 		m_pCamera;					///!< Camera object
			Phoenix::Scene::GameObjectList	m_lstGameObjects; 	///!< Temporary collection of visible gameobjects.
			Phoenix::Scene::GameObjectList	m_lstActiveLights;	///!< Temporary list of visible lights within frustum.
			RenderQueue											m_RenderQueue; 			///!< Renderqueue for this camera.
			bool														m_bColliderRendering;  /// Should colliders be rendered for game objects.
		public:
			CCameraProperty( ) :m_bColliderRendering(false) { }
			Phoenix::Scene::CCameraObject *     GetCamera() { return m_pCamera; }
			void													   SetCamera( Phoenix::Scene::CCameraObject* pCamera ) { m_pCamera = pCamera; }
			Phoenix::Scene::GameObjectList & GetGameObjectList() { return m_lstGameObjects; }
			Phoenix::Scene::GameObjectList & GetActiveLights() 	{ return m_lstActiveLights; }
			RenderQueue										 & GetRenderQueue() { return m_RenderQueue; }
			void															SetColliderRendering( bool bFlag ) { m_bColliderRendering = bFlag; }
			bool															IsColliderRendering() const { return m_bColliderRendering; }

		}; // CCameraProperty
		///////////////////////////////////////////////////////////////////////////////
		typedef std::map<const std::string, CCameraProperty *> 				 CameraMap;
		///////////////////////////////////////////////////////////////////////////////
		/// Scene where everything is.
		class CScene : public Phoenix::Scene::CGameObject
		{
		protected:
			Phoenix::Scene::CSpatialGraph	*							 m_pSpatialGraph;   ///<! Spatial graph used for collision and culling.
			Phoenix::Scene::CTransformGraph *  					 m_pTransformGraph; ///<! Transforms. By default, every object is child of root.
			GameObjectList               								 m_lstGameObjects;  ///<! list of objects in this scene.
			RenderQueue											m_PreRenderQueue;   			///!< Render queue rendered before anything else (using all but GUI camera).
			RenderQueue											m_PostObjectRenderQueue; 	///!< Render queue rendered after all objects have been rendered from current scene. ( using all but GUI camera )
			RenderQueue											m_PreGUIRenderQueue;    	///!< Render queue rendered before GUI objects (using GUI camera).
			RenderQueue											m_PostGUIRenderQueue;    	///!< Render queue rendered after  GUI objects (using GUI camera).
			CameraMap												m_mapCameras;       			///!< Storage for all cameras used in this scene.
		protected:
			void   					 CollectVisibleGameObjects( CCameraProperty & cameraProp );
			/// Removes object pointer from caches.
			void             RemoveFromCaches( Phoenix::Scene::CGameObject *pObject );
		public:

			CScene(unsigned int nNumLevels, float fWorldSize );
			CScene(const char *szName, unsigned int nNumLevels, float fWorldSize );
			virtual ~CScene();

			Phoenix::Scene::CSpatialGraph & GetSpatialGraph();
			Phoenix::Scene::CTransformGraph &  GetTransformGraph();
			// Create object, put it into transform graph and spatial graph.
			Phoenix::Scene::CGameObject * AddGameObject( Phoenix::Scene::CGameObject *pObj );
			Phoenix::Scene::CDirectionalLightObject * AddDirectionalLight( Phoenix::Scene::CDirectionalLightObject *pObj );

			void RemoveGameObject( Phoenix::Scene::CGameObject *pObj );
			void Update( float fSeconds );
			void Init();
			bool AddCamera( const std::string & name, Phoenix::Scene::CCameraObject *pCamera  );
			void RemoveCamera( const std::string & name );
			CCameraProperty * GetCameraProperty( const std::string & name );
			void Render( Phoenix::Graphics::COglRenderer & renderer );
			RenderQueue & GetPreRenderQueue();
			RenderQueue & GetPostRenderQueue();
			RenderQueue & GetPreGUIRenderQueue();
			RenderQueue & GetPostGUIRenderQueue();
			void RegisterUserCommands();

		};
		/////////////////////////////////////////////////////////////////
		/// Parses regular camera parameters from name-object map.
		int ParseCameraParameters( Tcl_Interp *pInterp, NameObjMap & params, Phoenix::Scene::CCameraObject & cam );
	} // Scene
} // Phoenix
///////////////////////////////////////////////////////////////////////////////////
#endif /* CSCENE_H_ */
