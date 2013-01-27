/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseScene.h"
#include "MooseExceptions.h"
#include "MooseSpatialGraph.h"
#include "MooseTransformGraph.h"
#include "MooseDefaultEntities.h"
#include "MooseDirectionalLightObject.h"
#include "MooseLogger.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include <MooseScreen.h>
#endif
#include "MooseCameraObject.h"
#include <MooseVector2.h>
#include "MooseCollisionEvent.h"
#include <Rocket/Core.h>
#include <sstream>
#include <iostream>
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Scene;
using namespace Moose::Core;
using namespace Moose::Exceptions;
#if !defined(MOOSE_APPLE_IPHONE)
using namespace Moose::Window;
#endif
using namespace Moose::Math;
using namespace Moose::AI;
using namespace Moose::Volume;
using namespace std;
#define COLLIDER_SPHERE_FACTOR 1.618
///////////////////////////////////////////////////////////////////////////////
/// Very simple sorter; sorts only by transparency.
class CGameObjectSorter
{
public:
  bool operator()( CRenderable * pFirst, CRenderable *pSecond ) 
  {
    return (pFirst->GetRenderState().GetRenderLayer() < pSecond->GetRenderState().GetRenderLayer());
  }
  inline void operator()( std::list< CRenderable * > & list )
  {
    list.sort(*this);
    /*std::list< CRenderable * >::iterator it = list.begin();
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
      listLights.clear();*/
  }
};
///////////////////////////////////////////////////////////////////////////////
CGameObjectSorter game_object_sorter;
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CScene::CScene( unsigned int nNumLevels, float fWorldSize ) : m_GUI(this)
{
  m_pSpatialGraph   = new CSpatialGraph(nNumLevels, fWorldSize);
  m_pTransformGraph = new CTransformGraph();
  m_bPrepared = false;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CScene::CScene( const char *szName, unsigned int nNumLevels, float fWorldSize ) : m_GUI(this)
{
  m_pSpatialGraph   = new CSpatialGraph(nNumLevels, fWorldSize);
  m_pTransformGraph = new CTransformGraph();
  SetName( szName );
  m_bPrepared = false;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CScene::~CScene()
{
  DeleteGameObjects();
  delete m_pSpatialGraph;
  delete m_pTransformGraph;
  while(m_StaticQueue.GetObjectList().empty() == false)
  {
    delete m_StaticQueue.GetObjectList().front();
    m_StaticQueue.GetObjectList().pop_front();
  };
  while(m_GUI.m_Queue.GetObjectList().empty() == false)
  {
    delete m_GUI.m_Queue.GetObjectList().front();
    m_GUI.m_Queue.GetObjectList().pop_front();
  };
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Init()
{
#if !defined(MOOSE_APPLE_IPHONE)
  LoadScript();
#endif
  // ensure that each scene is managed and have a unique name
  std::string tmpName = GetName();
  if ( tmpName.empty() )
  {
    tmpName = CreateUniqueNameStr("Scene_");
    SetName(tmpName);
  }
  // For messaging
  g_ObjectMgr->Create( this, tmpName, this->GetObjectHandle() );

  PrepareGUI();

}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CGameObject *
Moose::Scene::CScene::AddGameObject( Moose::Scene::CGameObject *pObj )
{
  // Create handle to object
  int iRetval = g_ObjectMgr->Create(pObj, pObj->GetName(), pObj->GetObjectHandle());
    
  if( iRetval != 0 ) 
  {
    throw Moose::Exceptions::CAlreadyExistsException(pObj->GetName().c_str());
  }
    
  // Intialize after object has been registered, so it can be accessed during script init via
  // objectmgr.
  pObj->Init();
  // so it is affected by transforms.
  /*CObjectTransform *pObjTr = */GetTransformGraph().Insert(pObj);
    
  // So it can be culled
  GetSpatialGraph().InsertObject(pObj);
    
  // insert local to local ptr list
  m_lstGameObjects.push_back( pObj );
  m_mapPotentialColliders[pObj] = new GameObjectList();
  m_mapColliders[pObj]          = new GameObjectList();
  return pObj;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject *
Moose::Scene::CScene::AddDirectionalLight( Moose::Scene::CDirectionalLightObject *pObj )
{
  // Set bounding sphere to be as large as world itself.
  pObj->GetBoundingSphere().SetRadius( GetSpatialGraph().GetWorldSize());

  pObj->Init();

  // Create handle to object
  g_ObjectMgr->Create(pObj, pObj->GetName(), pObj->GetObjectHandle());

  // Transforms won't be needed, since this object is not regular gameobject.

  // So it can be culled
  GetSpatialGraph().InsertObject(pObj);

  // object list
  m_lstGameObjects.push_back( pObj );
  m_mapPotentialColliders[pObj] = new GameObjectList();
  m_mapColliders[pObj]          = new GameObjectList();
  return pObj;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::RemoveFromCaches( Moose::Scene::CGameObject *pObject )
{
  CameraMap::iterator it = m_mapCameras.begin();
  for( ; it != m_mapCameras.end(); it++)
  {
    it->second->GetGameObjectList().remove( pObject ); // if regular object
    it->second->GetActiveLights().remove( pObject ); // if it was a light.
        
    if ( m_mapPotentialColliders.find(pObject) != m_mapPotentialColliders.end())
    {
      delete m_mapPotentialColliders[pObject]; // delete list 
      m_mapPotentialColliders.erase(pObject);  // remove from map
    }
    if  ( m_mapColliders.find(pObject) != m_mapColliders.end())
    {
      delete m_mapColliders[pObject]; // delete list
      m_mapColliders.erase(pObject); // remove from map
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::GameObjectList & 
Moose::Scene::CScene::GetGameObjects()
{
  return m_lstGameObjects;
}
///////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CScene::DeleteGameObjects()
{
  while( !m_lstGameObjects.empty() )
  {
    RemoveGameObject(m_lstGameObjects.front());
  }
  // Cameras are a weird bunch, as they aren't merely common objects.
  // as they should if collision is to be considered...
  while( !m_mapCameras.empty() ) 
  {
    RemoveCamera(m_mapCameras.begin()->first);
  }
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::RemoveGameObject( Moose::Scene::CGameObject *pObj )
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
      CGameObject *pChild = dynamic_cast<Moose::Scene::CGameObject *>(pTmp);

      if ( pChild == NULL )  // this is plain transformable.
      {
        delete pTmp;
        continue;
      }
      // In case of camera, we need to remove cameraproperties.
      if ( pChild->GetTag() & CAMERA_TAG )
      {
        CameraMap::iterator it = m_mapCameras.find(pChild->GetName());
        if ( it != m_mapCameras.end() )
        {
          // delete cameraproperty, leave gameobject itself for latter destruction.
          delete it->second;
          m_mapCameras.erase(it);                        
        }
      }
            
      // remove from spatial
      GetSpatialGraph().DeleteObject( pChild );
      m_lstGameObjects.remove(pChild);

      RemoveFromCaches(pChild);
      // remove from transform graph and delete object.
      delete pTmp->GetTransformNode();
      g_ObjectMgr->Destroy( pChild->GetObjectHandle() );
    }
  }
  else 
  {
    if ( pObj->GetTag() & CAMERA_TAG )
    {
      CameraMap::iterator it = m_mapCameras.find(pObj->GetName());
      if ( it != m_mapCameras.end() )
      {
        // delete cameraproperty, leave gameobject itself for latter destruction.
        delete it->second;
        m_mapCameras.erase(it);
                
      }
    }
    if ( pObj->GetObjectHandle().IsNull() == false)
      g_Objects.Destroy(pObj->GetObjectHandle());
    else
      delete pObj;
  }
  //assert( pObj->GetObjectHandle().IsNull() && "Object not deleted.");
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CSpatialGraph &
Moose::Scene::CScene::GetSpatialGraph()
{
  return *m_pSpatialGraph;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CTransformGraph &
Moose::Scene::CScene::GetTransformGraph()
{
  return *m_pTransformGraph;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Update( float fSeconds )
{
 
  // Run local script / messaging
  if ( IsEnabled()) UpdateScript(fSeconds);
    
  // update transform graph
  GetTransformGraph().Update();
  // Update objects themselves.
  GameObjectList::iterator it = m_lstGameObjects.begin();
  if ( IsEnabled()){
    for ( ; it != m_lstGameObjects.end(); it++ )
    {
      (*it)->Update( fSeconds );
    }
  }
  // update object positions in spatial graph.
  it = m_lstGameObjects.begin();
  for ( ; it != m_lstGameObjects.end(); it++ )
  {
    GetSpatialGraph().Update( *it );
  }
  
  CameraMap::iterator camIt = m_mapCameras.begin();
  for( ; camIt != m_mapCameras.end(); camIt++ )
  {
    bool bChanged = false;
    CCameraObject & c = *camIt->second->GetCamera();
    if ( IsEnabled()) c.Update( fSeconds );
    if ( c.IsProjectionChanged() )  {   bChanged = true; c.UpdateProjection();    }
    if ( c.IsViewChanged() )     {      bChanged = true; c.UpdateView();          }
    if ( bChanged )
    {
      c.CalculateFrustum();
      c.CalculateBoundingSphere();
      c.CalculateBoundingCone();
    }
    CollectVisibleGameObjects( *camIt->second );
    AssignLightsToObjects( camIt->second->GetActiveLights(), 
                           camIt->second->GetGameObjectList());

  }
  m_GUI.Update();
}
///////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CScene::IsPrepared() const
{
  return m_bPrepared;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::SetPrepared(bool bFlag )
{
  m_bPrepared = bFlag;
}
///////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CScene::PrepareGUI( )
{

  // Initialize GUI
  m_GUI.Init(GetName(), 
             CScreen::m_iWidth,
             CScreen::m_iHeight);
  m_GUI.m_pGUIRenderable = new CRocketRenderable(m_GUI.m_pContext);
  if ( m_GUI.m_pGUIRenderable->GetRenderState().Prepare() == false)
  {
    throw CMooseRuntimeError("Gui renderable flawed");
  }
  m_GUI.m_Queue.GetObjectList().push_back(m_GUI.m_pGUIRenderable);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Render( COglRenderer & renderer )
{


  CameraMap::iterator camIt = m_mapCameras.begin();
  // Pass global settings to renderer.
  renderer.GetRenderState().SetGlobalAmbient( &GetRenderSettings().GetGlobalAmbient());
  for( ; camIt != m_mapCameras.end(); camIt++ )
  {
    // CollectVisibleGameObjects( camIt->second );
      
    CCameraProperty & prop = *camIt->second;
    CCameraObject & c = *prop.GetCamera();
    // skip disabled cameras
    if ( c.IsEnabled() == false ) continue;

    prop.GetRenderQueue().Clear();
    CollectStaticRenderables( prop.GetRenderQueue());
    CollectRenderables( c, prop.GetGameObjectList(), prop.GetRenderQueue() );
    AssignLightsToRenderables( prop.GetActiveLights(), prop.GetRenderQueue() );
		
    prop.GetRenderQueue().Sort( game_object_sorter );

    renderer.CommitCamera( c );

    m_StaticQueue.Render( renderer );          // before scene objects
    prop.GetRenderQueue().Render( renderer );     // scene objects
    //prop.GetTransparentQueue().Render( renderer );     // transp. scene objects
	
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
  //m_OverlayQueue.Render( renderer );   // overlay objects
  renderer.CommitShader(NULL);
  renderer.DisableCaches();
  renderer.CommitCamera( GetGUICamera());
  renderer.CommitState( Moose::Graphics::STATE_DEPTH_WRITE );
  renderer.ClearBuffer( DEPTH_BUFFER );
  m_GUI.m_Queue.Render( renderer );   // GUI objects


  renderer.Finalize();
  // OpenGL context may be created also via other method.
#if !defined(MOOSE_APPLE_IPHONE)
  if (CScreen::Exists()){ CScreen::GetInstance()->SwapBuffers();}
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
Moose::Scene::CScene::CollectVisibleGameObjects( CCameraProperty & camProp )
{
  Moose::Scene::CCameraObject & camera = *camProp.GetCamera();
  GameObjectList & list = camProp.GetGameObjectList();
  list.clear();
  GameObjectList & lstLights = camProp.GetActiveLights();
  lstLights.clear();
	
  GetSpatialGraph().CollectObjects( camera, list, 	  LIGHT_TAG | CAMERA_TAG,  NOT_AND );
  GetSpatialGraph().CollectObjects( camera, lstLights,  LIGHT_TAG, AND );
}
///////////////////////////////////////////////////////////////////////////////
CCameraProperty *
Moose::Scene::CScene::GetCameraProperty( const std::string & name )
{
  CameraMap::iterator it = m_mapCameras.find(name);
  if ( it != m_mapCameras.end() )
  {
    return it->second;
  }
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CCamera & 
Moose::Scene::CScene::GetGUICamera()
{
  return m_GUI.m_Camera;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CGUI & 
Moose::Scene::CScene::GetGUI()
{
  return m_GUI;
}

///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::AssignLightsToRenderables( GameObjectList & lights, CRenderQueue<CRenderable *> & queue)
{
  std::list<CRenderable * >::iterator rIt = queue.GetObjectList().begin();
  for( ; rIt != queue.GetObjectList().end(); rIt++)
  {
    LightList & lightList = (*rIt)->GetRenderState().GetLights();
    lightList.clear();
    GameObjectList::iterator it = lights.begin();
    for( ; it != lights.end(); it++ )
    {
      CGameObject * obj = *it;
      if ( obj->IsEnabled() )
      {
        lightList.push_back( dynamic_cast<Moose::Graphics::CLight *>(obj));
      }
    }
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::AssignLightsToObjects ( GameObjectList & lights, GameObjectList & objects )
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
        (*objIt)->GetLights().push_back( dynamic_cast<Moose::Graphics::CLight *>(*lightIt) );
      }
    }
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::CollectRenderables( CCamera & camera, GameObjectList & gameObjects, CRenderQueue<CRenderable *> &queue )
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
void
Moose::Scene::CScene::CollectStaticRenderables( CRenderQueue<CRenderable *> &queue )
{
  std::copy( m_StaticQueue.GetObjectList().begin(), 
             m_StaticQueue.GetObjectList().end(), 
             std::back_inserter(queue.GetObjectList()));
}
///////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CScene::AddCamera( const std::string & name, Moose::Scene::CCameraObject *pCamera )
{
  if ( m_mapCameras.find( name ) == m_mapCameras.end() )
  {
    CCameraProperty *pProp = new CCameraProperty();
    pProp->SetCamera( pCamera );
    m_mapCameras[name] = pProp;
    pCamera->SetName( name );
    if ( g_ObjectMgr->Create(pCamera, pCamera->GetName(), pCamera->GetObjectHandle()) == 0 )
    {
      // so it is affected by transforms.
      GetTransformGraph().Insert(pCamera);
      return true;
    }
    else
    {
      delete m_mapCameras[name];
      m_mapCameras.erase( name);
      return false;
    }
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::RemoveCamera( const std::string & name )
{
  if ( m_mapCameras.find( name ) != m_mapCameras.end() )
  {
    RemoveGameObject( g_Objects(name.c_str()) );
  }

}
///////////////////////////////////////////////////////////////////////////////
RenderQueue &
Moose::Scene::CScene::GetStaticRenderQueue()
{
  return m_StaticQueue;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::PushToRenderQueue( Moose::Graphics::CRenderable *pRenderable )
{
  m_StaticQueue.GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::RemoveFromRenderQueue( Moose::Graphics::CRenderable *pRenderable )
{
  m_StaticQueue.GetObjectList().remove(pRenderable);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::PushToGUIQueue( Moose::Graphics::CRenderable *pRenderable )
{
  m_GUI.m_Queue.GetObjectList().push_back(pRenderable);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::RemoveFromGUIQueue( Moose::Graphics::CRenderable *pRenderable )
{
  m_GUI.m_Queue.GetObjectList().remove(pRenderable);
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CRenderSettings & 
Moose::Scene::CScene::GetRenderSettings()
{
  return m_RenderSettings;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::UpdateColliders()
{
    
  GameObjectList::iterator it = m_lstGameObjects.begin();
  for( ; it != m_lstGameObjects.end(); it++)
  {
        
        
    GameObjectList & potentialColliders = *m_mapPotentialColliders[*it];
    GameObjectList & currentColliders   = *m_mapColliders[*it];
    potentialColliders.clear();
    CSphere tmp = (*it)->GetWorldBoundingSphere();
    tmp.SetRadius( tmp.GetRadius()*COLLIDER_SPHERE_FACTOR);
    GetSpatialGraph().CollectObjects( tmp, potentialColliders, COLLIDER_TAG, AND );
    
    GameObjectList tmpList;
    // Sort existing lists for difference
    potentialColliders.sort();
    currentColliders.sort();
    
    // remove already colliding objects from potential set
    set_difference( potentialColliders.begin(), potentialColliders.end(),
                    currentColliders.begin(),   currentColliders.end(), 
                    back_inserter(tmpList));
    
    // update potential colliders to removed values.
    potentialColliders.swap(tmpList);
    tmpList.clear();
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::CheckCollisions( Moose::Scene::CGameObject *pObj )
{
  if ( pObj->IsEnabled() == false ) return;
  GameObjectList & lstPotentialColliders = *m_mapPotentialColliders[pObj];
  GameObjectList & currentColliders      = *m_mapColliders[pObj];
  GameObjectList::iterator it = currentColliders.begin();
  GameObjectList nonColliders;
    
  while ( it != currentColliders.end())
  {
    if ( (*it)->IsEnabled() == false || *it == pObj ) 
    {
      it++;
      continue; // skip self or disabled collider
    }
        
    if ( pObj->GetCollider()->Intersects( *(*it)->GetCollider()) == false)
    {
#if defined(MOOSE_APPLE_IPHONE)
      pObj->EnqueueMessage( new CCollisionExit(*it));      
#else
      pObj->EnqueueMessage( "OnCollisionExit");      
#endif
      // Insert this into 
      nonColliders.push_back( *it );
      it = currentColliders.erase(it);
    }
    else it++;
  }       
    
  it = lstPotentialColliders.begin();
    
  while ( it != lstPotentialColliders.end())
  {
    // ignore collision on itself or disabled collider
    if ( (*it)->IsEnabled() == false || *it == pObj) 
    {
      it++;
      continue;
    }
    // enqueue messages if intersection occurs
    if ( pObj->GetCollider()->Intersects( *(*it)->GetCollider() ) )
    {

      // Check new collisions and register them to colliders.
      if ( find( currentColliders.begin(), currentColliders.end(), *it) == currentColliders.end() )
      {
#if defined(MOOSE_APPLE_IPHONE)              
        pObj->EnqueueMessage( new CCollisionEnter(*it));
#else 
        pObj->EnqueueMessage("OnCollisionEnter");
#endif 
        currentColliders.push_back( *it );
        it = lstPotentialColliders.erase(it);
      } 
      else 
      {
#if defined(MOOSE_APPLE_IPHONE)              
        pObj->EnqueueMessage( new CCollisionStay(*it));
#else 
        pObj->EnqueueMessage("OnCollisionStay");
#endif
        it++;
      }
    }
    else it++;
  }
  // Copy non-colliders to potential colliders  
  lstPotentialColliders.insert(lstPotentialColliders.end(), 
                               nonColliders.begin(), nonColliders.end());
  nonColliders.clear();
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::CheckCollisions()
{
  GameObjectList::iterator it = m_lstGameObjects.begin();    
  for (  ; it != m_lstGameObjects.end(); it++)
  {
    CheckCollisions(*it);
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::OnEnter()
{
  EnqueueMessage("OnEnter");
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::OnExit()
{
  EnqueueMessage("OnExit");
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Load()
{

}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Unload()
{

}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CScene::Reload()
{
  Unload();
  Load();
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CGUI::CGUI( Moose::Scene::CScene *pScene ) : m_pContext(NULL), m_pDocument(NULL), 
                                                           m_pGUIRenderable(NULL), m_pScene(pScene)
{
  
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CGUI::~CGUI()
{
  
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::ProcessEvent(Rocket::Core::Event& event)
{
  ostringstream s;
  s << "OnGUI " << event.GetType().CString() << " " << event.GetTargetElement()->GetId().CString();
  m_pScene->EnqueueMessage( s.str(), true );
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::AddListener( const std::string & element, const std::string &event  )
{
  if ( m_pDocument )
  {
    Rocket::Core::Element * pE = m_pDocument->GetElementById( Rocket::Core::String(element.c_str()) );
    if ( pE ) 
    {
      pE->AddEventListener( Rocket::Core::String(event.c_str()), this);
    }
    else
    {
      CMooseRuntimeError err("AddListener - No such element '");
      err << element << "'";
      throw err;
    }
  } else throw CMooseRuntimeError("There is no document!");
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGUI::RemoveListener( const std::string & element, const std::string & event )
{
  if ( m_pDocument )
  {
    Rocket::Core::Element * pE = m_pDocument->GetElementById( Rocket::Core::String(element.c_str()) );
    if ( pE ) 
    {
      pE->RemoveEventListener( Rocket::Core::String(event.c_str()), this);
    }
    else
    {
      CMooseRuntimeError err("Removelistener - No such element '");
      err << element << "'";
      throw err;
    }
  } else throw CMooseRuntimeError("There is no document!");
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CCamera & 
Moose::Scene::CGUI::GetCamera()
{
  return m_Camera;
}
////////////////////////////////////////////////////////////////////////////////
RenderQueue &
Moose::Scene::CGUI::GetRenderQueue()
{
  return m_Queue;
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGUI::Init( const std::string & name, int width, int height )
{
  m_pContext = Rocket::Core::CreateContext(Rocket::Core::String(name.c_str()), 
                                           Rocket::Core::Vector2i( width,height));
  if ( m_pContext == NULL )
  {
    throw CMooseRuntimeError("Could not create rocket context!");
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGUI::Load( const std::string & file )
{
  if ( m_pContext == NULL ) throw CMooseRuntimeError("Cannot load GUI with NULL context");
  m_pDocument = m_pContext->LoadDocument( file.c_str() );
  if ( m_pDocument == NULL )
  {
    CMooseRuntimeError err("Could not load GUI document");
    err << " '" << file << "'";
    throw err;
  }
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::Show()
{
  if ( m_pDocument )  m_pDocument->Show();
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::Hide()
{
  if ( m_pDocument ) m_pDocument->Hide();
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::Render()
{
  if ( m_pContext )  m_pContext->Render();
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGUI::Update()
{
  if ( m_pContext )  m_pContext->Update();
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGUI::SetText( const std::string & element, 
                                const std::string & text )
{
  if ( m_pDocument )
  {
    Rocket::Core::Element * pE = m_pDocument->GetElementById( Rocket::Core::String(element.c_str()) );
    if ( !pE ) 
    {
      CMooseRuntimeError err("No such element ");
      err << "'" << element << "' in GUI";
      throw err;
    }
    
    pE->SetInnerRML(text.c_str());
    
  }
}
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
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
Moose::Scene::CScene::RegisterUserCommands()
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
Moose::Scene::ParseCameraParameters( Tcl_Interp *pInterp, NameObjMap & params, CCameraObject & cam )
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
    Moose::Math::CQuaternion q(vTmp[0], vTmp[1], vTmp[2], vTmp[3]);
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
Script_ParseOperation( const char *szOperation, TagCompare & comp )
{

  if      ( strcasecmp(szOperation, "not_used"  ) == 0 ) comp = NOT_USED;
  else if ( strcasecmp(szOperation, "equal"     ) == 0 ) comp = EQUAL;
  else if ( strcasecmp(szOperation, "not_equal" ) == 0 ) comp = EQUAL;
  else if ( strcasecmp(szOperation, "and"       ) == 0 ) comp = AND;
  else if ( strcasecmp(szOperation, "not_and"   ) == 0 ) comp = NOT_AND;
  else {
    comp = NOT_USED;
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
  //const char *szName = SCRIPT_GET_STR(1);
  int iTag = 0;
  SCRIPT_GET_INT(2, iTag);
  const char *szOp = SCRIPT_GET_STR(3);
  CRay selectionRay;
  NameObjMap rayParam;
  if ( ParseKeyValueMap(pInterp, rayParam, objv[1]) != TCL_OK ) return TCL_ERROR;
  if ( ParseRay( pInterp, rayParam, selectionRay) != TCL_OK ) SCRIPT_ERROR("Invalid ray parameters.");

  TagCompare comp;
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

  TagCompare comp;
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



  TagCompare comp;
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
  TagCompare comp;
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
  TagCompare comp;
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
  TagCompare comp;
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
