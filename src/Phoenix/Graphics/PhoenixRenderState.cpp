/*
 * PhoenixRenderState.cpp
 *
 *  Created on: Nov 6, 2009
 *      Author: entity
 */
#include "PhoenixRenderState.h"
#include "PhoenixConvert.h"
#include "PhoenixLogger.h"
#include "PhoenixDefaultEntities.h"
#include <iostream>
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
using namespace std;
///////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::ParseFrom( Phoenix::Graphics::NameValueMap & map )
{
	while( !map.empty() )
	{
		const std::string & key = map.begin()->first;
		const std::string & value = map.begin()->second;
		if      ( key == "UseLighting" ) GetLighting()   = convertTo<bool>( value );
		else if ( key == "DepthTest"  ) GetDepthTest()   = convertTo<bool>( value );
		else if ( key == "DepthWrite"  ) GetDepthWrite() = convertTo<bool>( value );
		else if ( key == "FaceCulling"  ) GetFaceCulling() = convertTo<bool>( value );
		else if ( key == "AlphaOperation") g_Error << "Alpha Not implemented" << endl;
		else if ( key == "BlendingOperation") g_Error << "Blending Not implemented" << endl;

		map.erase(map.begin());
	}
}
///////////////////////////////////////////////////////////////////////////////////

