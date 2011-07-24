/*
 * MooseTypeBase.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSESTATEBASE_H_
#define MOOSESTATEBASE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Core
    {
        /////////////////////////////////////////////////////////////////
        /// Template base for each state object.
        template <typename STATE>
        class MOOSE_API CStateBase
        {
        protected:
            /// Stores the type
            STATE m_Type;
        public:
            ////////////////////
            /// Assigns state.
            /// \param rType STATE to be set.
            void SetState( const STATE &rType ) { m_Type = rType;  }
            ////////////////////
            /// Return current state.
            /// \returns Currently set state.
            const STATE & GetState() const { return m_Type; }
        };
    } // Core
} // Moose
#endif /* MOOSESTATEBASE_H_ */
