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
