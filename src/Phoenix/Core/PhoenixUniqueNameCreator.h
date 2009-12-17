/*
 * PhoenixUniqueNameCreator.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef PHOENIXUNIQUENAMECREATOR_H_
#define PHOENIXUNIQUENAMECREATOR_H_
#include "PhoenixAPI.h"
#include "PhoenixSingleton.h"
#include "PhoenixTimer.h"
namespace Phoenix
{
	namespace Core
	{
		class PHOENIX_API CUniqueNameCreator : public CSingleton<CUniqueNameCreator>,
											   protected CTimer
		{
		  friend class Phoenix::Core::CSingleton<CUniqueNameCreator>;
		private:
		  size_t m_nCount;
		  CUniqueNameCreator() : m_nCount(0){}
		public:
		  std::string GetUniqueName( const char *szPrefix = NULL )
		  {
		std::ostringstream name;
		Reset();
		Phoenix::Core::CTimeStamp time = GetTime();
		if ( szPrefix )
		  name << szPrefix;
		else
		  name << "__PHOENIX__";
		name << time.GetSeconds() << time.GetMilliSeconds() << "_" << m_nCount++ ;
		return name.str();
		  }
		};
	}
}
#endif /* PHOENIXUNIQUENAMECREATOR_H_ */
