#include "MooseAPI.h"

#include <cassert>
#include "MooseAIScript.h"
#include "MooseVector3.h"
#include "MooseGameObject.h"
#include "MooseLogger.h"
#include "MooseMilkshapeLoader.h"
#include "MooseModelHelper.h"
#include "MooseOGLRenderer.h"
#include "MooseDefaultEntities.h"
#include "MooseCollision.h"
#include "MooseExceptions.h"
#include <string>

#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseSDLScreen.h"
#include <SDL/SDL.h>
#endif
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace Moose::Math;
using namespace Moose::Core;
using namespace Moose::AI;
using namespace Moose::Scene;
using namespace Moose::Graphics;
using namespace Moose::Volume;
using namespace Moose::Exceptions;
#if !defined(MOOSE_APPLE_IPHONE)
using namespace Moose::Window;
#endif
using namespace Moose::Collision;
#if !defined(MOOSE_APPLE_IPHONE)
///////////////////////////////////////////////////////////////////////////////
//
// We use regular functions instead of static member functions to reduce dependencies.
// 
SCRIPT_CMD_DECL( BroadcastMessage); ///!< This one sends message to ALL registered objects in a scene.
SCRIPT_CMD_DECL( SendMessage ); ///<! This one takes care of message passing between same script
SCRIPT_CMD_DECL( SendMessageToObj ); ///<! This one takes care of message passing between to separate objects
SCRIPT_CMD_DECL( GetObjectName );  ///!< Returns name of gameobject where script is ran.
////////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_DECL( GetObjVar );    /// Retrieves a global variable from an object.
SCRIPT_CMD_DECL( SetObjVar );    /// Assings a global variable of an object.
SCRIPT_CMD_DECL( GetPosition );  /// Returns world position from transform
SCRIPT_CMD_DECL( GetRotation ); /// Returns world rotation from transform.

SCRIPT_CMD_DECL( GetRotationLocal ); /// Returns local rotation from transform.
SCRIPT_CMD_DECL( SetRotationLocal );  /// Sets rotation quaternion in local coordinates.
SCRIPT_CMD_DECL( SetPositionLocal );
SCRIPT_CMD_DECL( GetPositionLocal );  /// Returns local position from transform
SCRIPT_CMD_DECL( GetObjectPosition );  /// Returns position of a gameobject.
SCRIPT_CMD_DECL( GetForwardVector );   /// Returns direction of forward vector in world coordinates.
SCRIPT_CMD_DECL( GetRightVector );   /// Returns direction of right vector in world coordinates.
SCRIPT_CMD_DECL( GetUpVector );   /// Returns direction of up vector in world coordinates.
SCRIPT_CMD_DECL( Move );
SCRIPT_CMD_DECL( SetPosition );
SCRIPT_CMD_DECL( MoveLocal );
SCRIPT_CMD_DECL( CreateModelFromFile );    /// Attempts to Load a model from given file, and if successful, manage it.
SCRIPT_CMD_DECL( LoadTexture2D );  /// Attempts to Load a texture from given file and manage it.
SCRIPT_CMD_DECL( LoadTextureCube ); // attempts to create a cube texture from given textures and manage it.
//SCRIPT_CMD_DECL( SetModelTexture ); /// Assigns texture to renderable model by resource name.
//SCRIPT_CMD_DECL( ResetModelTexture ); /// Assigns texture to renderable model by resource name.
SCRIPT_CMD_DECL( UseModel );          /// Assigns model handle according to name.

SCRIPT_CMD_DECL( ComputeBoundingSphere );
SCRIPT_CMD_DECL( ComputeBoundingBox );
SCRIPT_CMD_DECL( UseBoxCollider );
SCRIPT_CMD_DECL( UseSphereCollider );
SCRIPT_CMD_DECL( GetModelVertices );
SCRIPT_CMD_DECL( GetModelTexCoords );
SCRIPT_CMD_DECL( GetModelIndices );
SCRIPT_CMD_DECL( SetModelVertices );
SCRIPT_CMD_DECL( SetModelTexCoords );
SCRIPT_CMD_DECL( SetModelIndices );
SCRIPT_CMD_DECL( DuplicateModel );
SCRIPT_CMD_DECL( LoadModelFile);
SCRIPT_CMD_DECL( CreateModel ); // Creates a model from currently loaded data.
SCRIPT_CMD_DECL( CreateEmptyModel ); // Creates an Empty model

SCRIPT_CMD_DECL( CreateVertexResource ); // Creates vertex resource from current model data
SCRIPT_CMD_DECL( CreateTexCoordResource ); // Creates texcoord resource from current model data
SCRIPT_CMD_DECL( CreateNormalResource ); // Creates normal resource from current model data
SCRIPT_CMD_DECL( CreateIndexResource );// Creates normal resource from current model data
SCRIPT_CMD_DECL( CreateInterleavedResource );// Creates interleaved array resource from current model data

SCRIPT_CMD_DECL( GetNumLods ); // Returns number of LODs in this gameObject.
SCRIPT_CMD_DECL( GetRenderables ); // Returns a list of renderable ids from given lod.
SCRIPT_CMD_DECL( SetRenderState ); // Sets renderstate for a renderable in a lod.
SCRIPT_CMD_DECL( SetVisible );     // sets gameobject visibility.
SCRIPT_CMD_DECL( SetEnabled );     // sets gameobject enabled or disabled.

SCRIPT_CMD_DECL( RotationFromAxisAngles ); // returns Quaternion formed from rotations over axis angles x,y,z.
SCRIPT_CMD_DECL( GetScreenParams );        // returns Screen parameters currently stored as a key-value list.
SCRIPT_CMD_DECL( LoadShader );             // Loads shader from file(s) and manages it under shadermanager.
SCRIPT_CMD_DECL( GetObjectPlane );
SCRIPT_CMD_DECL( RayPlaneCollision );      // returns point where given ray and plane collide.
///////////////////////////////////////////////////////////////////////////////
/// User versions are slightly different from what is used in here.
#ifdef CREATE_CMD
#undef CREATE_CMD
#endif
#ifdef CREATE_CMD_PTR
#undef CREATE_CMD_PTR
#endif
///////////////////////////////////////////////////////////////////////////////
// Macro for creating commands. FUNC must be an existing static function
// declared with SCRIPT_CMD_DECL and implemented using SCRIPT_CMD_IMPL.
#define CREATE_CMD(  FUNC ) {                                           \
	Tcl_Command c = Tcl_CreateObjCommand( m_pInterp, #FUNC,             \
                                          FUNC, (ClientData)m_pEntity,	\
                                          NULL);                        \
    assert( c!= NULL );                                                 \
}
/////////////////////////////////////////////////////////////////
/// Same as CREATE_CMD, but with alternate data passed as client parameter.
#define CREATE_CMD_PTR(  FUNC, OBJPTR) {                            \
	Tcl_Command c =Tcl_CreateObjCommand( m_pInterp, #FUNC,          \
                                         FUNC, (ClientData)OBJPTR,	\
                                         NULL);                     \
    assert(c != NULL );                                             \
}
#endif // MOOSE_APPLE_IPHONE

