/////////////////////////////////////////////////////////////////
#ifndef __PhoenixParticleSystem_h__
#define __PhoenixParticleSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixGlobals.h>
#include <PhoenixVector3.h>
#include <PhoenixVolume.h>
#include <PhoenixVertexDescriptor.h>
#include <PhoenixIndexArray.h>
#include <list>
#include <iostream>
namespace Phoenix 
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// The Particle base class, contains properties of a single particle.
    class CParticle 
    {
    public:
      ///  previous position in the world.
      Phoenix::Math::CVector3<float> m_vOldPosition;
      ///  current position in the world.
      Phoenix::Math::CVector3<float> m_vPosition;
      ///  direction and speed.
      Phoenix::Math::CVector3<float> m_vVelocity;
      ///  vertex colors.
      Phoenix::Math::CVector4<unsigned char>  m_vColor;
      /// The energy of this particle.
      float	      m_fEnergy;
      /// The scaling factor for the particle shape defined in the ParticleSystem.
      float	      m_fSize;
      
    };
    /////////////////////////////////////////////////////////////////
    /// Generic interface for all particlesystems.
    class IParticleSystemBase 
    {
    public:
      /// Destructor.
      virtual ~IParticleSystemBase(){}
      /// Returns particle array.
      virtual const CParticle * GetParticles() const = 0;
      /// Is any particles alive in the system.
      virtual const int	IsAlive() const = 0;
      /// Updates particles.
      /// \param nPassedTimeInMs passed time in milliseconds.
      virtual void		Update( unsigned int nPassedTimeInMs ) = 0;
      /// Returns maximum number of particles in this system.
      virtual const size_t	GetMaxParticles() const = 0;
      /// Creates particles into the system.
      /// \param nNumParticles Number of particles to be created.
      /// \param vPosition initial position where particles are created.
      virtual void		Init(const size_t &nNumParticles, const Phoenix::Math::CVector3<float> &vPosition) = 0;
      /// Returns the number of particles alive.
      virtual const size_t	GetAliveCount() const = 0;
      /// Returns bounding sphere for this system.
      virtual const Phoenix::Volume::CSphere & GetBoundingSphere() const = 0;
      /// Returns Axis-aligned box for this system.
      virtual const Phoenix::Volume::CAxisAlignedBox & GetBoundingBox() const = 0;
    };
    /////////////////////////////////////////////////////////////////
    /// Particle System.
    template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
    class CParticleSystem 
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
      ~CParticleSystem();
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
      /// \param nPassedTimeInMs Passed time in milliseconds.
      void Update( unsigned int nPassedTimeInMs );
      ////////////////////
      /// An accessor method for particle array
      /// \returns Pointer to particle array.
      const PARTICLE_TYPE * GetParticles() const;
      ////////////////////
      /// Intializes system.
      /// \param nNumParticles Number of particles to be created.
      /// \param vPosition Initial position for each particle.
      void Init(const size_t &nNumParticles, const Phoenix::Math::CVector3<float> &vPosition );
      /////////////////////////////////////////////////////////////////
      /// Checks is system alive.
      /// \returns Non-zero if system has living particles ( energy > 0 )
      /// \returns Zero otherwise.
      int IsAlive() const;
      /////////////////////////////////////////////////////////////////
    }; // CParticleSystem


    /////////////////////////////////////////////////////////////////
    // Policies for ParticleSystems.
    /////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    /// CompletePolicy for handling size, color, velocity, energy, and position.
    template < class PARTICLE_TYPE, class ColorPolicy, class SizePolicy, class VelocityPolicy, 
    class EnergyPolicy, class PositionPolicy>
    class CCompletePolicy
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
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeMS )
      {
	m_SizePolicy.Prepare(nPassedTimeMS);
	m_VelocityPolicy.Prepare(nPassedTimeMS);
	m_EnergyPolicy.Prepare(nPassedTimeMS);
	m_PositionPolicy.Prepare(nPassedTimeMS);
	m_ColorPolicy.Prepare(nPassedTimeMS);
      }
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
    /////////////////////////////////////////////////////////////////

    // Individual policies:

    /////////////////////////////////////////////////////////////////
    /// Template for GravityAction.
    template <class PARTICLE_TYPE>
    class CGravityAction
    {
    private:
      /// Gravity values.
      Phoenix::Math::CVector3<float> m_vGravity;
      /// Passed time in seconds.
      float m_fPassedTimeInSec;
    public:
      /// Default constructor.
      CGravityAction() : m_vGravity(0,-9.81f,0),m_fPassedTimeInSec(0.0f) {};
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeInMS )
      {
	m_fPassedTimeInSec = (float)nPassedTimeInMS * 0.001f;
      }
      /// Modifies given particle.
      /// \param particle a particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &particle ) 
      {
	particle.m_vVelocity += m_vGravity * m_fPassedTimeInSec;
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
    class CMoveAction
    {
    private:
      /// Passed time in milliseconds.
      float m_fPassedTimeInSec;
    public:
      /// Default constructor.
      CMoveAction() : m_fPassedTimeInSec(0.0f){}
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeInMS )
      {
	m_fPassedTimeInSec = (float)nPassedTimeInMS * 0.001f;
      }
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	//cerr << p.m_vPosition << "|" << p.m_vVelocity << "|" << m_fPassedTimeInSec <<endl;
	p.m_vPosition += p.m_vVelocity * m_fPassedTimeInSec;
	//cerr <<  "+=" << p.m_vVelocity * m_fPassedTimeInSec <<endl;
      } 
    }; // end of CMoveAction
    /////////////////////////////////////////////////////////////////
    /// Template for SizeInitializer.
    template <class PARTICLE_TYPE>
    class CSizeInitializer
    {
    public:
      /// Size value.
      float m_fSize;
      /// Default constructor.
      CSizeInitializer() : m_fSize(1.0f){}
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
    class CNullPolicy
    {
    public:
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeInMS ){}
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const {}
    }; // end of CNullPolicy
    /////////////////////////////////////////////////////////////////
    template <class PARTICLE_TYPE>
    class CEnergyInitializer
    {
    public:
      /// Default constructor.
      CEnergyInitializer(){}
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeInMS ){}
      /// Modifies given particle.
      /// \param p particle which values are to be modified.
      inline void operator()(PARTICLE_TYPE &p) const
      {
	p.m_fEnergy = 1.0f;
      } 
    };
    /////////////////////////////////////////////////////////////////
    // A velocity initializer for particle system
    template <class PARTICLE_TYPE>
    class CVelocityInitializer
    {

    public:
      /// Velocity vector.
      Phoenix::Math::CVector3<float> m_vVelocity ;
      /// Default constructor.
      CVelocityInitializer() :m_vVelocity(0,0,0) {}
      /// Prepares this policy according to given time parameter.
      /// \param nPassedTimeInMS Passed time in milliseconds.
      inline void Prepare( unsigned int nPassedTimeInMS ){}
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
    class CColorInitializer
    {
    public:
      /// Color value.
      Phoenix::Math::CVector4<unsigned char> m_vColor;
      /// Default constructor.
      CColorInitializer() {}
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
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::~CParticleSystem()
{

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
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::Update( unsigned int nPassedTimeInMs )
{
  m_ActionPolicy.Prepare( nPassedTimeInMs );
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
  if ( GetAliveCount() > 0 )
  {
    Phoenix::Math::CVector3<float> vMaxValues( m_aParticles[0].GetPosition() );
    Phoenix::Math::CVector3<float> vMinValues( m_aParticles[0].GetPosition() );
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
      STORE_MAX_MIN(p.GetPosition()[0], vMaxValues[0], vMinValues[0]);
      STORE_MAX_MIN(p.GetPosition()[1], vMaxValues[1], vMinValues[1]);
      STORE_MAX_MIN(p.GetPosition()[2], vMaxValues[2], vMinValues[2]);
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
inline const PARTICLE_TYPE *	
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::GetParticles() const
{
  return m_aParticles;
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
void
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::Init(const size_t &nNumParticles, const Phoenix::Math::CVector3<float> &vPosition )
{
  size_t nAmount = nNumParticles;
    
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
      m_aParticles[nCount].m_vPosition = vPosition;
      // Init policy takes care of additional positioning, if necessary.
      m_InitializePolicy(m_aParticles[nCount]);
    }
  }
  ////////////////////
  // This is a bit bad way to do this, but hopefully it will 
  // work. If done properly, we would need the size of the largest 
  // particle and use that.
  m_BSphere.SetRadius( 1.0f );
  m_BSphere.SetPosition( vPosition );
  m_BBox.SetPosition( vPosition );
    
  m_BBox.SetWidth(  5.0f );
  m_BBox.SetHeight( 5.0f );
  m_BBox.SetLength( 5.0f );
    
}
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class PARTICLE_TYPE >
inline int 
Phoenix::Graphics::CParticleSystem<SIZE,InitializePolicy, ActionPolicy, PARTICLE_TYPE >::IsAlive() const
{
  return ( GetAliveCount() > 0 );
}
/////////////////////////////////////////////////////////////////
#endif