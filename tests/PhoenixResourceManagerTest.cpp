#include <UnitTest++/UnitTest++.h>
#include "PhoenixCore.h"
#include "PhoenixResourceManager.h"
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
typedef CResourceManager<int, CHandle<int> > CIntResourceMgr;
/////////////////////////////////////////////////////////////////
TEST( PhoenixResourceManager )
{

  CIntResourceMgr *pMgr = CIntResourceMgr::GetInstance();
  CIntResourceMgr *pMgr2 = CIntResourceMgr::GetInstance();
  CHECK_EQUAL( pMgr, pMgr2 );

}
/////////////////////////////////////////////////////////////////
TEST( PhoenixHandle )
{
  CHandle<int> handle;
  CHECK( handle.IsNull() != 0 );
  handle.Initialize(4);
  CHECK( handle.IsNull() == 0 );
  handle.Nullify();
  CHECK( handle.IsNull() != 0 );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixResourceManager_Create )
{

  CIntResourceMgr *pMgr = CIntResourceMgr::GetInstance();
  CHandle<int> handle;
  int iResource = 4;
  CHandle<int> handleNotFound;
  CHandle<int> handleNotFound2;
  handleNotFound2.Initialize(65);
  ////////////////////
  std::string strResName("resourcetest");
  std::string strSearch("resourcetest");
  std::string strSearchNotFound("thisIsNotHere");
  ////////////////////
  CHECK_EQUAL( 0, pMgr->Create( &iResource, strResName, handle ));
  CHECK( pMgr->GetResource( handle ) == &iResource );
  CHECK( pMgr->GetResource( strSearch) ==  &iResource );
  CHECK( pMgr->GetResource( strSearchNotFound) ==  NULL );
  CHECK( pMgr->GetResource( handleNotFound ) == NULL );
  CHECK( pMgr->GetResource( handleNotFound2 ) == NULL );
}
/////////////////////////////////////////////////////////////////
