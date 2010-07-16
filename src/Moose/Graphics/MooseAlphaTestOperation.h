#ifndef __MooseAlphaTestOperation_h__
#define __MooseAlphaTestOperation_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseOGLConsts.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class grouping alpha testing properties.
    class MOOSE_API CAlphaTestOperation : public Moose::Core::CEnableable
    {
    private:
      Moose::Graphics::ALPHA_TEST_TYPE m_tTestType;
      float	      m_fRefValue;
    public:
      ////////////////////
      /// Constructor.
      CAlphaTestOperation() : m_tTestType(Moose::Graphics::ALPHA_ALWAYS), m_fRefValue(0.0f) {}
      ////////////////////
      /// Assigns test type.
      /// \param tType ALPHA_TEST_TYPE.
      inline void SetTest( Moose::Graphics::ALPHA_TEST_TYPE tType )
      { 
	m_tTestType = tType; 
      }
      ////////////////////
      /// Returns current alpha test.
      /// \returns Current ALPHA_TEST_TYPE.
      inline Moose::Graphics::ALPHA_TEST_TYPE GetTest() const 
      { 
	return m_tTestType; 
      }
      ////////////////////
      /// Assigns reference value.
      /// \param fValue Reference value to be set.
      inline void SetReference( float fValue )
      {
	m_fRefValue = fValue;
      }
      ////////////////////
      /// Returns reference value.
      /// \brief Returns current reference value.
      inline float GetReference () const
      {
	return m_fRefValue;
      }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
