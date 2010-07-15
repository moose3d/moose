#ifndef __PhoenixApplication_h__
#define __PhoenixApplication_h__
#include <PhoenixScene.h>
#include <string>
#include <PhoenixEventQueue.h>
///////////////////////////////////////////////////////////////////////////////
#if !defined(PHOENIX_APPLE_IPHONE)
#include <SDL.h>
#endif
/*typedef SDLKey	  KEY_TYPE;
typedef SDL_Event MOUSEBUTTON_EVENT;
typedef SDL_Event MOUSEMOTION_EVENT;
typedef SDL_Event KEYPRESS_EVENT;*/
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Scene
	{

		///////////////////////////////////////////////////////////////////////////////
		typedef std::map<const std::string, Phoenix::Scene::CScene * > SceneMap;
		///////////////////////////////////////////////////////////////////////////////
		class CApplication : public Phoenix::Scene::CGameObject
		{
		protected:

			SceneMap  											m_mapScenes;      	///!< from names to scene pointers
			Phoenix::Scene::CScene *				m_pCurrentScene; 		///!< For faster access.
			Phoenix::Graphics::COglRenderer m_Renderer;					///!< This is usually needed. :)
			Phoenix::Core::CTimer 					m_Timer;						///!< Main timer.
			Phoenix::Core::CFpsCounter				m_FpsCounter;                   ///!< Fps counter.
			bool 				m_bSceneHasKeyUp;
			bool				m_bSceneHasKeyDown;
			bool 				m_bSceneHasMouseDown;
			bool 				m_bSceneHasMouseUp;
			bool 				m_bSceneHasMouseMotion;
			bool 				m_bHasQuit;
#if !defined(PHOENIX_APPLE_IPHONE)
			SDL_Thread *		m_pMainLoopThread;
#else
            void *  m_pMainLoopThread;
            Phoenix::Scene::CEventQueue   m_EventQueue;    
#endif
		protected:

			void 		  			 SetCurrentScene( Phoenix::Scene::CScene *pScene );
			void      			 ProcessInput();
			void 						 CheckSceneInputs();
            /// Loads default set of shaders, etc.
            void                         LoadDefaultResources();
		protected:
            unsigned short int           m_nUpdateIntervalMs;
            unsigned short int           m_nColliderUpdateIntervalMs;
            unsigned short int           m_nCollisionCheckIntervalMs;
            float                        m_fColliderUpdateTime;
            float                        m_fCollisionCheckTime;
            
        public:

			CApplication( );
			virtual ~CApplication();

			Phoenix::Scene::CScene * GetCurrentScene();
			void     SetCurrentScene( const std::string & name );
			void 		 AddScene( const std::string & name, Phoenix::Scene::CScene *pScene );
			void     RemoveScene( const std::string & name );
			Phoenix::Scene::CScene * GetScene( const std::string & name );
			void     Update();
			virtual void     Render();
			Phoenix::Graphics::COglRenderer & GetRenderer();
			Phoenix::Core::CTimer & GetTimer();
			void		 RegisterUserCommands();
			void 		 Init();
#if defined(PHOENIX_APPLE_IPHONE)
            Phoenix::Scene::CEventQueue & GetEventQueue();
#endif
			////////////////////
			/// Executes main loop in a thread.
			void 		 Run();
			void 		 Halt();
			Phoenix::Core::CFpsCounter & GetFPSCounter();
		}; // CApplication
	} // Scene
} // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif
