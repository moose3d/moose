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
#include <MooseSpatialGraph.h>
#include <MooseGameObject.h>
#include <string>
#include <iostream>
using namespace Moose::Spatial;
using namespace Moose::Volume;
using namespace Moose::Scene;
using namespace std;
using namespace Moose::Math;
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
  pOctree->CollectObjects( cullSphere, lstObjects, Light, Moose::Core::NOT_USED);
  CHECK_EQUAL( 3, lstObjects.size());

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Ship, Moose::Core::NOT_USED);
  CHECK_EQUAL( 3, lstObjects.size());

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Light, Moose::Core::EQUAL);
  CHECK_EQUAL( 1, lstObjects.size());
  CHECK( lstObjects.front() == pTmp );

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Light, Moose::Core::NOT_EQUAL);
  CHECK_EQUAL( 2, lstObjects.size());


  // reverse comparison
  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Ship, Moose::Core::NOT_EQUAL);
  CHECK_EQUAL( 2, lstObjects.size());


  pTmp->SetTag( Light | Ship );
  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Ship, Moose::Core::AND);
  CHECK_EQUAL( 2, lstObjects.size());

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Light, Moose::Core::AND);
  CHECK_EQUAL( 1, lstObjects.size());

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Light, Moose::Core::NOT_AND);
  CHECK_EQUAL( 2, lstObjects.size());

  lstObjects.clear();
  pOctree->CollectObjects( cullSphere, lstObjects, Ship, Moose::Core::NOT_AND);
  CHECK_EQUAL( 1, lstObjects.size());

}
