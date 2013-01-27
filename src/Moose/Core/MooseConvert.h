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

#ifndef __MooseConvert_h__
#define __MooseConvert_h__
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "MooseAPI.h"
///////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Core
	{
		/// Runtime exception for conversion.
		class MOOSE_API CBadConversion : public std::runtime_error {
		public:
			CBadConversion(const std::string& s)
				: std::runtime_error(s)
			{ }
		};
		/// Utility function to help conversions from string to other types.
		template<typename T> 
		inline T convertTo( const std::string & str )
		{
			std::istringstream input(str);
			T val;
			if (!(input >> val))
				throw CBadConversion("convertTo(\"" + str + "\")");
			return val;
		}
	}
}
///////////////////////////////////////////////////////////////
#endif
