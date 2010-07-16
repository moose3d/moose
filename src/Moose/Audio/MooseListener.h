#ifndef __CListener_h__
#define __CListener_h__
/////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MooseOrientable.h"
#include "MoosePositional.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
#ifndef SWIG
    class CAudioSystem;
#endif
    class MOOSE_API CListener : public Moose::Spatial::COrientable,
		      public Moose::Spatial::CPositional
    {
#ifndef SWIG
        friend class CAudioSystem;
#endif
    protected:
      Moose::Math::CVector3<float> m_vVelocity;

    public:
      CListener();
      virtual ~CListener();
      void SetVelocity(  const Moose::Math::CVector3<float> & vDir );
      const Moose::Math::CVector3<float> & GetVelocity() const;
      void CommitChanges();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
