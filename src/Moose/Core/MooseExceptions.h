#ifndef __MooseExceptions_h__
#define __MooseExceptions_h__
/////////////////////////////////////////////////////////////////
#include <exception>
#include <string>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Exceptions
  {
    class MOOSE_API CNullPointerException : public std::exception
    {
    private:
      std::string reason;
    public:
      CNullPointerException( const char *szReason ) ;
      ~CNullPointerException() throw() {};
      const char *what() const throw();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
