#ifndef __PhoenixScene_h__
#define __PhoenixScene_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSpatialGraph.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixDefaultEntities.h"
#if !defined(PHOENIX_APPLE_IPHONE)
#include "PhoenixAIScript.h"
#endif
#include <PhoenixGameObject.h>
#include <PhoenixRenderQueue.h>
#include <PhoenixCameraObject.h>
#include <PhoenixRenderable.h>
///////////////////////////////////////////////////////////////////////////////
#if !defined(PHOENIX_APPLE_IPHONE)
struct Tcl_Interp;
#endif
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
        typedef std::map<Phoenix::Scene::CGameObject *, GameObjectList *>    GameObjectCollidersMap;
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

            GameObjectCollidersMap                              m_mapPotentialColliders; 	///!< From GameObject ptr to List of possible colliders.
            GameObjectCollidersMap                              m_mapColliders;             ///!< From GameObject ptr to List of currently colliding objects.

        protected:
			void   					 CollectVisibleGameObjects( CCameraProperty & cameraProp );
			/// Removes object pointer from caches.
			void             RemoveFromCaches( Phoenix::Scene::CGameObject *pObject );
		public:

			CScene(unsigned int nNumLevels, float fWorldSize );
			CScene(const char *szName, unsigned int nNumLevels, float fWorldSize );
			virtual ~CScene();
            void UpdateColliders();
            /// Checks collisions between neighbors and enqueues messages for scripts accordingly.
            void CheckCollisions();
            void CheckCollisions( Phoenix::Scene::CGameObject * pObj );
			Phoenix::Scene::CSpatialGraph & GetSpatialGraph();
			Phoenix::Scene::CTransformGraph &  GetTransformGraph();
			// Create object, put it into transform graph and spatial graph.
			Phoenix::Scene::CGameObject * AddGameObject( Phoenix::Scene::CGameObject *pObj );
			Phoenix::Scene::CDirectionalLightObject * AddDirectionalLight( Phoenix::Scene::CDirectionalLightObject *pObj );
            /// Deletes all gameobjects registered to this scene. 
            void DeleteGameObjects();
            Phoenix::Scene::GameObjectList & GetGameObjects();
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

			void PushPreRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void PushPostRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void PushPreGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void PushPostGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );

			void RemovePreRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void RemovePostRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void RemovePreGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );
			void RemovePostGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable );

            
			virtual void OnEnter();
            virtual void OnExit();
            virtual void Load();
            virtual void Unload();
            virtual void Reload();
#if defined(PHOENIX_APPLE_IPHONE)
            virtual void OnTouchBegan( float x, float y, int iFlags ) {}
            virtual void OnTouchMoved( float x, float y, int iFlags ) {}
            virtual void OnTouchEnded( float x, float y, int iFlags ) {}
#else
            // These contain TCL scripting, not enabled on iPhone
            void RegisterUserCommands();
#endif
          static void AssignLightsToRenderables( GameObjectList & lights, RenderQueue & queue);
          static void AssignLightsToObjects ( GameObjectList & lights, GameObjectList & objects );
          static void CollectRenderables( Phoenix::Graphics::CCamera & camera, GameObjectList & gameObjects, RenderQueue &queue );
		};
#if !defined(PHOENIX_APPLE_IPHONE)

        
		/////////////////////////////////////////////////////////////////
		/// Parses regular camera parameters from name-object map.
		int ParseCameraParameters( Tcl_Interp *pInterp, NameObjMap & params, Phoenix::Scene::CCameraObject & cam );
#endif
	} // Scene
} // Phoenix
///////////////////////////////////////////////////////////////////////////////////
#endif /* CSCENE_H_ */
