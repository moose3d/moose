#ifndef __PhoenixAlphaTestOperation_h__
#define __PhoenixAlphaTestOperation_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixOGLConsts.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class grouping alpha testing properties.
    class CAlphaTestOperation : public Phoenix::Core::CEnableable
    {
    private:
      Phoenix::Graphics::ALPHA_TEST_TYPE m_tTestType;
      float	      m_fRefValue;
    public:
      ////////////////////
      /// Constructor.
      CAlphaTestOperation() : m_tTestType(Phoenix::Graphics::ALPHA_ALWAYS), m_fRefValue(0.0f) {}
      ////////////////////
      /// Assigns test type.
      /// \param tType ALPHA_TEST_TYPE.
      inline void SetTest( Phoenix::Graphics::ALPHA_TEST_TYPE tType )
      { 
	m_tTestType = tType; 
      }
      ////////////////////
      /// Returns current alpha test.
      /// \returns Current ALPHA_TEST_TYPE.
      inline Phoenix::Graphics::ALPHA_TEST_TYPE GetTest() const 
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
