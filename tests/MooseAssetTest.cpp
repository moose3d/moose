#include <UnitTest++/UnitTest++.h>
#include <MooseCore.h>
#include <MoosePointLight.h>
#include <MooseAssetBundle.h>
#include <MooseExceptions.h>
#include <string>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Core;
using namespace Moose::Graphics;
using namespace Moose::Exceptions;
///////////////////////////////////////////////////////////////////////////////
TEST( AssetBundle_throw )
{  
  CHECK_THROW( CAssetBundle::GetInstance()->GetAssetPath("hihihi"), CNoSuchAssetException);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AssetBundle_found )
{
  CHECK_EQUAL( CAssetBundle::GetInstance()->GetAssetPath("Shaders/skybox.vert"), std::string("/usr/share/Moose/Assets/Shaders/skybox.vert"));
}
////////////////////////////////////////////////////////////////////////////////
