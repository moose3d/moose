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

#ifndef __MooseAIScript_h__
#define __MooseAIScript_h__
/////////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#if !defined(MOOSE_APPLE_IPHONE)
#include <tcl.h>
#else
typedef void *Tcl_Obj;
typedef void *Tcl_Interp;
#endif
#include <list>
#include <string>
#include <map>
#include "MooseMessageSystem.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Math
	{
		class CRay;

	}
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
      Moose::AI::CMessageQueue  m_MessageQueue; ///!< Message queue for generic message passing.
      float 	   m_fPassedTime;                 ///!< Passed time since last update.
      CAIScript *  m_pAIScript;                   ///!< AI Script object.
      Moose::Scene::CGameObject *m_pGameObject; ///!< Game object pointer for scripting.

#if !defined(MOOSE_APPLE_IPHONE)
        std::string  m_strScriptFile;               ///!< Script file name.
    protected:
        Tcl_Interp * GetInterp();

#endif
      
    public:

      CAIObject( Moose::Scene::CGameObject *pGameObject );
      virtual ~CAIObject();

      void 	SetPassedTime( float fSeconds );
      float 	GetPassedTime() const;
      void    UpdateScript( float fSeconds );
      
#if !defined(MOOSE_APPLE_IPHONE)
      void    LoadScript();
      void  	SetScript( const char *szScript );
      const std::string & GetScript() const;
        
      
      void    ReloadScript();
      
      void    EnqueueMessage( const std::string & msg, bool bRequireReceiver = true );
      bool   CopyGlobalVarToResult( Tcl_Interp *pInterp, 
				    const std::string & varName );

      bool   SetGlobalVar( const std::string &varName, Tcl_Obj *pVar );

      
      bool   GetGlobalVar( const std::string & varName, bool & value );
      bool   GetGlobalVar( const std::string & varName, int & value );
      bool   GetGlobalVar( const std::string & varName, float & value );
      bool   GetGlobalVar( const std::string & varName, std::string & value );
      Tcl_Obj * GetGlobalVar( const std::string & name);

      bool	 HasCommand( const char *szName );

       
      
      void 				 RegisterCommands();
#else
        void EnqueueMessage( Moose::AI::CMessage *pMsg, const Moose::Core::CTimeStamp & time = Moose::Core::CTimeStamp(0,0));
        
#endif

        //////////////////////
        /// Overwrite this to create new commands or register listeners in messagequeue.
        virtual void RegisterUserCommands() {}
        Moose::Scene::CGameObject * GetGameObject();
        Moose::AI::CMessageQueue & GetMessageQueue();
    };
    /////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
      struct ScriptMessage
    {
    	std::string msg;
    	bool        bRequireReceiver;
    	ScriptMessage( const std::string & m, bool bFlag) : msg(m),bRequireReceiver(bFlag){}
    };
    /////////////////////////////////////////////////////////////////
    typedef std::list<ScriptMessage *> MessageList;
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
      void EnqueueMessage( const std::string & msg, bool bRequireReceiver  );

      Tcl_Obj * GetGlobalVar( const std::string & name);
      bool SetGlobalVar( const std::string & name, Tcl_Obj *pValue );
      void PrintResult();
      void 				 RegisterCommands();
      bool   GetGlobalVar( const std::string & varName, int & value );
      bool   GetGlobalVar( const std::string & varName, float & value );
      bool   GetGlobalVar( const std::string & varName, std::string & value );
      bool	 HasCommand( const char *szName );
    };
