#ifndef __MooseApplication_h__
#define __MooseApplication_h__
#include <MooseScene.h>
#include <string>
#include <MooseEventQueue.h>
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
#include <SDL.h>
#endif
/*typedef SDLKey	  KEY_TYPE;
typedef SDL_Event MOUSEBUTTON_EVENT;
typedef SDL_Event MOUSEMOTION_EVENT;
typedef SDL_Event KEYPRESS_EVENT;*/
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Scene
	{

		///////////////////////////////////////////////////////////////////////////////
		typedef std::map<const std::string, Moose::Scene::CScene * > SceneMap;
		///////////////////////////////////////////////////////////////////////////////
      class CApplication : public Moose::Scene::CGameObject,
                           public Moose::Core::CSingleton<Moose::Scene::CApplication>
		{
          friend class Moose::Core::CSingleton<Moose::Scene::CApplication>;
		protected:

			SceneMap  											m_mapScenes;      	///!< from names to scene pointers
			Moose::Scene::CScene *				m_pCurrentScene; 		///!< For faster access.
			Moose::Graphics::COglRenderer m_Renderer;					///!< This is usually needed. :)
			Moose::Core::CTimer 					m_Timer;						///!< Main timer.
			Moose::Core::CFpsCounter				m_FpsCounter;                   ///!< Fps counter.
			bool 				m_bSceneHasKeyUp;
			bool				m_bSceneHasKeyDown;
			bool 				m_bSceneHasMouseDown;
			bool 				m_bSceneHasMouseUp;
			bool 				m_bSceneHasMouseMotion;
			bool 				m_bHasQuit;
#if !defined(MOOSE_APPLE_IPHONE)
			SDL_Thread *		m_pMainLoopThread;
#else
            void *  m_pMainLoopThread;
            Moose::Scene::CEventQueue   m_EventQueue;    
#endif
		protected:

			void 		  			 SetCurrentScene( Moose::Scene::CScene *pScene );
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
            CApplication( );
        public:
          virtual ~CApplication();
			Moose::Scene::CScene * GetCurrentScene();
			void     SetCurrentScene( const std::string & name );
			void 		 AddScene( const std::string & name, Moose::Scene::CScene *pScene );
			void     RemoveScene( const std::string & name );
			Moose::Scene::CScene * GetScene( const std::string & name );
			void     Update();
			virtual void     Render();
			Moose::Graphics::COglRenderer & GetRenderer();
			Moose::Core::CTimer & GetTimer();
			void		 RegisterUserCommands();
			void 		 Init();
#if defined(MOOSE_APPLE_IPHONE)
            Moose::Scene::CEventQueue & GetEventQueue();
#endif
			////////////////////
			/// Executes main loop in a thread.
			void 		 Run();
			void 		 Halt();
			Moose::Core::CFpsCounter & GetFPSCounter();
		}; // CApplication
	} // Scene
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif
