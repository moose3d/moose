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

#include "MooseAssetBundle.h"
#include "MooseExceptions.h"
#include "MooseAPI.h"
#if defined(MOOSE_APPLE_IPHONE)
#import <Foundation/Foundation.h>
#endif
#include <fstream>
#include <sstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Core;
////////////////////////////////////////////////////////////////////////////////
prefix::CAssetBundle::CAssetBundle()
{
#if !defined(MOOSE_APPLE_IPHONE)
    AddAssetRoot("/usr/share/Moose/Assets");
    AddAssetRoot("/usr/share/Moose/Assets/Shaders");
#else
    AddAssetRoot( [[[NSBundle mainBundle] bundlePath] UTF8String]);
#endif
}
////////////////////////////////////////////////////////////////////////////////
prefix::CAssetBundle::~CAssetBundle()
{

}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CAssetBundle::AddAssetRoot( const std::string & dir )
{
  m_setPrefices.insert(dir);
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CAssetBundle::RemoveAssetRoot( const std::string & dir )
{
  std::set<std::string>::iterator pos = m_setPrefices.find(dir);
  if ( pos != m_setPrefices.end())
  {
    m_setPrefices.erase(pos);
  }
}
////////////////////////////////////////////////////////////////////////////////
std::string 
prefix::CAssetBundle::GetAssetPath( const std::string & assetFile ) throw (Moose::Exceptions::CNoSuchAssetException)
{
  std::set<std::string>::iterator it = m_setPrefices.begin();
  while ( it != m_setPrefices.end())
  {
    // Create full path
    ostringstream file;
    file << *it << "/" << assetFile;
    // Try to open
    ifstream s(file.str().c_str(), ifstream::in);
    // Was it readable?
    if ( s.good() ) {
      s.close();
      return file.str();
    }
    it++;
  }
  // no such asset
  throw (Moose::Exceptions::CNoSuchAssetException(assetFile.c_str()));  
}
////////////////////////////////////////////////////////////////////////////////
std::string 
prefix::CAssetBundle::GetAssetPaths() const
{

  ostringstream paths;
  std::set<std::string>::iterator it = m_setPrefices.begin();
  while ( it != m_setPrefices.end())
  {
    paths << *it << " ";
    it++;
  }
  return paths.str();
}
////////////////////////////////////////////////////////////////////////////////
