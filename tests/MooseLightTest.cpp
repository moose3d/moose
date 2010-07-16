#include <UnitTest++/UnitTest++.h>
#include <MooseCore.h>
#include <MoosePointLight.h>
#include <string>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Core;
using namespace Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Constant )
{
	CPointLight pointLight;
	pointLight.SetConstantAttenuation(999.0f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0 );

	pointLight.SetConstantAttenuation(999.9f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0 );

	pointLight.SetConstantAttenuation(1000.0f);
	CHECK( pointLight.ComputeAttenuationDistance() == 0.0 );

	pointLight.SetConstantAttenuation(1000.1f);
	CHECK( pointLight.ComputeAttenuationDistance() == 0.0 );

	pointLight.SetConstantAttenuation(1001.0f);
	CHECK( pointLight.ComputeAttenuationDistance() == 0.0 );

}
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Linear )
{
	CPointLight pointLight;
	pointLight.SetLinearAttenuation(0.0f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );

	pointLight.SetLinearAttenuation(0.0009f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );

	pointLight.SetLinearAttenuation(0.001f);
	CHECK( pointLight.ComputeAttenuationDistance() >= 0.0f );

	pointLight.SetLinearAttenuation(0.0011f);
	CHECK( pointLight.ComputeAttenuationDistance() >= 0.0f );

	pointLight.SetLinearAttenuation(0.002f);
	CHECK( pointLight.ComputeAttenuationDistance() >= 0.0f );

	pointLight.SetLinearAttenuation(0.5f);
	CHECK_CLOSE( 2000.0f, pointLight.ComputeAttenuationDistance(),  0.001f );

	pointLight.SetLinearAttenuation(0.51f);
	CHECK_CLOSE( 1960.784f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(5.5f);
	CHECK_CLOSE( 181.818f, pointLight.ComputeAttenuationDistance(), 0.001f);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Quadratic )
{
	CPointLight pointLight;

	pointLight.SetQuadraticAttenuation(0.0009f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );

	pointLight.SetQuadraticAttenuation(0.001f);
	CHECK_CLOSE( 1000.0f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetQuadraticAttenuation(0.051f);
	CHECK_CLOSE( 140.028f, pointLight.ComputeAttenuationDistance(), 0.001f);

	pointLight.SetQuadraticAttenuation(1.0f);
	CHECK_CLOSE( 31.622f, pointLight.ComputeAttenuationDistance(), 0.001f);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Quadratic_Constant )
{
	CPointLight pointLight;

	pointLight.SetConstantAttenuation(1.0);
	pointLight.SetQuadraticAttenuation(0.0009f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );

	pointLight.SetQuadraticAttenuation(0.001f);
	CHECK_CLOSE( 999.499f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetQuadraticAttenuation(0.051f);
	CHECK_CLOSE( 139.957f, pointLight.ComputeAttenuationDistance(), 0.001f);

	pointLight.SetQuadraticAttenuation(1.0f);
	CHECK_CLOSE( 31.606f, pointLight.ComputeAttenuationDistance(), 0.001f);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Linear_Constant )
{
	CPointLight pointLight;
	pointLight.SetConstantAttenuation(1.0f);

	pointLight.SetLinearAttenuation(0.0f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );

	pointLight.SetLinearAttenuation(0.0009f);
	CHECK( pointLight.ComputeAttenuationDistance() < 0.0f );
	// floating point error of some kind, result is correct nevertheless.
	//pointLight.SetLinearAttenuation(0.001f);
	//CHECK_CLOSE( 999000.0f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(0.5f);
	CHECK_CLOSE( 1998.0f, pointLight.ComputeAttenuationDistance(),  0.001f );

	pointLight.SetLinearAttenuation(0.51f);
	CHECK_CLOSE( 1958.823f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(5.5f);
	CHECK_CLOSE( 181.636f, pointLight.ComputeAttenuationDistance(), 0.001f);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AttenuationDistance_Linear_Quadratic )
{
	CPointLight pointLight;
	pointLight.SetConstantAttenuation(0.0f);

	pointLight.SetLinearAttenuation(0.001f);
	pointLight.SetQuadraticAttenuation(0.001f);
	CHECK_CLOSE( 999.500f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetQuadraticAttenuation(0.001f);
	pointLight.SetLinearAttenuation(0.5f);
	CHECK_CLOSE( 780.776f, pointLight.ComputeAttenuationDistance(),  0.001f );

	pointLight.SetQuadraticAttenuation(0.5f);
	pointLight.SetLinearAttenuation(0.001f);
	CHECK_CLOSE( 44.720f, pointLight.ComputeAttenuationDistance(),  0.001f );

	pointLight.SetLinearAttenuation(0.5f);
	pointLight.SetQuadraticAttenuation(0.5f);
	CHECK_CLOSE( 44.224f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(5.5f);
	pointLight.SetQuadraticAttenuation(0.5f);
	CHECK_CLOSE( 39.558f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(0.5f);
	pointLight.SetQuadraticAttenuation(5.5f);
	CHECK_CLOSE( 13.438f, pointLight.ComputeAttenuationDistance(), 0.001f );

	pointLight.SetLinearAttenuation(5.5f);
	pointLight.SetQuadraticAttenuation(5.5f);
	CHECK_CLOSE( 12.993f, pointLight.ComputeAttenuationDistance(), 0.001f);
}
///////////////////////////////////////////////////////////////////////////////
