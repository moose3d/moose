#include <PhoenixCamera.h>
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
namespace PhoenixCameraTest
{
  CCamera g_Camera;
};
/////////////////////////////////////////////////////////////////
using namespace PhoenixCameraTest;
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
  int aViewport[4] = { 1, 2, 43, 22 };
  g_Camera.SetViewport( aViewport[0], aViewport[1], aViewport[2], aViewport[3]);
  CHECK_ARRAY_EQUAL( aViewport, g_Camera.GetViewport(), 4 );

  int aViewport2[4] = { 0,0,0,0 };
  g_Camera.SetViewport( aViewport2[0], aViewport2[1], aViewport2[2], aViewport2[3]);
  CHECK_ARRAY_EQUAL( aViewport2, g_Camera.GetViewport(), 4 );

  int aViewport3[4] = { -10,-20,-30,-40 };
  g_Camera.SetViewport( aViewport3[0], aViewport3[1], aViewport3[2], aViewport3[3]);
  CHECK_ARRAY_EQUAL( aViewport3, g_Camera.GetViewport(), 4 );

  int aViewport4[4] = { -10,20,30,-40 };
  g_Camera.SetViewport( aViewport4[0], aViewport4[1], aViewport4[2], aViewport4[3]);
  CHECK_ARRAY_EQUAL( aViewport4, g_Camera.GetViewport(), 4 );

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
