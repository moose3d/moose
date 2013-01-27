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

#ifndef __MooseLogger_h__
#define __MooseLogger_h__
/////////////////////////////////////////////////////////////////
#include "MooseSingleton.h"
#include "MooseLog.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    /////////////////////////////////////////////////////////////////
    /// Logger facility.
    class CLogger : public Moose::Core::CSingleton<CLogger>
    {
      friend class Moose::Core::CSingleton<CLogger>;
    private:
      static Moose::Core::CDefaultLog m_Default;
    private:
      Moose::Core::CLog * m_pMessages;
      Moose::Core::CLog * m_pErrors;
      Moose::Core::CLog * m_pWarnings;
    private:
      CLogger();
    public:
      ////////////////////
      /// Returns error stream.
      /// \returns Error stream.
      inline std::ostream & Err()  { return m_pErrors->Stream();   }
      ////////////////////
      /// Returns message stream.
      /// \returns Message stream.
      inline std::ostream & Msg()  { return m_pMessages->Stream(); }
      ////////////////////
      /// Returns warning stream.
      /// \returns Warning stream.
      inline std::ostream & Warn() { return m_pWarnings->Stream(); }
      ////////////////////
      /// Sets error log.
      /// \param log CLog pointer. Passing NULL defaults log to system default log.
      inline void SetErr( Moose::Core::CLog *pLog )  { m_pErrors = pLog ? pLog : &CLogger::m_Default; }
      ////////////////////
      /// Sets message log.
      /// \param log CLog pointer. Passing NULL defaults log to system default log.
      inline void SetMsg( Moose::Core::CLog *pLog )  { m_pMessages = pLog ? pLog : &CLogger::m_Default; }
      ////////////////////
      /// Sets warning log.
      /// \param log CLog pointer. Passing NULL defaults log to system default log.
      inline void SetWarn( Moose::Core::CLog *pLog ) { m_pWarnings = pLog ? pLog : &CLogger::m_Default; }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