#endif
  } // AI
} // Moose
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
#define SCRIPT_GET_INTP( OBJ, VAR ) {\
	if ( Tcl_GetIntFromObj( pInterp, OBJ, &VAR ) != TCL_OK )\
	{\
		return TCL_ERROR;\
	}\
}
///////////////////////////////////////////////////////////////////////////////
/// Accesses script global variable as string.
#define SCRIPT_GET_STR( PARAM_NUM ) Tcl_GetString(objv[PARAM_NUM])
#define SCRIPT_GET_STRP( OBJ ) Tcl_GetString(OBJ)
///////////////////////////////////////////////////////////////////////////////
/// Accesses script global variable as double.
#define SCRIPT_GET_DOUBLE( PARAM_NUM, VAR ) {\
	if ( Tcl_GetDoubleFromObj( pInterp, objv[PARAM_NUM], &VAR ) != TCL_OK )\
	{\
		return TCL_ERROR;\
	}\
}
#define SCRIPT_GET_DOUBLEP( OBJ, VAR ) {\
	if ( Tcl_GetDoubleFromObj( pInterp, OBJ, &VAR ) != TCL_OK )\
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
#define SCRIPT_GET_FLOATP( OBJ, VAR ) {\
	double dScriptTmpDoubleVal = 0.0f;\
	if ( Tcl_GetDoubleFromObj( pInterp, OBJ, &dScriptTmpDoubleVal ) != TCL_OK )\
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
/// Sets result of script command.
#define SCRIPT_RESULT( RESULT ) 					Tcl_SetResult(pInterp, (char *)RESULT, NULL)
#define SCRIPT_INT_RESULT( RESULT ) 			Tcl_SetObjResult( pInterp, Tcl_NewIntObj( RESULT ) )
#define SCRIPT_DOUBLE_RESULT( RESULT )		Tcl_SetObjResult( pInterp, Tcl_NewDoubleObj( RESULT ) )
///////////////////////////////////////////////////////////////////////////////
/// Parses value map from list of strings. Each name-value pair must be separated with SEP.
/// PARAM_NUM is the number of list parameter passed to command. First parameter is 1, and so on.
#define SCRIPT_PARSE_VALUE_MAP( MAP, SEP, PARAM_NUM ){\
\
	int length = 0;\
\
	if ( Tcl_ListObjLength( pInterp, objv[PARAM_NUM], &length) != TCL_OK )\
	{\
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
// Parses list into floating-point vector.
#define SCRIPT_GET_FLOAT_VECP( OBJ, NUM_ELEM, VEC ){ \
	Tcl_Obj *pVecList = OBJ;\
	int len = 0;\
	if ( Tcl_ListObjLength( pInterp, pVecList, &len ) != TCL_OK ) return TCL_ERROR;\
	if ( len != NUM_ELEM )\
	{\
		ostringstream s;\
		Tcl_SetResult(pInterp, (char *)"Parameter is not list with " #NUM_ELEM " values.", NULL);\
		return TCL_ERROR;\
	}\
	Tcl_Obj *pValue;\
	double val;\
	for( int i=0; i< NUM_ELEM; i++) {\
		if ( Tcl_ListObjIndex( pInterp, pVecList, i, &pValue) != TCL_OK ) return TCL_ERROR;\
		if ( Tcl_GetDoubleFromObj( pInterp, pValue, &val) ) return TCL_ERROR;\
		VEC[i] = val;\
	}\
}
#define SCRIPT_GET_FLOAT_VEC( PARAM_NUM, NUM_ELEM, VEC ){\
	SCRIPT_GET_FLOAT_VECP( objv[PARAM_NUM], NUM_ELEM, VEC )\
}
///////////////////////////////////////////////////////////////////////////////
// Parses list into int vector.
#define SCRIPT_GET_INT_VECP( OBJ, NUM_ELEM, VEC ){ \
	Tcl_Obj *pVecList = OBJ;\
	int len = 0;\
	if ( Tcl_ListObjLength( pInterp, pVecList, &len ) != TCL_OK ) return TCL_ERROR;\
	if ( len != NUM_ELEM )\
	{\
		ostringstream s;\
		Tcl_SetResult(pInterp, (char *)"Parameter is not list with " #NUM_ELEM " values.", NULL);\
		return TCL_ERROR;\
	}\
	Tcl_Obj *pValue;\
	for( int i=0; i< NUM_ELEM; i++) {\
		if ( Tcl_ListObjIndex( pInterp, pVecList, i, &pValue) != TCL_OK ) return TCL_ERROR;\
		if ( Tcl_GetIntFromObj( pInterp, pValue, &VEC[i]) ) return TCL_ERROR;\
	}\
}
#define SCRIPT_GET_INT_VEC( PARAM_NUM, NUM_ELEM, VEC ){\
	SCRIPT_GET_INT_VECP( objv[PARAM_NUM], NUM_ELEM, VEC )\
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
#define SCRIPT_LIST_FROM_FLOAT_VECP( LIST, NUM_ELEM, VEC ) {\
	for( int i=0;i<NUM_ELEM; i++){\
		if ( Tcl_ListObjAppendElement( pInterp, LIST, Tcl_NewDoubleObj(VEC[i])) != TCL_OK ) return TCL_ERROR;\
	}\
}
	///////////////////////////////////////////////////////////////////////////////
