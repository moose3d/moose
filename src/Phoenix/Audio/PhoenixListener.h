#ifndef __CListener_h__
#define __CListener_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVector3.h"
#include "PhoenixOrientable.h"
#include "PhoenixPositional.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
#ifndef SWIG
    class CAudioSystem;
#endif
    class PHOENIX_API CListener : public Phoenix::Spatial::COrientable,
		      public Phoenix::Spatial::CPositional
    {
#ifndef SWIG
        friend class CAudioSystem;
#endif
    protected:
      Phoenix::Math::CVector3<float> m_vVelocity;

    public:
      CListener();
      virtual ~CListener();
      void SetVelocity(  const Phoenix::Math::CVector3<float> & vDir );
      const Phoenix::Math::CVector3<float> & GetVelocity() const;
      void CommitChanges();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
