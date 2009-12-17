
#ifndef PHOENIXFOCUSABLE_H_
#define PHOENIXFOCUSABLE_H_
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  class PHOENIX_API CFocusable
  {
  private:
    /// Does this element have current focus.
    bool	m_bHasFocus;
  public:
    ////////////////////
    /// Constructor. Unfocuses object.
    CFocusable() : m_bHasFocus(0) {}
    virtual ~CFocusable() {}
    ////////////////////
    /// Checks is this element focused.
    /// \returns Non-zero on focus, zero otherwise.
    bool HasFocus() const { return m_bHasFocus;  }
    ////////////////////
    /// Sets focus flag.
    /// \param bFlag Non-zero for focus on, 0 for unfocus.
    void SetFocus( bool bFlag ) { m_bHasFocus = bFlag; }
  };
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXFOCUSABLE_H_ */
