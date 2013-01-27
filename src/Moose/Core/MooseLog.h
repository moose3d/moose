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

#ifndef __MooseLog_h__
#define __MooseLog_h__
/////////////////////////////////////////////////////////////////
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    /////////////////////////////////////////////////////////////////
    /// Abstraction for log.
    class CLog
    {
    protected:
      /// Messages are printed into this stream.
      std::ostream & m_Stream; 
    protected:
      /// Constructor. Sets default stream to cout
      CLog( std::ostream & stream = std::cout) : m_Stream(stream) { }
      /// Destructor.
      virtual ~CLog() {}
    public:
      ////////////////////
      /// Returns reference to current stream this log redirects messages to.
      /// \returns Reference to std::ostream.
      virtual std::ostream & Stream() { return m_Stream; }
    };
    
    ////////////////////
    /// Log directed to standard error stream (unbuffered).
    class CDefaultLog : public CLog
    {
    public:
      ////////////////////
      /// Constructor. Sets default stream to std::cerr.
      CDefaultLog() : Moose::Core::CLog( std::cerr ) {}
    };
  } // namespace Core
} // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
