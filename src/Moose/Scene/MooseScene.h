#ifndef __MooseScene_h__
#define __MooseScene_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseSpatialGraph.h"
#include "MooseTransformGraph.h"
#include "MooseDefaultEntities.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseAIScript.h"
#endif
#include <MooseGameObject.h>
#include <MooseRenderQueue.h>
#include <MooseCameraObject.h>
#include <MooseRenderable.h>
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
struct Tcl_Interp;
#endif
namespace Moose
{
	namespace Scene
	{
		class CRTSCamera;
		class CDirectionalLightObject;
        typedef Moose::Graphics::CRenderQueue< Moose::Graphics::CRenderable *> RenderQueue;
		///////////////////////////////////////////////////////////////////////////////
		/// Camera with its properties related to rendering.
		class CCameraProperty
		{
		protected:

			Moose::Scene::CCameraObject * 	m_pCamera;					///!< Camera object
			Moose::Scene::GameObjectList	m_lstGameObjects; 	///!< Temporary collection of visible gameobjects.
			Moose::Scene::GameObjectList	m_lstActiveLights;	///!< Temporary list of visible lights within frustum.
			RenderQueue						m_RenderQueue; 			///!< Renderqueue for opaque objects in this camera.
			RenderQueue						m_TransparentQueue; 			///!< Renderqueue for transparent objects in this camera.
			bool							m_bColliderRendering;  /// Should colliders be rendered for game objects.
		public:
			CCameraProperty( ) :m_bColliderRendering(false) { }
			Moose::Scene::CCameraObject *     GetCamera() { return m_pCamera; }
			void													   SetCamera( Moose::Scene::CCameraObject* pCamera ) { m_pCamera = pCamera; }
			Moose::Scene::GameObjectList & GetGameObjectList() { return m_lstGameObjects; }
			Moose::Scene::GameObjectList & GetActiveLights() 	{ return m_lstActiveLights; }
			RenderQueue										 & GetRenderQueue() { return m_RenderQueue; }
			RenderQueue										 & GetTransparentQueue() { return m_TransparentQueue; }
			void															SetColliderRendering( bool bFlag ) { m_bColliderRendering = bFlag; }
			bool															IsColliderRendering() const { return m_bColliderRendering; }

		}; // CCameraProperty
		///////////////////////////////////////////////////////////////////////////////
		typedef std::map<const std::string, CCameraProperty *> 				 CameraMap;
        typedef std::map<Moose::Scene::CGameObject *, GameObjectList *>    GameObjectCollidersMap;
		///////////////////////////////////////////////////////////////////////////////
		/// Scene where everything is.
		class CScene : public Moose::Scene::CGameObject
		{
		protected:
			Moose::Scene::CSpatialGraph	*                       m_pSpatialGraph;   ///<! Spatial graph used for collision and culling.
			Moose::Scene::CTransformGraph *  					m_pTransformGraph; ///<! Transforms. By default, every object is child of root.
			GameObjectList               						m_lstGameObjects;  ///<! list of objects in this scene.
            
			RenderQueue											m_StaticQueue; 	   ///!< All objects not collected using a camera.
			CameraMap											m_mapCameras;      ///!< Storage for all cameras used in this scene.

            GameObjectCollidersMap                              m_mapPotentialColliders; 	///!< From GameObject ptr to List of possible colliders.
            GameObjectCollidersMap                              m_mapColliders;             ///!< From GameObject ptr to List of currently colliding objects.

        protected:
			void   					 CollectVisibleGameObjects( CCameraProperty & cameraProp );
			/// Removes object pointer from caches.
			void             RemoveFromCaches( Moose::Scene::CGameObject *pObject );
		public:

			CScene(unsigned int nNumLevels, float fWorldSize );
			CScene(const char *szName, unsigned int nNumLevels, float fWorldSize );
			virtual ~CScene();
            void UpdateColliders();
            /// Checks collisions between neighbors and enqueues messages for scripts accordingly.
            void CheckCollisions();
            void CheckCollisions( Moose::Scene::CGameObject * pObj );
			Moose::Scene::CSpatialGraph & GetSpatialGraph();
			Moose::Scene::CTransformGraph &  GetTransformGraph();
			// Create object, put it into transform graph and spatial graph.
			Moose::Scene::CGameObject * AddGameObject( Moose::Scene::CGameObject *pObj );
			Moose::Scene::CDirectionalLightObject * AddDirectionalLight( Moose::Scene::CDirectionalLightObject *pObj );
            /// Deletes all gameobjects registered to this scene. 
            void DeleteGameObjects();
            Moose::Scene::GameObjectList & GetGameObjects();
			void RemoveGameObject( Moose::Scene::CGameObject *pObj );
			void Update( float fSeconds );
			void Init();
			bool AddCamera( const std::string & name, Moose::Scene::CCameraObject *pCamera  );
			void RemoveCamera( const std::string & name );
			CCameraProperty * GetCameraProperty( const std::string & name );
			void Render( Moose::Graphics::COglRenderer & renderer );
			RenderQueue & GetStaticRenderQueue();
			
			void PushToRenderQueue( Moose::Graphics::CRenderable *pRenderable );
			void RemoveFromRenderQueue( Moose::Graphics::CRenderable *pRenderable );
            
			virtual void OnEnter();
            virtual void OnExit();
            virtual void Load();
            virtual void Unload();
            virtual void Reload();
#if defined(MOOSE_APPLE_IPHONE)
            virtual void OnTouchBegan( float x, float y, int iFlags ) {}
            virtual void OnTouchMoved( float x, float y, int iFlags ) {}
            virtual void OnTouchEnded( float x, float y, int iFlags ) {}
            virtual void OnAccelerate( float x, float y, float z, int iFlags) {}
#else
            // These contain TCL scripting, not enabled on iPhone
            void RegisterUserCommands();
#endif
            static void AssignLightsToRenderables( GameObjectList & lights, RenderQueue & queue);
            static void AssignLightsToObjects ( GameObjectList & lights, GameObjectList & objects );
            static void CollectRenderables( Moose::Graphics::CCamera & camera, GameObjectList & gameObjects, RenderQueue &queue );
            // Collects static renderables to given queue.
            void CollectStaticRenderables( RenderQueue & queue );
        };
#if !defined(MOOSE_APPLE_IPHONE)

        
		/////////////////////////////////////////////////////////////////
		/// Parses regular camera parameters from name-object map.
		int ParseCameraParameters( Tcl_Interp *pInterp, NameObjMap & params, Moose::Scene::CCameraObject & cam );
#endif
	} // Scene
} // Moose
///////////////////////////////////////////////////////////////////////////////////
#endif /* CSCENE_H_ */
