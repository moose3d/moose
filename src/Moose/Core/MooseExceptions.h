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
    ////////////////////
    /// Base moose exception.
    class MOOSE_API CMooseException : public std::exception
    {
    protected:
      std::string reason;
      CMooseException( const char *szReason );
    public:
      ~CMooseException() throw() {};
      const char *what() const throw();
    };
    ////////////////////
    /// Null pointer exception.
    class MOOSE_API CNullPointerException : public Moose::Exceptions::CMooseException
    {
    public:
      CNullPointerException( const char *szReason ) : Moose::Exceptions::CMooseException(szReason) {}
      ~CNullPointerException() throw() {};
    };
    ////////////////////
    /// Renaming exception.
    class MOOSE_API CRenameException : public Moose::Exceptions::CMooseException
    {
    public:
      CRenameException( const char *szReason ) : Moose::Exceptions::CMooseException(szReason) {}
      ~CRenameException() throw() {};
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
