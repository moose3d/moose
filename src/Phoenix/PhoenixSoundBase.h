#ifndef __CSoundBase_h__
#define __CSoundBase_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
#include "PhoenixVector3.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class CSoundBase : public Phoenix::Spatial::CPositional,
		       public Phoenix::Spatial::COneDirectional
    {
    protected:
      Phoenix::Math::CVector3<float> m_vVelocity;
    protected:
      CSoundBase();
      virtual ~CSoundBase() {}
    public:
      virtual void Play() = 0;
      virtual bool IsPlaying() const = 0;
      virtual bool IsLooping() = 0;
      virtual void SetLooping( bool flag) = 0;
      void SetVelocity(  const Phoenix::Math::CVector3<float> & vDir );
      const Phoenix::Math::CVector3<float> & GetVelocity() const;
  
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
