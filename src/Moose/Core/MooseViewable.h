
#ifndef MOOSEVIEWABLE_H_
#define MOOSEVIEWABLE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  class MOOSE_API CViewable
   {
   private:
   	bool m_bVisible;
   public:
   	CViewable() : m_bVisible(0){ }
   	virtual ~CViewable() {}
			////////////////////
			/// Returns is object visible or invisible.
			/// \returns true for visible, false for invisible.
			bool IsVisible() const {   return m_bVisible; }
			////////////////////
			/// Sets object visible or invisible.
			/// \param bFlag Boolean flag - true for visible, false for invisible.
			void SetVisible( bool bFlag ) { m_bVisible = bFlag; }

   };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEVIEWABLE_H_ */
