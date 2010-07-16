#ifndef __MOOSEDIMENSIONAL2D_H__
#define __MOOSEDIMENSIONAL2D_H__
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseDimensional1D.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
     /// A class which provides dimensions in 2D - width, height.
     class MOOSE_API CDimensional2D : public CDimensional1D
     {
     protected:
       /// The height.
       float m_fHeight;
       /// Height * 0.5
       float m_fHalfHeight;
       ////////////////////
       /// The constructor. Initializes paramaters.
       CDimensional2D() :
 	CDimensional1D(),
 	m_fHeight(0.0f),
 	m_fHalfHeight(0.0f)
       {

       }
       ////////////////////
       /// The parametrized constructor.
       /// \param fWidth The width of the object
       /// \param fHeight The height of the object
       CDimensional2D( float fWidth, float fHeight)
       {
 	SetWidth(fWidth);
 	SetHeight(fHeight);
       }
     public:
       ////////////////////
       /// Assigns the height.
       /// \param fHeight The value for height.
       inline void SetHeight( float fHeight )
       {
 	m_fHeight = fHeight;
 	m_fHalfHeight = m_fHeight * 0.5f;
       }
       ////////////////////
       /// Returns the height.
       /// \returns The height.
       inline float GetHeight() const
       {
 	return m_fHeight;
       }
       ////////////////////
       /// Returns half of the height
       /// \returns half of the height
       inline float GetHalfHeight() const
       {
 	return m_fHalfHeight;
       }
     }; // CDimensional2D
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEDIMENSIONAL2D_H_ */
