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
      //////////////////////
      /// Overwrite this to create new commands.
      virtual void RegisterUserCommands() {}
    protected:
      Tcl_Interp * GetInterp();


    };
    /////////////////////////////////////////////////////////////////
    typedef std::list<std::string> MessageList;
    class CAIScript
    {
    	friend class CAIObject;
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
/// Some macros for helping in writing new commands and checking parameters.
////////////////////////////////////////////////////////////////////////////////
/// Checks the number of arguments and prints error message if it does not match.
/// Must be used within SCRIPT_CMD_IMPL( .. ) {}
#define CHECK_ARGS( ARGC, USAGE_MSG ) {              \
	if ( (objc-1) != ARGC ) {                              \
		Tcl_WrongNumArgs(pInterp, 0, objv, USAGE_MSG);   \
		return TCL_ERROR;                                \
	}                                                  \
}
///////////////////////////////////////////////////////////////////////////////
/// Checks the number of arguments and prints error message if it does not match.
/// Must be used within SCRIPT_CMD_IMPL( .. ) {}
#define CHECK_ARGS_BETWEEN( ARGC_MIN, ARGC_MAX, USAGE_MSG ) {              \
	if ( (objc - 1) < ARGC_MIN || (objc - 1) > ARGC_MAX ) {                              \
		Tcl_WrongNumArgs(pInterp, 0, objv, USAGE_MSG);   \
		return TCL_ERROR;                                \
	}                                                  \
}
////////////////////////////////////////////////////////////////////////////////
/// Declares a new command C-style function call.
#define SCRIPT_CMD_DECL( NAME ) int NAME( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
/// Implements a new commands C-style function call.
#define SCRIPT_CMD_IMPL( NAME ) int NAME( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
/// Accesses script global variable as integer.
#define SCRIPT_GET_INT( PARAM_NUM, VAR ) {\
	if ( Tcl_GetIntFromObj( pInterp, objv[PARAM_NUM], &VAR ) != TCL_OK )\
	{\
		return TCL_ERROR;\
	}\
}
///////////////////////////////////////////////////////////////////////////////
/// Accesses script global variable as string.
#define SCRIPT_GET_STR( PARAM_NUM ) Tcl_GetString(objv[PARAM_NUM])
///////////////////////////////////////////////////////////////////////////////
/// Accesses script global variable as double.
#define SCRIPT_GET_DOUBLE( PARAM_NUM, VAR ) {\
	if ( Tcl_GetDoubleFromObj( pInterp, objv[PARAM_NUM], &VAR ) != TCL_OK )\
	{\
		return TCL_ERROR;\
	}\
}
///////////////////////////////////////////////////////////////////////////////
/// Accesses script global variable as float.
#define SCRIPT_GET_FLOAT( PARAM_NUM, VAR ) {\
	double dScriptTmpDoubleVal = 0.0f;\
	if ( Tcl_GetDoubleFromObj( pInterp, objv[PARAM_NUM], &dScriptTmpDoubleVal ) != TCL_OK )\
	{\
		return TCL_ERROR;\
	}\
	VAR = dScriptTmpDoubleVal;\
}
///////////////////////////////////////////////////////////////////////////////
/// Sets script error message.
#define SCRIPT_ERROR( ERROR_MSG ){\
	Tcl_SetResult(pInterp, (char *)ERROR_MSG, NULL);\
	return TCL_ERROR;\
}
///////////////////////////////////////////////////////////////////////////////
/// Parses value map from list of strings. Each name-value pair must be separated with SEP.
/// PARAM_NUM is the number of list parameter passed to command. First parameter is 1, and so on.
#define SCRIPT_PARSE_VALUE_MAP( MAP, SEP, PARAM_NUM ){\
\
	int length = 0;\
\
	if ( Tcl_ListObjLength( pInterp, objv[PARAM_NUM], &length) != TCL_OK )\
	{\
		Tcl_SetResult( pInterp, (char *)"Value parse error", NULL);\
		return TCL_ERROR;\
	}\
\
	Tcl_Obj *pTmp = NULL;\
	for( int i=0;i<length; i++)\
	{\
		Tcl_ListObjIndex( pInterp, objv[PARAM_NUM], i, &pTmp);\
		std::string strNameVal( Tcl_GetString(pTmp));\
		size_t pos;\
		if ( (pos = strNameVal.find("=")) != string::npos)\
		{\
			MAP.insert(std::make_pair(strNameVal.substr(0, pos),\
									strNameVal.substr(pos+1, strNameVal.size())));\
		}\
	}\
}
///////////////////////////////////////////////////////////////////////////////
// Macro for creating commands. FUNC must be an existing static function
// declared with SCRIPT_CMD_DECL and implemented using SCRIPT_CMD_IMPL.
#define CREATE_CMD(  FUNC ) {				\
	assert(Tcl_CreateObjCommand( GetInterp(), #FUNC,		\
				     FUNC, (ClientData)this,	\
				     NULL) != NULL );		\
}
/////////////////////////////////////////////////////////////////
/// Same as CREATE_CMD, but with alternate data passed as client parameter.
#define CREATE_CMD_PTR(  FUNC, OBJPTR) {			\
	assert(Tcl_CreateObjCommand( GetInterp(), #FUNC,		\
				     FUNC, (ClientData)OBJPTR,	\
				     NULL) != NULL );		\
}
///////////////////////////////////////////////////////////////////////////////
#endif

