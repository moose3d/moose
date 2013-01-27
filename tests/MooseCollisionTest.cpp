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

#include <MooseCollision.h>
#include <MooseMath.h>
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <MooseCapsuleCollider.h>
#include <MooseOBBCollider.h>
/////////////////////////////////////////////////////////////////
using namespace Moose::Collision;
using namespace Moose::Math;
using namespace Moose::Volume;
using std::cerr;
using std::endl;
const float SQRT_2 = 1.41421356237f;
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_Origo)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,0,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,0,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd );

  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_Not_Origo)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd );
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineIntersectsPlane_Not_Origo_Reverse)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,-3,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd);
  
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_BehindPlane)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,-1,0);
  CVector3<float> vLineEnd(3, -3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd);
  
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_FrontOfPlane)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,-1,0);
  CVector3<float> vLineEnd(3, -3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd);
  
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_LineInPlane)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,3,-1);
  CVector3<float> vLineEnd(3, -3,-1);
  CVector3<float> vLinePlaneCollisionPointReal(3,2.43f,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  
  CVector3<float> vPlanePoint(0,2.43f,0);
  plane.Calculate( CVector3<float>(0,0,1), vPlanePoint);
  ////////////////////
  /// 3,3.000,-1
  /// 3,-3.000,-1
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,-0.001
  /// 3,-3.000,-0.001
  vLineStart += CVector3<float>(0,0,0.999f);
  vLineEnd += CVector3<float>(0,0,0.999f);  
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

  ////////////////////
  /// 3,3.000,0.000
  /// 3,-3.000,0.000
  vLineStart = CVector3<float>(3,3,0);
  vLineEnd   = CVector3<float>(3,-3,0);  
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));

  ////////////////////
  /// 3,3.000,0.001
  /// 3,-3.000,0.001
  vLineStart += CVector3<float>(0,0,0.001f);
  vLineEnd += CVector3<float>(0,0,0.001f);  
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,1.000
  /// 3,-3.000,1.000
  vLineStart += CVector3<float>(0,0,0.999f);
  vLineEnd += CVector3<float>(0,0,0.999f);  
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_LineLength_YAxis)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,4,0);
  CVector3<float> vLineEnd(3, 3,0);
  CVector3<float> vLinePlaneCollisionPointReal(3,2,0);
  CVector3<float> vLinePlaneCollisionPointResult;
  CVector3<float> vPlanePoint(0,2.0f,0);
  plane.Calculate( CVector3<float>(0,-1,0), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd);
  
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.001,0
  /// 3,2.001,0
  vLineStart += CVector3<float>(0,-0.999f,0);
  vLineEnd += CVector3<float>(0,-0.999f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,3.000,0
  /// 3,2.000,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.999,0
  /// 3,1.999,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.998,0
  /// 3,1.998,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.001,0
  /// 3,1.001,0
  vLineStart += CVector3<float>(0,-0.997f,0);
  vLineEnd += CVector3<float>(0,-0.997f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,2.000,0
  /// 3,1.000,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,1.999,0
  /// 3,0.999,0
  vLineStart += CVector3<float>(0,-0.001f,0);
  vLineEnd += CVector3<float>(0,-0.001f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,1.000,0
  /// 3,0.000,0
  vLineStart += CVector3<float>(0,-0.999f,0);
  vLineEnd += CVector3<float>(0,-0.999f,0);
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentIntersectsPlane_LineLength_ZAxis)
{
  
  CPlane plane;
  CLineSegment line;

  CVector3<float> vLineStart(3,0,4);
  CVector3<float> vLineEnd(3, 0,3);
  CVector3<float> vLinePlaneCollisionPointReal(3,0,2);
  CVector3<float> vLinePlaneCollisionPointResult;
  CVector3<float> vPlanePoint(0,0,2.0f);
  plane.Calculate( CVector3<float>(0,0,-1), vPlanePoint);
  
  line.Set( vLineStart, vLineEnd);
  
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,3.001
  /// 3,0,2.001
  vLineStart[2] -= 0.999f;
  vLineEnd[2] -= 0.999f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_BEHIND_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,3.000
  /// 3,0,2.000
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.999
  /// 3,0,1.999
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.998
  /// 3,0,1.998
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.001
  /// 3,0,1.001
  vLineStart[2] -= 0.997f;
  vLineEnd[2] -= 0.997f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,2.000
  /// 3,0,1.000
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( POINT_IN_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  CHECK_ARRAY_CLOSE( vLinePlaneCollisionPointReal.GetArray(), vLinePlaneCollisionPointResult.GetArray(), 3, 0.001f);
  ////////////////////
  /// 3,0,1.999
  /// 3,0,0.999
  vLineStart[2] -= 0.001f;
  vLineEnd[2] -= 0.001f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
  ////////////////////
  /// 3,0,1.000
  /// 3,0,0.000
  vLineStart[2] -= 0.999f;
  vLineEnd[2] -= 0.999f;
  line.Set( vLineStart, vLineEnd);
  
  CHECK_EQUAL( LINE_FRONT_OF_PLANE, LineSegmentIntersectsPlane( plane, line, vLinePlaneCollisionPointResult ));
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
  
  ClosestPointOnRay( vPoint, ray, vResult);
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

  
  CHECK( PlaneIntersectsBox( plane, box ) == 0 );

  box.SetPosition(0,1.001f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 0 );

  box.SetPosition(0,1.000f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,0.999f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,0.000f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-0.999f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-1.000f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 1 );

  box.SetPosition(0,-1.001f,0);

  CHECK( PlaneIntersectsBox( plane, box ) == 0 );
  
  box.SetPosition(0,-5.000f,0);

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

  Rotate( v0, mRotation, vRes );
  //cerr << "v0 rotated: " << vRes << endl;
  Rotate( v1, mRotation, vRes );
  //cerr << "v1 rotated: " << vRes << endl;
  Rotate( v2, mRotation, vRes );
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

  Rotate( vTriangle[0], mRotation, vRes );
  //cerr << "Rotated v0 :" << vRes << endl;
  Rotate( vTriangle[1], mRotation, vRes );
  //cerr << "Rotated v1 :" << vRes << endl;
  Rotate( vTriangle[2], mRotation, vRes  );
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
TEST( RayIntersectsOBB_Origo_Coordinate_Axes )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  
  box.SetPosition(0,0,0);
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);

  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);

  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);

  // ray along negative x
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);

  // ray along positive x
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( 3.0f, fDistance, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST( RayIntersectsOBB_X_axes_moving_box )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  
  box.SetPosition(-2,0,0);
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  box.SetPosition(-1.001f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(-1.0f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(-1.0f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(-0.999f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0.0,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(0.999,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(1.0,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(1.001f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(2.0f,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(0,-4,0);
  ray.SetDirection(0,1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(0,4,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
}
/////////////////////////////////////////////////////////////////
TEST( RayIntersectsOBB_Y_axes_moving_box )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  
  box.SetPosition(0,-2,0);
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  box.SetPosition(0,-1.001f,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(0,-1.0f,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(0,-0.999f,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0.0,0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(0,0.999,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0,1.0,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0,1.001f,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(0,2.0f,0);
  // ray along negative z
  ray.SetPosition(0,0,4);
  ray.SetDirection(0,0,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(0,0,-4);
  ray.SetDirection(0,0,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
}
/////////////////////////////////////////////////////////////////
TEST( RayIntersectsOBB_Z_axes_moving_box )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  
  box.SetPosition(0,0,-2);
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(0,-1,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  box.SetPosition(0,0,-1.001f);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(0,0,-1.0f);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(0,0,-0.999f);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0.0,0,0);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );


  box.SetPosition(0,0,0.999);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0,0,1.0);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  box.SetPosition(0,0,1.001f);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  box.SetPosition(0,0,2.0f);
  // ray along negative z
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive z
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along positive y
  ray.SetPosition(-4,0,0);
  ray.SetDirection(1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  // ray along negative y
  ray.SetPosition(4,0,0);
  ray.SetDirection(-1,0,0);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
}
/////////////////////////////////////////////////////////////////
TEST( RayIntersectsOBB_real_world )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  const float SQRT_3 = 1.73205080757f;
  box.SetPosition(2,2,2);
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  ray.SetPosition(4,4,4);
  ray.SetDirection(-1,-1,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( SQRT_3, fDistance, 0.0001f);

  ray.SetPosition(4,0,4);
  ray.SetDirection(-1,1,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( SQRT_3, fDistance, 0.0001f);

  ray.SetPosition(4,4,0);
  ray.SetDirection(-1,-1,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( SQRT_3, fDistance, 0.0001f);
  
  ray.SetPosition(0,4,4);
  ray.SetDirection(1,-1,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( SQRT_3, fDistance, 0.0001f);

  ray.SetPosition(0,0,0);
  ray.SetDirection(1,1,1);
  
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  CHECK_CLOSE( SQRT_3, fDistance, 0.0001f);

  ray.SetPosition(0,0,0);
  ray.SetDirection(-1,-1,-1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  ray.SetPosition(4,4,4);
  ray.SetDirection(1,1,1);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
}
/////////////////////////////////////////////////////////////////
TEST( RayIntersectsOBB_real_world_2 )
{
  COrientedBox box;
  CRay ray;
  float fDistance;
  
  box.RotateAroundUp(45.0f);
  box.SetPosition(2,0,-2);
  
  box.SetWidth(2.0f);
  box.SetHeight(2.0f);
  box.SetLength(2.0f);

  CVector3<float> vTmp(-1,0,-1);
  vTmp.Normalize();

  ray.SetDirection(1,0,-1);


  ray.SetPosition( vTmp + vTmp*2.0f );
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
  
  ray.SetPosition( vTmp + vTmp*0.001f );
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  ray.SetPosition( vTmp - vTmp*0.000001f);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );
  
  ray.SetPosition( vTmp - vTmp*0.001f);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  ray.SetPosition( -vTmp );
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == true );

  ray.SetPosition( -vTmp - vTmp*0.00001f);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  ray.SetPosition( -vTmp - vTmp*0.001f);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );

  ray.SetPosition( -vTmp - vTmp*2.0f);
  CHECK( RayIntersectsOBB( ray, box, &fDistance ) == false );
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
TEST( SphereIntersectsCapsule )
{

  CCapsule capsule(CVector3<float>(2.0f,0,0), CVector3<float>(2.0f,0,-2.0f), 1.0f);
  CSphere sphere( CVector3<float>(5.0f,0,0), 1.0f);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == false);
  
  sphere.SetPosition(4.0,0,0);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == false);

  sphere.SetPosition(4.0,0,-1.0f);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == false);
  
  sphere.SetPosition(4.0f-EPSILON,0,0);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == true);

  sphere.SetPosition(4.0f-EPSILON,0,-1.0f);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == true);
  
  sphere.SetPosition(3.9f,0,0);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == true);

  sphere.SetPosition(3.9f,0,-1.0f);
  CHECK( SphereIntersectsCapsule(sphere,capsule) == true);

}
/////////////////////////////////////////////////////////////////
// TEST( OBBIntersectsCapsule )
// {
  
// }
/////////////////////////////////////////////////////////////////
TEST(LineSegmentToLineSegmentDistanceSquared_ParallelLines)
{
   CLineSegment line0, line1;
   line1.Set(CVector3<float>(0,0,0), CVector3<float>(2,0,0));

   // no overlap, far
   line0.Set( CVector3<float>(-2,1,0), CVector3<float>( -1,1,0));
   CHECK_CLOSE( 2.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 2.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // no overlap, closer
   line0.Set( CVector3<float>( -1.01,1.0,0.0), CVector3<float>( -0.01,1.0,0.0));
   CHECK_CLOSE( 1.0001f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.0001f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, border
   line0.Set(CVector3<float>( -1.00,1.0,0.0), CVector3<float>( 0.0,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);

   // overlap, close
   line0.Set(CVector3<float>( -0.5,1.0,0.0),   CVector3<float>( 0.5,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, full
   line0.Set(CVector3<float>( 0.5,1.0,0.0),   CVector3<float>( 1.5,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, border
   line0.Set(CVector3<float>( 1.0,1.0,0.0),   CVector3<float>( 2.0,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, partial
   line0.Set(CVector3<float>( 1.001,1.0,0.0),   CVector3<float>( 2.001,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, partial
   line0.Set(CVector3<float>( 1.999,1.0,0.0),   CVector3<float>( 2.999,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // overlap, border
   line0.Set(CVector3<float>( 2.0,1.0,0.0),   CVector3<float>( 3.0,1.0,0.0));
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // no overlap, very close
   line0.Set(CVector3<float>( 2.001,1.0,0.0),   CVector3<float>( 3.001,1.0,0.0));
   CHECK_CLOSE( 1.000001f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 1.000001f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);
   // no overlap, far
   line0.Set(CVector3<float>( 3.0,1.0,0.0),   CVector3<float>( 4.0,1.0,0.0));
   CHECK_CLOSE( 2.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);
   CHECK_CLOSE( 2.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0), 0.000001f);


}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentToLineSegmentDistanceSquared_Perpendicular)
{
    float fS, fT;

    CLineSegment line0, line1;

    line1.Set( CVector3<float>(0,0,0), CVector3<float>(2,0,0));
    line0.Set( CVector3<float>(0,2,0), CVector3<float>(0,1,0));
    CHECK_CLOSE( 1.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);

    CHECK_CLOSE( 1.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);



    line0.Set( CVector3<float>(0,1.001,0), CVector3<float>(0,0.001,0));
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);

    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);


    line0.Set( CVector3<float>(0,1.0,0), CVector3<float>(0,0.0,0));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);

    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);


    line0.Set( CVector3<float>(0,0.999,0), CVector3<float>(0,-0.001,0));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);

    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(0,0.001,0), CVector3<float>(0,-0.999,0));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);


    line0.Set( CVector3<float>(0,0.0,0), CVector3<float>(0,-1.0,0));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(0,-0.001,0), CVector3<float>(0,-1.001,0));
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(0,-0.002,0), CVector3<float>(0,-1.002,0));
    CHECK_CLOSE( 0.000004f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 0.000004f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(0, -1.0,0), CVector3<float>(0,-2.0,0));
    CHECK_CLOSE( 1.0, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 1.0, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    // same distances, different coordinate axes.
    line1.Set( CVector3<float>(7,10,-10), CVector3<float>(9,10,-10));
    line0.Set( CVector3<float>(7,12,-10), CVector3<float>(7,11,-10));
    CHECK_CLOSE( 1.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 1.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(7,11.001,-10), CVector3<float>(7,10.001,-10));
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(7,11.0,-10), CVector3<float>(7,10.0,-10));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(7,10.999,-10), CVector3<float>(7,9.999,-10));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(7,10.001,-10), CVector3<float>(7, 9.001,-10));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(7,10.0,-10), CVector3<float>(7,9.0,-10));
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.001f);
    CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.001f);

    line0.Set( CVector3<float>(7, 9.999,-10), CVector3<float>(7,8.999,-10));
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 0.000001f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(7, 9.998,-10), CVector3<float>(7,8.998,-10));
    CHECK_CLOSE( 0.000004f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 0.000004f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);

    line0.Set( CVector3<float>(7, 9.0,-10), CVector3<float>(7, 8.0,-10));
    CHECK_CLOSE( 1.0, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
    CHECK_CLOSE( 1.0, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);
}
/////////////////////////////////////////////////////////////////
TEST(LineSegmentToLineSegmentDistanceSquared_Crossing)
{
   CLineSegment line0, line1;

   line1.Set( CVector3<float>(0,0,0), CVector3<float>( 2,0,0));
   line0.Set( CVector3<float>( 0,2,0), CVector3<float>(  0,1,0));

   CHECK_CLOSE( 1.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1), 0.000001f);

   line0.Set( CVector3<float>(0,0,0), CVector3<float>( 2,0,0));
   line1.Set( CVector3<float>( 0,2,0), CVector3<float>(  2,-2,0));
   float fS, fT;
   CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
   CHECK_CLOSE( 0.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);
   line0.Set( CVector3<float>(0,0,0), CVector3<float>( 2,0,0));
   line1.Set( CVector3<float>( 0,2,2), CVector3<float>(  2,-2,2));

   CHECK_CLOSE( 4.0f, LineSegmentToLineSegmentDistanceSquared( line0, line1, &fS, &fT), 0.000001f);
   CHECK_CLOSE( 4.0f, LineSegmentToLineSegmentDistanceSquared( line1, line0, &fS, &fT), 0.000001f);
}
/////////////////////////////////////////////////////////////////
struct OBBFixture
{
  COrientedBox a;
  COrientedBox b;
  OBBFixture()
  {
      a.SetWidth(1);
      a.SetHeight(2);
      a.SetLength(4);
      
      a.SetPosition(0,0,0);
      b = a;
  }
};
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( OBBFixture, OBBintersectsOBB_Xaxis)
{
//   COrientedBox a,b;

//   a.SetWidth(1);
//   a.SetHeight(2);
//   a.SetLength(4);

//   a.SetPosition(0,0,0);
//   b = a;
   b.SetPosition( 15,0,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 2,0,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );
  
  b.SetPosition( 1.999f,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 1.5f,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 1.001,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 1.000,0,0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0.999,0,0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition(0,0,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( -0.999,0,0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( -1.0,0,0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( -1.001,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( -1.5,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( -15,0,0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( OBBFixture, OBBintersectsOBB_Zaxis)
{
 
  b.SetPosition( 0,0,5);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,4.001);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,4);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );
  
  b.SetPosition( 0,0,3.999);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,1);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,-1);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,-3.999);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,-4.0);
  
  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,-4.001);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0,-5.0);
  
  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( OBBFixture, OBBintersectsOBB_Yaxis)
{
 
  b.SetPosition( 0,3,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,2.001,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,2.00,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,1.999,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,0.0,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,-1.999,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,-2.0,0);

  CHECK_EQUAL( true, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( true, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,-2.001,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );

  b.SetPosition( 0,-3.0,0);

  CHECK_EQUAL( false, OBBIntersectsOBB( a,b) );
  CHECK_EQUAL( false, OBBIntersectsOBB( b,a) );
}
////////////////////////////////////////////////////////////////////////////////
struct CapsuleFixture 
{
  CCapsule a;
  CCapsule b;
  CapsuleFixture()
  {
    a.Set( CVector3<float>(0,0,1), CVector3<float>( 0,0,-1));
    a.SetRadius(0.5);
    b = a;
  }
};
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleX_plus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0.999,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0.001,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0.5,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleX_minus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(-0.999,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(-0.001,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(-0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(-0.5,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleY_plus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.999,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.001,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.001,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.5,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleY_minus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.999,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.001,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.001,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.5,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleZ_plus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,2.0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,0.999));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,0.5));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleFixture, CapsuleZ_minus)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-2.0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-0.999));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0,-0.5));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
struct CapsuleNonParallelFixture 
{
  CCapsule a;
  CCapsule b;
  CapsuleNonParallelFixture()
  {
    a.Set( CVector3<float>(0,0,1), CVector3<float>( 0,0,-1));
    a.SetRadius(0.5);
    
    b.Set( CVector3<float>(0,0,-1), CVector3<float>(-2,0,1) );
    b.SetRadius(0.5);
  }
};
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleX)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  b.Move(CVector3<float>(-1,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  b.Move(CVector3<float>(-0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  b.Move(CVector3<float>(-0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleX2)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  
  b.Move(CVector3<float>(3.0,0,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));

  b.Move(CVector3<float>(0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  
  b.Move(CVector3<float>(0.001,0,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleY_p)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.999,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.001,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.001,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,0.5,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleY_m)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.999,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.001,0));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.001,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
  a.Move(CVector3<float>(0,-0.5,0));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleZ_p)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,0.999));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  
  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,0.5));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleNonParallelFixture, CapsuleZ_m)
{
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,-2.999));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( true, CapsuleIntersectsCapsule(a,b));
  
  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,-0.001));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));

  a.Move(CVector3<float>(0,0,-0.5));
  CHECK_EQUAL( false, CapsuleIntersectsCapsule(a,b));
}
///////////////////////////////////////////////////////////////////////////////
struct CapsuleOBBColliderFixture 
{
  CCapsuleCollider a;
  COBBCollider b;
  CapsuleOBBColliderFixture()
  {
    a.GetBoundingCapsule().SetRadius(0.5);
    a.GetBoundingCapsule().Set( CVector3<float>(0,0,0), CVector3<float>(0,-1,0));
    b.GetBoundingBox().SetPosition(0,0,0);
    b.GetBoundingBox().SetWidth(1.0);
    b.GetBoundingBox().SetHeight(1.0);
    b.GetBoundingBox().SetLength(1.0);

  }

};

