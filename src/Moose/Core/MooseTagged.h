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

/////////////////////////////////////////////////////////////////
#ifndef __MooseTagged_h__
#define __MooseTagged_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Core
	{
		typedef unsigned int TAG;  /* To be used in CTagged*/
        enum TagCompare
        {
          NOT_USED, 	///< Tag comparison is not used (default) (all tag values match).
          EQUAL,    	///< Matches only if tag is equal to given value.
          NOT_EQUAL, 	///< Matches only if tag is not equal to given value.
          AND, 		///< Matches if binary AND between object tag and given value yields non-zero value.
          NOT_AND 	///< Maches if binary AND between object tag and given value yields zero value.
        };
		/////////////////////////////////////////////////////////////////
		/// Class for taggable objects. Tag is an integer value which can be used in setting
		/// certain properties for a game object.
		class MOOSE_API CTagged
		{
		public:
			/// Comparision flags used when comparing tags in Octree CollectObjects();
			
		private:
			TAG                        m_Tag; ///< Tag value.
		public:
			////////////////////
			/// By default, tag is 0.
			CTagged() : m_Tag(0) {}
			///////////////////
			virtual ~CTagged () {};
			////////////////////
			/// Returns tag.
			/// \returns tag reference.
			const TAG & GetTag() const { return m_Tag;}
			////////////////////
			/// Sets tag.
			/// \param tag TAG to be set.
			void SetTag( TAG tag ) { m_Tag = tag; }
		};
	} // Core
} // Moose
/////////////////////////////////////////////////////////////////
#endif /* MOOSETAGGED_H_ */
