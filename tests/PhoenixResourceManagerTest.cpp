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
  int *pResource = new int;
  *pResource = 4;
  CHandle<int> handleNotFound;
  CHandle<int> handleNotFound2;
  handleNotFound2.Initialize(65);
  ////////////////////
  std::string strResName("resourcetest");
  std::string strSearch("resourcetest");
  std::string strSearchNotFound("thisIsNotHere");
  ////////////////////
  CHECK_EQUAL( 0, pMgr->Create( pResource, strResName, handle ));
  CHECK( pMgr->GetResource( handle ) == pResource );
  CHECK( pMgr->GetResource( strSearch) ==  pResource );
  CHECK( pMgr->GetResource( strSearchNotFound) ==  NULL );
  CHECK( pMgr->GetResource( handleNotFound ) == NULL );
  CHECK( pMgr->GetResource( handleNotFound2 ) == NULL );
  
  CIntResourceMgr::DestroyInstance();
  
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
TEST( PhoenixResourceManager_Delete )
{

  CIntResourceMgr *pMgr = CIntResourceMgr::GetInstance();
  CHandle<int> handle1;
  CHandle<int> handle2;
  CHandle<int> handle3;
  CHandle<int> handle4;
  CHandle<int> handle5;
  CHandle<int> handleNull;
  
  handle5.Initialize(5);
  
  int *pIntRes1 = new int;
  int *pIntRes2 = new int;
  int *pIntRes3 = new int;
  int *pIntRes4 = new int;

  *pIntRes1 = 1;
  *pIntRes2 = 2;
  *pIntRes3 = 3;
  *pIntRes4 = 4;
  
  ////////////////////
  std::string strResName;
  ////////////////////
  /// Create resources
  CHECK_EQUAL( 0, pMgr->Create( pIntRes1, (strResName = "one"), handle1 ));
  CHECK_EQUAL( 0, pMgr->Create( pIntRes2, (strResName = "two"), handle2 ));
  CHECK_EQUAL( 0, pMgr->Create( pIntRes3, (strResName = "three"), handle3 ));
  CHECK_EQUAL( 0, pMgr->Create( pIntRes4, (strResName = "four"), handle4 ));

  /// Check that resources exist
  CHECK( pMgr->GetResource( handle1 ) == pIntRes1 );
  CHECK( pMgr->GetResource( handle2 ) == pIntRes2 );
  CHECK( pMgr->GetResource( handle3 ) == pIntRes3 );
  CHECK( pMgr->GetResource( handle4 ) == pIntRes4 );
  CHECK( pMgr->GetResource( handle5 ) == NULL );
  CHECK( pMgr->GetResource( handleNull ) == NULL );

  CHECK_EQUAL( 0, handle1.GetIndex());
  CHECK_EQUAL( 1, handle2.GetIndex());
  CHECK_EQUAL( 2, handle3.GetIndex());
  CHECK_EQUAL( 3, handle4.GetIndex());
  const char *szName = "one";
  CHECK_ARRAY_EQUAL( szName, pMgr->GetResourceName( handle1 ).c_str(), 3);
  const char *szName2 = "two";
  CHECK_ARRAY_EQUAL( szName2, pMgr->GetResourceName( handle2 ).c_str(), 3);
  // Verify that they something else is not found.
  CHECK( pMgr->GetResource( (strResName = "five")) ==  NULL );
  CHECK( pMgr->GetResource( (strResName = "zero")) ==  NULL );
  CHECK( pMgr->GetResource( (strResName = "")) ==  NULL );

  pMgr->Destroy( (strResName="one") );

  CHECK( pMgr->GetResource( handle1 ) == NULL );
  CHECK( pMgr->GetResource( handle2 ) == pIntRes2 );
  CHECK( pMgr->GetResource( handle3 ) == pIntRes3 );
  CHECK( pMgr->GetResource( handle4 ) == pIntRes4 );
  CHECK( pMgr->GetResource( handle5 ) == NULL );
  CHECK( pMgr->GetResource( handleNull ) == NULL );
  CHECK( handle1.IsNull());
  CHECK_EQUAL( 1, handle2.GetIndex());
  CHECK_EQUAL( 2, handle3.GetIndex());
  CHECK_EQUAL( 0, handle4.GetIndex());
  
}
/////////////////////////////////////////////////////////////////
