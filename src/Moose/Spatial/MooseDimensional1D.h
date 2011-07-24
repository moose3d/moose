#ifndef __MOOSEDIMENSIONAL1D_H__
#define __MOOSEDIMENSIONAL1D_H__
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  ////////////////////
  /// A class which provides dimensions in 1D (or n-dimensional object with equivalent size in all dimensions.
  class MOOSE_API CDimensional1D
  {
  protected:
    /// The width.
    float m_fWidth;
    /// Width * 0.5
    float m_fHalfWidth;
    ////////////////////
    /// Constructor. Initializes paramaters.
    CDimensional1D() :
	m_fWidth(0.0f),
	m_fHalfWidth(0.0f)
    {

    }
    ////////////////////
    /// The parametrized constructor.
    /// \param fWidth The width of the object
    CDimensional1D( float fWidth )
    {
	SetWidth(fWidth);
    }
  public:
    virtual ~CDimensional1D(){}
    ////////////////////
    /// Assigns the width.
    /// \param fWidth The value for width.
    inline void SetWidth ( float fWidth  )
    {
	m_fWidth = fWidth;
	m_fHalfWidth = m_fWidth * 0.5f;
    }
    ////////////////////
    /// Returns the width.
    /// \returns The width.
    inline float GetWidth() const
    {
	return m_fWidth;
    }
    ////////////////////
    /// Returns half of the width
    /// \returns half of the width
    inline float GetHalfWidth() const
    {
	return m_fHalfWidth;
    }

  }; // CDimensional1D
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEONEDIMENSIONAL_H_ */
