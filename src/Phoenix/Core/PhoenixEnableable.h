
#ifndef PHOENIXENABLEABLE_H_
#define PHOENIXENABLEABLE_H_
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  	  class PHOENIX_API CEnableable
        {
        private:
          /// Is object enabled.
          bool		m_bEnabled;
        public:
          ////////////////////
          /// Default constructor. Disables object.
          CEnableable() : m_bEnabled(0) { }

          virtual ~CEnableable() {}
          ////////////////////
          /// Returns is object enabled or disabled.
          /// \returns true for enabled, false for disabled.
          bool IsEnabled() const {   return m_bEnabled; }
          ////////////////////
          /// Sets object enabled or disabled.
          /// \param bFlag Boolean flag - true for enabling, false for disabling.
          void SetEnabled(bool bFlag ) { m_bEnabled = bFlag; }

          inline void operator=( bool bFlag ) { SetEnabled(bFlag); }
        };
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXENABLEABLE_H_ */
