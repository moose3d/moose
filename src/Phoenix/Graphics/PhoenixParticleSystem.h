/////////////////////////////////////////////////////////////////
#ifndef __PhoenixParticleSystem_h__
#define __PhoenixParticleSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixGlobals.h>
#include <PhoenixVector3.h>
#include <PhoenixAABB.h>
#include <PhoenixSphere.h>
#include <PhoenixVertexDescriptor.h>
#include <PhoenixIndexArray.h>
#include "PhoenixAPI.h"
#include <list>
#include <iostream>
namespace Phoenix 
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// The Particle base class, contains properties of a single particle.
    struct PHOENIX_API CParticle 
    {
      virtual ~CParticle(){}
      ///  Current position in the world. 
      ///  \par previous position in the world is a special case, 
      ///  \par must be implemented in inherited class.
      Phoenix::Math::CVector3<float> m_vPosition;
      ///  Direction and speed.
      Phoenix::Math::CVector3<float> m_vVelocity;
      ///  Vertex colors.
      Phoenix::Math::CVector4<unsigned char>  m_vColor;
      ///  The energy of this particle.
      float	      m_fEnergy;
      ///  The scaling factor for the particle shape defined in the 
      ///  \par ParticleSystem.
      float	      m_fSize;
    };
    /////////////////////////////////////////////////////////////////
    /// Particle System.
    template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
    class PHOENIX_API CParticleSystem 
    {
    private:
      /// Particle array
      PARTICLE_TYPE m_aParticles[SIZE];
      /// The number of particles that are alive in the system.
      size_t	m_nNumParticlesAlive;  
      /// Number of particles generated in one second. (not used).
      float		m_fThroughPut;
      /// Axis-aligned bounding box.
      Phoenix::Volume::CAxisAlignedBox m_BBox;
      /// Bounding sphere.
      Phoenix::Volume::CSphere	     m_BSphere;
    private:
      ////////////////////
      /// Determines bounding volume for this system.
      void DetermineBoundingVolume();
    public:
      /// Initialization policy.
      InitializePolicy m_InitializePolicy;
      /// Action policy.
      ActionPolicy     m_ActionPolicy;
      ////////////////////
      /// Default constructor.
      CParticleSystem();
      ////////////////////
      /// Destructor.
      virtual ~CParticleSystem() {}
      ////////////////////
      /// Returns bounding sphere for this particle system.
      /// \returns Bounding sphere.
      const Phoenix::Volume::CSphere & GetBoundingSphere() const;
      ////////////////////
      /// Returns bounding box for this particle system.
      /// \returns Bounding box.
      const Phoenix::Volume::CAxisAlignedBox & GetBoundingBox() const;
      ////////////////////
      /// Returns maximum number of particles in this system.
      size_t GetMaxParticles() const;
      ////////////////////
      /// Returns number of particles alive in this system.
      size_t GetAliveCount() const;
      ////////////////////
      /// Updates the particles with ActionPolicy. 
      /// Particles are ordered so that the living particles are in the front
      /// of particle array.
      /// \param fSeconds Passed time in seconds.
      void Update( float fSecondsPassed );
      ////////////////////
      /// An accessor method for particle array
      /// \returns Pointer to particle array.
      const PARTICLE_TYPE * GetParticles() const;
      ////////////////////
      /// An accessor method for particle array
      /// \returns Pointer to particle array.
      PARTICLE_TYPE * GetParticles();
      ////////////////////
      /// Intializes system.
      /// \param nNumParticles Number of particles to be created.
      void Init(const size_t nNumParticles );
      /////////////////////////////////////////////////////////////////
      /// Checks is system alive.
      /// \returns Non-zero if system has living particles ( energy > 0 )
      /// \returns Zero otherwise.
      int IsAlive() const;
      /////////////////////////////////////////////////////////////////
    }; // CParticleSystem


    /////////////////////////////////////////////////////////////////
    // Policies for ParticleSystems - these babies make the systems
    // tick.
    /////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    /// CompletePolicy for handling size, color, velocity, energy, and position.
    template < class PARTICLE_TYPE, class ColorPolicy, class SizePolicy, class VelocityPolicy, 
    class EnergyPolicy, class PositionPolicy>
    class PHOENIX_API CCompletePolicy
    {
    public:
      /// Size policy.
      SizePolicy     m_SizePolicy;
      /// Velocity policy.
      VelocityPolicy m_VelocityPolicy;
      /// Energy policy.
      EnergyPolicy     m_EnergyPolicy;
      /// Position policy.
      PositionPolicy m_PositionPolicy;
      /// Color policy.
      ColorPolicy    m_ColorPolicy;
      ////////////////////
      /// Prepares this policy according to given time parameter.
      /// \param fSecondsPassed Passed time in seconds.
      inline void Prepare(  float fSecondsPassed )
      {
	m_SizePolicy.Prepare(fSecondsPassed);
	m_VelocityPolicy.Prepare( fSecondsPassed );
	m_EnergyPolicy.Prepare(fSecondsPassed);
	m_PositionPolicy.Prepare(fSecondsPassed);
	m_ColorPolicy.Prepare(fSecondsPassed);
      }
      ////////////////////
      /// Modifies given particle.
      /// \param particle a particle which values are to be modified.
      inline void operator()( PARTICLE_TYPE & particle ) 
      {
	m_SizePolicy(particle);
	m_VelocityPolicy(particle);
	m_EnergyPolicy(particle);
	m_PositionPolicy(particle);
	m_ColorPolicy(particle);
      }
    }; // end of CCompletePolicy
    // /////////////////////////////////////////////////////////////////
