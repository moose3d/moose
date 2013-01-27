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
 * MooseSingleton.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSESINGLETON_H_
#define MOOSESINGLETON_H_
#include "MooseAPI.h"
#include <cstdlib>
namespace Moose
{
	namespace Core
	{
	/////////////////////////////////////////////////////////////////
    /// Singleton template, which makes creating singleton objects easier.
    template <class T>
    class MOOSE_API CSingleton
    {

    private:
      /// Static pointer to only instance.
      static T *m_pSingleton;
      ////////////////////
      /// Copy constructor.
      CSingleton( const CSingleton &ref){}
    protected:
      ////////////////////
      /// Default constructor.
      CSingleton() {}
      ////////////////////
      /// Destructor.
      virtual ~CSingleton() {}
    public:
      ////////////////////
      /// Returns a pointer to this singleton.
      inline static T *GetInstance()
      {
	if ( m_pSingleton == NULL )
	{
	  m_pSingleton = new T();
	}
	return m_pSingleton;
      }
      /// Destroys this singleton.
      inline static void DestroyInstance()
      {
	if ( m_pSingleton != NULL )
	{
	  delete m_pSingleton;
	  m_pSingleton = NULL;
	}
      }
      /// Checks is this object instantiated
      static bool Exists()
      {
	return (m_pSingleton != NULL);
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Initialize the static member in the derived classes.
    template<class T> T *CSingleton<T>::m_pSingleton = NULL;
	}
}
#endif /* MOOSESINGLETON_H_ */
