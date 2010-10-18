#ifndef __CNullable_h__
#define __CNullable_h__
#include "MooseAPI.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Core
	{
	  ////////////////////////////////////////////////////////////////
	  /// Nullable entity.
	  class MOOSE_API CNullable
	  {
	  private:
		  /// State
		  bool m_bIsNull;
	  public:
		  ////////////////////
		  /// Constructor.
		  CNullable() : m_bIsNull(true) { }

		  virtual ~CNullable() {}
		  ////////////////////
		  /// Assigns null state.
		  /// \param bNull Nullification state.
		  inline void SetNull(bool bNull) { m_bIsNull = bNull; }
		  ////////////////////
		  /// Returns nullification state.
		  /// \returns state.
		  inline bool IsNull() const { return m_bIsNull; }
	  };
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* CNULLABLE_H_ */