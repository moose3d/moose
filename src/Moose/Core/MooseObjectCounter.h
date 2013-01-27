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

#ifndef __MooseObjectCounter_h__
#define __MooseObjectCounter_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    template <typename CountedType> 
    class MOOSE_API CObjectCounter { 
    private: 
      static size_t m_nCount;  // number of existing objects 
    protected: 
      ////////////////////
      /// Default constructor.
      CObjectCounter() 
      { 
	++CObjectCounter<CountedType>::m_nCount; 
      } 
      ////////////////////
      /// Copy constructor .
      CObjectCounter (CObjectCounter<CountedType> const&) 
      { 
	++CObjectCounter<CountedType>::m_nCount; 
      } 
      ////////////////////
      /// Destructor.
      ~CObjectCounter() 
      { 
	--CObjectCounter<CountedType>::m_nCount; 
      } 
    public: 
      ////////////////////
      /// Return number of existing objects.
      /// \returns amount of created objects with this type.
      static size_t GetAliveCount() 
      { 
	return CObjectCounter<CountedType>::m_nCount; 
      } 
    }; // CObjectCounter
  }; // Core
}; // Moose
///////////////////////////////////////////////////////////////// 
// initialize counter with zero .
template <typename CountedType> 
size_t Moose::Core::CObjectCounter<CountedType>::m_nCount = 0; 
/////////////////////////////////////////////////////////////////
#endif