#define SCRIPT_RESULT_FLOAT_VECP( NUM_ELEM, VEC ){\
	Tcl_Obj *pVecList = Tcl_NewListObj( 0, NULL );\
	SCRIPT_LIST_FROM_FLOAT_VECP( pVecList, NUM_ELEM, VEC );\
	Tcl_SetObjResult( pInterp, pVecList);\
}
///////////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, Tcl_Obj *> NameObjMap;
///////////////////////////////////////////////////////////////////////////////
int
ParseRay( Tcl_Interp *pInterp, NameObjMap & rayParam, Moose::Math::CRay & ray );
///////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
inline int
ParseKeyValueMap( Tcl_Interp *pInterp,
									NameObjMap & mapNameObj,
									Tcl_Obj *pList )
{

	// Do some sanity checking first
	int lstLength = 0;
	if ( Tcl_ListObjLength( pInterp, pList, &lstLength) != TCL_OK  ) return TCL_ERROR;
	if ( (lstLength % 2) != 0  ) SCRIPT_ERROR( "List does not have even number of parameters. ");


	Tcl_Obj *pKey = NULL;
	Tcl_Obj *pValue = NULL;
	// Walk through every name-value pair in list
	for( int i=0;i<lstLength; i+=2)
	{
		if ( Tcl_ListObjIndex( pInterp, pList, i,   &pKey  ) != TCL_OK ) return TCL_ERROR;
		if ( Tcl_ListObjIndex( pInterp, pList, i+1, &pValue) != TCL_OK ) return TCL_ERROR;
		mapNameObj[ std::string(Tcl_GetString(pKey))] = pValue;
	}
	return TCL_OK;
}
#endif
///////////////////////////////////////////////////////////////////////////////
#define MAP_HAS( MAP, PARAM ) ( MAP.find(PARAM) != MAP.end() )
///////////////////////////////////////////////////////////////////////////////
#define SCRIPT_LIST_ADD_INT( LISTPTR, KEY, VALUE ) {\
	if ( Tcl_ListObjAppendElement(pInterp, LISTPTR, Tcl_NewStringObj(KEY, strlen(KEY))) != TCL_OK ) return TCL_ERROR;\
	if ( Tcl_ListObjAppendElement(pInterp, LISTPTR, Tcl_NewIntObj(VALUE)) != TCL_OK ) return TCL_ERROR;\
}
///////////////////////////////////////////////////////////////////////////////
#define MAP_GET_INT( MAP, KEY, VAR ){\
	if ( MAP_HAS( MAP, KEY) ) {\
		SCRIPT_GET_INTP( MAP[KEY], VAR );\
	}\
}
///////////////////////////////////////////////////////////////////////////////
#define MAP_GET_FLOAT( MAP, KEY, VAR ){\
	if ( MAP_HAS( MAP, KEY) ) {\
		SCRIPT_GET_FLOATP( MAP[KEY], VAR );\
	}\
}
///////////////////////////////////////////////////////////////////////////////
#define MAP_GET_FLOAT_VEC( MAP, KEY, SIZE, VEC ){\
	if ( MAP_HAS( MAP, KEY) ) {\
		SCRIPT_GET_FLOAT_VECP( MAP[KEY], SIZE, VEC );\
	}\
}
///////////////////////////////////////////////////////////////////////////////
#define MAP_GET_INT_VEC( MAP, KEY, SIZE, VEC ){\
	if ( MAP_HAS( MAP, KEY) ) {\
		SCRIPT_GET_INT_VECP( MAP[KEY], SIZE, VEC );\
	}\
}
///////////////////////////////////////////////////////////////////////////////
#endif