/////////////////////////////////////////////////////////////////
Moose::AI::CAIObject::CAIObject(Moose::Scene::CGameObject *pObj) : m_pAIScript(NULL), m_pGameObject(pObj)
{
  
}
/////////////////////////////////////////////////////////////////
Moose::AI::CAIObject::~CAIObject()
{
#if !defined(MOOSE_APPLE_IPHONE)
	delete m_pAIScript;
#endif
}
/////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::LoadScript()
{
	if ( !m_pAIScript && !m_strScriptFile.empty() )
	{
		m_pAIScript = new CAIScript( this, m_strScriptFile.c_str() );
		RegisterCommands();
		RegisterUserCommands();
		ReloadScript();
	}
}
///////////////////////////////////////////////////////////////////
Tcl_Interp *
Moose::AI::CAIObject::GetInterp()
{
	if ( m_pAIScript )
		return m_pAIScript->m_pInterp;
	else return NULL;
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::SetScript( const char *szScript )
{
	m_strScriptFile = szScript;
}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::HasCommand( const char *szName )
{
	if ( m_pAIScript ) return m_pAIScript->HasCommand( szName );
	else return false;
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::ReloadScript()
{
	if ( m_pAIScript )
		m_pAIScript->ReloadScript(m_strScriptFile.c_str());

}

#endif // MOOSE_APPLE_IPHONE
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::UpdateScript( float fSeconds )
{
#if !defined(MOOSE_APPLE_IPHONE)
	if ( m_pAIScript) m_pAIScript->Update(fSeconds);
#endif
    m_MessageQueue.Update();
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::SetPassedTime( float fSeconds )
{
	m_fPassedTime = fSeconds;

}
/////////////////////////////////////////////////////////////////
float
Moose::AI::CAIObject::GetPassedTime() const
{
	return m_fPassedTime;
}
Moose::AI::CMessageQueue & 
Moose::AI::CAIObject::GetMessageQueue()
{
    return m_MessageQueue;
}
////////////////////////////////////////////////////////////////////////////////
#if defined(MOOSE_APPLE_IPHONE)
void 
Moose::AI::CAIObject::EnqueueMessage( Moose::AI::CMessage *pMsg, const Moose::Core::CTimeStamp & time )
{
    m_MessageQueue.EnqueueMessage(pMsg, time);
}
#endif
////////////////////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
const std::string &
Moose::AI::CAIObject::GetScript() const
{
	return m_strScriptFile;
}
////////////////////////////////////////////////////////////////////////////////
Moose::AI::CAIScript::CAIScript( CAIObject *pEntity, const char *szScript )
{
  Tcl_FindExecutable("/usr/bin/tclsh");
  m_pEntity = pEntity;
  m_pInterp = Tcl_CreateInterp();

  assert( m_pInterp != NULL );
  assert( m_pEntity != NULL );

  m_fPassedTime = 0.0f;

}
///////////////////////////////////////////////////////////////////////////////
void
Moose::AI::CAIObject::RegisterCommands()
{
	if ( m_pAIScript )
	{
		m_pAIScript->RegisterCommands();
	}
}
///////////////////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIScript::HasCommand( const char *szName )
{
	Tcl_CmdInfo info;
	if ( Tcl_GetCommandInfo( m_pInterp, szName, &info ) == 0 ) return false;
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::AI::CAIScript::RegisterCommands()
{
	CREATE_CMD( SendMessage );
  CREATE_CMD( SendMessageToObj );
  CREATE_CMD( GetObjectName );
  CREATE_CMD( GetObjVar   );
  CREATE_CMD( SetObjVar   );

  CREATE_CMD( CreateModelFromFile );
  CREATE_CMD( LoadTexture2D );
  CREATE_CMD( LoadTextureCube );
  CREATE_CMD( UseModel );
  //CREATE_CMD( SetModelTexture );
  //CREATE_CMD( ResetModelTexture );
  CREATE_CMD( GetScreenParams );
  CREATE_CMD( ComputeBoundingBox );
  CREATE_CMD( ComputeBoundingSphere );

  CREATE_CMD( GetModelVertices );
  CREATE_CMD( GetModelTexCoords );
  CREATE_CMD( GetModelIndices );
  CREATE_CMD( SetModelVertices );
  CREATE_CMD( SetModelTexCoords );
  CREATE_CMD( SetModelIndices );
  CREATE_CMD( DuplicateModel );
  CREATE_CMD( LoadModelFile );
  CREATE_CMD( CreateEmptyModel );
  CREATE_CMD( CreateModel );
  CREATE_CMD( RotationFromAxisAngles );
  CREATE_CMD( CreateVertexResource ); // Creates vertex resource from current model data
  CREATE_CMD( CreateTexCoordResource ); // Creates texcoord resource from current model data
  CREATE_CMD( CreateNormalResource ) // Creates normal resource from current model data
  CREATE_CMD( CreateIndexResource );// Creates normal resource from current model data
  CREATE_CMD( CreateInterleavedResource );// Creates interleaved array resource from current model data
  CREATE_CMD( BroadcastMessage );
  CREATE_CMD( LoadShader );
  CREATE_CMD( GetObjectPosition );
  CREATE_CMD( RayPlaneCollision );
  // Register commands with GameObject clientData param
  if ( m_pEntity->GetGameObject() != NULL )
  {
    CREATE_CMD_PTR( GetPosition, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( GetRotation, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( GetRotationLocal, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( SetRotationLocal, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( Move, m_pEntity->GetGameObject()  );
    CREATE_CMD_PTR( SetPosition, m_pEntity->GetGameObject()  );

    CREATE_CMD_PTR( GetPositionLocal, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( SetPositionLocal, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( MoveLocal, m_pEntity->GetGameObject()  );
		CREATE_CMD_PTR( SetPositionLocal, m_pEntity->GetGameObject()  );
		CREATE_CMD_PTR( UseBoxCollider, m_pEntity->GetGameObject()  );
		CREATE_CMD_PTR( UseSphereCollider, m_pEntity->GetGameObject()  );
    CREATE_CMD_PTR( GetNumLods, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( GetRenderables, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( SetRenderState, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( SetVisible, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( SetEnabled, m_pEntity->GetGameObject() );
    CREATE_CMD_PTR( GetForwardVector, m_pEntity->GetGameObject() );   /// Returns direction of forward vector in world coordinates.
    CREATE_CMD_PTR( GetRightVector,   m_pEntity->GetGameObject() );   /// Returns direction of right vector in world coordinates.
    CREATE_CMD_PTR( GetUpVector,      m_pEntity->GetGameObject() );   /// Returns direction of up in world coordinates.
    CREATE_CMD_PTR( GetObjectPlane, m_pEntity->GetGameObject());
  }
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::AI::CAIScript::Update(  float fSeconds  )
{
  if ( !m_pEntity) return;
  m_pEntity->SetPassedTime(fSeconds);

  if ( Tcl_Eval(m_pInterp, "Update") != TCL_OK )
  {
    g_Error << "No Update{} found or error : "
						<< m_pEntity->GetScript() << ":" << m_pInterp->errorLine << " "
						<< Tcl_GetStringResult(m_pInterp) << std::endl;
  }

  // Handle all enqueued proc calls
  while( !m_lstProcCalls.empty()  )
  {
    ScriptMessage * pProcCall= m_lstProcCalls.front();

    if ( (Tcl_Eval( m_pInterp, pProcCall->msg.c_str() ) != TCL_OK) &&
    		 (pProcCall->bRequireReceiver == true) )
    {
      g_Error << this << " says : Either proc is missing or error in " 
	      << pProcCall->msg << " : "
	      << Tcl_GetStringResult( m_pInterp) << endl;

    }
    delete pProcCall;
    m_lstProcCalls.pop_front();
  }
  
}
/////////////////////////////////////////////////////////////////
Moose::AI::CAIScript::~CAIScript()
{
	if ( m_pInterp )
		Tcl_DeleteInterp( m_pInterp );
}
/////////////////////////////////////////////////////////////////
void
Moose::AI::CAIScript::ReloadScript( const char * szScript )
{

	if ( strlen( szScript ) == 0 ) return;

	int status = Tcl_EvalFile(m_pInterp, szScript);
	if ( status != TCL_OK )
	{
	  g_Error << "Error in script  "
						<< m_pEntity->GetScript()<< ":" << m_pInterp->errorLine << " "
						<< Tcl_GetStringResult(m_pInterp) << std::endl;
	  return;
	}

	if ( Tcl_Eval(m_pInterp, "Start") != TCL_OK )
	{
	  g_Error << "No Start{} found or error, "
						<< m_pEntity->GetScript()<< ":" << m_pInterp->errorLine << " "
	  				<< Tcl_GetStringResult(m_pInterp) << std::endl;
	}
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::AI::CAIScript::EnqueueMessage( const std::string & msg, bool bRequireReceiver )
{
  m_lstProcCalls.push_back( new ScriptMessage(msg, bRequireReceiver) );
}
////////////////////////////////////////////////////////////////////////////////
Tcl_Obj *
Moose::AI::CAIScript::GetGlobalVar( const std::string & name)
{
  return  Tcl_GetVar2Ex( m_pInterp, name.c_str(), 
			 NULL,  TCL_LEAVE_ERR_MSG | TCL_GLOBAL_ONLY );
}
////////////////////////////////////////////////////////////////////////////////
bool 
Moose::AI::CAIScript::SetGlobalVar( const std::string & name, Tcl_Obj *pValue )
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
Moose::AI::CAIScript::PrintResult()
{
  g_Error << this << "Says: : " << Tcl_GetStringResult(m_pInterp) << endl;
}
////////////////////////////////////////////////////////////////////////////////
Tcl_Obj *
Moose::AI::CAIObject::GetGlobalVar( const std::string & name)
{
  if ( m_pAIScript ) return m_pAIScript->GetGlobalVar(name);
  else		     return NULL;
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::CopyGlobalVarToResult( Tcl_Interp *pInterp, const std::string & varName )
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
Moose::AI::CAIObject::SetGlobalVar( const std::string &varName, Tcl_Obj *pVar )
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
Moose::AI::CAIObject::EnqueueMessage( const std::string &msg, bool bRequireReceiver )
{
	if ( m_pAIScript )
		m_pAIScript->EnqueueMessage( msg, bRequireReceiver );
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CGameObject * 
Moose::AI::CAIObject::GetGameObject()
{
  return m_pGameObject;
}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::GetGlobalVar( const std::string & varName, bool & value )
{
	int val;
	if ( m_pAIScript->GetGlobalVar( varName, val ) == false)
	{
		m_pAIScript->PrintResult();
		return false;
	}

	value = val;
	return true;

}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::GetGlobalVar( const std::string & varName, int & value )
{
	if ( m_pAIScript->GetGlobalVar( varName, value ) == false)
	{
		m_pAIScript->PrintResult();
		return false;
	}
	return true;
}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::GetGlobalVar( const std::string & varName, float & value )
{
	if ( m_pAIScript->GetGlobalVar( varName, value ) == false)
		{
			m_pAIScript->PrintResult();
			return false;
		}
	return true;
}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIObject::GetGlobalVar( const std::string & varName, std::string & value )
{
	if ( m_pAIScript->GetGlobalVar( varName, value ) == false)
		{
			m_pAIScript->PrintResult();
			return false;
		}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIScript::GetGlobalVar( const std::string & varName, int & value )
{
	Tcl_Obj * pTmp = GetGlobalVar( varName );
	if ( pTmp  == NULL ) return false;
	return Tcl_GetIntFromObj( m_pInterp, pTmp, &value ) == TCL_OK ? true : false;
}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIScript::GetGlobalVar( const std::string & varName, float & value )
{
	Tcl_Obj * pTmp = GetGlobalVar( varName );
	if ( pTmp  == NULL ) return false;
	double dVal;

	if ( Tcl_GetDoubleFromObj( m_pInterp, pTmp, &dVal) == TCL_OK )
	{
		value = dVal;
		return true;
	}
	else return false;

}
/////////////////////////////////////////////////////////////////
bool
Moose::AI::CAIScript::GetGlobalVar( const std::string & varName, std::string & value )
{
	Tcl_Obj * pTmp = GetGlobalVar( varName );
	if ( pTmp  == NULL ) return false;
	int length;
	const char *szString = Tcl_GetStringFromObj(pTmp, &length);

	if ( szString == NULL ) return false;
	else value = szString;

	return true;
}
////////////////////////////////////////////////////////////////////////////////
// static int
// Moose::AI::CAIScript::Accelerate( ClientData clientData, Tcl_Interp *pInterp, int objc, Tcl_Obj * CONST objv[])
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
  const CVector3<float> & vTmp = pThis->GetWorldTransform().GetTranslation();

  Tcl_Obj *pValues[3];
  pValues[0] = Tcl_NewDoubleObj( vTmp[0] );
  pValues[1] = Tcl_NewDoubleObj( vTmp[1] );
  pValues[2] = Tcl_NewDoubleObj( vTmp[2] );

  Tcl_Obj *pVec = Tcl_NewListObj( 3, pValues );
  Tcl_FreeResult( pInterp );
  Tcl_SetObjResult( pInterp, pVec );

  return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetPositionLocal )
{
  CGameObject *pThis = reinterpret_cast<CGameObject *>(clientData);
  const CVector3<float> & vTmp = pThis->GetLocalTransform().GetTranslation();

  Tcl_Obj *pValues[3];
  pValues[0] = Tcl_NewDoubleObj( vTmp[0] );
  pValues[1] = Tcl_NewDoubleObj( vTmp[1] );
  pValues[2] = Tcl_NewDoubleObj( vTmp[2] );
  
  Tcl_Obj *pVec = Tcl_NewListObj( 3, pValues );
  Tcl_FreeResult( pInterp );
  Tcl_SetObjResult( pInterp, pVec );

  return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateModel )
{
	CHECK_ARGS(1, "loaderParamList {resourceName {dataLoadingFlags} groupName storeDataInterleaved }");
	int length = 0;

	Tcl_Obj * pResource;
	Tcl_Obj * pFlags;
	Tcl_Obj * pGroupName;
	Tcl_Obj * pInterleaved;

	if ( (Tcl_ListObjLength( pInterp, objv[1], &length) != TCL_OK  ||
			length != 4)  )
	{
		Tcl_WrongNumArgs(pInterp, 0, objv, "loaderParamList {resourceName {dataLoadingFlags} groupName storeDataInterleaved }");
		return TCL_ERROR;
	}


	Tcl_ListObjIndex( pInterp, objv[1], 0, &pResource);
	Tcl_ListObjIndex( pInterp, objv[1], 1, &pFlags );
	Tcl_ListObjIndex( pInterp, objv[1], 2, &pGroupName );
	Tcl_ListObjIndex( pInterp, objv[1], 3, &pInterleaved );

	const char *szResource = Tcl_GetString( pResource );


	////////////////////
	// flags
	int iFlags = 0;

	Tcl_Obj ** pListObj     = NULL;
	if ( Tcl_ListObjGetElements(pInterp, pFlags, &length, &pListObj) != TCL_OK )
	{
		SCRIPT_ERROR( Tcl_GetStringResult(pInterp));
	}
	// Collect given int values as a single ORed int value.
	for( int i=0;i<length;i++)
	{
		int tmpVal;
		Tcl_GetIntFromObj( pInterp, pListObj[i], &tmpVal );
		iFlags |= tmpVal;
	}
	pListObj = NULL;
	////////////////////
	// Collect group name
	const char *szGroupName = Tcl_GetStringFromObj( pGroupName, &length );
	if ( length == 0 ) szGroupName = NULL;

	// interleaved flags
	int       bInterleaved = 0;
	Tcl_GetIntFromObj(pInterp, pInterleaved, &bInterleaved );

	CModel *pModel = g_ModelHelper->CreateModel( iFlags, szGroupName, bInterleaved );
	if ( g_ModelMgr->Create( pModel, szResource) != 0 )
	{
		ostringstream s;
		s << "Could not create resource " << szResource;
		SCRIPT_ERROR( s.str().c_str() );
	}

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateModelFromFile )
{
	CHECK_ARGS(1, "loaderParamList {fileName resourceName {dataLoadingFlags} groupName storeDataInterleaved }");
	int length = 0;
	Tcl_Obj * pFilename;
	Tcl_Obj * pResource;
	Tcl_Obj * pFlags;
	Tcl_Obj * pGroupName;
	Tcl_Obj * pInterleaved;

	if ( (Tcl_ListObjLength( pInterp, objv[1], &length) != TCL_OK  ||
			length != 5)  )
	{
		Tcl_WrongNumArgs(pInterp, 0, objv, "loaderParamList {fileName resourceName {dataLoadingFlags} groupName storeDataInterleaved }");
		return TCL_ERROR;
	}

	Tcl_ListObjIndex( pInterp, objv[1], 0, &pFilename);
	Tcl_ListObjIndex( pInterp, objv[1], 1, &pResource);
	Tcl_ListObjIndex( pInterp, objv[1], 2, &pFlags );
	Tcl_ListObjIndex( pInterp, objv[1], 3, &pGroupName );
	Tcl_ListObjIndex( pInterp, objv[1], 4, &pInterleaved );

	const char *szFilename = Tcl_GetString( pFilename );
	const char *szResource = Tcl_GetString( pResource );


	////////////////////
	// flags
	int iFlags = 0;

	Tcl_Obj ** pListObj     = NULL;
	if ( Tcl_ListObjGetElements(pInterp, pFlags, &length, &pListObj) != TCL_OK )
	{
		SCRIPT_ERROR( Tcl_GetStringResult(pInterp));
	}
	// Collect given int values as a single ORed int value.
	for( int i=0;i<length;i++)
	{
		int tmpVal;
		Tcl_GetIntFromObj( pInterp, pListObj[i], &tmpVal );
		iFlags |= tmpVal;
	}
	pListObj = NULL;
	////////////////////
	// group name

	// Collect group names into array
	const char *szGroupName = Tcl_GetStringFromObj( pGroupName, &length );
	if ( length == 0 ) szGroupName = NULL;

	// interleaved flags
	int       bInterleaved = 0;
	Tcl_GetIntFromObj(pInterp, pInterleaved, &bInterleaved );
    try 
    {
        g_ModelHelper->Load(szFilename);
    }
	catch ( CMooseException & ex ) {
		SCRIPT_ERROR(ex.what());
	}
	//g_Error << "Creating resource '" << szResource << "'" << endl;
	//g_Error << "Group name is '" << (szGroupName == NULL ? "NULL" : szGroupName ) << "'" << endl;
	//g_Error << "Use interleaved " << bInterleaved << endl;
	//g_Error << "Flags are : " << iFlags << endl;
	CModel *pModel = g_ModelHelper->CreateModel( iFlags, szGroupName, bInterleaved );
	if ( g_ModelMgr->Create( pModel, szResource) != 0 )
	{
		ostringstream s;
		s << "Could not create resource " << szResource;
		SCRIPT_ERROR( s.str().c_str() );
	}

	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( LoadTexture2D )
{
	CHECK_ARGS( 2, "textureFile resourceName");
	int len = 0;
	int len_tex = 0;
	const char *szTextureFile = Tcl_GetStringFromObj( objv[1], &len_tex);
	const char *szResourceName = Tcl_GetStringFromObj( objv[2], &len );
	COglRenderer tmp;
	COglTexture *pTex = NULL;
	//g_Log << "Loading textures '" << &szTextureFile[len_tex-4] << "'" << endl;
	if ( strncasecmp( &szTextureFile[len_tex-4], ".dds", 4) == 0 )
	{

		pTex= tmp.CreateCompressedTexture( szTextureFile, TEXTURE_2D);
	}
	else
	{
		//g_Log << "Loading regular textures" << endl;
		pTex =tmp.CreateTexture( szTextureFile, TEXTURE_2D);
	}

	if (pTex == NULL )
	{
		ostringstream tmp;
		tmp << "No such texture file '" << szTextureFile << "'";
		SCRIPT_ERROR( tmp.str().c_str());
	}

	if ( g_TextureMgr->Create(pTex, szResourceName) )
	{
		delete pTex;
		ostringstream tmp;
		tmp << "Cannot create resource '" << szResourceName << "'";
		SCRIPT_ERROR(tmp.str().c_str());
	}
	//else g_Log << "created texture resource '" << szResourceName << "'" << endl;
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( LoadTextureCube )
{
	CHECK_ARGS( 2, "cubeTexParams resourceName");
	int len = 0;
	NameObjMap mapNameObj;
	char *files[6] = { NULL };
	if ( ParseKeyValueMap(pInterp, mapNameObj, objv[1]) != TCL_OK) return TCL_ERROR;
	if ( MAP_HAS(mapNameObj, ".left")    ) files[0] = Tcl_GetString(mapNameObj[".left"]);
	if ( MAP_HAS(mapNameObj, ".right") ) files[1] = Tcl_GetString(mapNameObj[".right"]);;
	if ( MAP_HAS(mapNameObj, ".top")  ) files[2] = Tcl_GetString(mapNameObj[".top"]);
	if ( MAP_HAS(mapNameObj, ".bottom")   ) files[3] = Tcl_GetString(mapNameObj[".bottom"]);
	if ( MAP_HAS(mapNameObj, ".back")   ) files[4] = Tcl_GetString(mapNameObj[".back"]);
	if ( MAP_HAS(mapNameObj, ".front")  ) files[5] = Tcl_GetString(mapNameObj[".front"]);

	const char *szResourceName = Tcl_GetStringFromObj( objv[2], &len );
	COglRenderer tmp;
	COglTexture *pTex = NULL;
	//g_Log << "Loading textures '" << &szTextureFile[len_tex-4] << "'" << endl;
	if ( files[0] == NULL ) 	SCRIPT_ERROR(".left texture is invalid.");
	if ( files[1] == NULL ) 	SCRIPT_ERROR(".right texture is invalid.");
	if ( files[2] == NULL ) 	SCRIPT_ERROR(".top texture is invalid.");
	if ( files[3] == NULL ) 	SCRIPT_ERROR(".bottom texture is invalid.");
	if ( files[4] == NULL ) 	SCRIPT_ERROR(".back texture is invalid.");
	if ( files[5] == NULL ) 	SCRIPT_ERROR(".front texture is invalid.");

	pTex= tmp.CreateCubeTexture( (const char **)files );

	if (pTex == NULL )
	{
		ostringstream tmp;
		tmp << "Could not create cube texture '" << szResourceName<< "'. Problem with TGA files?";
		SCRIPT_ERROR( tmp.str().c_str());
	}

	if ( g_TextureMgr->Create(pTex, szResourceName) )
	{
		delete pTex;
		ostringstream tmp;
		tmp << "Cannot create resource '" << szResourceName << "'";
		SCRIPT_ERROR(tmp.str().c_str());
	}

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
int
ParseRay( Tcl_Interp *pInterp, NameObjMap & rayParam, CRay & ray )
{
	bool bHasDir = false;
	bool bHasOrigin = false;
	if ( MAP_HAS(rayParam, ".dir") )
	{
		CVector3<float> vTmp;
		SCRIPT_GET_FLOAT_VECP( rayParam[".dir"], 3, vTmp );
		ray.SetDirection(vTmp);
		bHasDir = true;
	}
	if ( MAP_HAS(rayParam, ".origin") )
	{
		CVector3<float> vTmp;
		SCRIPT_GET_FLOAT_VECP( rayParam[".origin"],3, vTmp );
		ray.SetPosition(vTmp);
		bHasOrigin = true;
	}
	if ( (bHasOrigin && bHasDir) == false ) return TCL_ERROR;
	else return TCL_OK;
}
/////////////////////////////////////////////////////////////////
int
ParseRenderState( Tcl_Interp *pInterp, Tcl_Obj *pRenderStateList,
									std::map<string,Tcl_Obj *> & mapNameObj,
									std::map<string,Tcl_Obj *> & mapMaterial)
{
	if ( ParseKeyValueMap(pInterp, mapNameObj, pRenderStateList) != TCL_OK ) return TCL_ERROR;
	if ( MAP_HAS(mapNameObj, std::string(".material")) )
	{
		if ( ParseKeyValueMap( pInterp, mapMaterial, mapNameObj[string(".material")] ) != TCL_OK ) return TCL_ERROR;
	}
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
void PrintMap( std::map<string,Tcl_Obj *> & map)
{
	std::map<string,Tcl_Obj *>::iterator it = map.begin();
	for( ; it != map.end(); it++ )
	{
		g_Log << it->first << " = " << Tcl_GetString(it->second) << endl;
	}

}
/////////////////////////////////////////////////////////////////
/// Parses the renderstate list and passes proper values into given renderstate.
int
ApplyRenderState( Tcl_Interp *pInterp,
									std::map<string,Tcl_Obj *> & mapNameObj,
									std::map<string, Tcl_Obj *> & mapMaterial,
									CRenderState & state )
{

	using std::string;
	CVector4<float> vColor;
	if ( ! mapMaterial.empty() )
	{
		CMaterial & mat = state.GetMaterial();
		if ( MAP_HAS(mapMaterial, string(".diffuse") ) )
		{
			SCRIPT_GET_FLOAT_VECP( mapMaterial[string(".diffuse")], 4, vColor );
			mat.SetDiffuse( vColor );
		}
		if ( MAP_HAS(mapMaterial, string(".ambient") ) )
		{
			SCRIPT_GET_FLOAT_VECP( mapMaterial[string(".ambient")], 4, vColor );
			mat.SetAmbient( vColor );
		}
		if ( MAP_HAS(mapMaterial, string(".emission") ) )
		{
			SCRIPT_GET_FLOAT_VECP( mapMaterial[string(".emission")], 4, vColor );
			mat.SetEmission( vColor );
		}
		if ( MAP_HAS(mapMaterial, string(".shininess") ) )
		{
			float fTmp;
			SCRIPT_GET_FLOATP( mapMaterial[string(".shininess")], fTmp );
			mat.SetShininess( fTmp );
		}

	}
	int bValue;

	if ( MAP_HAS( mapNameObj, string(".useLighting")) )
	{
		// Int param to bool works properly only via this hoop
		SCRIPT_GET_INTP( mapNameObj[string(".useLighting")], bValue);
		state.GetLighting() = bValue;
	}
	if ( MAP_HAS( mapNameObj, string(".depthTest")) )
	{
		// Int param to bool works properly only via this hoop
		SCRIPT_GET_INTP( mapNameObj[string(".depthTest")], bValue);
		state.GetDepthTest() = bValue;
	}
	if ( MAP_HAS( mapNameObj, string(".depthWrite")) )
	{
		// Int param to bool works properly only via this hoop
		SCRIPT_GET_INTP( mapNameObj[string(".depthWrite")], bValue);
		state.GetDepthWrite() = bValue;
	}
	if ( MAP_HAS( mapNameObj, string(".faceCulling")) )
	{
		// Int param to bool works properly only via this hoop
		SCRIPT_GET_INTP( mapNameObj[string(".faceCulling")], bValue);
		state.GetFaceCulling() = bValue;
	}
	// ability to map all textures with syntax.tex[0...MAX_TEXTURES-1]  textureResourceName
	for( size_t i=0;i<TEXTURE_HANDLE_COUNT;i++)
	{
		ostringstream s;
		s << ".tex" << i;
		if ( MAP_HAS( mapNameObj, s.str()) )
		{
			state.GetTextureHandle(i) = SCRIPT_GET_STRP( mapNameObj[s.str()] );
		}
	}
	// some helpful reminders
	if ( MAP_HAS( mapNameObj, string(".diffuse")) )
	{
		state.GetTextureHandle(0) = SCRIPT_GET_STRP( mapNameObj[string(".diffuse")] );
	}
	if ( MAP_HAS( mapNameObj, string(".bump") ) )
	{
		state.GetTextureHandle(1) = SCRIPT_GET_STRP( mapNameObj[string(".bump")] );
	}
	if ( MAP_HAS( mapNameObj, ".shader") )
	{
		state.GetShaderHandle() = SCRIPT_GET_STRP( mapNameObj[".shader"] );
	}

    if ( MAP_HAS( mapNameObj, ".color" ) )
    {
      SCRIPT_GET_FLOAT_VECP( mapNameObj[string(".color")], 4, vColor );
      state.GetBaseColor()[0] = vColor[0] * 255;
      state.GetBaseColor()[1] = vColor[1] * 255;
      state.GetBaseColor()[2] = vColor[2] * 255;
      state.GetBaseColor()[3] = vColor[3] * 255;
    }
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
// TODO Finish this missing parameters from renderstate.
SCRIPT_CMD_IMPL( UseModel )
{
	CHECK_ARGS(4, "modelName lod id renderState");
	const char *szModelname = Tcl_GetString(objv[1]);
	int lod = 0;
	SCRIPT_GET_INT( 2, lod );
	int id = 0;
	SCRIPT_GET_INT( 3, id );

    if ( g_ModelMgr->HasResource( szModelname ) == false )
    {
        ostringstream s;
        s << "There is no such model '" << szModelname << "'";
        SCRIPT_ERROR(s.str().c_str());
    }
	CGameObject *pGameObject = reinterpret_cast<CAIObject *>(clientData)->GetGameObject();
	CRenderableModelShared *pRenderable = pGameObject->AddRenderableModel( szModelname, lod, &pGameObject->GetWorldTransform() );
	pRenderable->SetId(id);
	std::map<string,Tcl_Obj *> mapNameObj;
	std::map<string,Tcl_Obj *> mapMaterial;
	if ( ParseRenderState( pInterp, objv[4], mapNameObj, mapMaterial ) != TCL_OK ) return TCL_ERROR;
	return ApplyRenderState( pInterp, mapNameObj, mapMaterial, pRenderable->GetRenderState() );
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( Move )
{

	CHECK_ARGS(1, "{x y z}" );
	CVector3<float> vFwd;
	SCRIPT_GET_FLOAT_VEC(1, 3, vFwd );

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
  //SCRIPT_GET_FLOATP( 1, fX );
	//SCRIPT_GET_FLOATP( 2, fY );
	//SCRIPT_GET_FLOATP( 3, fZ );

	// Rotate given vector so that it is applied via local transformation.

  RotateVector( pObj->GetWorldTransform().GetRotation().GetReverse(), vFwd );

  // Move ship
  pObj->GetLocalTransform().Move( vFwd );
  pObj->SetChanged(true);

  //pObj->AddMovement( vFwd.Length());
  Tcl_ResetResult( pInterp );
  return TCL_OK;
}
//////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( MoveLocal )
{

	CHECK_ARGS(1, "{x y z}" );
	CVector3<float> vFwd;
	SCRIPT_GET_FLOAT_VEC(1, 3, vFwd );

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
  //SCRIPT_GET_FLOATP( 1, fX );
	//SCRIPT_GET_FLOATP( 2, fY );
	//SCRIPT_GET_FLOATP( 3, fZ );

	// Rotate given vector so that it is applied via local transformation.
  //RotateVector( pObj->GetWorldTransform().GetRotation().GetReverse(), vFwd );

  // Move ship
  pObj->GetLocalTransform().Move( vFwd );
  pObj->SetChanged(true);

  //pObj->AddMovement( vFwd.Length());
  Tcl_ResetResult( pInterp );
  return TCL_OK;
}
//////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetPosition )
{
	CHECK_ARGS( 1, "{x y z}");
	CVector3<float> vParam;
	SCRIPT_GET_FLOAT_VEC( 1, 3, vParam );
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);

  CVector3<float> vMovement = vParam - pObj->GetWorldTransform().GetTranslation();
  RotateVector( pObj->GetWorldTransform().GetRotation().GetReverse(), vMovement );

  // Set ship position
  pObj->GetLocalTransform().Move( vMovement);
  pObj->SetChanged(true);


  Tcl_ResetResult( pInterp );
  return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetPositionLocal )
{
	CHECK_ARGS( 1, "{x y z}");
	CVector3<float> vParam;
	SCRIPT_GET_FLOAT_VEC( 1, 3, vParam );
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);

	//CVector3<float> vMovement = vParam - pObj->GetWorldTransform().GetTranslation();
  //RotateVector( pObj->GetWorldTransform().GetRotation().GetReverse(), vMovement );

  // Set ship position
	pObj->GetLocalTransform().SetTranslation( vParam );
  pObj->SetChanged(true);


  Tcl_ResetResult( pInterp );
  return TCL_OK;
}
//////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ComputeBoundingSphere )
{
	CHECK_ARGS( 2, "modelName sphereResourceName");
	const char *szModelName = SCRIPT_GET_STR( 1 );
	const char *szResourceName = SCRIPT_GET_STR( 2 );
	CModel *pModel = g_ModelMgr->GetResource( szModelName );
	if ( pModel == NULL )
	{
		ostringstream s;
		s << "No such model '" << szModelName << "'";
		SCRIPT_ERROR(s.str().c_str() );
	}
	CSphere *pSphere = new CSphere();
	*pSphere = Moose::Volume::CalculateBoundingSphereTight( **pModel->GetVertexHandle(),
																														**pModel->GetIndices() );
	CResourceManager<CSphere, CHandle<CSphere> >::GetInstance()->Create( pSphere, szResourceName );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ComputeBoundingBox )
{
	CHECK_ARGS( 2, "modelName boxResourceName");
	const char *szModelName = SCRIPT_GET_STR( 1 );
	const char *szResourceName = SCRIPT_GET_STR( 2 );
	CModel *pModel = g_ModelMgr->GetResource( szModelName );
	if ( pModel == NULL )
	{
		ostringstream s;
		s << "No such model '" << szModelName << "'";
		SCRIPT_ERROR( s.str().c_str() );
	}

	COrientedBox *pBox = new COrientedBox();
	*pBox = Moose::Volume::CalculateOrientedBoundingBox( **pModel->GetVertexHandle(),
																														**pModel->GetIndices() );
	CResourceManager<COrientedBox, CHandle<COrientedBox> >::GetInstance()->Create( pBox, szResourceName );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( UseBoxCollider  )
{
	CHECK_ARGS( 1, "boxResource");
	//const char *szBox = SCRIPT_GET_STR(1);

	SCRIPT_ERROR("Not implemented");

}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( UseSphereCollider )
{
	CHECK_ARGS( 1, "sphereResource");
	const char *szSphere = SCRIPT_GET_STR(1);
	CSphere *pSphere = CResourceManager<CSphere, CHandle<CSphere> >::GetInstance()->GetResource(szSphere);
	if ( pSphere == NULL ) SCRIPT_ERROR( "No such sphere. ");
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	pObj->GetBoundingSphere() = *pSphere;
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetModelVertices )
{
	CHECK_ARGS(1, "model");
	const char *szModel = SCRIPT_GET_STR(1);
	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}
	std::string tmp = g_VertexMgr->GetResourceName(pModel->GetVertexHandle());
	SCRIPT_RESULT( tmp.c_str() );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetModelTexCoords )
{
	CHECK_ARGS(1, "model texId");
	const char *szModel = SCRIPT_GET_STR(1);
	int iTexID = 0;
	SCRIPT_GET_INT(2, iTexID );

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}
	std::string tmp = g_VertexMgr->GetResourceName(pModel->GetTextureCoordinateHandle(iTexID));
	SCRIPT_RESULT( tmp.c_str() );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetModelIndices )
{
	CHECK_ARGS(1, "model texId");
	const char *szModel = SCRIPT_GET_STR(1);
	int iTexID = 0;
	SCRIPT_GET_INT(2, iTexID );

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}

	std::string tmp = g_IndexMgr->GetResourceName(pModel->GetIndices());
	SCRIPT_RESULT( tmp.c_str() );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetModelVertices )
{
	CHECK_ARGS(2, "model vertexResource");
	const char *szModel = SCRIPT_GET_STR(1);
	const char *szResource = SCRIPT_GET_STR(2);

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}


	pModel->GetVertexHandle() = szResource;
	Tcl_ResetResult( pInterp );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetModelTexCoords )
{
	CHECK_ARGS(2, "model texId texResource");
	const char *szModel = SCRIPT_GET_STR(1);
	int texId = 0;        SCRIPT_GET_INT( 2, texId );
	const char *szResource = SCRIPT_GET_STR(3);

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}

	pModel->GetTextureCoordinateHandle(texId) = szResource;
	Tcl_ResetResult( pInterp );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetModelIndices )
{
	CHECK_ARGS(2, "model indexResource");
	const char *szModel = SCRIPT_GET_STR(1);
	const char *szResource = SCRIPT_GET_STR(2);

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}

	pModel->GetIndices() = szResource;
	Tcl_ResetResult( pInterp );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( DuplicateModel )
{

	CHECK_ARGS(2, "model newResource");
	const char *szModel = SCRIPT_GET_STR(1);
	const char *szNew= SCRIPT_GET_STR(2);

	CModel *pModel = g_ModelMgr->GetResource(szModel);
	if ( pModel == NULL )
	{
		ostringstream tmp;
		tmp << "No such model " << szModel;
		SCRIPT_ERROR( tmp.str().c_str() );
	}

	CModel *pNew = new CModel( *pModel );
	g_ModelMgr->Create( pNew, szNew );
	Tcl_ResetResult( pInterp );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( LoadModelFile )
{
	CHECK_ARGS(1, "filename");
	const char *szModel = SCRIPT_GET_STR(1);
	try {
      g_ModelHelper->Load( szModel );
    }
    catch( CMooseException & ex )
	{
		SCRIPT_ERROR( ex.what() );
	}
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateEmptyModel )
{
	CHECK_ARGS(1, "resourceName");
	const char *szRes = SCRIPT_GET_STR(1);
	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_ModelMgr->Create( new CModel(), szRes ) != 0 )
	{
		SCRIPT_ERROR("Invalid resource name.");
	}
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateVertexResource )
{
	CHECK_ARGS(1, "resourceName");
	const char *szRes = SCRIPT_GET_STR(1);

	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_VertexMgr->Create( g_ModelHelper->GetModelLoader()->GetVertexArray(), szRes ) != 0 )
	{
		SCRIPT_ERROR("Failed to create vertex resource.");
	}
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateTexCoordResource )
{
	CHECK_ARGS(1, "resourceName");
	const char *szRes = SCRIPT_GET_STR(1);

	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_VertexMgr->Create( g_ModelHelper->GetModelLoader()->GetTexCoordArray(), szRes ) != 0 )
	{
		SCRIPT_ERROR("Failed to create texcoord resource.");
	}
	return TCL_OK;

}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateNormalResource )
{
	CHECK_ARGS(1, "resourceName");
	const char *szRes = SCRIPT_GET_STR(1);

	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_VertexMgr->Create( g_ModelHelper->GetModelLoader()->GetNormalArray(), szRes ) != 0 )
	{
		SCRIPT_ERROR("Failed to create normal resource.");
	}
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateIndexResource )
{
	CHECK_ARGS(2, "resourceName");

	const char *szRes = SCRIPT_GET_STR(1);
	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_IndexMgr->Create( g_ModelHelper->GetModelLoader()->GetIndexArray(), szRes ) != 0 )
	{
		ostringstream s;
		s << "Failed to create index resource '" << szRes << "' ";
		s << "from all vertices.";

		SCRIPT_ERROR(s.str().c_str());
	}
	return TCL_OK;

}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( CreateInterleavedResource )
{
	CHECK_ARGS(1, "resourceName");
	const char *szRes = SCRIPT_GET_STR(1);

	if ( szRes == NULL || strlen(szRes) == 0 )
		SCRIPT_ERROR("Invalid resource name");

	if ( g_VertexMgr->Create( g_ModelHelper->GetModelLoader()->GetInterleavedArray(), szRes ) != 0 )
	{
		SCRIPT_ERROR("Failed to create interleaved array resource.");
	}
	return TCL_OK;

}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetNumLods )
{
	CHECK_ARGS( 0, "this command accepts no arguments");
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	int i = pObj->GetNumLodLevels();
	SCRIPT_INT_RESULT(i);
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetRenderables )
{
	CHECK_ARGS( 1, "lodLevel");
	int lod = 0;
	SCRIPT_GET_INT( 1, lod );

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	RenderableList::iterator it = pObj->GetRenderableObjects(lod).begin();
	Tcl_Obj *pList = Tcl_NewListObj( 0, NULL );

	for( ; it != pObj->GetRenderableObjects(lod).end(); it++)
	{
		 if ( Tcl_ListObjAppendElement( pInterp, pList, Tcl_NewIntObj( (*it)->GetId() )) != TCL_OK )
		 {
			 return TCL_ERROR;
		 }
	}
	Tcl_SetObjResult( pInterp, pList );
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetRenderState )
{
	CHECK_ARGS( 3, "lodLevel id renderstate");
	int lod = 0;
	int id = 0;
	SCRIPT_GET_INT( 1, lod );
	SCRIPT_GET_INT( 2, id );
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( (unsigned int)lod >= pObj->GetNumLodLevels() )
	{
		ostringstream s;
		s << "Lod level " << lod << " is too high, max is : " << pObj->GetNumLodLevels();
		SCRIPT_ERROR(s.str().c_str());
	}
	RenderableList::iterator it = pObj->GetRenderableObjects(lod).begin();



	std::map<string,Tcl_Obj *> mapNameObj;
	std::map<string,Tcl_Obj *> mapMaterial;
	if ( ParseRenderState( pInterp, objv[3], mapNameObj, mapMaterial ) != TCL_OK ) return TCL_ERROR;

	bool bFoundRenderable= false;
	for( ; it != pObj->GetRenderableObjects(lod).end(); it++)
	{
		if ( (*it)->GetId() == id )
		{
			bFoundRenderable = true;
			// TODO make this more efficient
			if ( ApplyRenderState(pInterp, mapNameObj, mapMaterial, (*it)->GetRenderState()) != TCL_OK )
				return TCL_ERROR;
		}
	}
	if ( bFoundRenderable == false )
	{
		ostringstream s;
		s << "Could not find renderable with id " << id;
		SCRIPT_ERROR(s.str().c_str());
	}
	return TCL_OK;

}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetEnabled )
{
	CHECK_ARGS(1, "boolean")
	int bFlag = 0;
	SCRIPT_GET_INT(1, bFlag);
	reinterpret_cast<CGameObject *>(clientData)->SetEnabled(bFlag);
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetVisible )
{
	CHECK_ARGS(1, "boolean")
	int bFlag = 0;
	SCRIPT_GET_INT(1, bFlag);
	reinterpret_cast<CGameObject *>(clientData)->SetVisible(bFlag);
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RotationFromAxisAngles )
{
	CHECK_ARGS(1, "listAxisAngles");
	CVector3<float> vAngles;
	SCRIPT_GET_FLOAT_VECP( objv[1], 3, vAngles);
	CQuaternion q;
	q.CreateFromAxisAngles( vAngles );
	SCRIPT_RESULT_FLOAT_VECP( 4, q);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetScreenParams )
{
	Tcl_Obj *pList = Tcl_NewListObj(0, NULL );
	CSDLScreenParams & p = CSDLScreen::m_SDLScreenParams;
	SCRIPT_LIST_ADD_INT( pList, ".redBits", p.m_iRedSize);
	SCRIPT_LIST_ADD_INT( pList, ".greenBits", p.m_iGreenSize);
	SCRIPT_LIST_ADD_INT( pList, ".blueBits", p.m_iBlueSize);
	SCRIPT_LIST_ADD_INT( pList, ".doubleBuffer", p.m_bDoubleBuffer);
	SCRIPT_LIST_ADD_INT( pList, ".depthBits", p.m_iDepthBufferSize);
	SCRIPT_LIST_ADD_INT( pList, ".width", p.m_iWidth);
	SCRIPT_LIST_ADD_INT( pList, ".height", p.m_iHeight);
	SCRIPT_LIST_ADD_INT( pList, ".colorDepth", p.m_iScreenDepth);
	int bFullscreen = p.m_iVideoModeFlags & SDL_FULLSCREEN;
	SCRIPT_LIST_ADD_INT( pList, ".fullscreen", bFullscreen);
	SCRIPT_LIST_ADD_INT( pList, ".multiSampleBuffers", p.m_iMultiSampleBuffers);
	SCRIPT_LIST_ADD_INT( pList, ".multiSampleSamples", p.m_iMultiSampleSamples);
	Tcl_SetObjResult(pInterp, pList);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
struct SendMsg_f {
	const char *szMessage;
    int requireReceiver;
	inline void operator()( CGameObject * pObj )
	{
      pObj->EnqueueMessage( szMessage, requireReceiver );
	}
};
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( BroadcastMessage )
{
    CHECK_ARGS_BETWEEN(1, 2, "message [requireReceiver]");
	SendMsg_f f;
	f.szMessage = SCRIPT_GET_STR(1);

    if ( (objc - 1) == 2 ) 
    {
      SCRIPT_GET_INT(2,f.requireReceiver);
    }
    else
    {
      f.requireReceiver = 0;
    }

	g_ObjectMgr->ForEachObjectPtr( f);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( LoadShader )
{
	CHECK_ARGS(3, "vertexShaderFile fragmentShaderFile resourceName");
	const char *szVertSh = SCRIPT_GET_STR(1);
	const char *szFragSh = SCRIPT_GET_STR(2);
	const char *szResource = SCRIPT_GET_STR(3);
	COglRenderer r;
	CShader *pShader = new CShader();
    pShader->LoadVertexShader( szVertSh);
    pShader->LoadFragmentShader( szFragSh );
	if ( pShader == NULL )
	{
		ostringstream s;
		s << "Could not load shader from files vert = '" << szVertSh << "' and frag = '" << szFragSh << "'";
		SCRIPT_ERROR( s.str().c_str() );
	}

	if ( g_ShaderMgr->Create( pShader, szResource ) != 0 )
	{
		ostringstream s;
		s << "Could not register shader under name '" << szResource << "'";
		SCRIPT_ERROR( s.str().c_str() );
	}
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetObjectName )
{
	CGameObject *pObj = reinterpret_cast<CAIObject *>(clientData)->GetGameObject();
	if ( pObj )
	{
		SCRIPT_RESULT( pObj->GetName().c_str());
	}
	else SCRIPT_ERROR("Script not ran in gameobject.");
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetObjectPosition )
{
	CHECK_ARGS(1, "objectName");
	const char *szName = SCRIPT_GET_STR(1);
	CGameObject *pObj = g_ObjectMgr->GetResource( szName );
	if  ( pObj == NULL )
	{
		ostringstream s;
		s << "No such object '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	SCRIPT_RESULT_FLOAT_VECP( 3, pObj->GetWorldTransform().GetTranslation() );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetRotation )
{
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( pObj )
	{
		SCRIPT_RESULT_FLOAT_VECP( 4, pObj->GetWorldTransform().GetRotation());
	}
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetRotationLocal )
{
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( pObj == NULL ) SCRIPT_ERROR("Not a gameobject");

	SCRIPT_RESULT_FLOAT_VECP( 4, pObj->GetLocalTransform().GetRotation());

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetRotationLocal )
{
	CHECK_ARGS(1, "{ x y z w }")
	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);

	if ( pObj == NULL ) SCRIPT_ERROR("Not a gameobject.");

	CQuaternion q;
	SCRIPT_GET_FLOAT_VEC(1,4,q);
	pObj->GetLocalTransform().SetRotation( q );

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetForwardVector)
{

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( pObj == NULL ) SCRIPT_ERROR("Not a gameobject.");



	const CMatrix4x4<float> & m = pObj->GetWorldTransform().GetMatrix();
	CVector3<float> vVec( m(0,2), m(1,2), m(2,2));
	SCRIPT_RESULT_FLOAT_VECP( 3, vVec);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetRightVector)
{

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( pObj == NULL ) SCRIPT_ERROR("Not a gameobject.");



	const CMatrix4x4<float> & m = pObj->GetWorldTransform().GetMatrix();
	CVector3<float> vVec( m(0,0), m(1,0), m(2,0));
	SCRIPT_RESULT_FLOAT_VECP( 3, vVec);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetUpVector)
{

	CGameObject *pObj = reinterpret_cast<CGameObject *>(clientData);
	if ( pObj == NULL ) SCRIPT_ERROR("Not a gameobject.");

	const CMatrix4x4<float> & m = pObj->GetWorldTransform().GetMatrix();
	CVector3<float> vVec( m(0,1), m(1,1), m(2,1) );
	SCRIPT_RESULT_FLOAT_VECP( 3, vVec);

	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( GetObjectPlane )
{
	CHECK_ARGS(1,"objectName");
	const char *szName = SCRIPT_GET_STR(1);
	CGameObject *pObj = g_ObjectMgr->GetResource(szName);
	if ( pObj == NULL )
	{
		ostringstream s;
		s << "No such object '" << szName << "'";
		SCRIPT_ERROR(s.str().c_str());
	}
	/// distance = -(normal · point), and since 0,1,0 and position, we take a shortcut.
	CPlane p( 0, 1, 0, -pObj->GetWorldTransform().GetTranslation()[1]);
	SCRIPT_RESULT_FLOAT_VECP( 4, p );
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( RayPlaneCollision )
{
	CHECK_ARGS(2, "ray plane");

	CRay ray;
	NameObjMap rayParam;
	if ( ParseKeyValueMap(pInterp, rayParam, objv[1]) != TCL_OK ) return TCL_ERROR;
	if ( ParseRay( pInterp, rayParam, ray) != TCL_OK ) SCRIPT_ERROR("Invalid ray parameters.");
	CPlane plane;
	SCRIPT_GET_FLOAT_VEC( 2, 4, plane );
	CVector3<float> vPos;
	RayIntersectsPlane( plane, ray, vPos);
	SCRIPT_RESULT_FLOAT_VECP( 3, vPos);
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
#endif
