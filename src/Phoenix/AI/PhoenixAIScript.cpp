#include <cassert>
#include "PhoenixAIScript.h"
#include "PhoenixVector3.h"
#include "PhoenixGameObject.h"
#include "PhoenixLogger.h"
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace Phoenix::Math;
using namespace Phoenix::Core;
using namespace Phoenix::AI;
using namespace Phoenix::Scene;
////////////////////////////////////////////////////////////////////////////////
#define CHECK_ARGS( ARGC, USAGE_MSG ) {              \
	if ( (objc-1) != ARGC ) {                              \
		Tcl_WrongNumArgs(pInterp, 0, objv, USAGE_MSG);   \
		return TCL_ERROR;                                \
	}                                                  \
}
///////////////////////////////////////////////////////////////////////////////
#define CHECK_ARGS_BETWEEN( ARGC_MIN, ARGC_MAX, USAGE_MSG ) {              \
	if ( (objc - 1) < ARGC_MIN || (objc - 1) > ARGC_MAX ) {                              \
		Tcl_WrongNumArgs(pInterp, 0, objv, USAGE_MSG);   \
		return TCL_ERROR;                                \
	}                                                  \
}
////////////////////////////////////////////////////////////////////////////////
#define SCRIPT_CMD_DECL( NAME ) int NAME( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
#define SCRIPT_CMD_IMPL( NAME ) int NAME( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
///////////////////////////////////////////////////////////////////////////////
//
// We use regular functions instead of static member functions to reduce dependencies.
// 
SCRIPT_CMD_DECL( SendMessage ); ///<! This one takes care of message passing between same script
SCRIPT_CMD_DECL( SendMessageToObj ); ///<! This one takes care of message passing between to separate objects
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_DECL( GetObjVar );    /// Retrieves a global variable from an object.
SCRIPT_CMD_DECL( SetObjVar );    /// Assings a global variable of an object.
SCRIPT_CMD_DECL( GetPosition );  /// Returns local position from transform
/////////////////////////////////////////////////////////////////
// Macro for creating commands. FUNC must be an existing static function
// declared with SCRIPT_CMD_DECL and implemented using SCRIPT_CMD_IMPL.
#define CREATE_CMD(  FUNC ) {				\
	assert(Tcl_CreateObjCommand( m_pInterp, #FUNC,		\
				     FUNC, (ClientData)m_pEntity,	\
				     NULL) != NULL );		\
}
/////////////////////////////////////////////////////////////////
#define CREATE_CMD_PTR(  FUNC, OBJPTR) {			\
	assert(Tcl_CreateObjCommand( m_pInterp, #FUNC,		\
				     FUNC, (ClientData)OBJPTR,	\
				     NULL) != NULL );		\
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CAIObject::CAIObject(Phoenix::Scene::CGameObject *pObj) : m_pAIScript(NULL), m_pGameObject(pObj)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CAIObject::~CAIObject()
{

}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::LoadScript( const char *szScript )
{
	if ( szScript )
		m_pAIScript = new CAIScript( this, szScript);
	m_strScriptFile = szScript;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::ReloadScript()
{
	if ( m_pAIScript )
		m_pAIScript->ReloadScript(m_strScriptFile.c_str());

}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::SetPassedTime( float fSeconds )
{
	m_fPassedTime = fSeconds;

}
/////////////////////////////////////////////////////////////////
float
Phoenix::AI::CAIObject::GetPassedTime() const
{
	return m_fPassedTime;
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::AI::CAIScript::CAIScript( CAIObject *pEntity, const char *szScript )
{
  m_pEntity = pEntity;
  m_pInterp = Tcl_CreateInterp();
  assert( m_pInterp != NULL );
  assert( m_pEntity != NULL );

  CREATE_CMD( SendMessage );
  CREATE_CMD( SendMessageToObj );

  CREATE_CMD( GetObjVar   );
  CREATE_CMD( SetObjVar   );
  CREATE_CMD_PTR( GetPosition, pEntity->GetGameObject() );
  m_fPassedTime = 0.0f;
  ////////////////////////////////////////////////////////////////
  ReloadScript( szScript );
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIScript::Update(  float fSeconds  )
{
  if ( !m_pEntity) return;
  m_pEntity->SetPassedTime(fSeconds);

  if ( Tcl_Eval(m_pInterp, "Update") != TCL_OK )
  {
    g_Error << "CAIScript says: No Update{} found or error, " 
	    << Tcl_GetStringResult(m_pInterp) << std::endl;
  }

  // Handle all enqueued proc calls
  while( !m_lstProcCalls.empty()  )
  {
    std::string & strProcName = m_lstProcCalls.front(); 

    if ( Tcl_Eval( m_pInterp, strProcName.c_str() ) != TCL_OK )
    {
      g_Error << this << " says : Either proc is missing or error in " 
	      << strProcName << " : " 
	      << Tcl_GetStringResult( m_pInterp) << endl;

    }
    
    m_lstProcCalls.pop_front();
  }
  
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CAIScript::~CAIScript()
{
	if ( m_pInterp )
		Tcl_DeleteInterp( m_pInterp );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIScript::ReloadScript( const char * szScript )
{
	int status = Tcl_EvalFile(m_pInterp, szScript);
	if ( status != TCL_OK )
	{
	  g_Error << "CAIScript says:  Error in script, " << Tcl_GetStringResult(m_pInterp) << std::endl;
	  return;
	}

	if ( Tcl_Eval(m_pInterp, "Start") != TCL_OK )
	{
	  g_Error << "CAIScript says: No Start{} found or error, " << Tcl_GetStringResult(m_pInterp) << std::endl;
	}
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIScript::EnqueueMessage( std::string & msg )
{
  m_lstProcCalls.push_back(msg);
}
////////////////////////////////////////////////////////////////////////////////
Tcl_Obj *
Phoenix::AI::CAIScript::GetGlobalVar( const std::string & name)
{
  return  Tcl_GetVar2Ex( m_pInterp, name.c_str(), 
			 NULL,  TCL_LEAVE_ERR_MSG | TCL_GLOBAL_ONLY );
}
////////////////////////////////////////////////////////////////////////////////
bool 
Phoenix::AI::CAIScript::SetGlobalVar( const std::string & name, Tcl_Obj *pValue )
{
  if ( Tcl_SetVar2Ex(m_pInterp, name.c_str(), NULL, 
		     pValue, TCL_LEAVE_ERR_MSG | TCL_GLOBAL_ONLY ) == NULL )
  {
    return false;
  }
  else return true;
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIScript::PrintResult()
{
  g_Error << this << "Says: : " << Tcl_GetStringResult(m_pInterp) << endl;
}
////////////////////////////////////////////////////////////////////////////////
bool
Phoenix::AI::CAIObject::CopyGlobalVarToResult( Tcl_Interp *pInterp, const std::string & varName )
{
  Tcl_Obj *pObj =  m_pAIScript->GetGlobalVar( varName ) ;
  
  if ( pObj == NULL ) 
  {
    m_pAIScript->PrintResult();
    Tcl_ResetResult( pInterp );
    return false;
  }
  else
  {
    Tcl_SetObjResult( pInterp, pObj);
    return true;
  }

}
////////////////////////////////////////////////////////////////////////////////
bool
Phoenix::AI::CAIObject::SetGlobalVar( const std::string &varName, Tcl_Obj *pVar )
{
  if ( m_pAIScript->SetGlobalVar( varName, pVar ) == false)
  {
    m_pAIScript->PrintResult();
    return false;
  }
  return true;
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::EnqueueMessage( std::string &msg )
{
  m_pAIScript->EnqueueMessage( msg );
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject * 
Phoenix::AI::CAIObject::GetGameObject()
{
  return m_pGameObject;
}
////////////////////////////////////////////////////////////////////////////////
// static int
// Phoenix::AI::CAIScript::Accelerate( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
SCRIPT_CMD_IMPL( SendMessage )
{
  CHECK_ARGS( 1, "message" );
  
  int iLen = 0;
  std::string strMsg(Tcl_GetStringFromObj( objv[1], &iLen ));
  reinterpret_cast<CAIObject *>(clientData)->EnqueueMessage(strMsg);
  
  Tcl_ResetResult( pInterp );

  return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SendMessageToObj )
{
  CHECK_ARGS( 2, "objName message");  
  int iLen = 0;
  
  const char * strObjName( Tcl_GetStringFromObj( objv[1], &iLen ));
  std::string strMsg( Tcl_GetStringFromObj( objv[2], &iLen ));
  CGameObject *pObj = g_ObjectMgr->GetResource( strObjName);
  
  if ( pObj ) 
    pObj->EnqueueMessage(strMsg);
  else
  {
    cerr << reinterpret_cast<CAIObject *>(clientData) << "says : No such object '" << strObjName << "' found."  << endl;
    Tcl_ResetResult( pInterp );    
    return TCL_ERROR;
  }
  Tcl_ResetResult( pInterp );    

  return TCL_OK;
}

////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetObjVar )
{
  CHECK_ARGS( 2, "objName varName" );
  int iLen = 0;
  const char *szObjName = Tcl_GetStringFromObj( objv[1], &iLen );
  const char *szVarName = Tcl_GetStringFromObj( objv[2], &iLen );
  CGameObject *pObj = g_ObjectMgr->GetResource( szObjName );
  
  // this sets appropriate result and errors.
  if ( (pObj == NULL) || 
       (pObj->CopyGlobalVarToResult( pInterp, std::string(szVarName) ) == false) )
  {
    return TCL_ERROR;
  }
  
  return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetObjVar )
{
  CHECK_ARGS( 3, "objName varName value" );
  int iLen = 0;
  const char *szObjName = Tcl_GetStringFromObj( objv[1], &iLen );
  const char *szVarName = Tcl_GetStringFromObj( objv[2], &iLen );
  
  CGameObject *pObj = g_ObjectMgr->GetResource( szObjName );
  
  if ( pObj == NULL) 
  {
    g_Error << "No such object '" << szObjName << "'" << endl;
    return TCL_ERROR;
  }

  if ( pObj->SetGlobalVar( szVarName, objv[3]) == false)
  {
    return TCL_ERROR;
  }

  return TCL_OK;
}
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetPosition )
{
  CGameObject *pThis = reinterpret_cast<CGameObject *>(clientData);
  const CVector3<float> & vTmp = pThis->GetLocalTransform().GetTranslation();

  Tcl_Obj *pValues[3];
  pValues[0] = Tcl_NewDoubleObj( vTmp[0] );
  pValues[1] = Tcl_NewDoubleObj( vTmp[1] );
  pValues[2] = Tcl_NewDoubleObj( vTmp[2] );
  
  Tcl_Obj *pVec = Tcl_NewListObj( 0, pValues );
  Tcl_FreeResult( pInterp );
  Tcl_SetObjResult( pInterp, pVec );

  return TCL_OK;
}
/////////////////////////////////////////////////////////////////
// void
// Phoenix::AI::CAIObject::AddProperty( IProperty * pProp )
// {
//   m_Properties[pProp->GetName()] = pProp;
// }
// ////////////////////////////////////////////////////////////////////////////////
// void
// Phoenix::AI::CAIObject::RemoveProperty( const std::string & name  )
// {
//   PropertyMap::iterator it = m_Properties.find(name);
//   if ( it != m_Properties.end()) m_Properties.erase(it);
// }
// ////////////////////////////////////////////////////////////////////////////////
// IProperty * 
// Phoenix::AI::CAIObject::GetProperty( const std::string & name )
// {
//   PropertyMap::iterator it = m_Properties.find(name);
  
//   if ( it != m_Properties.end() ) return it->second;
//   else				  return NULL;
// }
// ////////////////////////////////////////////////////////////////////////////////
// bool
// Phoenix::AI::CAIObject::HasProperty( const std::string & name )
// {
//   return (m_Properties.find(name) != m_Properties.end());
// }
////////////////////////////////////////////////////////////////////////////////
// SCRIPT_CMD_IMPL( SetFloatProperty )
// {
//   CHECK_ARGS( 2, "propertyName floatValue" );
//   int iLen = 0;
//   const char *szPropName = Tcl_GetStringFromObj( objv[1], &iLen );
  
//   CAIObject *pAIObj = reinterpret_cast<CAIObject *>(clientData);
//   IProperty *pProp = pAIObj->GetProperty( std::string(szPropName) );
    
//   if (  pProp != NULL )
//   {
//     if ( pProp->AlterValue( pInterp, objv[2] ) == false ) 
//     {
//       return TCL_ERROR;
//     }
//     pProp->SetResult( pInterp );
//   }
//   else
//   {
//     double dTmp = 0;

//     if ( Tcl_GetDoubleFromObj(pInterp, objv[2], &dTmp) != TCL_OK )
//     {
//       return TCL_ERROR;
//     }
    
//     FloatProperty * pFloatProperty = new FloatProperty( std::string(szPropName), dTmp );
//     pAIObj->AddProperty( pFloatProperty );
//     pFloatProperty->SetResult( pInterp );
//   }

  
//   return TCL_OK;
// }

////////////////////////////////////////////////////////////////////////////////
// SCRIPT_CMD_IMPL( GetProperty )
// {
//   CHECK_ARGS( 1, "propertyName" );
//   int iLen = 0;
//   const char *szPropName = Tcl_GetStringFromObj( objv[1], &iLen );
//   // g_Log << "Asking for " << szPropName << endl;
//   CAIObject *pAIObj = reinterpret_cast<CAIObject *>(clientData);
//   IProperty *pTmp = pAIObj->GetProperty( std::string(szPropName) );

//   if ( pTmp == NULL ) 
//   {
//     Tcl_ResetResult( pInterp);
//     return TCL_ERROR;
//   }
//   else 
//   {
//     pTmp->SetResult( pInterp );
    
//     return TCL_OK;

//   }

// }
// ////////////////////////////////////////////////////////////////////////////////
// SCRIPT_CMD_IMPL( SetProperty )
// {
//   CHECK_ARGS( 2, "propertyName list " );
//   int iLen = 0;
//   const char *szPropName = Tcl_GetStringFromObj( objv[1], &iLen );
  
//   CAIObject *pAIObj = reinterpret_cast<CAIObject *>(clientData);
//   IProperty *pProp = pAIObj->GetProperty( std::string(szPropName) );
    
//   if (  pProp == NULL )
//   {
//     pProp = new CListProperty( std::string(szPropName) );    
//     pAIObj->AddProperty( pProp );
//   }

//   if ( pProp->AlterValue( pInterp, objv[2] ) == false)
//   {
//     return TCL_ERROR;
//   }

//   pProp->SetResult( pInterp );
  
//   return TCL_OK;
// }
