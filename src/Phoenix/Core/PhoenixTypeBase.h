/*
 * PhoenixTypeBase.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef PHOENIXTYPEBASE_H_
#define PHOENIXTYPEBASE_H_
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
   /////////////////////////////////////////////////////////////////
    /// Template base for each typed object.
    template <typename TYPE>
    class PHOENIX_API CTypeBase
    {
    protected:
      /// Stores the type
      TYPE m_Type;
    public:
      ////////////////////
      /// Assigns type.
      /// \param rType TYPE to be set.
      void SetType( const TYPE &rType ) { m_Type = rType;  }
      ////////////////////
      /// Return current type.
      /// \returns Currently set type.
      const TYPE & GetType() const { return m_Type; }
    };
  } // Core
} // Phoenix
#endif /* PHOENIXTYPEBASE_H_ */
