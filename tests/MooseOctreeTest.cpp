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
#include <MooseOctree.h>
#include <MooseGameObject.h>
#include <string>
#include <iostream>
using namespace Moose::Spatial;
using namespace Moose::Volume;
using namespace Moose::Scene;
using namespace Moose::Math;
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
