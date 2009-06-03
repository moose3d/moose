#include <UnitTest++/UnitTest++.h>
#include <PhoenixOctree.h>
#include <PhoenixGameObject.h>
#include <string>
#include <iostream>
using namespace Phoenix::Spatial;
using namespace Phoenix::Volume;
using namespace Phoenix::Scene;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( COctree )
{
  float fTmp = 1.0;
  float fTmp2 = 2.0;

  CSphere sphere( CVector3<float>(30,30,30), 100.0f);
  CSphere sphere2( CVector3<float>(-30,-30,-30), 10.0f);
  COctree<float> *pOctree = new COctree<float>( 3, 100.0f);

  //cerr << "edge length at 0 : " << pOctree->GetEdgeLength( 0 ) << endl;
  //cerr << "edge length at 1 : " << pOctree->GetEdgeLength( 1 ) << endl;

  pOctree->InsertObject( fTmp, sphere );
  pOctree->InsertObject( fTmp2, sphere2);

  CHECK( pOctree->DeleteObject(  fTmp, sphere ) == 0);
  CHECK( pOctree->DeleteObject( fTmp2, sphere2) == 0);
  CHECK( pOctree->DeleteObject(  fTmp, sphere ) != 0);
  CHECK( pOctree->DeleteObject( fTmp2, sphere2) != 0);
}
/////////////////////////////////////////////////////////////////
TEST( COctree_GetIndex1D )
{

  CVector3<float> vPos(99.999,-10,0);
  CVector3<float> vPosNeg(-100.0,-10,0);
  COctree<float> *pOctree = new COctree<float>( 4, 200.0f);
  CHECK_CLOSE( 200.0f, pOctree->GetEdgeLength( 0 ), 0.001f);
  CHECK_CLOSE( 100.0f, pOctree->GetEdgeLength( 1 ), 0.001f);
  CHECK_CLOSE( 50.0f, pOctree->GetEdgeLength( 2 ), 0.001f);
  CHECK_CLOSE( 25.0f, pOctree->GetEdgeLength( 3 ), 0.001f);
  //cerr << "edge length at 1 : " << pOctree->GetEdgeLength( 1 ) << endl;

  unsigned int nDepth = 4;//pOctree->GetObjectDepth(23.56f);
  unsigned int nIndex = pOctree->GetIndex1D( nDepth, vPos);
  //cerr << "depth:" << nDepth << ", index: " << nIndex << endl;
  nIndex = pOctree->GetIndex1D( nDepth, vPosNeg);
  //cerr << "depth:" << nDepth << ", index: " << nIndex << endl;
}
/////////////////////////////////////////////////////////////////
/// Test collecting with various tags.
TEST( COctree_CollectObjects )
{

	std::list<CGameObject *> lstObjects;
	enum TAGS
	{
		Light = 1,
		Ship = 2
	};
	COctree<CGameObject *> *pOctree = new COctree<CGameObject *>( 4, 200.0f);

	CGameObject * pSome = new CGameObject();
	pSome->GetBoundingSphere() = CSphere(CVector3<float>(0,0,20), 1.0f);
	pSome->SetTag( 0 );


	CGameObject * pTmp = new CGameObject();
	pTmp->GetBoundingSphere() = CSphere(CVector3<float>(20,0,0), 1.0f);
	pTmp->SetTag( Light );

	CGameObject * pTmp2 = new CGameObject();
	pTmp2->GetBoundingSphere() = CSphere( CVector3<float>(-20, 0,0), 2.0f);
	pTmp2->SetTag( Ship ) ;

	pOctree->InsertObject(pTmp, pTmp->GetBoundingSphere());
	pOctree->InsertObject(pTmp2, pTmp2->GetBoundingSphere());
	pOctree->InsertObject(pSome, pSome->GetBoundingSphere());

	CSphere cullSphere(CVector3<float>(0,0,0),40.0f);

	pOctree->CollectObjects( cullSphere,lstObjects );
	CHECK_EQUAL( 3, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Light, Phoenix::Core::CTagged::NOT_USED);
	CHECK_EQUAL( 3, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Ship, Phoenix::Core::CTagged::NOT_USED);
	CHECK_EQUAL( 3, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Light, Phoenix::Core::CTagged::EQUAL);
	CHECK_EQUAL( 1, lstObjects.size());
	CHECK( lstObjects.front() == pTmp );

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Light, Phoenix::Core::CTagged::NOT_EQUAL);
	CHECK_EQUAL( 2, lstObjects.size());


	// reverse comparison
	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Ship, Phoenix::Core::CTagged::NOT_EQUAL);
	CHECK_EQUAL( 2, lstObjects.size());


	pTmp->SetTag( Light | Ship );
	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Ship, Phoenix::Core::CTagged::AND);
	CHECK_EQUAL( 2, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Light, Phoenix::Core::CTagged::AND);
	CHECK_EQUAL( 1, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Light, Phoenix::Core::CTagged::NOT_AND);
	CHECK_EQUAL( 2, lstObjects.size());

	lstObjects.clear();
	pOctree->CollectObjects( cullSphere, lstObjects, Ship, Phoenix::Core::CTagged::NOT_AND);
	CHECK_EQUAL( 1, lstObjects.size());
}
