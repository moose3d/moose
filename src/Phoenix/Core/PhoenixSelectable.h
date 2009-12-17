
#ifndef PHOENIXSELECTABLE_H_
#define PHOENIXSELECTABLE_H_
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
   class PHOENIX_API CSelectable
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
#endif /* PHOENIXSELECTABLE_H_ */
