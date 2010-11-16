#include "MooseApplication.h"
#include "MooseLogger.h"

#include <sstream>
#include <iostream>
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Scene;
using namespace Moose::Core;
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
prefix::CApplication::CApplication() : m_pCurrentScene(NULL)
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
prefix::CApplication::SetCurrentScene( const std::string & name )
{
	SceneMap::iterator it = m_mapScenes.find( name );

	if ( m_pCurrentScene ) m_pCurrentScene->OnExit();
    
    if ( it != m_mapScenes.end() ) 
    {
        m_pCurrentScene = it->second;
        m_pCurrentScene->OnEnter();
	}
    else 
    {
        m_pCurrentScene = NULL;
	}
    CheckSceneInputs();
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
	if ( GetCurrentScene() == NULL ) return;
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
                scene.OnTouchBegan( e.x, e.y, e.flags);
                break;
            case kTouchMoved:
                scene.OnTouchMoved( e.x, e.y, e.flags);
                break;
            case kTouchEnded:
                scene.OnTouchEnded( e.x, e.y, e.flags);
                break;
            case kAccelerate:
                //g_Error << "Moose:Received accel event \n";
                scene.OnAccelerate( e.x, e.y, e.z, e.flags);
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
				ostringstream s;
				s << "OnMouseMotion "
					<< event.motion.x << " "
					<< event.motion.y;
				scene.EnqueueMessage( s.str().c_str(), m_bSceneHasMouseMotion );
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
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
				ostringstream s;
				s << "OnMouseUp "
					<< (unsigned int)event.button.button << " "
					<< event.button.x 		 << " "
					<< event.button.y;
				scene.EnqueueMessage( s.str().c_str(), m_bSceneHasMouseUp );
			}
			break;
			case SDL_QUIT:
				this->EnqueueMessage( "OnQuit", m_bHasQuit );
				break;
		}
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Render()
{
	if ( GetCurrentScene() != NULL )
	{
		GetCurrentScene()->Render( GetRenderer());
	}
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::Update()
{
	ProcessInput();

	m_Timer.Update();
    m_fColliderUpdateTime += m_Timer.GetPassedTime().ToSeconds();
	m_fCollisionCheckTime += m_Timer.GetPassedTime().ToSeconds();
    if ( m_Timer.HasPassed(0, m_nUpdateIntervalMs )) 
	{
      // Update our own script
      UpdateScript( m_Timer.GetPassedTime().ToSeconds() );
      // TODO add pause capability.
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
void
prefix::CApplication::LoadDefaultResources()
{
    // For default volume renderables 
    if ( g_ShaderMgr->HasResource("moose_default_shader") == false )
    {
      
      CShader *pShader = new CShader();
      pShader->LoadVertexShader(g_AssetBundle->GetAssetPath("Shaders/default.vert"));
      pShader->LoadFragmentShader(g_AssetBundle->GetAssetPath("Shaders/default.frag"));
      int iRetval = g_ShaderMgr->Create(pShader, "moose_default_shader");
      assert( iRetval == 0);
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
    }
    
    if ( g_IndexMgr->HasResource("moose_linerenderable_indices") == false)
    {
        unsigned short int indices[] = { 0,1,2,3 };
        CIndexArray *pTmp = new CIndexArray(PRIMITIVE_TRI_STRIP,4);
        pTmp->Copy(indices);
        g_IndexMgr->Create(pTmp, "moose_linerenderable_indices");
    }
    
    if ( g_ShaderMgr->HasResource("moose_line_shader") == false )
    {
        // does NOT use model transform
        const char vsh[] = "attribute vec3 a_vertex;"\
            "attribute vec4 a_endpos_thickness;"\
            "uniform mat4 m_viewMatrix;"\
            "uniform mat4 m_projMatrix;"\
            "void main()"\
            "{"\
            "vec4 endPos   = m_viewMatrix * vec4(a_endpos_thickness.xyz,1.0);"\
            "vec4 startPos = m_viewMatrix * vec4(a_vertex,1.0);"\
            "vec3 linedir = (endPos.xyz - startPos.xyz);"\
            "vec3 offsetVec = normalize(cross(startPos.xyz,linedir));"\
            "startPos = vec4(startPos.xyz + (offsetVec * a_endpos_thickness.w),startPos.w);"\
            "gl_Position = m_projMatrix * startPos;"\
            "}";
            
            
        const char fsh[] = "#ifdef GL_ES\nprecision mediump float;\n#endif\n"\
                            "uniform vec4 color;void main(){gl_FragColor = color;}";
        CShader *pShader = new CShader();
        pShader->CreateVertexShaderFromSource(vsh, "line vsh");
        pShader->CreateFragmentShaderFromSource(fsh,"line fsh");
        int iRetval = g_ShaderMgr->Create(pShader, "moose_line_shader");
        assert( iRetval == 0);

        CShader *pColor = new CShader();
        pColor->LoadVertexShader(g_AssetBundle->GetAssetPath("Shaders/color.vert"));
        pColor->LoadFragmentShader(g_AssetBundle->GetAssetPath("Shaders/color.frag"));
        iRetval  = g_ShaderMgr->Create(pColor, "moose_color_shader");
        assert( iRetval  == 0);
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
	if ( CSDLScreen::Exists() ) SCRIPT_ERROR( "Screen is already open.");

	NameObjMap mapNameObj;
	if ( ParseKeyValueMap(pInterp, mapNameObj, objv[1]) != TCL_OK ) return TCL_ERROR;

	CSDLScreenParams p;
	int bFullscreen = 0;
	MAP_GET_INT( mapNameObj, ".redBits", p.m_iRedSize);
	MAP_GET_INT( mapNameObj, ".greenBits", p.m_iGreenSize);
	MAP_GET_INT( mapNameObj, ".blueBits", p.m_iBlueSize);
	MAP_GET_INT( mapNameObj, ".doubleBuffer", p.m_bDoubleBuffer);
	MAP_GET_INT( mapNameObj, ".depthBits", p.m_iDepthBufferSize);
	MAP_GET_INT( mapNameObj, ".width", p.m_iWidth);
	MAP_GET_INT( mapNameObj, ".height", p.m_iHeight);
	MAP_GET_INT( mapNameObj, ".colorDepth", p.m_iScreenDepth);
	MAP_GET_INT( mapNameObj, ".fullscreen", bFullscreen);
	if ( bFullscreen ) p.m_iVideoModeFlags |= SDL_FULLSCREEN;
	// videomodeflags is intentionally left without parsing.
	MAP_GET_INT( mapNameObj, ".multiSampleBuffers", p.m_iMultiSampleBuffers);
	MAP_GET_INT( mapNameObj, ".multiSampleSamples", p.m_iMultiSampleSamples);
	CSDLScreen::m_SDLScreenParams = p;
	if ( CSDLScreen::GetInstance() == NULL )
	{
		SCRIPT_ERROR("Could not open screen.");
	}
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
#endif
