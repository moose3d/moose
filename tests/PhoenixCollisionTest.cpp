#include "../src/PhoenixCollision.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Collision;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
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
TEST(ClosestPointOnRay)
{
  CRay ray;
  ray.SetPosition( 0,0,0 );
  ray.SetDirection( 0,1,0);

  CVector3<float> vPoint( 1,0,0);
  CVector3<float> vReal( 0,0,0);
  CVector3<float> vResult;
  
  vResult = ClosestPointOnRay( vPoint, ray);
  CHECK_ARRAY_CLOSE( vReal.GetArray(), vResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(PlaneIntersectsBox)
{

  CVector3<float> vNormal(0,1,0);
  CVector3<float> vPoint( 0,0,0);
  CVector3<float> vUp(0,1,0), vRight(1,0,0), vFwd(0,0,-1);
  // Create plane
  CPlane plane;
  plane.Calculate( vNormal, vPoint );
  // Create OBB
  COrientedBox box;
  box.SetPosition(0,5.0f,0);
  box.SetOrientation( vUp, vFwd, vRight );
  box.SetWidth(1.0f);
  box.SetHeight(2.0f);
  box.SetLength(3.0f);
  box.CalculateCorners();
  box.CalculatePlanes();
  
  CHECK( PlaneIntersectsBox( plane, box ) == 0 );

  box.SetPosition(0,1.001f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 0 );

  box.SetPosition(0,1.000f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,0.999f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,0.000f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-0.999f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-1.000f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-1.001f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 0 );
  
  box.SetPosition(0,-5.000f,0);
  box.CalculateCorners();
  box.CalculatePlanes();
  CHECK( PlaneIntersectsBox( plane, box ) == 0 );
}
/////////////////////////////////////////////////////////////////
TEST(TriangleIntersectsOBB_AABB_Like)
{

  COrientedBox box;
  CVector3<float> vTriangle[3];
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);
  ////////////////////
  /// Check along Y-axis
  vTriangle[0][0] = -0.5f;
  vTriangle[0][1] = 2.0f;
  vTriangle[0][2] = 0.0f;

  vTriangle[1][0] = 0.5f;
  vTriangle[1][1] = 2.0f;
  vTriangle[1][2] = 0.0f;

  vTriangle[2][0] = 0.5f;
  vTriangle[2][1] = 3.0f;
  vTriangle[2][2] = 0.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.999f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.001f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-2.999f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-0.5f,-2.0f,0.0f);
  vTriangle[1] = CVector3<float>( 0.5f,-2.0f,0.0f);
  vTriangle[2] = CVector3<float>( 0.5f,-1.0f,0.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.001f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-5.00f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  ////////////////////
  /// Check along X-axis 
  vTriangle[0][0] = -3.0f;
  vTriangle[0][1] = -0.5f;
  vTriangle[0][2] = 0.0f;

  vTriangle[1][0] = -2.0f;
  vTriangle[1][1] = -0.5f;
  vTriangle[1][2] =  0.0f;

  vTriangle[2][0] = -2.0f;
  vTriangle[2][1] =  0.5f;
  vTriangle[2][2] =  0.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.999f,0, 0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.001f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(2.999f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>( 1.0f,-0.5f,0.0f);
  vTriangle[1] = CVector3<float>( 2.0f,-0.5f,0.0f);
  vTriangle[2] = CVector3<float>( 2.0f, 0.5f,0.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.001f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(5.00f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  ////////////////////
  /// Check along Z-axis 
  vTriangle[0][0] =  0.0f;
  vTriangle[0][1] = -0.5f;
  vTriangle[0][2] = -3.0f;

  vTriangle[1][0] =  0.0f;
  vTriangle[1][1] = -0.5f;
  vTriangle[1][2] = -2.0f;
  
  vTriangle[2][0] =  0.0f;
  vTriangle[2][1] =  0.5f;
  vTriangle[2][2] = -2.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0, 0, 0.999f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,0.001f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,2.999f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>( 0.0f,-0.5f, 1.0f);
  vTriangle[1] = CVector3<float>( 0.0f,-0.5f, 2.0f);
  vTriangle[2] = CVector3<float>( 0.0f, 0.5f, 2.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,0.001f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,5.00f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
}
/////////////////////////////////////////////////////////////////
TEST(TriangleIntersectsOBB_AABB_Like_Miss)
{

  COrientedBox box;
  CVector3<float> vTriangle[3];
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);
  ////////////////////
  /// Check along Y-axis
  vTriangle[0][0] = -0.5f;
  vTriangle[0][1] = 2.0f;
  vTriangle[0][2] = 5.0f;

  vTriangle[1][0] = 0.5f;
  vTriangle[1][1] = 2.0f;
  vTriangle[1][2] = 5.0f;

  vTriangle[2][0] = 0.5f;
  vTriangle[2][1] = 3.0f;
  vTriangle[2][2] = 5.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.999f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.001f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-2.999f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-0.5f,-2.0f,5.0f);
  vTriangle[1] = CVector3<float>( 0.5f,-2.0f,5.0f);
  vTriangle[2] = CVector3<float>( 0.5f,-1.0f,5.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-0.001f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,-5.00f,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  ////////////////////
  /// Check along X-axis 
  vTriangle[0][0] = -3.0f;
  vTriangle[0][1] = -0.5f;
  vTriangle[0][2] = 5.0f;

  vTriangle[1][0] = -2.0f;
  vTriangle[1][1] = -0.5f;
  vTriangle[1][2] =  5.0f;

  vTriangle[2][0] = -2.0f;
  vTriangle[2][1] =  0.5f;
  vTriangle[2][2] =  5.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.999f,0, 0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.001f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(2.999f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>( 1.0f,-0.5f,5.0f);
  vTriangle[1] = CVector3<float>( 2.0f,-0.5f,5.0f);
  vTriangle[2] = CVector3<float>( 2.0f, 0.5f,5.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0.001f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(5.00f,0,0);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  ////////////////////
  /// Check along Z-axis 
  vTriangle[0][0] =  5.0f;
  vTriangle[0][1] = -0.5f;
  vTriangle[0][2] = -3.0f;

  vTriangle[1][0] =  5.0f;
  vTriangle[1][1] = -0.5f;
  vTriangle[1][2] = -2.0f;
  
  vTriangle[2][0] =  5.0f;
  vTriangle[2][1] =  0.5f;
  vTriangle[2][2] = -2.0f;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0, 0, 0.999f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,0.001f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
  
  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,2.999f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>( 5.0f,-0.5f, 1.0f);
  vTriangle[1] = CVector3<float>( 5.0f,-0.5f, 2.0f);
  vTriangle[2] = CVector3<float>( 5.0f, 0.5f, 2.0f);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,0.001f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  for(int i=0;i<3;i++)
  {
    vTriangle[i] += CVector3<float>(0,0,5.00f);
  }
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
}
/////////////////////////////////////////////////////////////////
