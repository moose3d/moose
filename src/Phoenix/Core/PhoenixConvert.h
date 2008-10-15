#ifndef __PhoenixConvert_h__
#define __PhoenixConvert_h__
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "PhoenixAPI.h"
///////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Core
	{
		/// Runtime exception for conversion.
		class PHOENIX_API CBadConversion : public std::runtime_error {
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
