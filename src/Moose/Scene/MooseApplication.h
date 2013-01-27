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

#ifndef __MooseApplication_h__
#define __MooseApplication_h__
#include <Rocket/Core.h>
#include <MooseScene.h>
#include <string>
#include <MooseEventQueue.h>
#include <MooseThreadedTask.h>
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
#include <SDL.h>
#endif
#include <sstream>
/*typedef SDLKey	  KEY_TYPE;
typedef SDL_Event MOUSEBUTTON_EVENT;
typedef SDL_Event MOUSEMOTION_EVENT;
typedef SDL_Event KEYPRESS_EVENT;*/
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    class IThreadedTask;
  }
  namespace Graphics 
  {
    class CRocketRenderInterface;
  }
  namespace Scene
  {

    ///////////////////////////////////////////////////////////////////////////////
    typedef std::map<const std::string, Moose::Scene::CScene * > SceneMap;
    typedef std::list<Moose::Core::IThreadedTask *> TaskList;
    ///////////////////////////////////////////////////////////////////////////////
    class CApplication : public Moose::Scene::CGameObject,
                         public Moose::Core::CSingleton<Moose::Scene::CApplication>,
                         public Rocket::Core::SystemInterface
    {
      friend class Moose::Core::CSingleton<Moose::Scene::CApplication>;
    protected:

      SceneMap  						m_mapScenes;      	///< from names to scene pointers
      Moose::Scene::CScene *			m_pCurrentScene;	///< For faster access.
      Moose::Scene::CScene *            m_pNextScene;      ///< To enforce proper order of update->render during scene change.
      Moose::Graphics::COglRenderer     m_Renderer;		    ///< This is usually needed. :)
      Moose::Core::CTimer 				m_Timer;			///< Main timer.
      Moose::Core::CTimer               m_RunningTime;      ///< Keeps track of how long program has been running.
      Moose::Core::CFpsCounter          m_FpsCounter;       ///< Fps counter.
      Moose::Graphics::CRocketRenderInterface *   m_pRenderInterface; ///< Render interface for Rocket.
      bool 				m_bSceneHasKeyUp;
      bool				m_bSceneHasKeyDown;
      bool 				m_bSceneHasMouseDown;
      bool 				m_bSceneHasMouseUp;
      bool 				m_bSceneHasMouseMotion;
      bool 				m_bHasQuit;
      TaskList            m_lstTasks;
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
      float             GetElapsedTime(); ///< Returns elapsed time since program started.
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
      void         AddTask( Moose::Core::IThreadedTask *pTask );
    }; // CApplication
    ////////////////////
    /// Loads and initializes scenes in background.
    class MOOSE_API CSceneLoadTask : public Moose::Core::IThreadedTask 
    {
    private:
      Moose::Scene::CScene *m_pScene;
    public:
      CSceneLoadTask( Moose::Scene::CScene * pScene )
      {
        m_pScene = pScene;
      }
      void Run()
      {
        // Load scene and inform currently active scene about it. 
        m_pScene->Load();
        m_pScene->Init();
        std::ostringstream s;
        s << "SceneLoaded " << m_pScene->GetName();
        CApplication::GetInstance()->GetCurrentScene()->EnqueueMessage( s.str());
      }
    }; // CSceneLoadTask

  } // Scene
} // Moose
///////////////////////////////////////////////////////////////////////////////
#endif
