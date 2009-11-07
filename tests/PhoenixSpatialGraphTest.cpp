#include <UnitTest++/UnitTest++.h>
#include <PhoenixSpatialGraph.h>
#include <PhoenixGameObject.h>
#include <string>
#include <iostream>
using namespace Phoenix::Spatial;
using namespace Phoenix::Volume;
using namespace Phoenix::Scene;
using namespace std;
/////////////////////////////////////////////////////////////////
/// Test collecting with various tags.
TEST( CollectObjects )
{

  std::list<CGameObject *> lstObjects;
  enum TAGS
  {
    Light = 1,
    Ship = 2
  };
  CSpatialGraph *pOctree = new CSpatialGraph( 4, 200.0f);
  
  CGameObject * pSome = new CGameObject();
  pSome->GetBoundingSphere() = CSphere(CVector3<float>(0,0,20), 1.0f);
  pSome->SetTag( 0 );


  CGameObject * pTmp = new CGameObject();
  pTmp->GetBoundingSphere() = CSphere(CVector3<float>(20,0,0), 1.0f);
  pTmp->SetTag( Light );

  CGameObject * pTmp2 = new CGameObject();
  pTmp2->GetBoundingSphere() = CSphere( CVector3<float>(-20, 0,0), 2.0f);
  pTmp2->SetTag( Ship ) ;

  pOctree->InsertObject(pTmp);
  pOctree->InsertObject(pTmp2);
  pOctree->InsertObject(pSome);

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
