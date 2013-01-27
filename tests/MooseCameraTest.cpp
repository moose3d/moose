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

#include <MooseCamera.h>
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
namespace MooseCameraTest
{
  CCamera g_Camera;
};
/////////////////////////////////////////////////////////////////
using namespace MooseCameraTest;
/////////////////////////////////////////////////////////////////
TEST(CCamera)
{
  CCamera camera;
}
/////////////////////////////////////////////////////////////////
TEST(CCamera_SetFieldOfView)
{
  g_Camera.SetFieldOfView( 20.0f );
  CHECK_CLOSE( 20.0f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( 0.001f );
  CHECK_CLOSE( 0.001f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( 0.0f );
  CHECK_CLOSE( 0.0f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( -0.001f );
  CHECK_CLOSE( -0.001f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( -10.0f );
  CHECK_CLOSE( -10.0f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( -998234298.0f );
  CHECK_CLOSE( -998234298.0f, g_Camera.GetFieldOfView(), 0.001f);
  g_Camera.SetFieldOfView( 998234298.0f );
  CHECK_CLOSE( 998234298.0f, g_Camera.GetFieldOfView(), 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CCamera_SetViewport)
{
  Viewport_t aViewport = { 1, 2, 43, 22 };
  g_Camera.SetViewport( aViewport);
  CHECK_EQUAL( aViewport, g_Camera.GetViewport() );

  Viewport_t aViewport2 = { 0,0,0,0 };
  g_Camera.SetViewport( aViewport2);
  CHECK_EQUAL( aViewport2, g_Camera.GetViewport() );

  Viewport_t aViewport3 = { -10,-20,-30,-40 };
  g_Camera.SetViewport( aViewport3);
  CHECK_EQUAL( aViewport3, g_Camera.GetViewport() );

  Viewport_t aViewport4 = { -10,20,30,-40 };
  g_Camera.SetViewport( aViewport4);
  CHECK_EQUAL( aViewport4, g_Camera.GetViewport() );

}
/////////////////////////////////////////////////////////////////
TEST(CCamera_SetNearClipping)
{
  g_Camera.SetNearClipping( 340.0f );
  CHECK_CLOSE( 340.0f, g_Camera.GetNearClipping(), 0.001f);
  g_Camera.SetNearClipping( 0.001f );
  CHECK_CLOSE( 0.001f, g_Camera.GetNearClipping(), 0.001f);
  g_Camera.SetNearClipping( 0.0f );
  CHECK_CLOSE( 0.0f, g_Camera.GetNearClipping(), 0.001f);
  g_Camera.SetNearClipping( -0.001f );
  CHECK_CLOSE( -0.001f, g_Camera.GetNearClipping(), 0.001f);
  g_Camera.SetNearClipping( -20.001f );
  CHECK_CLOSE( -20.001f, g_Camera.GetNearClipping(), 0.001f);
}
/////////////////////////////////////////////////////////////////  
TEST(CCamera_SetFarClipping)
{
  g_Camera.SetFarClipping( 340.0f );
  CHECK_CLOSE( 340.0f, g_Camera.GetFarClipping(), 0.001f);
  g_Camera.SetFarClipping( 0.001f );
  CHECK_CLOSE( 0.001f, g_Camera.GetFarClipping(), 0.001f);
  g_Camera.SetFarClipping( 0.0f );
  CHECK_CLOSE( 0.0f, g_Camera.GetFarClipping(), 0.001f);
  g_Camera.SetFarClipping( -0.001f );
  CHECK_CLOSE( -0.001f, g_Camera.GetFarClipping(), 0.001f);
  g_Camera.SetFarClipping( -20.001f );
  CHECK_CLOSE( -20.001f, g_Camera.GetFarClipping(), 0.001f);
}
/////////////////////////////////////////////////////////////////  
TEST(CCamera_SetLensFlaresEnabled)
{
  g_Camera.SetLensFlaresEnabled( 0 );
  CHECK( g_Camera.IsLensFlaresEnabled() != 1 );

  g_Camera.SetLensFlaresEnabled( 1 );
  CHECK( g_Camera.IsLensFlaresEnabled() != 0 );
}
/////////////////////////////////////////////////////////////////  
TEST(CCamera_SetViewOrtho_Perspective)
{
  g_Camera.SetViewOrtho( -1.0, 1.0, -1.0, 1.0 );
  CHECK( g_Camera.IsOrthogonal()  != 0 );

  g_Camera.SetFieldOfView( 42.0f );
  CHECK( g_Camera.IsOrthogonal()  == 0 );
}
/////////////////////////////////////////////////////////////////  
TEST(CCamera_Movements)
{
  CVector3<float> fwd(0,0,-1);
  CVector3<float> up(0,1,0);
  // Set initial position and direction.
  g_Camera.SetPosition(0,0,0);
  g_Camera.SetDirectionForwardUp( fwd, up );
  float aPos[3] = { 0,0,0 };
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  // forward / backward
  aPos[2] -= 3.0f;
  g_Camera.Move( 3.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[2] -= 0.001f;
  g_Camera.Move( 0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[2] -= 0.0f;
  g_Camera.Move( 0.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[2] += 0.001f;
  g_Camera.Move( -0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[2] += 20.0f;
  g_Camera.Move( -20.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  // strafing
  aPos[0] += 3.0f;
  g_Camera.Strafe( 3.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[0] += 0.001f;
  g_Camera.Strafe( 0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[0] += 0.0f;
  g_Camera.Strafe( 0.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[0] -= 0.001f;
  g_Camera.Strafe( -0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[0] -= 20.0f;
  g_Camera.Strafe( -20.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  // elevate
  aPos[1] += 3.0f;
  g_Camera.Elevate( 3.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[1] += 0.001f;
  g_Camera.Elevate( 0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[1] += 0.0f;
  g_Camera.Elevate( 0.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

  aPos[1] -= 0.001f;
  g_Camera.Elevate( -0.001f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);
  
  aPos[1] -= 20.0f;
  g_Camera.Elevate( -20.0f );
  CHECK_ARRAY_CLOSE( aPos, const_cast<CVector3<float> &>(g_Camera.GetPosition()).GetArray(), 3, 0.001f);

}
/////////////////////////////////////////////////////////////////
TEST( CCamera_UnProject_Ortho )
{
  CCamera c;
  c.SetFarClipping(1.0);
  c.SetNearClipping(0.001);
  c.SetPosition(0,0,0);
  c.SetViewOrtho(0.0, 541.0, 0.0, 992.0);
  c.SetViewport(0,0,640,480);
  c.UpdateView();
  c.UpdateProjection();
  {
    CVector3<float> vNear = c.UnProject( 640.0, 480.0, 0.0 );
    CVector3<float> vFar  = c.UnProject( 640.0, 480.0, 1.0 );
    const float result[] = { 541.0 / 2.0, 992.0/2.0, -c.GetNearClipping() };
    const float result2[] = { 541.0/ 2.0, 992.0/2.0, -c.GetFarClipping() };

    CHECK_ARRAY_CLOSE( result, vNear.GetArray(), 3, 0.001f);
    CHECK_ARRAY_CLOSE( result2, vFar.GetArray(), 3, 0.001f);
  }
  

}
/////////////////////////////////////////////////////////////////
