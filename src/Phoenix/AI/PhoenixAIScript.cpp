#include <cassert>
#include "PhoenixAIScript.h"
#include "PhoenixVector3.h"
#include "PhoenixGameObject.h"
#include "PhoenixLogger.h"
#include "PhoenixMilkshapeLoader.h"
#include "PhoenixModelHelper.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixDefaultEntities.h"
////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace Phoenix::Math;
using namespace Phoenix::Core;
using namespace Phoenix::AI;
using namespace Phoenix::Scene;
using namespace Phoenix::Graphics;
////////////////////////////////////////////////////////////////////////////////
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
SCRIPT_CMD_DECL( CreateModelFromFile );    /// Attempts to Load a model from given file, and if successful, manage it.
SCRIPT_CMD_DECL( LoadTexture2D );  /// Attempts to Load a texture from given file and manage it.
SCRIPT_CMD_DECL( SetModelTexture ); /// Assigns texture to renderable model by resource name.
SCRIPT_CMD_DECL( ResetModelTexture ); /// Assigns texture to renderable model by resource name.
SCRIPT_CMD_DECL( UseModel );          /// Assigns model handle according to name.
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
#define CREATE_CMD(  FUNC ) {				\
	assert(Tcl_CreateObjCommand( m_pInterp, #FUNC,		\
				     FUNC, (ClientData)m_pEntity,	\
				     NULL) != NULL );		\
}
/////////////////////////////////////////////////////////////////
/// Same as CREATE_CMD, but with alternate data passed as client parameter.
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
Tcl_Interp *
Phoenix::AI::CAIObject::GetInterp()
{
	if ( m_pAIScript )
		return m_pAIScript->m_pInterp;
	else return NULL;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::SetScript( const char *szScript )
{
	m_strScriptFile = szScript;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CAIObject::LoadScript()
{
	if ( !m_pAIScript && !m_strScriptFile.empty() )
	{
		m_pAIScript = new CAIScript( this, m_strScriptFile.c_str() );
		RegisterUserCommands();
		ReloadScript();
	}
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
Phoenix::AI::CAIObject::UpdateScript( float fSeconds )
{
	if ( m_pAIScript) m_pAIScript->Update(fSeconds);
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
  CREATE_CMD( CreateModelFromFile );
  CREATE_CMD( LoadTexture2D );
  CREATE_CMD( UseModel );
  CREATE_CMD( SetModelTexture );
  CREATE_CMD( ResetModelTexture );
  m_fPassedTime = 0.0f;
  ////////////////////////////////////////////////////////////////

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

	if ( strlen( szScript ) == 0 ) return;

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
Phoenix::AI::CAIScript::EnqueueMessage( const std::string & msg )
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
Phoenix::AI::CAIObject::EnqueueMessage( const std::string &msg )
{
	if ( m_pAIScript )
		m_pAIScript->EnqueueMessage( msg );
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject * 
Phoenix::AI::CAIObject::GetGameObject()
{
  return m_pGameObject;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::AI::CAIObject::GetGlobalVar( const std::string & varName, bool & value )
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
Phoenix::AI::CAIObject::GetGlobalVar( const std::string & varName, int & value )
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
Phoenix::AI::CAIObject::GetGlobalVar( const std::string & varName, float & value )
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
Phoenix::AI::CAIObject::GetGlobalVar( const std::string & varName, std::string & value )
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
Phoenix::AI::CAIScript::GetGlobalVar( const std::string & varName, int & value )
{
	Tcl_Obj * pTmp = GetGlobalVar( varName );
	if ( pTmp  == NULL ) return false;
	return Tcl_GetIntFromObj( m_pInterp, pTmp, &value ) == TCL_OK ? true : false;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::AI::CAIScript::GetGlobalVar( const std::string & varName, float & value )
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
Phoenix::AI::CAIScript::GetGlobalVar( const std::string & varName, std::string & value )
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
		g_Error << Tcl_GetStringResult(pInterp) << endl;
		return TCL_ERROR;
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

	if ( Phoenix::Data::CMilkshapeLoader::IsMilkshapeFile(szFilename ) )
	{
		if ( g_ModelHelper->LoadMilkshapeData(szFilename) != 0 )
		{
			g_Error << "There was an error while loading milkshape file " << szFilename << endl;
			return TCL_ERROR;
		}
	}
	else if ( g_ModelHelper->LoadObjData( szFilename) ) // this succeeds even though it ain't obj file.
	{
		g_Error << "There was an error while loading obj file" << szFilename << endl;
		return TCL_ERROR;
	}
	g_Error << "Creating resource '" << szResource << "'" << endl;
	g_Error << "Group name is '" << (szGroupName == NULL ? "NULL" : szGroupName ) << "'" << endl;
	g_Error << "Use interleaved " << bInterleaved << endl;
	g_Error << "Flags are : " << iFlags << endl;
	CModel *pModel = g_ModelHelper->CreateModel( iFlags, szGroupName, bInterleaved );
	if ( g_ModelMgr->Create( pModel, szResource) != 0 ) return TCL_ERROR;

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
	g_Log << "Loading textures '" << &szTextureFile[len_tex-4] << "'" << endl;
	if ( strncasecmp( &szTextureFile[len_tex-4], ".dds", 4) == 0 )
	{

		pTex= tmp.CreateCompressedTexture( szTextureFile, TEXTURE_2D);
	}
	else
	{
		g_Log << "Loading regular textures" << endl;
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
	else g_Log << "created texture resource '" << szResourceName << "'" << endl;
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( UseModel )
{
	CHECK_ARGS(3, "modelName lod renderState");
	const char *szModelname = Tcl_GetString(objv[1]);
	int lod = 0;
	SCRIPT_GET_INT( 2, lod );
	std::map<string,string> mapValueList;
	SCRIPT_PARSE_VALUE_MAP( mapValueList, "=", 3);
	CGameObject *pGameObject = reinterpret_cast<CAIObject *>(clientData)->GetGameObject();
	CRenderableModelShared *pRenderable = pGameObject->AddRenderableModel( szModelname, lod, true, &pGameObject->GetWorldTransform() );
	pRenderable->GetRenderState().ParseFrom(mapValueList);
	return TCL_OK;
}
/////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( SetModelTexture )
{
	CHECK_ARGS(3, "modelName textureId textureName");
	const char *szModelName = SCRIPT_GET_STR(1);
	const char *szTexName   = SCRIPT_GET_STR(3);
	int texId = 0;

	SCRIPT_GET_INT( 2, texId);
	CModel *pModel = g_ModelMgr->GetResource(szModelName);
	ostringstream tmp;
	tmp << "No such model " << szModelName;
	if ( ! pModel ) SCRIPT_ERROR( tmp.str().c_str() );

	pModel->GetTextureHandle(texId) = szTexName;
	if ( pModel->GetTextureHandle(texId).IsNull() )
	{
		g_Warn << "Setting texture to null (if you really want this,"
					 << " use command ResetModelTexture. Null texture was '" << szTexName << "'" << endl;
	}
	return TCL_OK;
}
///////////////////////////////////////////////////////////////////////////////
SCRIPT_CMD_IMPL( ResetModelTexture )
{
	CHECK_ARGS(2, "modelName textureId");
	const char *szModelName = SCRIPT_GET_STR(1);
	int texId = 0;
	SCRIPT_GET_INT( 2, texId);

	CModel *pModel = g_ModelMgr->GetResource(szModelName);
	if ( ! pModel ) SCRIPT_ERROR("No such model");

	pModel->GetTextureCoordinateHandle(texId).Release();

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
