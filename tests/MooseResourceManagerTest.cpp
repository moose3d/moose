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

#include <UnitTest++/UnitTest++.h>
#include <MooseCore.h>
#include <MooseResourceManager.h>
#include <MooseDefaultEntities.h>
#include <string>
#include <iostream>
#include <MooseLogger.h>
/////////////////////////////////////////////////////////////////
using namespace Moose::Core;
using namespace Moose::Default;
/////////////////////////////////////////////////////////////////
typedef CResourceManager<int, CHandle<int> > CIntResourceMgr;
#define UINT unsigned int
/////////////////////////////////////////////////////////////////
TEST( MooseResourceManager )
{

  CIntResourceMgr *pMgr = CIntResourceMgr::GetInstance();
  CIntResourceMgr *pMgr2 = CIntResourceMgr::GetInstance();
  CHECK_EQUAL( pMgr, pMgr2 );

}
/////////////////////////////////////////////////////////////////
TEST( MooseHandle )
{
  CHandle<int> handle;
  CHECK( handle.IsNull() != 0 );
  handle.Initialize(4);
  CHECK( handle.IsNull() == 0 );
  handle.Nullify();
  CHECK( handle.IsNull() != 0 );
}
/////////////////////////////////////////////////////////////////
TEST( MooseResourceManager_Create )
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
  handleNotFound2.Nullify();
}
/////////////////////////////////////////////////////////////////
TEST( MooseResourceManager_Create_NoHandle )
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
  CHECK_EQUAL( 0, pMgr->Create( pResource, strResName ));
  CHECK_EQUAL( 0, pMgr->AttachHandle( "resourcetest", handle) );

  CHECK( pMgr->GetResource( handle ) == pResource );
  CHECK( pMgr->GetResource( strSearch) ==  pResource );
  CHECK( pMgr->GetResource( strSearchNotFound) ==  NULL );
  CHECK( pMgr->GetResource( handleNotFound ) == NULL );
  CHECK( pMgr->GetResource( handleNotFound2 ) == NULL );

  CIntResourceMgr::DestroyInstance();
  handleNotFound2.Nullify();
}
/////////////////////////////////////////////////////////////////
TEST( MooseResourceManager_Delete )
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

  CHECK_EQUAL( (UINT)0, handle1.GetIndex());
  CHECK_EQUAL( (UINT)1, handle2.GetIndex());
  CHECK_EQUAL( (UINT)2, handle3.GetIndex());
  CHECK_EQUAL( (UINT)3, handle4.GetIndex());
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
  CHECK_EQUAL( (UINT)1, handle2.GetIndex());
  CHECK_EQUAL( (UINT)2, handle3.GetIndex());
  CHECK_EQUAL( (UINT)0, handle4.GetIndex());

  CIntResourceMgr::DestroyInstance();
  handle5.Nullify();
}
////////////////////////////////////////////////////////////////////////////////
struct RenamingFixture
{
  RenamingFixture() { 
    CIntResourceMgr *pMgr = CIntResourceMgr::GetInstance();
    pMgr->Create( new int(5), "five");
    pMgr->Create( new int(6), "six");
  }
  ~RenamingFixture() { 
    CIntResourceMgr::DestroyInstance();
  }
  
};
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE(RenamingFixture,  MooseResourceManager_Rename_NonExistent)
{
  
  CHandle<int> tmp = "this";
  
  CHECK_THROW( CIntResourceMgr::GetInstance()->Rename("this", "that"), Moose::Exceptions::CRenameException);
  CHECK_THROW( CIntResourceMgr::GetInstance()->Rename(tmp, "that"), Moose::Exceptions::CRenameException);
  
 
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE(RenamingFixture,  MooseResourceManager_RenameToExisting)
{
  
  CHandle<int> tmp = "this";
  
  CHECK_THROW( CIntResourceMgr::GetInstance()->Rename("five", "six"), Moose::Exceptions::CRenameException);
  CHECK_THROW( CIntResourceMgr::GetInstance()->Rename(tmp, "six"),    Moose::Exceptions::CRenameException);
 
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( RenamingFixture, MooseResourceManager_Rename_Success )
{

  try 
  {
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("five"),5);
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("six"),6);
    CIntResourceMgr::GetInstance()->Rename("five", "this");
    CIntResourceMgr::GetInstance()->Rename("six", "that");
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("this"),5);
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("that"),6);
    CHECK(CIntResourceMgr::GetInstance()->GetResource("five") == NULL);
    CHECK(CIntResourceMgr::GetInstance()->GetResource("six") == NULL);

    CIntResourceMgr::GetInstance()->Rename("this", "five");
    CIntResourceMgr::GetInstance()->Rename("that", "six");
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("five"),5);
    CHECK_EQUAL(*CIntResourceMgr::GetInstance()->GetResource("six"),6);
    CHECK(CIntResourceMgr::GetInstance()->GetResource("this") == NULL);
    CHECK(CIntResourceMgr::GetInstance()->GetResource("that") == NULL);
  } 
  catch ( const Moose::Exceptions::CRenameException & ex )
  {
    g_Error << ex.what() << "\n";
    CHECK(false);
  }
}
/////////////////////////////////////////////////////////////////
class CBaseObject : public CHandled<CBaseObject>
{
public:
	virtual ~CBaseObject(){}
};
/////////////////////////////////////////////////////////////////
typedef CResourceManager<CBaseObject, CHandle<CBaseObject> > CBaseObjectMgr;
/////////////////////////////////////////////////////////////////
class CInternal : public CBaseObject
{
public:
	CInternal()
	{

	}
	~CInternal()
	{

	}
};
/////////////////////////////////////////////////////////////////
class CExternal : public CBaseObject
{
private:
	CInternal *m_pInt;
public:
	CExternal()
	{
		CInternal *pInt = new CInternal();
		CBaseObjectMgr::GetInstance()->Create(pInt, g_UniqueName, pInt->GetObjectHandle());
		m_pInt = pInt;
	}
	~CExternal()
	{
		CBaseObjectMgr::GetInstance()->Destroy(m_pInt->GetObjectHandle());
	}
};
/////////////////////////////////////////////////////////////////
TEST( Internal_Object_Release )
{
	CExternal *pExternal = new CExternal();
	CBaseObjectMgr::GetInstance()->Create(pExternal, g_UniqueName, pExternal->GetObjectHandle());
	CHECK_EQUAL(2, CBaseObjectMgr::GetInstance()->GetSize() );
	CBaseObjectMgr::GetInstance()->Destroy( pExternal->GetObjectHandle());
	CHECK_EQUAL(0, CBaseObjectMgr::GetInstance()->GetSize() );
}
/////////////////////////////////////////////////////////////////
