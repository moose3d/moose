#include "../src/PhoenixCollision.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Collision;
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_Origo)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,0,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,0,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_Not_Origo)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_Not_Origo_Reverse)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_BehindPlane)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,-1,0);
  CVector3<float> vLineEnd(3, -3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_FrontOfPlane)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,-1,0);
  CVector3<float> vLineEnd(3, -3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_LineInPlane)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,3,-1);
  CVector3<float> vLineEnd(3, -3,-1);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,0,1), vPlanePoint);
  ////////////////////
  /// 3,3.000,-1
  /// 3,-3.000,-1
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,-0.001
  /// 3,-3.000,-0.001
  vLineStart += CVector3<float>(0,0,0.999f);
  vLineEnd += CVector3<float>(0,0,0.999f);  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

  ////////////////////
  /// 3,3.000,0.000
  /// 3,-3.000,0.000
  vLineStart = CVector3<float>(3,3,0);
  vLineEnd   = CVector3<float>(3,-3,0);  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

  ////////////////////
  /// 3,3.000,0.001
  /// 3,-3.000,0.001
  vLineStart += CVector3<float>(0,0,0.001f);
  vLineEnd += CVector3<float>(0,0,0.001f);  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,1.000
  /// 3,-3.000,1.000
  vLineStart += CVector3<float>(0,0,0.999f);
  vLineEnd += CVector3<float>(0,0,0.999f);  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_LineLength_YAxis)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,4,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  CVector3<float> vPlanePoint(0,2.0f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.001,0
  /// 3,2.001,0
  vLineStart += CVector3<float>(0,-0.999f,0);
  vLineEnd += CVector3<float>(0,-0.999f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,0
  /// 3,2.000,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.999,0
  /// 3,1.999,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.998,0
  /// 3,1.998,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.001,0
  /// 3,1.001,0
  vLineStart += CVector3<float>(0,-0.997f,0);
  vLineEnd += CVector3<float>(0,-0.997f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.000,0
  /// 3,1.000,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,1.999,0
  /// 3,0.999,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,1.000,0
  /// 3,0.000,0
  vLineStart += CVector3<float>(0,-0.999f,0);
  vLineEnd += CVector3<float>(0,-0.999f,0);
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_LineLength_ZAxis)
{
  
  CPlane plane;
  CLine line;

  CVector3<float> vLineStart(3,0,4);
  CVector3<float> vLineEnd(3, 0,3);
  CVector3<float> vLinePlaneCollisionPointReal(3,0,2);
  CVector3<float> vLinePlaneCollisionPointResult;
  CVector3<float> vPlanePoint(0,0,2.0f);
  plane.Calculate( CVector3<float>(0,0,-1), vPlanePoint);
  
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,3.001
  /// 3,0,2.001
  vLineStart[2] -= 0.999f;
  vLineEnd[2] -= 0.999f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,3.000
  /// 3,0,2.000
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.999
  /// 3,0,1.999
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.998
  /// 3,0,1.998
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.001
  /// 3,0,1.001
  vLineStart[2] -= 0.997f;
  vLineEnd[2] -= 0.997f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.000
  /// 3,0,1.000
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( POINT_IN_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,1.999
  /// 3,0,0.999
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,1.000
  /// 3,0,0.000
  vLineStart[2] -= 0.999f;
  vLineEnd[2] -= 0.999f;
  line.SetStart( vLineStart );
  line.SetEnd( vLineEnd );
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
}
/////////////////////////////////////////////////////////////////
