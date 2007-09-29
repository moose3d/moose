#include <UnitTest++/UnitTest++.h>
#include "PhoenixOctree.h"
#include <string>
#include <iostream>
using namespace Phoenix::Spatial;
using namespace Phoenix::Volume;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( COctree )
{
  float fTmp = 1.0;
  float fTmp2 = 2.0;
  
  CSphere sphere( CVector3<float>(30,30,30), 100.0f);
  CSphere sphere2( CVector3<float>(-30,-30,-30), 10.0f);
  COctree<float> *pOctree = new COctree<float>( 3, 100.0f);

  cerr << "edge length at 0 : " << pOctree->GetEdgeLength( 0 ) << endl;
  cerr << "edge length at 1 : " << pOctree->GetEdgeLength( 1 ) << endl;

  pOctree->InsertObject( fTmp, sphere );
  pOctree->InsertObject( fTmp2, sphere2);
  
  CHECK( pOctree->DeleteObject(  fTmp, sphere ) == 0);
  CHECK( pOctree->DeleteObject( fTmp2, sphere2) == 0);
  CHECK( pOctree->DeleteObject(  fTmp, sphere ) != 0);
  CHECK( pOctree->DeleteObject( fTmp2, sphere2) != 0);
} 


