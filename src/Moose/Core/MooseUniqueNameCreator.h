/*
 * MooseUniqueNameCreator.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSEUNIQUENAMECREATOR_H_
#define MOOSEUNIQUENAMECREATOR_H_
#include "MooseAPI.h"
#include "MooseSingleton.h"
#include "MooseTimer.h"
namespace Moose
{
	namespace Core
	{
		class MOOSE_API CUniqueNameCreator : public CSingleton<CUniqueNameCreator>,
											   protected CTimer
		{
		  friend class Moose::Core::CSingleton<CUniqueNameCreator>;
		private:
		  size_t m_nCount;
		  CUniqueNameCreator() : m_nCount(0){}
		public:
		  std::string GetUniqueName( const char *szPrefix = NULL )
		  {
		std::ostringstream name;
		Reset();
		Moose::Core::CTimeStamp time = GetTime();
		if ( szPrefix )
		  name << szPrefix;
		else
		  name << "__MOOSE__";
		name << time.GetSeconds() << time.GetMilliSeconds() << "_" << m_nCount++ ;
		return name.str();
		  }
		};
	}
}
#endif /* MOOSEUNIQUENAMECREATOR_H_ */
