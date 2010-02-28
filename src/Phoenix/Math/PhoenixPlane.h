#ifndef PHOENIXPLANE_H_
#define PHOENIXPLANE_H_
/////////////////////////////////////////////////////////////
#include <PhoenixAPI.h>
#include <PhoenixVector3.h>
#include <PhoenixVector4.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
	  /////////////////////////////////////////////////////////////////
	  // The plane class, always normalized. ( || Normal || == 1 )
	  /////////////////////////////////////////////////////////////////
	  class PHOENIX_API CPlane : public Phoenix::Math::CVector4<float>
	  {
	  public:
		CPlane();
		CPlane( const Phoenix::Math::CVector3<float> &vNormal, float fD );
		CPlane( float fX, float fY, float fZ, float fD );
		~CPlane();
		void SetNormal( Phoenix::Math::CVector3<float> vNormal);
		void SetDistance( float fDistance );
		void Normalize();
		void Calculate( Phoenix::Math::CVector3<float> vNormal, const Phoenix::Math::CVector3<float> & vPoint );
#ifndef SWIG
		friend std::ostream& Phoenix::Math::operator<<( std::ostream &stream, const Phoenix::Math::CPlane & plane );
#endif
	  };
#ifndef SWIG
	  std::ostream& operator<<( std::ostream &stream, const Phoenix::Math::CPlane & plane );
#endif
  } // Math
} // Phoenix
/////////////////////////////////////////////////////////////
#endif /* PHOENIXPLANE_H_ */
