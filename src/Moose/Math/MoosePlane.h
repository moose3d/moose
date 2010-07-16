#ifndef MOOSEPLANE_H_
#define MOOSEPLANE_H_
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseVector3.h>
#include <MooseVector4.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
	  /////////////////////////////////////////////////////////////////
	  // The plane class, always normalized. ( || Normal || == 1 )
	  /////////////////////////////////////////////////////////////////
	  class MOOSE_API CPlane : public Moose::Math::CVector4<float>
	  {
	  public:
		CPlane();
		CPlane( const Moose::Math::CVector3<float> &vNormal, float fD );
		CPlane( float fX, float fY, float fZ, float fD );
		~CPlane();
		void SetNormal( Moose::Math::CVector3<float> vNormal);
		void SetDistance( float fDistance );
		void Normalize();
		void Calculate( Moose::Math::CVector3<float> vNormal, const Moose::Math::CVector3<float> & vPoint );
#ifndef SWIG
		friend std::ostream& Moose::Math::operator<<( std::ostream &stream, const Moose::Math::CPlane & plane );
#endif
	  };
#ifndef SWIG
	  std::ostream& operator<<( std::ostream &stream, const Moose::Math::CPlane & plane );
#endif
  } // Math
} // Moose
/////////////////////////////////////////////////////////////
#endif /* MOOSEPLANE_H_ */
