#ifndef __PhoenixAIScript_h__
#define __PhoenixAIScript_h__
/////////////////////////////////////////////////////////////////
#include <tcl.h>
#include <list>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    class CGameObject;
  }
  namespace AI
  {
    class CAIScript;
    class CAIObject
    {
    protected:
      
      float 	   m_fPassedTime;
      CAIScript *  m_pAIScript;
      std::string  m_strScriptFile;
      Phoenix::Scene::CGameObject *m_pGameObject;
      
    public:

      CAIObject( Phoenix::Scene::CGameObject *pGameObject );
      virtual ~CAIObject();

      void 	SetPassedTime( float fSeconds );
      float 	GetPassedTime() const;

      void  	SetScript( const char *szScript );
      void    LoadScript();
      void    ReloadScript();
      void    UpdateScript( float fSeconds );
      void    EnqueueMessage( const std::string & msg );

      bool   CopyGlobalVarToResult( Tcl_Interp *pInterp, 
				    const std::string & varName );

      bool   SetGlobalVar( const std::string &varName, Tcl_Obj *pVar );
      Phoenix::Scene::CGameObject * GetGameObject();

      bool   GetGlobalVar( const std::string & varName, bool & value );
      bool   GetGlobalVar( const std::string & varName, int & value );
      bool   GetGlobalVar( const std::string & varName, float & value );
      bool   GetGlobalVar( const std::string & varName, std::string & value );
    };
    /////////////////////////////////////////////////////////////////
    typedef std::list<std::string> MessageList;
    class CAIScript
    {
    protected:

      CAIObject *				m_pEntity;
      Tcl_Interp *				m_pInterp;
      float 				m_fPassedTime;
      MessageList				m_lstProcCalls;
  
    public:

      CAIScript( CAIObject *pEntity, const char * szScript );
      virtual ~CAIScript();

      void Update( float fSeconds );
      void ReloadScript( const char * szScript );
      void EnqueueMessage( const std::string & msg );

      Tcl_Obj * GetGlobalVar( const std::string & name);
      bool SetGlobalVar( const std::string & name, Tcl_Obj *pValue );
      void PrintResult();

      bool   GetGlobalVar( const std::string & varName, int & value );
      bool   GetGlobalVar( const std::string & varName, float & value );
      bool   GetGlobalVar( const std::string & varName, std::string & value );
    };
    /////////////////////////////////////////////////////////////////
  } // AI
} // Phoenix
////////////////////////////////////////////////////////////////////////////////
#endif

