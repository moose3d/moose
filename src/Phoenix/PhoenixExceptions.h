#ifndef __PhoenixExceptions_h__
#define __PhoenixExceptions_h__
/////////////////////////////////////////////////////////////////
#include <exception>
#include <string>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Exceptions
  {
    class PHOENIX_API CNullPointerException : public std::exception
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
