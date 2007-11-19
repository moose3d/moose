#include "../src/PhoenixCollision.h"
#include "../src/PhoenixMath.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Collision;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
using std::cerr;
using std::endl;
const float SQRT_2 = 1.41421356237f;
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
  box.SetPosition(0,0,0);

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

  box.SetPosition(0,0,0);
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
TEST(TriangleIntersectsOBB_BoxRotated)
{

  COrientedBox box;
  CVector3<float> vTriangle[3];
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);
  
  box.RotateAroundUp( -45.0f );
  
  CVector4<float> vVertex0(-1,0,2,1);
  CVector4<float> vVertex1( 1,0,2,1);
  CVector4<float> vVertex2( 1,0,4,1);
  CVector4<float> vResult;
  
  CMatrix4x4<float> mRotation(  box.GetRightVector()[0],   box.GetUpVector()[0],  box.GetForwardVector()[0],  0,
				box.GetRightVector()[1],   box.GetUpVector()[1],  box.GetForwardVector()[1],  0,         
				box.GetRightVector()[2],   box.GetUpVector()[2],  box.GetForwardVector()[2],  0,
				0,                         0,                     0,                          1 );
  mRotation.Transpose();
  vResult = mRotation * vVertex0;
  //cerr << "v0 rotated: " << vResult << endl;
  vResult = mRotation * vVertex1;
  //cerr << "v1 rotated: " << vResult << endl;
  vResult = mRotation * vVertex2;
  //cerr << "v2 rotated: " << vResult << endl;
  CVector3<float> v0; v0[0] = vVertex0[0]; v0[1] = vVertex0[1]; v0[2] = vVertex0[2];
  CVector3<float> v1; v1[0] = vVertex1[0]; v1[1] = vVertex1[1]; v1[2] = vVertex1[2];
  CVector3<float> v2; v2[0] = vVertex2[0]; v2[1] = vVertex2[1]; v2[2] = vVertex2[2];
  CVector3<float> vRes;

  vRes = Rotate( v0, mRotation);
  //cerr << "v0 rotated: " << vRes << endl;
  vRes = Rotate( v1, mRotation);
  //cerr << "v1 rotated: " << vRes << endl;
  vRes = Rotate( v2, mRotation);
  //cerr << "v2 rotated: " << vRes << endl;
  
  vTriangle[0] = v0;
  vTriangle[1] = v1;
  vTriangle[2] = v2;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,SQRT_2+0.001f);
  vTriangle[1] = CVector3<float>( 1,0,SQRT_2+0.001f);
  vTriangle[2] = CVector3<float>(-1,0,SQRT_2+2.0f+0.001f);
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,SQRT_2-0.001f);
  vTriangle[1] = CVector3<float>( 1,0,SQRT_2-0.001f);
  vTriangle[2] = CVector3<float>(-1,0,SQRT_2+1.999f);
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2+0.001f);
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2+0.001f);
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2+0.001f);
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2);
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2);
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2);
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2-0.001f);
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2-0.001f);
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2-0.001f);
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,-23.0f);
  vTriangle[1] = CVector3<float>( 1,0,-23.0f);
  vTriangle[2] = CVector3<float>(-1,0,-21.0f);
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );
}
/////////////////////////////////////////////////////////////////
TEST(TriangleIntersectsOBB_BoxRotated_Translated)
{

  COrientedBox box;
  CVector3<float> vTriangle[3];
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);
  CVector3<float> vPosition(0,0,-0.3);
  box.SetPosition( vPosition);
  box.RotateAroundUp( -45.0f );
  
  CVector3<float> vVertex0(-1,0,2);
  CVector3<float> vVertex1( 1,0,2);
  CVector3<float> vVertex2( 1,0,4);
  
  vTriangle[0] = vVertex0 + vPosition;
  vTriangle[1] = vVertex1 + vPosition;
  vTriangle[2] = vVertex2 + vPosition;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,SQRT_2+0.001f) + vPosition;
  vTriangle[1] = CVector3<float>( 1,0,SQRT_2+0.001f) + vPosition;
  vTriangle[2] = CVector3<float>(-1,0,SQRT_2+2.0f+0.001f) + vPosition;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,SQRT_2) + vPosition;
  vTriangle[1] = CVector3<float>( 1,0,SQRT_2) + vPosition;
  vTriangle[2] = CVector3<float>(-1,0,SQRT_2+2.0f) + vPosition;
  
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2+0.001f) + vPosition;
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2+0.001f) + vPosition;
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2+0.001f) + vPosition;
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2) + vPosition;;
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2) + vPosition;;
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2) + vPosition;;
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );

  vTriangle[0] = CVector3<float>(-1,0,-1.0f-SQRT_2-0.001f) + vPosition;
  vTriangle[1] = CVector3<float>( 1,0,-1.0f-SQRT_2-0.001f) + vPosition;
  vTriangle[2] = CVector3<float>(-1,0, 1.0f-SQRT_2-0.001f) + vPosition;
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

  vTriangle[0] = CVector3<float>(-1,0,-23.0f) + vPosition;
  vTriangle[1] = CVector3<float>( 1,0,-23.0f) + vPosition;
  vTriangle[2] = CVector3<float>(-1,0,-21.0f) + vPosition;
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 0 );

}
/////////////////////////////////////////////////////////////////
TEST(TriangleIntersectsOBB_RealWorldExample )
{

  COrientedBox box;
  CVector3<float> vTriangle[3];

  box.SetWidth(1.0f);
  box.SetHeight(1.0f);
  box.SetLength(99.9f);
  CVector3<float> vPosition(0,0,-38.05f);
  box.SetPosition( vPosition);
  box.CalculateCorners();
  
  // cerr << box << endl;
  // F = 0,0,-1, scale 99.9
//   R = 1,0,0, scale 1
//   U = 0,1,0, scale 1
//   C = 0,0,-38.05
//   TLF:-0.5,0.5,-88
//   TLB:-0.5,0.5,11.9
//   TRF:0.5,0.5,-88
//   TRB:0.5,0.5,11.9
//   BLF:-0.5,-0.5,-88
//   BLB:-0.5,-0.5,11.9
//   BRF:0.5,-0.5,-88
//   BRB:0.5,-0.5,11.9
  

  CMatrix4x4<float> mRotation( box.GetRightVector()[0],   box.GetUpVector()[0],  box.GetForwardVector()[0],  0,
			       box.GetRightVector()[1],   box.GetUpVector()[1],  box.GetForwardVector()[1],  0,         
			       box.GetRightVector()[2],   box.GetUpVector()[2],  box.GetForwardVector()[2],  0,
			       0,                         0,                     0,                          1 );
  mRotation.Transpose();

  CVector3<float> vRes;

  
  vTriangle[0] = CVector3<float>(-1,-1,0);
  vTriangle[1] = CVector3<float>(1,-1,0);
  vTriangle[2] = CVector3<float>(1,1,0);

  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );
  
  vTriangle[0] = CVector3<float>(-1,-1,0);
  vTriangle[1] = CVector3<float>(1,1,0);
  vTriangle[2] = CVector3<float>(-1,1,0);

  vRes = Rotate( vTriangle[0], mRotation );
  //cerr << "Rotated v0 :" << vRes << endl;
  vRes = Rotate( vTriangle[1], mRotation );
  //cerr << "Rotated v1 :" << vRes << endl;
  vRes = Rotate( vTriangle[2], mRotation );
  //cerr << "Rotated v2 :" << vRes << endl;
  CHECK( TriangleIntersectsOBB( vTriangle[0], vTriangle[1], vTriangle[2], box ) == 1 );
}
/////////////////////////////////////////////////////////////////
TEST(PointIntersectsOBB )
{

  COrientedBox box;
  CVector3<float> vPoint;

  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);
  box.SetPosition(0,0,0);
  // Check y-axis
  vPoint = CVector3<float>(0,2,0);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[1] = 1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[1] = 1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[1] = 0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[1] = -0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[1] = -1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[1] = -1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[1] = -2.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  // Check x-axis
  vPoint = CVector3<float>(2,0,0);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[0] = 1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[0] = 1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[0] = 0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[0] = -0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[0] = -1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[0] = -1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[0] = -2.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  // Check x-axis
  vPoint = CVector3<float>(0,0,2);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[2] = 1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[2] = 1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[2] = 0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[2] = -0.999f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[2] = -1.000f;
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint[2] = -1.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint[2] = -2.001f;
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );

  // Check corners
  vPoint = CVector3<float>(2,2,2);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.001,1.001,1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.000,1.000,1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(0.999,0.999,0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-0.999,-0.999,-0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.000,-1.000,-1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.001,-1.001,-1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(-2.001,-2.001,-2.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );

  // Check corners
  vPoint = CVector3<float>(-2,2,2);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(-1.001,1.001,1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(-1.000,1.000,1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-0.999,0.999,0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(0.999,-0.999,-0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(1.000,-1.000,-1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(1.001,-1.001,-1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(2.001,-2.001,-2.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );

  // Check corners
  vPoint = CVector3<float>(2,-2,2);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.001,-1.001,1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.000,-1.000,1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(0.999,-0.999,0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-0.999,0.999,-0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.000,1.000,-1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.001,1.001,-1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(-2.001,2.001,-2.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );

  // Check corners
  vPoint = CVector3<float>(2,2,-2);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.001,1.001,-1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(1.000,1.000,-1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(0.999,0.999,-0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-0.999,0-.999,0.999);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.000,-1.000,1.000);
  CHECK( PointIntersectsOBB( vPoint, box ) != 0 );
  vPoint = CVector3<float>(-1.001,-1.001,1.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );
  vPoint = CVector3<float>(-2.001,-2.001,2.001);
  CHECK( PointIntersectsOBB( vPoint, box ) == 0 );

}
/////////////////////////////////////////////////////////////////
TEST( CollisionPoint3Planes )
{
  CPlane plane1, plane2, plane3;

  CVector3<float> vCenter(0,0,0);
  CVector3<float> vReal(0,0,0);
  CVector3<float> vResult;
  // Situation when intersection point is origo
  plane1.Calculate( CVector3<float>(1,0,0), vCenter);
  plane2.Calculate( CVector3<float>(0,1,0), vCenter);
  plane3.Calculate( CVector3<float>(0,0,1), vCenter);

  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) == 0 );
  CHECK_ARRAY_CLOSE( vReal.GetArray(), vResult.GetArray(), 3, 0.001f );

  // Situation when intersection point is arbitrary point
  vCenter[0] = 10.01f;
  vCenter[1] = -310.0f;
  vCenter[2] = 0.567f;
  vReal = vCenter;
  
  plane1.Calculate( CVector3<float>(1,0,0), vCenter);
  plane2.Calculate( CVector3<float>(0,1,0), vCenter);
  plane3.Calculate( CVector3<float>(0,0,1), vCenter);
  
  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) == 0 );
  CHECK_ARRAY_CLOSE( vReal.GetArray(), vResult.GetArray(), 3, 0.001f );
  vCenter[0] = 0.0f;
  vCenter[1] = 0.0f;
  vCenter[2] = 0.0f;

  vReal = vCenter;
  // Situation where planes are not aligned with axes
  plane1.Calculate( CVector3<float>(1,1,0), vCenter);
  plane2.Calculate( CVector3<float>(1,0,1), vCenter);
  plane3.Calculate( CVector3<float>(0,1,1), vCenter);
  
  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) == 0 );
  CHECK_ARRAY_CLOSE( vReal.GetArray(), vResult.GetArray(), 3, 0.001f );
  
  vCenter[0] = 10.0f;
  vCenter[1] = 20.0f;
  vCenter[2] = -30.0f;

  vReal = vCenter;
  // Situation where planes are not aligned with axes
  plane1.Calculate( CVector3<float>(1,1,0), vCenter);
  plane2.Calculate( CVector3<float>(1,0,1), vCenter);
  plane3.Calculate( CVector3<float>(0,1,1), vCenter);
  
  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) == 0 );
  CHECK_ARRAY_CLOSE( vReal.GetArray(), vResult.GetArray(), 3, 0.001f );

  /////////////////////////////////////////////////////////////////
  // Situations when normal vectors are in same plane (no intersection)
  // normals in XY plane
  plane1.Calculate( CVector3<float>(1,10,0), vCenter);
  plane2.Calculate( CVector3<float>(1,1,0), vCenter);
  plane3.Calculate( CVector3<float>(-1,0.3,0), vCenter);

  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) != 0 );
  // normals in YZ plane
  plane1.Calculate( CVector3<float>(0,1,10), vCenter);
  plane2.Calculate( CVector3<float>(0,1,1), vCenter);
  plane3.Calculate( CVector3<float>(0,-1,0.3), vCenter);

  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) != 0 );

  // normals in XZ plane
  plane1.Calculate( CVector3<float>(1,0,10), vCenter);
  plane2.Calculate( CVector3<float>(1,0,1), vCenter);
  plane3.Calculate( CVector3<float>(-1,0,0.3), vCenter);

  CHECK( CollisionPoint3Planes( plane1, plane2, plane3, vResult ) != 0 );
}
/////////////////////////////////////////////////////////////////
TEST(SphereIntersectsPlane)
{
  CPlane plane;
  plane.Calculate( CVector3<float>(1,0,0), CVector3<float>(2,0,0));

  CSphere sphere;
  sphere.SetPosition(0,0,0);
  sphere.SetRadius(1.0f);
  float fTmp;
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) != 0);

  sphere.SetPosition(0.9,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) != 0);

  sphere.SetPosition(0.999,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) != 0);

  sphere.SetPosition(1.0,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(1.001,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(1.99,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(1.999,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(2.0,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(2.999,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(3.0,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) == 0);

  sphere.SetPosition(3.001,0,0);
  CHECK( SphereIntersectsPlane( plane, sphere, fTmp) != 0);

}
/////////////////////////////////////////////////////////////////
TEST(SphereIntersectsCube)
{
  CSphere sphere;
  sphere.SetRadius(1.0f);


  CAxisAlignedCube cube;
  cube.SetPosition(0,0,0);
  cube.SetWidth(1.0f);


  sphere.SetPosition(2,0,0);  
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);

  sphere.SetPosition(1.51,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);

  sphere.SetPosition(1.501,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);

  sphere.SetPosition(1.5,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(1.499,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(0,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(-1.49,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(-1.499,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(-1.5,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 1);

  sphere.SetPosition(-1.501,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);

  sphere.SetPosition(-1.51,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);

  sphere.SetPosition(-2.0,0,0);
  CHECK( SphereIntersectsAACube(sphere, cube) == 0);
}
/////////////////////////////////////////////////////////////////
