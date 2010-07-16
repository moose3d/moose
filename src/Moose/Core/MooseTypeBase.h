/*
 * MooseTypeBase.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSETYPEBASE_H_
#define MOOSETYPEBASE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
   /////////////////////////////////////////////////////////////////
    /// Template base for each typed object.
    template <typename TYPE>
    class MOOSE_API CTypeBase
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
} // Moose
#endif /* MOOSETYPEBASE_H_ */