//     // Classe for measuring time difference within policies (using milliseconds). 
//     class PHOENIX_API CMillisecondPolicyBase 
//     {
//     protected:
//       /// Passed time in milliseconds
//       unsigned int m_nPassedTimeInMS;
//     public:
//       ////////////////////
//       /// Constructor.
//       CMillisecondPolicyBase() : m_nPassedTimeInMS(0) {}
//       ////////////////////
//       /// Prepares this policy according to given time parameter.
//       /// \param nPassedTimeInMS Passed milliseconds are stored as milliseconds.
//       inline void Prepare( unsigned int nPassedTimeInMS )
//       {
// 	m_nPassedTimeInMS = nPassedTimeInMS;
//       }
//       ////////////////////
//       /// Returns passed time.
//       /// \returns Passed time in milliseconds.
//       inline unsigned int GetPassedTimeMS() const
//       {
// 	return m_nPassedTimeInMS;
//       }
      
//     };
    /////////////////////////////////////////////////////////////////
    /// Class for measuring time difference within policies (using seconds).
    class PHOENIX_API CSecondPolicyBase
    {
    protected:
      /// Passed time in seconds
      float m_fPassedTimeInSec;
    public:
      ////////////////////
      /// Constructor.
      CSecondPolicyBase() : m_fPassedTimeInSec(0.0f) {}      
      ////////////////////
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed milliseconds are stored as seconds.
      inline void Prepare( float fSecondsPassed )
      {
	m_fPassedTimeInSec = fSecondsPassed;
      }
      ////////////////////
      /// Returns passed time.
      /// \returns Passed time in milliseconds.
      inline float GetPassedTime() const
      {
	return m_fPassedTimeInSec;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Class for measuring time difference within policies (using timestamp).
    class PHOENIX_API CTimeStampPolicyBase
    {
    protected:
      /// Passed time in seconds
      Phoenix::Core::CTimeStamp m_PassedTime;
    public:
      ////////////////////
      /// Constructor.
      CTimeStampPolicyBase()  { m_PassedTime.SetSeconds(0); m_PassedTime.SetMilliSeconds(0);}      
      ////////////////////
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed milliseconds are stored as seconds.
      inline void Prepare( const Phoenix::Core::CTimeStamp & rPassedTime )
      {
	m_PassedTime = rPassedTime;
      }
      ////////////////////
      /// Returns passed time.
      /// \returns Passed time.
      inline const Phoenix::Core::CTimeStamp & GetPassedTime() const
      {
	return m_PassedTime;
      }
    };
    /////////////////////////////////////////////////////////////////
    // Individual policies:
    /////////////////////////////////////////////////////////////////
    /// Template for GravityAction.
    template <class PARTICLE_TYPE>
    class PHOENIX_API CGravityAction : public CSecondPolicyBase
    {
    private:
      /// Gravity values.
      Phoenix::Math::CVector3<float> m_vGravity;
    public:
      ////////////////////
      /// Default constructor.
      CGravityAction() : m_vGravity(0,-9.81f,0) {};
      ////////////////////
      /// Modifies given particle.
      /// \param particle a particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &particle ) 
      {
	particle.m_vVelocity += m_vGravity * GetPassedTime();
      }
      /// Sets gravity vector.
      /// \param vDir a gravity vector.
      inline void SetGravity( const Phoenix::Math::CVector3<float> &vDir )
      {
	m_vGravity = vDir;
      }

    }; // end of CGravityAction
    /////////////////////////////////////////////////////////////////
    // Template for MoveAction.
    template <class PARTICLE_TYPE>
    class PHOENIX_API CMoveAction : public CSecondPolicyBase
    {
    public:
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	p.m_vPosition += p.m_vVelocity * GetPassedTime();
      } 
    }; // end of CMoveAction
    /////////////////////////////////////////////////////////////////
    /// Template for SizeInitializer.
    template <class PARTICLE_TYPE>
    class PHOENIX_API CSizeInitializer
    {
    public:
      /// Size value.
      float m_fSize;
      ////////////////////
      /// Default constructor.
      CSizeInitializer() : m_fSize(1.0f){}
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline const void operator()(PARTICLE_TYPE &p) const
      {
	p.m_fSize = m_fSize;
      }
    }; // end of CSizeInitializer
    /////////////////////////////////////////////////////////////////
    /// Template for policy which does not do a thing. 
    /// Can be used when some policy will not matter.
    template <class PARTICLE_TYPE>
    class PHOENIX_API CNullPolicy
    {
    public:
      ////////////////////
      /// Prepares this policy according to given time parameter.
      /// \param fSecondsPassed Passed time in seconds.
      inline void Prepare( float fSecondsPassed ){}
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const {}
    }; // end of CNullPolicy
    /////////////////////////////////////////////////////////////////
    template <class PARTICLE_TYPE>
    class PHOENIX_API CEnergyInitializer
    {
    public:
      /// Initial energy.
      float m_fEnergy;
      ////////////////////
      /// Default constructor.
      CEnergyInitializer() : m_fEnergy(1.0f){}
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	p.m_fEnergy = m_fEnergy;
      } 
    };
    /////////////////////////////////////////////////////////////////
    // A velocity initializer for particle system
    template <class PARTICLE_TYPE>
    class PHOENIX_API CVelocityInitializer
    {
    public:
      /// Velocity vector.
      Phoenix::Math::CVector3<float> m_vVelocity ;
      ////////////////////
      /// Default constructor.
      CVelocityInitializer() :m_vVelocity(0,0,0) {}
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	p.m_vVelocity = m_vVelocity;
      } 
    };
    /////////////////////////////////////////////////////////////////
    /// A color initializer for particle system.
    template <class PARTICLE_TYPE>
    class PHOENIX_API CColorInitializer
    {
    public:
      /// Color value.
      Phoenix::Math::CVector4<unsigned char> m_vColor;
      ////////////////////
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	p.m_vColor = m_vColor;
      }
    };
    /////////////////////////////////////////////////////////////////
  } // namespace Graphics
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::CParticleSystem()
{
  m_nNumParticlesAlive = 0;
  m_BSphere.SetRadius(0.0f);
  
  m_BBox.SetWidth(0.0f);
  m_BBox.SetHeight(0.0f);
  m_BBox.SetLength(0.0f);
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline const Phoenix::Volume::CSphere & 
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetBoundingSphere() const
{
  return m_BSphere;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline const Phoenix::Volume::CAxisAlignedBox & 
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetBoundingBox() const
{
  return m_BBox;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline size_t  
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetMaxParticles() const
{
  return SIZE;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline size_t  
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetAliveCount() const
{
  return m_nNumParticlesAlive;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
void
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::DetermineBoundingVolume()
{
  if ( GetAliveCount() > 0 )
  {
    Phoenix::Math::CVector3<float> vMaxValues( m_aParticles[0].m_vPosition );
    Phoenix::Math::CVector3<float> vMinValues( m_aParticles[0].m_vPosition );
    Phoenix::Math::CVector3<float> vPos;
    Phoenix::Math::CVector3<float> vExtents;    
    float m_fMaxSize = m_aParticles[0].m_fSize;
    ////////////////////
    // Determine bounding volumes
    for(unsigned int i = 1;i<GetAliveCount();i++)
    {
      PARTICLE_TYPE &p = m_aParticles[i];
      if ( p.m_fSize > m_fMaxSize )
      {
	m_fMaxSize = p.m_fSize;
      }
      //cerr << "particle pos : " << p.m_vPosition << endl;
      STORE_MAX_MIN(p.m_vPosition[0], vMaxValues[0], vMinValues[0]);
      STORE_MAX_MIN(p.m_vPosition[1], vMaxValues[1], vMinValues[1]);
      STORE_MAX_MIN(p.m_vPosition[2], vMaxValues[2], vMinValues[2]);
    }
    m_fMaxSize *= 0.5f;
    /// Increase the volume so the all drawn polygons fit in
    vMaxValues[0] += m_fMaxSize;
    vMaxValues[1] += m_fMaxSize;
    vMaxValues[2] += m_fMaxSize;
    vMinValues[0] -= m_fMaxSize;
    vMinValues[1] -= m_fMaxSize;
    vMinValues[2] -= m_fMaxSize;
      
    vExtents = vMaxValues-vMinValues;
    vPos = vMinValues + vExtents*0.5f;
    m_BSphere.SetRadius( vExtents.Length()*0.5f);
    m_BSphere.SetPosition( vPos );
    m_BBox.SetPosition( vPos );
    m_BBox.SetWidth(  vExtents[0] );
    m_BBox.SetHeight( vExtents[1] );
    m_BBox.SetLength( vExtents[2] );  
  }
  else
  {
    m_BSphere.SetRadius( 0.0f);
    m_BBox.SetWidth(  0.0f);
    m_BBox.SetHeight( 0.0f);
    m_BBox.SetLength( 0.0f);
  }
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
void
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::Update( float fSecondsPassed )
{
  m_ActionPolicy.Prepare( fSecondsPassed );
  for(unsigned int i = 0;i<GetAliveCount();)
  {
    PARTICLE_TYPE &p = m_aParticles[i];
    m_ActionPolicy(p);
    // Is particle is dead
    if ( p.m_fEnergy <= 0.0f && m_nNumParticlesAlive > 0)
    {
      // Replace dead particle with last known live one
      m_aParticles[i] = m_aParticles[m_nNumParticlesAlive - 1];
      m_nNumParticlesAlive--;
    } 
    else 
    {
      i++;
    }
  }
  // update bounds
  DetermineBoundingVolume();
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline const PARTICLE_TYPE *	
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetParticles() const
{
  return m_aParticles;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline PARTICLE_TYPE *	
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetParticles() 
{
  return m_aParticles;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
void
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::Init(const size_t nNumParticles )
{
  size_t nAmount = nNumParticles;
  float  fParticleMaxSize = 0.0f;
  if ( GetAliveCount() + nAmount > GetMaxParticles() )
  {
    nAmount = GetMaxParticles() - GetAliveCount();
  }
  ////////////////////
  /// Create the particles.
  if ( nAmount > 0 )
  {
    size_t nCount = GetAliveCount();
    m_nNumParticlesAlive += nAmount ;

    for(; nCount < GetAliveCount(); nCount++) 
    {
      // init policy must take care of all positioning, we might have different types of 
      // particles that require additional structures to be set (Such as tails of energy weapons)
      // m_aParticles[nCount].m_vPosition = vPosition;
      // Init policy takes care of positioning, if necessary.
      m_InitializePolicy(m_aParticles[nCount]);
      
      if ( m_aParticles[nCount].m_fSize > fParticleMaxSize)
      {
	fParticleMaxSize = m_aParticles[nCount].m_fSize; 
      }
    }
  }
  // update bounds
  DetermineBoundingVolume();
  
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline int 
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::IsAlive() const
{
  return ( GetAliveCount() > 0 );
}
/////////////////////////////////////////////////////////////////

/*     ///////////////////////////////////////////////////////////////// */
/*     /// Generic interface for all particlesystems. */
/*     class IParticleSystemBase  */
/*     { */
/*     public: */
/*       /// Destructor. */
/*       virtual ~IParticleSystemBase(){} */
/*       /// Returns particle array. */
/*       virtual const CParticle * GetParticles() const = 0; */
/*       /// Is any particles alive in the system. */
/*       virtual const int	IsAlive() const = 0; */
/*       /// Updates particles. */
/*       /// \param nPassedTimeInMs passed time in milliseconds. */
/*       virtual void		Update( unsigned int nPassedTimeInMs ) = 0; */
/*       /// Returns maximum number of particles in this system. */
/*       virtual const size_t	GetMaxParticles() const = 0; */
/*       /// Creates particles into the system. */
/*       /// \param nNumParticles Number of particles to be created. */
/*       /// \param vPosition initial position where particles are created. */
/*       virtual void		Init(const size_t &nNumParticles, const Phoenix::Math::CVector3<float> &vPosition) = 0; */
/*       /// Returns the number of particles alive. */
/*       virtual const size_t	GetAliveCount() const = 0; */
/*       /// Returns bounding sphere for this system. */
/*       virtual const Phoenix::Volume::CSphere & GetBoundingSphere() const = 0; */
/*       /// Returns Axis-aligned box for this system. */
/*       virtual const Phoenix::Volume::CAxisAlignedBox & GetBoundingBox() const = 0; */
/*     }; */
#endif
