#ifndef PHOENIXDIMENSIONAL3D_H_
#define PHOENIXDIMENSIONAL3D_H_
/////////////////////////////////////////////////////////////
#include <PhoenixAPI.h>
#include <PhoenixDimensional2D.h>
namespace Phoenix
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
     /// A class which provides dimensions in 3D - width, height and length.
     class PHOENIX_API CDimensional3D : public CDimensional2D
     {
     protected:
       /// The length.
       float m_fLength;
       /// Length * 0.5
       float m_fHalfLength;
       ////////////////////
       /// The constructor. Initializes paramaters.
       CDimensional3D() :
 	CDimensional2D(),
 	m_fLength(0.0f),
 	m_fHalfLength(0.0f)
       {

       }
       ////////////////////
       /// The parametrized constructor.
       /// \param fWidth The width of the object
       /// \param fHeight The height of the object
       /// \param fLength The length of the object
       CDimensional3D( float fWidth, float fHeight, float fLength )
       {
 	SetWidth(fWidth);
 	SetHeight(fHeight);
 	SetLength(fLength);
       }
     public:
       ////////////////////
       /// Assigns the length.
       /// \param fLength The value for length.
       inline void SetLength( float fLength )
       {
 	m_fLength = fLength;
 	m_fHalfLength = m_fLength * 0.5f;
       }
       ////////////////////
       /// Returns the length.
       /// \returns The length.
       inline float GetLength() const
       {
 	return m_fLength;
       }
       ////////////////////
       /// Returns half of the length
       /// \returns half of the length
       inline float GetHalfLength() const
       {
 	return m_fHalfLength;
       }
     }; // CDimensional3D
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXDIMENSIONAL3D_H_ */
