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
#define DECL_MOOSE_EXCEPTION( NAME ) \
    \
    class MOOSE_API NAME : public Moose::Exceptions::CMooseException \
    {\
    public:\
      NAME( const char *szReason ) : Moose::Exceptions::CMooseException(szReason) {}\
      ~NAME() throw() {};\
    };
      
      DECL_MOOSE_EXCEPTION( CNullPointerException );
      DECL_MOOSE_EXCEPTION( CRenameException );
      DECL_MOOSE_EXCEPTION( CNoSuchAssetException );
      DECL_MOOSE_EXCEPTION( CAlreadyExistsException );
  }
}
/////////////////////////////////////////////////////////////////
#endif
