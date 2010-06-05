#include "PhoenixScene.h"
#include "PhoenixSpatialGraph.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixDirectionalLightObject.h"
#include "PhoenixLogger.h"
#if !defined(PHOENIX_APPLE_IPHONE)
#include <PhoenixSDLScreen.h>
#endif
#include "PhoenixCameraObject.h"
#include <PhoenixVector2.h>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
#if !defined(PHOENIX_APPLE_IPHONE)
using namespace Phoenix::Window;
#endif
using namespace Phoenix::Math;
using namespace std;
///////////////////////////////////////////////////////////////////////////////
void AssignLightsToRenderables( GameObjectList & lights, CRenderQueue<CRenderable *> & queue);
void AssignLightsToObjects ( GameObjectList & lights, GameObjectList & objects );
void CollectRenderables( CCamera & camera, GameObjectList & gameObjects, CRenderQueue<CRenderable *> &queue );
///////////////////////////////////////////////////////////////////////////////
/// Very simple sorter; sorts only by transparency.
class CGameObjectSorter
{
public:
	inline void Sort( std::list< CRenderable * > & list )
  {
    std::list< CRenderable * >::iterator it = list.begin();
    std::list< CRenderable * > listNew;
    std::list< CRenderable * > listLights;
    while( it != list.end())
    {
      if ( (*it)->GetRenderState().IsTransparent() )
    	  listNew.push_back( *it );
      else if ( (*it)->GetRenderState().IsLightSource())
    	  listLights.push_back( *it );
   	  else
    	  listNew.push_front( *it );
      it++;
    }
    it = listLights.begin();

    // Insert lights in front of the list.
    while( it != listLights.end() )
    {
    	listNew.push_front(*it);
    	it++;
    }

    list.swap(listNew);
    listNew.clear();
    listLights.clear();
  }
};
///////////////////////////////////////////////////////////////////////////////
CGameObjectSorter game_object_sorter;
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CScene::CScene( unsigned int nNumLevels, float fWorldSize )
{
	m_pSpatialGraph   = new CSpatialGraph(nNumLevels, fWorldSize);
	m_pTransformGraph = new CTransformGraph();
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CScene::CScene( const char *szName, unsigned int nNumLevels, float fWorldSize )
{
	m_pSpatialGraph   = new CSpatialGraph(nNumLevels, fWorldSize);
	m_pTransformGraph = new CTransformGraph();
	SetName( szName );
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CScene::~CScene()
{
	delete m_pSpatialGraph;
	delete m_pTransformGraph;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::Init()
{
#if !defined(PHOENIX_APPLE_IPHONE)
	LoadScript();
#endif
	// ensure that each scene is managed and have a unique name
	std::string tmpName = GetName();
	if ( tmpName.empty() )
	{
		tmpName = CreateUniqueNameStr("Scene_");
	}
	// For messaging
	g_ObjectMgr->Create( this, tmpName, this->GetObjectHandle());
	// Regular Update()s.
	g_ObjectUpdater->Manage( this->GetObjectHandle() );
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject *
Phoenix::Scene::CScene::AddGameObject( Phoenix::Scene::CGameObject *pObj )
{


	// Create handle to object
	assert( g_ObjectMgr->Create(pObj, pObj->GetName(), pObj->GetObjectHandle()) == 0 );
    // Intialize after object has been registered, so it can be accessed during script init via
    // objectmgr.
    pObj->Init();
	// so it is affected by transforms.
	/*CObjectTransform *pObjTr = */GetTransformGraph().Insert(pObj);

	// So it can be culled
	GetSpatialGraph().InsertObject(pObj);

	// Call Update periodically
	g_DefaultUpdater->Manage( pObj->GetObjectHandle() );
	// insert local to local ptr list
	m_lstGameObjects.push_back( pObj );
	return pObj;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CDirectionalLightObject *
Phoenix::Scene::CScene::AddDirectionalLight( Phoenix::Scene::CDirectionalLightObject *pObj )
{
	// Set bounding sphere to be as large as world itself.
	pObj->GetBoundingSphere().SetRadius( GetSpatialGraph().GetWorldSize());

	pObj->Init();

	// Create handle to object
	g_ObjectMgr->Create(pObj, pObj->GetName(), pObj->GetObjectHandle());

	// Transforms won't be needed, since this object is not regular gameobject.

	// So it can be culled
	GetSpatialGraph().InsertObject(pObj);

	// Call Update periodically
	g_DefaultUpdater->Manage( pObj->GetObjectHandle() );
    return pObj;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemoveFromCaches( Phoenix::Scene::CGameObject *pObject )
{
	CameraMap::iterator it = m_mapCameras.begin();
	for( ; it != m_mapCameras.end(); it++)
	{
		it->second->GetGameObjectList().remove( pObject ); // if regular object
		it->second->GetActiveLights().remove( pObject ); // if it was a light.
	}
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
			CGameObject *pChild = dynamic_cast<Phoenix::Scene::CGameObject *>(pTmp);
			GetSpatialGraph().DeleteObject( pChild );
			m_lstGameObjects.remove(pChild);

			RemoveFromCaches(pChild);
			// remove from transform graph and delete object.
			delete pTmp->GetTransformNode();
			g_ObjectMgr->Destroy( pChild->GetObjectHandle() );
		}
	}
	assert( pObj->GetObjectHandle().IsNull() && "Object not deleted.");
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
void
Phoenix::Scene::CScene::Update( float fSeconds )
{
#if !defined(PHOENIX_APPLE_IPHONE)
  // Run local script
	UpdateScript(fSeconds);
#endif
	// update transform graph
	GetTransformGraph().Update();
	// update object positions in spatial graph.
	GameObjectList::iterator it = m_lstGameObjects.begin();
	for ( ; it != m_lstGameObjects.end(); it++ )
	{
		GetSpatialGraph().Update( *it );
	}

	CameraMap::iterator camIt = m_mapCameras.begin();
	for( ; camIt != m_mapCameras.end(); camIt++ )
	{
		bool bChanged = false;
		CCameraObject & c = *camIt->second->GetCamera();
		c.Update( fSeconds );
		if ( c.IsProjectionChanged() )  {   bChanged = true; c.UpdateProjection();    }
		if ( c.IsViewChanged() )     {      bChanged = true; c.UpdateView();          }
		if ( bChanged )
		{
			c.CalculateFrustum();
			c.CalculateBoundingSphere();
			c.CalculateBoundingCone();
		}
		CollectVisibleGameObjects( *camIt->second );
		AssignLightsToObjects( camIt->second->GetActiveLights(), camIt->second->GetGameObjectList());

	}
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::Render( COglRenderer & renderer )
{
	CameraMap::iterator camIt = m_mapCameras.begin();
	for( ; camIt != m_mapCameras.end(); camIt++ )
	{
		// CollectVisibleGameObjects( camIt->second );
		CCameraProperty & prop = *camIt->second;
		CCameraObject & c = *prop.GetCamera();
		prop.GetRenderQueue().Clear();
		CollectRenderables( c, prop.GetGameObjectList(), prop.GetRenderQueue() );
		AssignLightsToRenderables( prop.GetActiveLights(), prop.GetRenderQueue() );
		prop.GetRenderQueue().Sort( game_object_sorter );

		renderer.CommitCamera( c );

		m_PreRenderQueue.Render( renderer );          // before scene objects
		prop.GetRenderQueue().Render( renderer );     // scene objects
		m_PostObjectRenderQueue.Render( renderer );   // after scene objects

		if ( prop.IsColliderRendering() ) // For collider rendering.
		{
			renderer.CommitShader( NULL );
			renderer.DisableCaches();
			for( GameObjectList::iterator it = prop.GetGameObjectList().begin();
					it != prop.GetGameObjectList().end(); it++)
			{
				(*it)->GetCollider()->Render( renderer );
			}
		}
		//g_Log << "Num of renderables: " << prop.GetRenderQueue().GetObjectList().size() << endl;
	}
	m_PreGUIRenderQueue.Render( renderer );       // before GUI
	                                                  // GUI
	m_PostGUIRenderQueue.Render( renderer );      // After GUI

	renderer.Finalize();
	// OpenGL context may be created also via other method.
#if !defined(PHOENIX_APPLE_IPHONE)
	if (CSDLScreen::Exists()){ CSDLScreen::GetInstance()->SwapBuffers();}
#endif
	// for each camera do
	// CollectVisibleGameObjects();
	// for each game object
	//    CollectRenderables
	// for each renderable
	//    sort
	// Render(renderables)
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::CollectVisibleGameObjects( CCameraProperty & camProp )
{
	Phoenix::Scene::CCameraObject & camera = *camProp.GetCamera();
	GameObjectList & list = camProp.GetGameObjectList();
	list.clear();
	GameObjectList & lstLights = camProp.GetActiveLights();
	lstLights.clear();
	int iTag = LIGHT_TAG | COLLIDER_TAG;
	GetSpatialGraph().CollectObjects( camera, list, 		  iTag, CTagged::NOT_AND );
	GetSpatialGraph().CollectObjects( camera, lstLights,  LIGHT_TAG, CTagged::AND );
}
///////////////////////////////////////////////////////////////////////////////
CCameraProperty *
Phoenix::Scene::CScene::GetCameraProperty( const std::string & name )
{
	CameraMap::iterator it = m_mapCameras.find(name);
	if ( it != m_mapCameras.end() )
	{
		return it->second;
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////
void
AssignLightsToRenderables( GameObjectList & lights, CRenderQueue<CRenderable *> & queue)
{
	std::list<CRenderable * >::iterator rIt = queue.GetObjectList().begin();
	for( ; rIt != queue.GetObjectList().end(); rIt++)
	{
		(*rIt)->GetRenderState().GetLights().clear();
		GameObjectList::iterator it = lights.begin();
		for( ; it != lights.end(); it++ )
		{
			CGameObject & obj = **it;
			if ( obj.IsEnabled() )
			{
				obj.GetRenderableObjects(0).front();
				(*rIt)->GetRenderState().GetLights().push_back( obj.GetRenderableObjects(0).front());
			}
		}
	}
}
/////////////////////////////////////////////////////////////////
void
AssignLightsToObjects ( GameObjectList & lights, GameObjectList & objects )
{
	GameObjectList::iterator objIt = objects.begin();
	for( ; objIt != objects.end(); objIt++)
	{
		// Remove previous lights, if any
		(*objIt)->GetLights().clear();
		//CSphere s = (*objIt)->GetWorldBoundingSphere();

		// Insert new light pointer if it collides with object.
		GameObjectList::iterator lightIt = lights.begin();
		for( ; lightIt != lights.end(); lightIt++)
		{
			if ( (*lightIt)->IsEnabled() && (*lightIt)->GetCollider()->Intersects( **objIt )  )
			{
				(*objIt)->GetLights().push_back( (*lightIt)->GetRenderableObjects(0).front() );
			}
		}
	}
}
/////////////////////////////////////////////////////////////////
void
CollectRenderables( CCamera & camera, GameObjectList & gameObjects, CRenderQueue<CRenderable *> &queue )
{
  GameObjectList::iterator it = gameObjects.begin();
  for( ; it != gameObjects.end(); it++)
  {
    CGameObject & obj = **it;
    if ( obj.IsEnabled() )
    {
      // \TODO select LOD level by object distance, etc. here.
      //
      float fDistSqr= (camera.GetPosition() - obj.GetWorldTransform().GetTranslation()).LengthSqr();
      size_t lodLevel = obj.GetLodLevel( fDistSqr );
      RenderableList & list = obj.GetRenderableObjects(lodLevel);
      RenderableList::iterator itRend = list.begin();

      for( ; itRend != list.end(); itRend++)
      {
    	  CRenderable *pRenderable = *itRend;

//    	  g_Log << "Renderable has lights: " << pRenderable->GetRenderState().GetLights().size() << endl;
//    	  g_Log << "Object '" << obj.GetName() << "' has lights: " << obj.GetLights().size() << endl;
    	  pRenderable->GetRenderState().GetLights() = obj.GetLights();

    	  // add renderable to queue
    	  queue.GetObjectList().push_back( pRenderable );
		  // attach lights to renderable
    	  //(***itRend)->GetRenderState().GetLights().clear();
    	  //(***itRend)->GetRenderState().GetLights() = obj.GetLights();

      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CScene::AddCamera( const std::string & name, Phoenix::Scene::CCameraObject *pCamera )
{
	if ( m_mapCameras.find( name ) == m_mapCameras.end() )
	{
		CCameraProperty *pProp = new CCameraProperty();
		pProp->SetCamera( pCamera );
		m_mapCameras[name] = pProp;
		if ( g_ObjectMgr->Create(pCamera, CreateUniqueName(name.c_str()), pCamera->GetObjectHandle()) == 0 )
		{
			// so it is affected by transforms.
			GetTransformGraph().Insert(pCamera);
			return true;
		}
		else
		{
			RemoveCamera( name );
			return false;
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemoveCamera( const std::string & name )
{
	CameraMap::iterator it = m_mapCameras.find(name);
	if ( it != m_mapCameras.end() )
	{
		g_ObjectMgr->Destroy( it->second->GetCamera()->GetObjectHandle() );
		delete it->second;
		m_mapCameras.erase(it);

	}
}
///////////////////////////////////////////////////////////////////////////////
RenderQueue &
Phoenix::Scene::CScene::GetPreRenderQueue()
{
	return m_PreRenderQueue;
}
///////////////////////////////////////////////////////////////////////////////
RenderQueue &
Phoenix::Scene::CScene::GetPostRenderQueue()
{
	return m_PostObjectRenderQueue;
}
///////////////////////////////////////////////////////////////////////////////
RenderQueue &
Phoenix::Scene::CScene::GetPreGUIRenderQueue()
{
	return m_PreGUIRenderQueue;
}
///////////////////////////////////////////////////////////////////////////////
RenderQueue &
Phoenix::Scene::CScene::GetPostGUIRenderQueue()
{
	return m_PostGUIRenderQueue;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::PushPreRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPreRenderQueue().GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::PushPostRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPostRenderQueue().GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::PushPreGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPreGUIRenderQueue().GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::PushPostGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPostGUIRenderQueue().GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemovePreRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPreRenderQueue().GetObjectList().remove(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemovePostRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPostRenderQueue().GetObjectList().remove(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemovePreGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPreGUIRenderQueue().GetObjectList().remove(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RemovePostGUIRenderQueue( Phoenix::Graphics::CRenderable *pRenderable )
{
	GetPostGUIRenderQueue().GetObjectList().remove(pRenderable);
}
void
Phoenix::Scene::CScene::OnEnter()
{

}
void
Phoenix::Scene::CScene::OnExit()
{

}
void
Phoenix::Scene::CScene::Load()
{

}
void
Phoenix::Scene::CScene::Unload()
{

}
void
Phoenix::Scene::CScene::Reload()
{
    Unload();
    Load();
}
///////////////////////////////////////////////////////////////////////////////
#if !defined(PHOENIX_APPLE_IPHONE)
SCRIPT_CMD_DECL( Destroy );          ///!< Kill switch for any object in the scene.
SCRIPT_CMD_DECL( CreateCamera );
SCRIPT_CMD_DECL( MoveCamera );
SCRIPT_CMD_DECL( ElevateCamera );
SCRIPT_CMD_DECL( StrafeCamera );
SCRIPT_CMD_DECL( GetCameraPosition );
SCRIPT_CMD_DECL( SetCameraPosition );
SCRIPT_CMD_DECL( SetCameraColliderRendering ); ///!< Enables / disables collider rendering for given camera.
SCRIPT_CMD_DECL( ToggleCameraColliderRendering ); ///!< Enables / disables collider rendering for given camera.
SCRIPT_CMD_DECL( StopCamera );
SCRIPT_CMD_DECL( TrackballRotate );
SCRIPT_CMD_DECL( ZoomCamera );
SCRIPT_CMD_DECL( RayCastCoords );          ///!< Performs ray collision test on all objects in scene.
SCRIPT_CMD_DECL( RayCastCoordsVisible );   ///!< Performs ray collision test on all currently visible objects in scene.
SCRIPT_CMD_DECL( RayCast );          ///!< Performs ray collision test on all objects in scene.
SCRIPT_CMD_DECL( RayCastVisible );   ///!< Performs ray collision test on all currently visible objects in scene.
SCRIPT_CMD_DECL( RectCastCoords );         ///!< Performs frustum collision test on all objects in scene.
SCRIPT_CMD_DECL( RectCastCoordsVisible );  ///!< Performs frustum collision test on all currentl visible objects in scene.
SCRIPT_CMD_DECL( CameraCreateRay );  ///!< Creates a ray for raycasting from screen coordinates.
SCRIPT_CMD_DECL( SetParent );               // Sets parent of an object to given object [name], if it exists.
SCRIPT_CMD_DECL( ResetParent );	           // Sets parent of this object in transform graph as root.
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CScene::RegisterUserCommands()
{
	CREATE_CMD_PTR( CreateCamera, this );
	CREATE_CMD_PTR( MoveCamera, this );
	CREATE_CMD_PTR( StrafeCamera, this );
	CREATE_CMD_PTR( ElevateCamera, this );
	CREATE_CMD_PTR( GetCameraPosition, this );
	CREATE_CMD_PTR( SetCameraPosition, this );
	CREATE_CMD_PTR( StopCamera, this );
	CREATE_CMD_PTR( TrackballRotate, this );
	CREATE_CMD_PTR( ZoomCamera, this );
	CREATE_CMD_PTR( RayCast, this );
	CREATE_CMD_PTR( RayCastVisible, this );
	CREATE_CMD_PTR( RayCastCoords, this );
	CREATE_CMD_PTR( RayCastCoordsVisible, this );
	CREATE_CMD_PTR( RectCastCoords, this );
	CREATE_CMD_PTR( RectCastCoordsVisible, this );
	CREATE_CMD_PTR( CameraCreateRay, this );
	CREATE_CMD_PTR( Destroy, this );
	CREATE_CMD_PTR( SetParent, this );
	CREATE_CMD_PTR( ResetParent, this );
	CREATE_CMD_PTR( SetCameraColliderRendering, this);
	CREATE_CMD_PTR( ToggleCameraColliderRendering, this);
}
///////////////////////////////////////////////////////////////////////////////
int
Phoenix::Scene::ParseCameraParameters( Tcl_Interp *pInterp, NameObjMap & params, CCameraObject & cam )
{

	if ( MAP_HAS( params, ".viewport") )
	{
		CVector4<int> vViewport;
		SCRIPT_GET_INT_VECP( params[".viewport"], 4, vViewport );
		cam.SetViewport( vViewport[0], vViewport[1], vViewport[2], vViewport[3] );
	}

	if ( MAP_HAS( params, ".fov"))
	{
		int fTmp;
		SCRIPT_GET_FLOATP( params[".fov"], fTmp );
		cam.SetFieldOfView( fTmp);
	}
	if ( MAP_HAS( params, ".orthogonal") )
	{
		CVector4<int> vTmp;
		SCRIPT_GET_INT_VECP( params[".orthogonal"], 4, vTmp);
		cam.SetViewOrtho( vTmp[0], vTmp[1], vTmp[2], vTmp[3]);
	}
	if ( MAP_HAS( params, ".nearClipping"))
	{
		float fTmp;
		SCRIPT_GET_FLOATP(params[".nearClipping"], fTmp );
		cam.SetNearClipping(fTmp);
	}
	if ( MAP_HAS( params, ".farClipping"))
	{
		float fTmp;
		SCRIPT_GET_FLOATP(params[".farClipping"], fTmp );
		cam.SetFarClipping(fTmp);
	}
	if ( MAP_HAS( params, ".position") )
	{
		CVector3<float> vTmp;
		SCRIPT_GET_FLOAT_VECP( params[".position"], 3, vTmp);
		cam.GetLocalTransform().SetTranslation(vTmp);
		g_Error << vTmp << " vs. " << cam.GetPosition() << endl;
	}
	if ( MAP_HAS( params, ".rotation"))
	{
		CVector4<float> vTmp;
		SCRIPT_GET_FLOAT_VECP( params[".rotation"], 4, vTmp);
		Phoenix::Math::CQuaternion q(vTmp[0], vTmp[1], vTmp[2], vTmp[3]);
		cam.SetRotation(q);
	}
	if ( MAP_HAS( params, ".script"))
	{
		cam.SetScript( Tcl_GetString( params[ ".script"] ) );
	}
	/*.orthogonal { left, right, top, bottom }
	    .fov 42.3
	    .viewport { 0, 0, 640, 480 }
	    .nearClipping 0.01
	    .farClipping 1000.0
	    .position { x y z }
	    .rotation { x y z w }
	    */
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateCamera )
{
	CHECK_ARGS(2, "resourceName listOfCameraParams");
	const char *szName = SCRIPT_GET_STR(1);
	NameObjMap camParams;
	CCameraObject *pCamera= new CCameraObject();
	pCamera->Init();

	if ( ParseKeyValueMap( pInterp, camParams, objv[2]) != TCL_OK ) return TCL_ERROR;
	if ( ParseCameraParameters( pInterp, camParams, *pCamera ) != TCL_OK ) return TCL_ERROR;
	if ( reinterpret_cast<CScene *>(clientData)->AddCamera( szName, pCamera ) == false )
	{
		ostringstream s;
		s << "Camera '" << szName << "' already exists.";
		SCRIPT_ERROR(s.str().c_str());
	}
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( MoveCamera )
{
	CHECK_ARGS(2, "cameraName amount");
	const char *szName = SCRIPT_GET_STR(1);
	float fTmp;
	SCRIPT_GET_FLOAT(2, fTmp);
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	//pProp->GetCamera()->Move( fTmp );
	if ( fTmp < 0.0 ) pProp->GetCamera()->MoveBackward();
	if ( fTmp > 0.0 ) pProp->GetCamera()->MoveForward();

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( StrafeCamera )
{
	CHECK_ARGS(2, "cameraName amount");
	const char *szName = SCRIPT_GET_STR(1);
	float fTmp;
	SCRIPT_GET_FLOAT(2, fTmp);
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	//pProp->GetCamera()->Strafe( fTmp );
	if ( fTmp < 0.0 ) pProp->GetCamera()->MoveLeft();
	if ( fTmp > 0.0 ) pProp->GetCamera()->MoveRight();
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ElevateCamera )
{
	CHECK_ARGS(2, "cameraName amount");
	const char *szName = SCRIPT_GET_STR(1);
	float fTmp;
	SCRIPT_GET_FLOAT(2, fTmp);
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	pProp->GetCamera()->Elevate( fTmp );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetCameraPosition )
{
	CHECK_ARGS(1, "cameraName");
	const char *szName = SCRIPT_GET_STR(1);
	CCameraProperty *pTmp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pTmp == NULL ) SCRIPT_ERROR("No such camera");
	SCRIPT_RESULT_FLOAT_VECP(3,pTmp->GetCamera()->GetPosition());
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetCameraPosition )
{
	CHECK_ARGS(2, "cameraName position");
	const char *szName = SCRIPT_GET_STR(1);
	CCameraProperty *pTmp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pTmp == NULL ) SCRIPT_ERROR("No such camera");
	CVector3<float> vTmp;
	SCRIPT_GET_FLOAT_VEC( 2, 3, vTmp );
	pTmp->GetCamera()->SetPosition(vTmp);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( StopCamera )
{
	CHECK_ARGS(1, "cameraName");
	const char *szName = SCRIPT_GET_STR(1);
	CCameraProperty *pTmp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pTmp == NULL ) SCRIPT_ERROR("No such camera");
	pTmp->GetCamera()->Stop();
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( TrackballRotate )
{
	CHECK_ARGS(3, "cameraName oldPos newPos");
	const char *szName = SCRIPT_GET_STR(1);
	CCameraProperty *pTmp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pTmp == NULL ) SCRIPT_ERROR("No such camera");
	CVector2<int> vOld, vNew;
	SCRIPT_GET_INT_VEC(2,2, vOld);
	SCRIPT_GET_INT_VEC(3,2, vNew);

	pTmp->GetCamera()->HandleMouseRotation(vOld, vNew);


	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ZoomCamera )
{
	CHECK_ARGS(2, "cameraName dir (positive for in, negative for out)");
	const char *szName = SCRIPT_GET_STR(1);
	int iTmp;
	SCRIPT_GET_INT(2, iTmp);
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	if ( iTmp > 0 ) 		 pProp->GetCamera()->ZoomIn();
	else if ( iTmp < 0 ) pProp->GetCamera()->ZoomOut();
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
/// On success, returns zero and sets comp to proper value.
/// On failure, returns 1 and comp is set to NOT_USED.
int
Script_ParseOperation( const char *szOperation, CTagged::TagCompare & comp )
{

	if      ( strcasecmp(szOperation, "not_used"  ) == 0 ) comp = CTagged::NOT_USED;
	else if ( strcasecmp(szOperation, "equal"     ) == 0 ) comp = CTagged::EQUAL;
	else if ( strcasecmp(szOperation, "not_equal" ) == 0 ) comp = CTagged::EQUAL;
	else if ( strcasecmp(szOperation, "and"       ) == 0 ) comp = CTagged::AND;
	else if ( strcasecmp(szOperation, "not_and"   ) == 0 ) comp = CTagged::NOT_AND;
	else {
		comp = CTagged::NOT_USED;
		return 1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CameraCreateRay )
{
	CHECK_ARGS(3, "cameraName x y");
	int iX, iY;
	const char *szName = SCRIPT_GET_STR(1);
	SCRIPT_GET_INT( 2, iX );
	SCRIPT_GET_INT( 3, iY );


	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
			ostringstream s;
			s << "No such camera '" << szName << "'";
			SCRIPT_ERROR(s.str().c_str());
	}
	CRay ray;
	pProp->GetCamera()->CreateRay(iX, iY, ray);
	Tcl_Obj * pList = Tcl_NewListObj(0, NULL );
	Tcl_Obj * pDir =  Tcl_NewListObj(0, NULL );
	Tcl_Obj * pOrigin =  Tcl_NewListObj(0, NULL);

	SCRIPT_LIST_FROM_FLOAT_VECP( pDir, 3, ray.GetDirection());
	SCRIPT_LIST_FROM_FLOAT_VECP( pOrigin, 3, ray.GetPosition());

	if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj(".dir", 4)) != TCL_OK ) return TCL_ERROR;
	if ( Tcl_ListObjAppendElement( pInterp, pList, pDir) != TCL_OK ) return TCL_ERROR;
	if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj(".origin", 7)) != TCL_OK ) return TCL_ERROR;
	if ( Tcl_ListObjAppendElement( pInterp, pList, pOrigin) != TCL_OK ) return TCL_ERROR;
	Tcl_SetObjResult( pInterp, pList );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RayCast )
{
	CHECK_ARGS(3, "ray tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iTag = 0;
	SCRIPT_GET_INT(2, iTag);
	const char *szOp = SCRIPT_GET_STR(3);
	CRay selectionRay;
	NameObjMap rayParam;
	if ( ParseKeyValueMap(pInterp, rayParam, objv[1]) != TCL_OK ) return TCL_ERROR;
	if ( ParseRay( pInterp, rayParam, selectionRay) != TCL_OK ) SCRIPT_ERROR("Invalid ray parameters.");

	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}

	GameObjectList lstObjects;
	reinterpret_cast<CScene *>(clientData)->GetSpatialGraph().CollectObjects(selectionRay, lstObjects, iTag, comp);
	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		const string & n = (*it)->GetName();
		if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
	}
	Tcl_SetObjResult( pInterp, pList);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RayCastVisible )
{
	CHECK_ARGS(3, "ray tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iTag = 0;
	SCRIPT_GET_INT(2, iTag);
	const char *szOp = SCRIPT_GET_STR(3);
	CRay selectionRay;
	NameObjMap rayParam;
	if ( ParseKeyValueMap(pInterp, rayParam, objv[1]) != TCL_OK ) return TCL_ERROR;
	if ( ParseRay( pInterp, rayParam, selectionRay) != TCL_OK ) SCRIPT_ERROR("Invalid ray parameters.");

	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}

	GameObjectList & lstObjects = pProp->GetGameObjectList();

	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		if ( (*it)->Intersects( selectionRay ) )
		{
			const string & n = (*it)->GetName();
			if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
		}
	}
	Tcl_SetObjResult( pInterp, pList);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RayCastCoords )
{
	CHECK_ARGS(5, "cameraName x y tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iTag = 0;
	int iX, iY;
	SCRIPT_GET_INT(2, iX);
	SCRIPT_GET_INT(3, iY);
	SCRIPT_GET_INT(4, iTag);
	const char *szOp = SCRIPT_GET_STR(5);



	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	CRay selectionRay;
	pProp->GetCamera()->CreateRay(iX, iY, selectionRay);

	GameObjectList lstObjects;
	reinterpret_cast<CScene *>(clientData)->GetSpatialGraph().CollectObjects(selectionRay, lstObjects, iTag, comp);

	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		const string & n = (*it)->GetName();
		if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
	}
	Tcl_SetObjResult( pInterp, pList);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RayCastCoordsVisible )
{
	CHECK_ARGS(5, "cameraName x y tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iX, iY, iTag;
	SCRIPT_GET_INT(2, iX);
	SCRIPT_GET_INT(3, iY);
	SCRIPT_GET_INT(4, iTag);
	const char *szOp = SCRIPT_GET_STR(5);
	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	CRay selectionRay;
	pProp->GetCamera()->CreateRay(iX, iY, selectionRay);


	GameObjectList & lstObjects = pProp->GetGameObjectList();

	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		// Collect only object that intersect ray
		if ( (*it)->Intersects(selectionRay,NULL ) == false ) continue;

		const string & n = (*it)->GetName();
		if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
	}
	Tcl_SetObjResult( pInterp, pList);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RectCastCoords)
{
	CHECK_ARGS( 7, "camera x1 y1 x2 y2 tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iX1, iY1, iX2, iY2, iTag;
	SCRIPT_GET_INT(2, iX1);
	SCRIPT_GET_INT(3, iY1);
	SCRIPT_GET_INT(4, iX2);
	SCRIPT_GET_INT(5, iY2);
	SCRIPT_GET_INT(6, iTag);
	const char *szOp = SCRIPT_GET_STR(7);
	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}

	CQuad quad;
	quad.CreateFrom2DCoords(iX1, iY1, iX2, iY2 );
	CFrustum f;
	pProp->GetCamera()->CreateSelectionFrustum( quad,f );
	GameObjectList lstObjects;
	reinterpret_cast<CScene *>(clientData)->GetSpatialGraph().CollectObjects(f, lstObjects, iTag, comp);

	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		const string & n = (*it)->GetName();
		if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
	}
	Tcl_SetObjResult( pInterp, pList);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RectCastCoordsVisible )
{
	CHECK_ARGS( 7, "camera x1 y1 x2 y2 tag operation");
	const char *szName = SCRIPT_GET_STR(1);
	int iX1, iY1, iX2, iY2, iTag;
	SCRIPT_GET_INT(2, iX1);
	SCRIPT_GET_INT(3, iY1);
	SCRIPT_GET_INT(4, iX2);
	SCRIPT_GET_INT(5, iY2);
	SCRIPT_GET_INT(6, iTag);
	const char *szOp = SCRIPT_GET_STR(7);
	CTagged::TagCompare comp;
	if ( Script_ParseOperation( szOp, comp ) )
	{
		ostringstream s;
		s << "Invalid operation '" << szOp << "' passed to raycast.";
		SCRIPT_ERROR(s.str().c_str());
	}
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(szName);

	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}

	CQuad quad;
	quad.CreateFrom2DCoords(iX1, iY1, iX2, iY2 );
	CFrustum f;
	pProp->GetCamera()->CreateSelectionFrustum( quad,f );
	GameObjectList & lstObjects = pProp->GetGameObjectList();

	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	GameObjectList::iterator it = lstObjects.begin();
	for( ; it != lstObjects.end(); it++)
	{
		const string & n = (*it)->GetName();
		if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewStringObj( n.c_str(), n.size() )) != TCL_OK ) return TCL_ERROR;
	}
	Tcl_SetObjResult( pInterp, pList);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( Destroy )
{
  CHECK_ARGS( 1, "objName");
	int iLen = 0;
	const char *szName = SCRIPT_GET_STR(1);
	CGameObject *pObj = g_ObjectMgr->GetResource( szName );

	if ( pObj )
	{
		reinterpret_cast<CScene *>(clientData)->RemoveGameObject( pObj );
	}
	else
	{
		ostringstream s;
		s << "No such object '" << szName << "'";
		SCRIPT_ERROR( s.str().c_str());
	}
	Tcl_ResetResult( pInterp );

	return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetParent )
{
	CHECK_ARGS( 2, "object newParent");
	const char *szObject = SCRIPT_GET_STR(1);
	const char *szParent = SCRIPT_GET_STR(2);
	CGameObject *pObj = NULL;
	if ( (pObj =g_ObjectMgr->GetResource( szObject )) == NULL )
	{
		ostringstream s;
		s << "No such object '" << szObject << "'";
		SCRIPT_ERROR( s.str().c_str());
	}
	CGameObject *pParent = NULL;
	if ( (pParent = g_ObjectMgr->GetResource( szParent )) == NULL )
	{
		ostringstream s;
		s << "No such object '" << szParent << "'";
		SCRIPT_ERROR( s.str().c_str());
	}

	pObj->Reparent( pParent );

	return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ResetParent )
{
	CHECK_ARGS(1, "objectName");
	const char *szObj = SCRIPT_GET_STR(1);
	CGameObject *pObj = g_ObjectMgr->GetResource(szObj);
	if ( pObj == NULL )
	{
			ostringstream s;
			s << "No such object '" << szObj<< "'";
			SCRIPT_ERROR( s.str().c_str());
	}

	pObj->Reparent( reinterpret_cast<CScene *>(clientData)->GetTransformGraph().GetRoot()->GetTransformable());

	return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetCameraColliderRendering )
{
	CHECK_ARGS(2, "cameraName boolean");
	const char *szCamera = SCRIPT_GET_STR(1);
	int bEnabled = 0;
	SCRIPT_GET_INT( 2, bEnabled );
	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(string(szCamera));
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szCamera << "'";
		SCRIPT_ERROR( s.str().c_str() );
	}
	pProp->SetColliderRendering( bEnabled );
	return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ToggleCameraColliderRendering )
{
	CHECK_ARGS(1, "cameraName");
	const char *szCamera = SCRIPT_GET_STR(1);

	CCameraProperty *pProp = reinterpret_cast<CScene *>(clientData)->GetCameraProperty(string(szCamera));
	if ( pProp == NULL )
	{
		ostringstream s;
		s << "No such camera '" << szCamera << "'";
		SCRIPT_ERROR( s.str().c_str() );
	}
	pProp->SetColliderRendering( ! pProp->IsColliderRendering() );
	return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
#endif
