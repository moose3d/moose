#ifndef __MooseExceptions_h__
#define __MooseExceptions_h__
/////////////////////////////////////////////////////////////////
#include <exception>
#include <string>
#include <sstream>
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
        std::string m_Reason;
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
      NAME( const std::string & s ) : Moose::Exceptions::CMooseException(s.c_str()) {}\
      ~NAME() throw() {};\
        template<class T> NAME & operator<<( const T & val ) \
        {\
            std::ostringstream s;\
            s << m_Reason;\
            s << val;\
            m_Reason = s.str();\
            return *this;\
        }\
    };
      DECL_MOOSE_EXCEPTION( CMooseRuntimeError );
      DECL_MOOSE_EXCEPTION( CNullPointerException );
      DECL_MOOSE_EXCEPTION( CRenameException );
      DECL_MOOSE_EXCEPTION( CNoSuchAssetException );
      DECL_MOOSE_EXCEPTION( CAlreadyExistsException );
  }
}
/////////////////////////////////////////////////////////////////
#endif
