#ifndef __CSoundBase_h__
#define __CSoundBase_h__
/////////////////////////////////////////////////////////////////
#include "MoosePositional.h"
#include "MooseOneDirectional.h"
#include "MooseVector3.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CSoundBase : public Moose::Spatial::CPositional,
				   public Moose::Spatial::COneDirectional
    {
    protected:
      Moose::Math::CVector3<float> m_vVelocity;
    protected:
      CSoundBase();
      virtual ~CSoundBase() {}
    public:
      virtual void Play() = 0;
      virtual bool IsPlaying() const = 0;
      virtual bool IsLooping() = 0;
      virtual void SetLooping( bool flag) = 0;
      void SetVelocity(  const Moose::Math::CVector3<float> & vDir );
      const Moose::Math::CVector3<float> & GetVelocity() const;
  
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
