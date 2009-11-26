#include "PhoenixApplication.h"
#include "PhoenixLogger.h"
#include "PhoenixSDLScreen.h"
#include <sstream>
#include <iostream>
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
using namespace Phoenix::Window;
using std::ostringstream;
using namespace std;
namespace prefix = Phoenix::Scene;

///////////////////////////////////////////////////////////////////////////////
prefix::CApplication::CApplication() : m_pCurrentScene(NULL)
{
	SetEnabled(true);
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
	if ( it != m_mapScenes.end() ) m_pCurrentScene = it->second;
	else m_pCurrentScene = NULL;
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
Phoenix::Scene::CScene *
prefix::CApplication::GetScene( const std::string & name )
{
	SceneMap::iterator it = m_mapScenes.find( name );
	if ( it == m_mapScenes.end() ) return NULL;
	else return it->second;

}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::ProcessInput()
{
	if ( GetCurrentScene() == NULL ) return;
	CScene & scene = *GetCurrentScene();
	SDL_Event event;
	while ( SDL_PollEvent(&event ))
	{
		switch(event.type)
		{
		  case SDL_KEYUP:
			{
				ostringstream s;
				s << "OnKeyUp " << SDL_GetKeyName(event.key.keysym.sym);
				scene.EnqueueMessage(s.str().c_str(), false );
			}
			break;
			case SDL_KEYDOWN:
			{
				ostringstream s;
				s << "OnKeyDown "
					<< SDL_GetKeyName(event.key.keysym.sym);
				scene.EnqueueMessage(s.str().c_str(), false );
			}
			break;
			case SDL_MOUSEMOTION:
			{
				ostringstream s;
				s << "OnMouseMotion "
					<< event.motion.x << " "
					<< event.motion.y;
				scene.EnqueueMessage( s.str().c_str(), false );
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				ostringstream s;
				s << "OnMouseDown "
					<< (unsigned int)event.button.button << " "
					<< event.button.x 		 << " "
					<< event.button.y;
				scene.EnqueueMessage( s.str().c_str(), false );
			}
			break;
			case SDL_MOUSEBUTTONUP:
			{
				ostringstream s;
				s << "OnMouseUp "
					<< (unsigned int)event.button.button << " "
					<< event.button.x 		 << " "
					<< event.button.y;
				scene.EnqueueMessage( s.str().c_str(), false );
			}
			break;
			case SDL_QUIT:
				this->EnqueueMessage( "OnQuit", false );
				break;
		}
	}
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
	// Update our own script
	UpdateScript( m_Timer.GetPassedTime().ToSeconds() );
	// Update every object put under management.
	g_DefaultUpdater->Update( m_Timer.GetPassedTime().ToSeconds() );
	if ( m_Timer.HasPassed(0, 5 )) // TODO change this into reasonable property
	{
		// TODO add pause capability.
		GetCurrentScene()->Update( m_Timer.GetPassedTime().ToSeconds() );
		m_Timer.Reset();
	}
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRenderer &
prefix::CApplication::GetRenderer()
{
	return m_Renderer;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Core::CTimer &
prefix::CApplication::GetTimer()
{
	return m_Timer;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_DECL( OpenScreen );
///////////////////////////////////////////////////////////////////////////////
void
prefix::CApplication::RegisterUserCommands()
{
	CREATE_CMD( OpenScreen );
}
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
