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
