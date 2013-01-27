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

#include "MooseApplication.h"
#include "MooseLogger.h"
#include "MooseRocketRenderInterface.h"
#include "MooseExceptions.h"
#include <Rocket/Core.h>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Scene;
using namespace Moose::Core;
using namespace Moose::Exceptions;
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseSDLScreen.h"
using namespace Moose::Window;
#endif
using std::ostringstream;
using namespace std;
namespace prefix = Moose::Scene;
#define DEFAULT_UPDATE_INTERVAL_MS          5
#define DEFAULT_COLLIDER_UPDATE_INTERVAL_MS 100
#define DEFAULT_COLLISION_CHECK_INTERVAL_MS 25
///////////////////////////////////////////////////////////////////////////////
prefix::CApplication::CApplication() : m_pCurrentScene(NULL), m_pNextScene(NULL)
{
  SetEnabled(true);
  m_bSceneHasKeyUp = false;
  m_bSceneHasKeyDown = false;
  m_bSceneHasMouseDown = false;
  m_bSceneHasMouseUp = false;
  m_bSceneHasMouseMotion = false;
  m_bHasQuit = false;
  m_pMainLoopThread = NULL;
  m_nUpdateIntervalMs         = DEFAULT_UPDATE_INTERVAL_MS;
  m_nColliderUpdateIntervalMs = DEFAULT_COLLIDER_UPDATE_INTERVAL_MS;
  m_nCollisionCheckIntervalMs = DEFAULT_COLLISION_CHECK_INTERVAL_MS;
  m_fColliderUpdateTime = 0;
  m_fCollisionCheckTime = 0;
  GetTimer().Reset();
  m_pRenderInterface = new CRocketRenderInterface();
  // Initialize rocket library
  Rocket::Core::SetRenderInterface(m_pRenderInterface);
  Rocket::Core::SetSystemInterface(this);
  //Rocket::Core::SetFileInterface();
  Rocket::Core::Initialise();  
  Rocket::Core::String font_names[4];
  font_names[0] = "Delicious-Roman.otf";
  font_names[1] = "Delicious-Italic.otf";
  font_names[2] = "Delicious-Bold.otf";
  font_names[3] = "Delicious-BoldItalic.otf";
  // Right, this needs to be fixed.
  for (size_t i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
  {
    Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("/home/entity/downloads/libRocket/Samples/assets/") + font_names[i]);
  }
}
///////////////////////////////////////////////////////////////////////////////
prefix::CApplication::~CApplication()
{
  m_pCurrentScene = NULL;
  while( m_mapScenes.empty() == false)
  {
    delete m_mapScenes.begin()->second;
    m_mapScenes.erase( m_mapScenes.begin());
  }
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Init()
{
  if ( GetName().empty() )
  {
    g_Error << "Application must have an unique non-empty string as a name!" << endl;
    abort();
  }
  g_ObjectMgr->Create(this, GetName());
  CGameObject::Init();
#if !defined(MOOSE_APPLE_IPHONE)
  m_bHasQuit 									= HasCommand("OnQuit");
#endif
  LoadDefaultResources();
}
///////////////////////////////////////////////////////////////////////////////
prefix::CScene *
prefix::CApplication::GetCurrentScene()
{
  return m_pCurrentScene;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::SetCurrentScene( Moose::Scene::CScene * pScene )
{
  if ( m_pCurrentScene ) m_pCurrentScene->OnExit();
    
  if ( pScene ) 
  {
    m_pCurrentScene = pScene;
    m_pRenderInterface->SetRenderable(m_pCurrentScene->GetGUI().m_pGUIRenderable);
    m_pCurrentScene->OnEnter();
  }
  else 
  {
    m_pCurrentScene = NULL;
  }
  m_pNextScene = NULL;
  CheckSceneInputs();
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::SetCurrentScene( const std::string & name )
{
  SceneMap::iterator it = m_mapScenes.find( name );
  if ( it != m_mapScenes.end() )  m_pNextScene = it->second;
  else                            m_pNextScene = NULL;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::CheckSceneInputs()
{
  if ( m_pCurrentScene == NULL ) return;
#if !defined(MOOSE_APPLE_IPHONE)
  m_bSceneHasKeyUp 						= m_pCurrentScene->HasCommand("OnKeyUp");
  m_bSceneHasKeyDown 					= m_pCurrentScene->HasCommand("OnKeyDown");
  m_bSceneHasMouseDown 				= m_pCurrentScene->HasCommand("OnMouseDown");
  m_bSceneHasMouseUp 					= m_pCurrentScene->HasCommand("OnMouseUp");
  m_bSceneHasMouseMotion 			= m_pCurrentScene->HasCommand("OnMouseMotion");
#endif

}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::AddScene( const std::string & name, CScene *pScene )
{
  // just in case
  pScene->SetName( name );
  if ( GetScene( name ) == NULL ) m_mapScenes[name] = pScene;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::RemoveScene( const std::string & name )
{
  SceneMap::iterator it = m_mapScenes.find( name );
  if ( it != m_mapScenes.end() )
  {
    if ( it->second == GetCurrentScene() ) m_pCurrentScene = NULL;
    delete it->second;
    m_mapScenes.erase( it );
  }

}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CScene *
prefix::CApplication::GetScene( const std::string & name )
{
  SceneMap::iterator it = m_mapScenes.find( name );
  if ( it == m_mapScenes.end() ) return NULL;
  else return it->second;

}
///////////////////////////////////////////////////////////////////////////////
#if defined(MOOSE_APPLE_IPHONE)
Moose::Scene::CEventQueue & 
prefix::CApplication::GetEventQueue()
{
  return m_EventQueue;
}
#endif
///////////////////////////////////////////////////////////////////////////////

void
prefix::CApplication::ProcessInput()
{
  //g_Error << "Pre-check\n";
  if ( GetCurrentScene() == NULL ) 
  {
#if defined(MOOSE_APPLE_IPHONE)
    m_EventQueue.Clear();
#endif
    return;
  }
  CScene & scene = *GetCurrentScene();
  //g_Error << "Processing input\n";
#if defined(MOOSE_APPLE_IPHONE)
  while ( !m_EventQueue.Empty() )
  {
    Event e = m_EventQueue.Front();
    m_EventQueue.Pop();
    switch( e.type )
    {
    case kTouchBegan:
      {
        Touches lstTouches;
        assert(e.touches < 5 && "touch count is too large!");
        for(int i=0;i<e.touches;i++)
        {
          lstTouches.push_back( Touch(e.x[i],e.y[i]) );
        }
        scene.OnTouchBegan( lstTouches );
      }
      break;
    case kTouchMoved:
      {
        Touches lstTouches;
        assert(e.touches < 5 && "touch count is too large!");
        for(int i=0;i<e.touches;i++)
        {
          lstTouches.push_back( Touch(e.x[i],e.y[i]) );
        }
        scene.OnTouchMoved( lstTouches);
      }
      break;
    case kTouchEnded:
      {
        Touches lstTouches;
        assert(e.touches < 5 && "touch count is too large!");
        for(int i=0;i<e.touches;i++)
        {
          lstTouches.push_back( Touch(e.x[i],e.y[i]) );
        }
        scene.OnTouchEnded( lstTouches);
      }
      break;
    case kAccelerate:
      //g_Error << "Moose:Received accel event \n";
      scene.OnAccelerate( e.x[0], e.y[0], e.z, e.flags);
      break;
    default:
      assert( NULL && "Event type is not defined!!" );
      break;
    }
  }
#else
	
  SDL_Event event;
  while ( SDL_PollEvent(&event ))
  {
    switch(event.type)
    {
    case SDL_KEYUP:
      {
        ostringstream s;
        s << "OnKeyUp " << SDL_GetKeyName(event.key.keysym.sym);

        scene.EnqueueMessage(s.str().c_str(), m_bSceneHasKeyUp );
      }
      break;
    case SDL_KEYDOWN:
      {
        ostringstream s;
        s << "OnKeyDown "
          << SDL_GetKeyName(event.key.keysym.sym);
        scene.EnqueueMessage(s.str().c_str(), m_bSceneHasKeyDown );
      }
      break;
    case SDL_MOUSEMOTION:
      {
        scene.GetGUI().m_pContext->ProcessMouseMove( event.motion.x, 
                                                     event.motion.y, 0 );
        ostringstream s;
        s << "OnMouseMotion "
          << event.motion.x << " "
          << event.motion.y;
        scene.EnqueueMessage( s.str().c_str(), m_bSceneHasMouseMotion );
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      {
        scene.GetGUI().m_pContext->ProcessMouseButtonDown( event.button.button-1, 0 );
        ostringstream s;
        s << "OnMouseDown "
          << (unsigned int)event.button.button << " "
          << event.button.x 		 << " "
          << event.button.y;
        scene.EnqueueMessage( s.str().c_str(), m_bSceneHasMouseDown );
      }
      break;
    case SDL_MOUSEBUTTONUP:
      {
        scene.GetGUI().m_pContext->ProcessMouseButtonUp( event.button.button-1, 0 );
        ostringstream s;
        s << "OnMouseUp "
          << (unsigned int)event.button.button << " "
          << event.button.x 		 << " "
          << event.button.y;
        scene.EnqueueMessage( s.str().c_str(), m_bSceneHasMouseUp );
      }
      break;
    case SDL_QUIT:
      if ( m_bHasQuit ) this->EnqueueMessage( "OnQuit", m_bHasQuit );
      else              this->SetEnabled(false);
      break;
    }
  }
#endif
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Render()
{
  CScene *pScene = GetCurrentScene();
  if ( pScene != NULL )
  {
    pScene->Render( GetRenderer());
  }
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Update()
{
  ProcessInput();

  m_Timer.Update();
  m_RunningTime.Update();
  m_fColliderUpdateTime += m_Timer.GetPassedTime().ToSeconds();
  m_fCollisionCheckTime += m_Timer.GetPassedTime().ToSeconds();

  // Take care of tasks starting and erasing
  TaskList::iterator it = m_lstTasks.begin();
  while( it != m_lstTasks.end() )
  {
    if ( (*it)->GetState() == kFinalized )
    {
      delete *it;
      it = m_lstTasks.erase(it);
            
    }
    else if ( (*it)->GetState() == kInit )
    {
      (*it)->Start();
      it++;

    } else { it++; }
  }

  if ( m_Timer.HasPassed(0, m_nUpdateIntervalMs )) 
  {
    // Update our own script
    UpdateScript( m_Timer.GetPassedTime().ToSeconds() );
    // TODO add pause capability.
    // If next scene is marked to be changed, then process it here.
    if ( m_pNextScene ) SetCurrentScene(m_pNextScene);


    if ( GetCurrentScene() != NULL)
    {
      GetCurrentScene()->Update( m_Timer.GetPassedTime().ToSeconds() );
    }  
    m_Timer.Reset();
  }
    
  if ( m_fColliderUpdateTime*1000 >= m_nColliderUpdateIntervalMs)
  {
    if ( GetCurrentScene() != NULL)
    {
      GetCurrentScene()->UpdateColliders();
    }
    m_fColliderUpdateTime = 0;
  }
    
  if ( m_fCollisionCheckTime*1000 >= m_nCollisionCheckIntervalMs )
  {
    if ( GetCurrentScene() != NULL ) GetCurrentScene()->CheckCollisions();
    m_fCollisionCheckTime = 0;
  }

    
}
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::COglRenderer &
prefix::CApplication::GetRenderer()
{
  return m_Renderer;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Core::CTimer &
prefix::CApplication::GetTimer()
{
  return m_Timer;
}
//////////////////////////////////////////////////////////////////////////////
float 
prefix::CApplication::GetElapsedTime()
{
  return m_RunningTime.GetPassedTime().ToSeconds();
}
//////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::LoadDefaultResources()
{
  // For default volume renderables 
  if ( g_ShaderMgr->HasResource("moose_default_shader") == false )
  {
      
    CShader *pShader = new CShader();

    pShader->LoadVertexShader(g_AssetBundle->GetAssetPath("default.vert"));
    pShader->LoadFragmentShader(g_AssetBundle->GetAssetPath("default.frag"));
    int iRetval = g_ShaderMgr->Create(pShader, "moose_default_shader");
    if( iRetval != 0)
      throw CMooseRuntimeError("Could not create moose_default_shader");

  }
  // For boxrenderable
  if ( g_IndexMgr->HasResource("moose_boxrenderable_indices") == false )
  {
    unsigned short int indices[] = { 0,1,2,3,0,4,5,6,7,4,7,3,2,6,5,1};
    CIndexArray *pTmp = new CIndexArray(PRIMITIVE_LINE_STRIP, 16);
    pTmp->Copy(indices);
    g_IndexMgr->Create( pTmp, "moose_boxrenderable_indices");
  }
    
  // For sphererenderable
  if ( g_IndexMgr->HasResource("moose_sphererenderable_indices") == false )
  {
    unsigned short int indices[] = { 0, 1,  2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,0,
                                     16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,16,
                                     32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,32 };
    CIndexArray *pTmp = new CIndexArray(PRIMITIVE_LINE_STRIP, 51);
    pTmp->Copy(indices);
    g_IndexMgr->Create( pTmp, "moose_sphererenderable_indices");
    assert(g_IndexMgr->HasResource("moose_sphererenderable_indices"));
  }
    
  // For capsulerenderable
  if ( g_IndexMgr->HasResource("moose_capsulerenderable_indices") == false )
  {
    unsigned short int indices[] = { 3,  4,  5,  6,  
                                     7,  8,  9, 
                                     10, 11, 12, 13, 
                                     /* lower horiz ring*/
                                     34,35,25,36,37,7,38,39,20,40,41,13,
                                     0,  
                                     /* higher horiz ring*/
                                     26,27,14,28,29,6,30,31,19,32,33,0,
                                     1,  2,  
                                     3, 17, 18, 19, 20, 21, 22, 
                                     10, 23, 24, 25, 14,15,16, 
                                     3 
    };
      
    CIndexArray *pTmp = new CIndexArray(PRIMITIVE_LINE_STRIP, 53);
    pTmp->Copy(indices);
    g_IndexMgr->Create( pTmp, "moose_capsulerenderable_indices");
    assert(g_IndexMgr->HasResource("moose_capsulerenderable_indices"));
  }

  if ( g_IndexMgr->HasResource("moose_linerenderable_indices") == false)
  {
    unsigned short int indices[] = { 0,1,2,3 };
    CIndexArray *pTmp = new CIndexArray(PRIMITIVE_TRI_STRIP,4);
    pTmp->Copy(indices);
    g_IndexMgr->Create(pTmp, "moose_linerenderable_indices");
    assert(g_IndexMgr->HasResource("moose_linerenderable_indices"));
  }
    
  if ( g_ShaderMgr->HasResource("moose_line_shader") == false )
  {
    
    CShader *pShader = new CShader();
    pShader->LoadVertexShader(g_AssetBundle->GetAssetPath("line.vert"));
    pShader->LoadFragmentShader(g_AssetBundle->GetAssetPath("line.frag"));
    int iRetval = g_ShaderMgr->Create(pShader, "moose_line_shader");
    assert( iRetval == 0);
  }
  if ( g_ShaderMgr->HasResource("moose_color_shader") == false)
  {
    CShader *pColor = new CShader();

    pColor->LoadVertexShader(g_AssetBundle->GetAssetPath("color.vert"));
    pColor->LoadFragmentShader(g_AssetBundle->GetAssetPath("color.frag"));
    int iRetval  = g_ShaderMgr->Create(pColor, "moose_color_shader");
    assert( iRetval  == 0);
  }
  
  if ( g_ShaderMgr->HasResource("moose_billboard_particle_shader") == false)
  {
    CShader *pColor = new CShader();

    pColor->LoadVertexShader(g_AssetBundle->GetAssetPath("bbps.vert"));
    pColor->LoadFragmentShader(g_AssetBundle->GetAssetPath("bbps.frag"));
    int iRetval  = g_ShaderMgr->Create(pColor, "moose_billboard_particle_shader");
    assert( iRetval  == 0);
  }
  // For default volume renderables 
  if ( g_ShaderMgr->HasResource("moose_gui_shader") == false )
  {
      
    CShader *pShader = new CShader();

    pShader->LoadVertexShader(g_AssetBundle->GetAssetPath("gui.vert"));
    pShader->LoadFragmentShader(g_AssetBundle->GetAssetPath("gui.frag"));
    int iRetval = g_ShaderMgr->Create(pShader, "moose_gui_shader");
    if( iRetval != 0)
      throw CMooseRuntimeError("Could not create moose_gui_shader");
  
  }
}
///////////////////////////////////////////////////////////////////////////////
int MainLoop( void * data )
{
  CApplication *pApp = reinterpret_cast<CApplication *>(data);
  if ( !pApp ) return 1;
  CFpsCounter & fps = pApp->GetFPSCounter();
  while( pApp->IsEnabled() )
  {
    pApp->Update();
    pApp->Render();
    fps++;
    fps.Update();
    if ( fps.HasPassed(1,0))
    {
      //cout << "FPS: " << counter.GetFPS() << endl;
      fps.Reset();
    }
  }

  return 0;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Run()
{
#if !defined(MOOSE_APPLE_IPHONE)
  m_pMainLoopThread = SDL_CreateThread(MainLoop, this);
#endif
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Halt()
{
  SetEnabled(false);
#if !defined(MOOSE_APPLE_IPHONE)
  if ( m_pMainLoopThread ) SDL_WaitThread(m_pMainLoopThread, NULL);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::AddTask( Moose::Core::IThreadedTask *pTask )
{
  m_lstTasks.push_back(pTask);
}
///////////////////////////////////////////////////////////////////////////////
Moose::Core::CFpsCounter &
prefix::CApplication::GetFPSCounter()
{
  return m_FpsCounter;
}
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
SCRIPT_CMD_DECL( OpenScreen );
#endif
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::RegisterUserCommands()
{
#if !defined(MOOSE_APPLE_IPHONE)
  CREATE_CMD( OpenScreen );
#endif
}
#if !defined(MOOSE_APPLE_IPHONE)
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( OpenScreen )
{

  CHECK_ARGS( 1, "screenParams")
  if ( CScreen::Exists() ) SCRIPT_ERROR( "Screen is already open.");

  NameObjMap mapNameObj;
  if ( ParseKeyValueMap(pInterp, mapNameObj, objv[1]) != TCL_OK ) return TCL_ERROR;

  CScreenParams p;
  int bFullscreen = 0;
  MAP_GET_INT( mapNameObj, ".redBits", CScreen::m_iRedSize);
  MAP_GET_INT( mapNameObj, ".greenBits", CScreen::m_iGreenSize);
  MAP_GET_INT( mapNameObj, ".blueBits", CScreen::m_iBlueSize);
  MAP_GET_INT( mapNameObj, ".doubleBuffer", CScreen::m_bDoubleBuffer);
  MAP_GET_INT( mapNameObj, ".depthBits", CScreen::m_iDepthBufferSize);
  MAP_GET_INT( mapNameObj, ".width", CScreen::m_iWidth);
  MAP_GET_INT( mapNameObj, ".height", CScreen::m_iHeight);
  MAP_GET_INT( mapNameObj, ".colorDepth", CScreen::m_iScreenDepth);
  MAP_GET_INT( mapNameObj, ".fullscreen", bFullscreen);
  if ( bFullscreen ) CScreen::m_iVideoModeFlags |= SDL_FULLSCREEN;
  // videomodeflags is intentionally left without parsing.
  MAP_GET_INT( mapNameObj, ".multiSampleBuffers", CScreen::m_iMultiSampleBuffers);
  MAP_GET_INT( mapNameObj, ".multiSampleSamples", CScreen::m_iMultiSampleSamples);

  if ( CScreen::GetInstance() == NULL )
  {
    SCRIPT_ERROR("Could not open screen.");
  }
  return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
#endif
