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

#ifndef __MooseAssetBundle_h__
#define __MooseAssetBundle_h__
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <set>
#include <MooseSingleton.h>
#include <MooseExceptions.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    ////////////////////
    /// Utility to get full asset paths just by typing
    /// asset filename.
    class CAssetBundle : public Moose::Core::CSingleton<Moose::Core::CAssetBundle>
    {
      friend class Moose::Core::CSingleton<Moose::Core::CAssetBundle>;
    protected:
      std::set<std::string> m_setPrefices;
      CAssetBundle();
      virtual ~CAssetBundle();
    public:
      void AddAssetRoot( const std::string & dir );
      void RemoveAssetRoot( const std::string & dir );
      std::string GetAssetPath( const std::string & assetFile ) throw (Moose::Exceptions::CNoSuchAssetException);
      std::string GetAssetPaths() const;
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
