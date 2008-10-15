#ifndef __PhoenixLogger_h__
#define __PhoenixLogger_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixLog.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    /////////////////////////////////////////////////////////////////
    /// Logger facility.
    class CLogger : public Phoenix::Core::CSingleton<CLogger>
    {
      friend class Phoenix::Core::CSingleton<CLogger>;
    private:
      static Phoenix::Core::CDefaultLog m_Default;
    private:
      Phoenix::Core::CLog * m_pMessages;
      Phoenix::Core::CLog * m_pErrors;
      Phoenix::Core::CLog * m_pWarnings;
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
      inline void SetErr( Phoenix::Core::CLog *pLog )  { m_pErrors = pLog ? pLog : &CLogger::m_Default; }
      ////////////////////
      /// Sets message log.
      /// \param log CLog pointer. Passing NULL defaults log to system default log.
      inline void SetMsg( Phoenix::Core::CLog *pLog )  { m_pMessages = pLog ? pLog : &CLogger::m_Default; }
      ////////////////////
      /// Sets warning log.
      /// \param log CLog pointer. Passing NULL defaults log to system default log.
      inline void SetWarn( Phoenix::Core::CLog *pLog ) { m_pWarnings = pLog ? pLog : &CLogger::m_Default; }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