////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleOBBColliderFixture, capsuleOBBTest)
{

  
  CHECK_EQUAL( true, OBBIntersectsCapsule(b.GetBoundingBox(),a.GetBoundingCapsule() ));
  CHECK_EQUAL( true, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-1.999,0) );
  CHECK_EQUAL( true, a.Intersects(b));

  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( true, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( false, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( false, a.Intersects(b));

  
}
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( CapsuleOBBColliderFixture, capsuleOBBTestTr)
{
  CTransform tr;
  tr.SetScaling(1,2.0,1);
  a.SetColliderTransform(&tr);
  
  CHECK_EQUAL( true, OBBIntersectsCapsule(b.GetBoundingBox(),a.GetBoundingCapsule() ));
  CHECK_EQUAL( true, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-2.999,0) );
  CHECK_EQUAL( true, a.Intersects(b));

  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( true, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( false, a.Intersects(b));
  
  b.GetBoundingBox().Move( CVector3<float>(0,-0.001,0) );
  CHECK_EQUAL( false, a.Intersects(b));

  
}
////////////////////////////////////////////////////////////////////////////////
struct DifferentOBBCapsuleTest 
{
  CCapsuleCollider c;
  COBBCollider     b;
  
  DifferentOBBCapsuleTest()
  {
    c.GetBoundingCapsule().Set(CVector3<float>(0,0,0), CVector3<float>(0,-1,0));
    c.GetBoundingCapsule().SetRadius(0.125);

    b.GetBoundingBox().SetPosition(0,0,0);
    b.GetBoundingBox().SetOrientation( CVector3<float>(0,1,0),
                      CVector3<float>(0,0,1),
                      CVector3<float>(1,0,0));
    b.GetBoundingBox().SetLength(2.0);
    b.GetBoundingBox().SetHeight(0.06);
    b.GetBoundingBox().SetWidth(2.0);
  }
  bool
  intersectionTest( const Moose::Volume::COrientedBox & box, const Moose::Volume::CCapsule & capsule )
{
  CVector3<float> vPointOnSegment;
  cerr << "boxpos: " <<  box.GetPosition() << "\n";
  cerr << "capsulestart: " << capsule.GetStart() << "\n";
  cerr << "capsuleend: " << capsule.GetEnd() << "\n";
  ClosestPointOnLineSegment( box.GetPosition(), capsule, vPointOnSegment );
  cerr << "pointOnSegment: " << vPointOnSegment << "\n";
  CVector3<float> vSegmentToBox = box.GetPosition()-vPointOnSegment;
  float fDistanceSqr = vSegmentToBox.LengthSqr();

  cerr << "segm2box: " << vSegmentToBox << "\n";
  cerr << "distSqr: " << fDistanceSqr << "\n";
  // In case we get too close, then problems start emerging.
  if ( TOO_CLOSE_TO_ZERO(fDistanceSqr))
    vSegmentToBox[0] = vSegmentToBox[1] = vSegmentToBox[2] = 0.0;
  else     vSegmentToBox.Normalize();
  float fEffRadius = 0.5f * ( fabsf((box.GetForwardVector()*box.GetLength()).Dot(vSegmentToBox)) +
			      fabsf((box.GetRightVector()*box.GetWidth()).Dot(vSegmentToBox)) +
			      fabsf((box.GetUpVector()*box.GetHeight()).Dot(vSegmentToBox)) );

  cerr << "effrad: " << fEffRadius << "\n";
  cerr << "result: " << "( " << fDistanceSqr << " <= ( " << (capsule.GetRadiusSqr() + (2.0f*fEffRadius * capsule.GetRadius()) + (fEffRadius*fEffRadius)) << " )\n";
  // c^2 <= (a+b)^2 
  return ( fDistanceSqr <= (capsule.GetRadiusSqr() + (2.0f*fEffRadius * capsule.GetRadius()) + (fEffRadius*fEffRadius)) );
}
};
////////////////////////////////////////////////////////////////////////////////
TEST_FIXTURE( DifferentOBBCapsuleTest, capsuleOBBTestTr2)
{
  CTransform ctr;
  CTransform btr;
  
  ctr.SetTranslation(0,3.25,0);
  ctr.SetRotation(CQuaternion(0,-0.989795,0,-0.142454));
  ctr.SetScaling(1,2.33921,1);

  btr.SetTranslation( 0.00378628,1.39942,0.0618424);
  btr.SetRotation(CQuaternion(0.0220692, -0.0071816, -0.00151039, 0.99973));
  btr.SetScaling(0.24,0.24,0.24);

 
  CVector3<float > vStart, vEnd;
  Transform( c.GetBoundingCapsule().GetStart(), ctr, vStart);
  Transform( c.GetBoundingCapsule().GetEnd(), ctr, vEnd);
  CCapsule capsule(vStart, vEnd, c.GetBoundingCapsule().GetRadius());
  
  COrientedBox thisbox = b.GetBoundingBox();
  thisbox.Move( btr.GetTranslation());
  thisbox.AppendToRotation( btr.GetRotation() );
  CVector3<float> vScale = btr.GetScaling();
  thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
  thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
  thisbox.SetLength(thisbox.GetLength()*vScale[2]);
  
  //c.SetColliderTransform( &ctr);
  //b.SetColliderTransform( &btr);
  //CHECK_EQUAL( true,  c.Intersects(b));
  CHECK_EQUAL( true,  intersectionTest(thisbox,capsule));
}
////////////////////////////////////////////////////////////////////////////////
