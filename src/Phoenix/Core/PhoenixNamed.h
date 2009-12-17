
#ifndef PHOENIXNAMED_H_
#define PHOENIXNAMED_H_
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
  ///////////////////
  /// Property class for names.
  class PHOENIX_API CNamed
  {
  protected:
    std::string m_strName;  ///<! Name of this gameobject
    CNamed() {}
  public:
    ///////////////////
    /// Accessor for name.
    /// \returns name of this gameobject.
    const std::string & GetName() const
    {
	return m_strName;
    }
    ////////////////////
    /// Assigns name for this gameobject.
    /// \param name New name
    void SetName( const std::string & name )
    {
  	  m_strName = name;
    }
  }; // CNamed
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXNAMED_H_ */
