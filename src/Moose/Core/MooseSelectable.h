
#ifndef MOOSESELECTABLE_H_
#define MOOSESELECTABLE_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
   class MOOSE_API CSelectable
   {
   private:
     /// Is object selected.
     bool		m_bSelected;
   public:
     ////////////////////
     /// Default constructor. Unselects object.
     CSelectable() : m_bSelected(0){ }
     virtual ~CSelectable() {}
     ////////////////////
     /// Returns is object selected or unselected.
     /// \returns true for selected, false for unselected.
     bool IsSelected() const {   return m_bSelected; }
     ////////////////////
     /// Sets object selected or unselected.
     /// \param bFlag Boolean flag - true for selecting, false for unselecting.
     void SetSelected( bool bFlag ) { m_bSelected = bFlag; }
   };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSESELECTABLE_H_ */
