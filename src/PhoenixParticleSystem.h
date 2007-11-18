/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_particlesystems_h__
#define __GSE_particlesystems_h__
/////////////////////////////////////////////////////////////////
#include "GSE_globals.h"
#include "GSE_structures.h"
#include "GSE_geometry.h"
#include "GSE_Configuration.h"
#include <list>
#include <iostream>
/////////////////////////////////////////////////////////////////
using std::list;
using Geometry::GSE_Sphere;
using Geometry::GSE_AxisAlignedBox;
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
/// The Particle base class, contains properties of a single particle.
/////////////////////////////////////////////////////////////////
class GSE_Particle 
{
 public:
  /// The previous position in the world.
  GSE_Vector3 m_vOldPosition;
  /// The current position in the world.
  GSE_Vector3 m_vPosition;
  /// The direction and speed.
  GSE_Vector3 m_vVelocity;
  /// The vertex colors.
  GSE_Color   m_vColor;
  /// The energy of this particle.
  float	      m_fEnergy;
  /// The scaling factor for the particle shape defined in the ParticleSystem.
  float	      m_fSize;
  
};
/////////////////////////////////////////////////////////////////
/// Generic interface for all particlesystems.
/////////////////////////////////////////////////////////////////
class GSE_ParticleSystemBase
{
  public:
  /// Destructor.
  virtual ~GSE_ParticleSystemBase(){}
  /// Returns particle array.
  virtual const GSE_Particle * GetParticles() const = 0;
  /// Is any particles alive in the system.
  virtual const bool	IsAlive() const = 0;
  /// Updates particles.
  /// \param nPassedTimeInMs passed time in milliseconds.
  virtual void		Update( unsigned int nPassedTimeInMs ) = 0;
  /// Returns maximum number of particles in this system.
  virtual const size_t	GetMaxParticles() const = 0;
  /// Creates particles into the system.
  /// \param nNumParticles Number of particles to be created.
  /// \param vPosition initial position where particles are created.
  virtual void		Init(const size_t &nNumParticles, const GSE_Vector3 &vPosition) = 0;
  /// Returns the number of particles alive.
  virtual const size_t	GetAliveCount() const = 0;
  /// Returns bounding sphere for this system.
  virtual const GSE_Sphere & GetBoundingSphere() const = 0;
  /// Returns Axis-aligned box for this system.
  virtual const GSE_AxisAlignedBox & GetBoundingBox() const = 0;
};
/////////////////////////////////////////////////////////////////
/// Particle System template.
/////////////////////////////////////////////////////////////////
template <size_t SIZE, class InitializePolicy, class ActionPolicy, class ParticleType >
class GSE_ParticleSystem : public GSE_ParticleSystemBase
{
 private:
  /// Particle array
  ParticleType m_aParticles[SIZE];
  /// The number of particles that are alive in the system.
  size_t	m_nNumParticlesAlive;  

  /// The number of particles generated in one second. (not used).
  float		m_fThroughPut;
  /// The axis-aligned bounding box.
  GSE_AxisAlignedBox m_BBox;
  /// The bounding sphere.
  GSE_Sphere	     m_BSphere;
  
 public:
  /// Initialization policy.
  InitializePolicy m_InitializePolicy;
  /// Action policy.
  ActionPolicy     m_ActionPolicy;

  ///////////////////////////////////////////////////////////////////
  /// Default constructor.
  GSE_ParticleSystem()
  {
    ////////////////////
    m_nNumParticlesAlive = 0;
    m_BSphere.SetRadius(0.0f);
    ////////////////////
    m_BBox.SetWidth(0.0f);
    m_BBox.SetHeight(0.0f);
    m_BBox.SetLength(0.0f);
    
  }
  /////////////////////////////////////////////////////////////////
  /// Destructor.
  ~GSE_ParticleSystem() {}
  /////////////////////////////////////////////////////////////////
  inline const GSE_Sphere & GetBoundingSphere() const
  {
    return m_BSphere;
  }
  /////////////////////////////////////////////////////////////////
  inline const GSE_AxisAlignedBox & GetBoundingBox() const
  {
    return m_BBox;
  }
  /////////////////////////////////////////////////////////////////
  inline const size_t GetMaxParticles() const
  {
    return SIZE;
  }
  /////////////////////////////////////////////////////////////////
  inline const size_t GetAliveCount() const
  {
    return m_nNumParticlesAlive;
  }
  /////////////////////////////////////////////////////////////////
  /// Updates the particles with ActionPolicy. 
  /// Particles are ordered so that the living particles are in the front
  /// of particle array.
  void		Update( unsigned int nPassedTimeInMs )
  {
    m_ActionPolicy.Prepare( nPassedTimeInMs );
    for(unsigned int i = 0;i<GetAliveCount();)
    {
      ParticleType &p = m_aParticles[i];
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
      GSE_Vector3 vMaxValues( m_aParticles[0].m_vPosition );
      GSE_Vector3 vMinValues( m_aParticles[0].m_vPosition );
      GSE_Vector3 vPos;
      GSE_Vector3 vExtents;    
      float m_fMaxSize = m_aParticles[0].m_fSize;
      ////////////////////
      // Determine bounding volumes
      for(unsigned int i = 1;i<GetAliveCount();i++)
      {
	ParticleType &p = m_aParticles[i];
	if ( p.m_fSize > m_fMaxSize )
	{
	  m_fMaxSize = p.m_fSize;
	}
	//cerr << "particle pos : " << p.m_vPosition << endl;
	STORE_MAX_MIN(p.m_vPosition.m_pValues[GSE_Vector3::X], vMaxValues[GSE_Vector3::X], vMinValues[GSE_Vector3::X]);
	STORE_MAX_MIN(p.m_vPosition.m_pValues[GSE_Vector3::Y], vMaxValues[GSE_Vector3::Y], vMinValues[GSE_Vector3::Y]);
	STORE_MAX_MIN(p.m_vPosition.m_pValues[GSE_Vector3::Z], vMaxValues[GSE_Vector3::Z], vMinValues[GSE_Vector3::Z]);
      }
      m_fMaxSize *= 0.5f;
      /// Increase the volume so the all drawn polygons fit in
      vMaxValues[GSE_Vector3::X] += m_fMaxSize;
      vMaxValues[GSE_Vector3::Y] += m_fMaxSize;
      vMaxValues[GSE_Vector3::Z] += m_fMaxSize;
      vMinValues[GSE_Vector3::X] -= m_fMaxSize;
      vMinValues[GSE_Vector3::Y] -= m_fMaxSize;
      vMinValues[GSE_Vector3::Z] -= m_fMaxSize;
      
      vExtents = vMaxValues-vMinValues;
      vPos = vMinValues + vExtents*0.5f;
      m_BSphere.SetRadius( vExtents.Length()*0.5f);
      m_BSphere.SetPosition( vPos );
      m_BBox.SetPosition( vPos );
      m_BBox.SetWidth(  vExtents[GSE_Vector3::X] );
      m_BBox.SetHeight( vExtents[GSE_Vector3::Y] );
      m_BBox.SetLength( vExtents[GSE_Vector3::Z] );  
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
  /// The accessor method for particle array
  inline const ParticleType *	GetParticles() const
  {
    return m_aParticles;
  }
  /////////////////////////////////////////////////////////////////
  /// Intializes the system with nNumParticles 
  void	Init(const size_t &nNumParticles, const GSE_Vector3 &vPosition )
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
  /// true if system has living particles ( energy > 0 ), false otherwise.
  inline const bool IsAlive() const
  {
    return ( m_nNumParticlesAlive > 0 );
  }
  /////////////////////////////////////////////////////////////////
}; // GSE_ParticleSystem

/////////////////////////////////////////////////////////////////
/// The particlesystem manager class. Manages several particlesystems.
/////////////////////////////////////////////////////////////////
class GSE_ParticleSystemMgr : public GSE_Container<GSE_ParticleSystemBase>,
			      public GSE_Singleton<GSE_ParticleSystemMgr>
{
  friend class GSE_Singleton<GSE_ParticleSystemMgr>;
 private:
  // Constructor
  GSE_ParticleSystemMgr(){}
  // Destructor
  ~GSE_ParticleSystemMgr(){}
  
 public:
  
};// GSE_ParticleSystemMgr
/////////////////////////////////////////////////////////////////
// Policies for ParticleSystems.

/////////////////////////////////////////////////////////////////
/// CompletePolicy for handling size, color, velocity, energy, and position.
template < class ParticleType, class ColorPolicy, class SizePolicy, class VelocityPolicy, 
	   class EnergyPolicy, class PositionPolicy>
class GSE_CompletePolicy
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
  inline void operator()( ParticleType & particle ) 
  {
    m_SizePolicy(particle);
    m_VelocityPolicy(particle);
    m_EnergyPolicy(particle);
    m_PositionPolicy(particle);
    m_ColorPolicy(particle);
  }
}; // end of GSE_CompletePolicy
/////////////////////////////////////////////////////////////////

// Individual policies:

/////////////////////////////////////////////////////////////////
/// Template for GravityAction.
template <class ParticleType>
class GSE_GravityAction
{
 private:
  /// Gravity values.
  GSE_Vector3 m_vGravity;
  /// Passed time in seconds.
  float m_fPassedTimeInSec;
 public:
  /// Default constructor.
  GSE_GravityAction() : m_vGravity(0,-9.81f,0),m_fPassedTimeInSec(0.0f) {};
  /// Prepares this policy according to given time parameter.
  /// \param nPassedTimeInMS Passed time in milliseconds.
  inline void Prepare( unsigned int nPassedTimeInMS )
  {
    m_fPassedTimeInSec = (float)nPassedTimeInMS * 0.001f;
  }
  /// Modifies given particle.
  /// \param particle a particle which values are to be modified.
  inline void operator()(ParticleType &particle ) 
  {
    particle.m_vVelocity += m_vGravity * m_fPassedTimeInSec;
  }
  /// Sets gravity vector.
  /// \param vDir a gravity vector.
  inline void SetGravity( const GSE_Vector3 &vDir )
  {
    m_vGravity = vDir;
  }

}; // end of GSE_GravityAction
/////////////////////////////////////////////////////////////////
// Template for MoveAction.
template <class ParticleType>
class GSE_MoveAction
{
 private:
  /// Passed time in milliseconds.
  float m_fPassedTimeInSec;
 public:
  /// Default constructor.
  GSE_MoveAction() : m_fPassedTimeInSec(0.0f){}
  /// Prepares this policy according to given time parameter.
  /// \param nPassedTimeInMS Passed time in milliseconds.
  inline void Prepare( unsigned int nPassedTimeInMS )
  {
    m_fPassedTimeInSec = (float)nPassedTimeInMS * 0.001f;
  }
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(ParticleType &p) const
  {
    //cerr << p.m_vPosition << "|" << p.m_vVelocity << "|" << m_fPassedTimeInSec <<endl;
    p.m_vPosition += p.m_vVelocity * m_fPassedTimeInSec;
    //cerr <<  "+=" << p.m_vVelocity * m_fPassedTimeInSec <<endl;
  } 
}; // end of GSE_MoveAction
/////////////////////////////////////////////////////////////////
/// Template for SizeInitializer.
template <class ParticleType>
class GSE_SizeInitializer
{
 public:
  /// Size value.
  float m_fSize;
  /// Default constructor.
  GSE_SizeInitializer() : m_fSize(1.0f){}
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline const void operator()(ParticleType &p) const
  {
    p.m_fSize = m_fSize;
  }
}; // end of GSE_SizeInitializer
/////////////////////////////////////////////////////////////////
/// Template for policy which does not do a thing. 
/// Can be used when some policy will not matter.
template <class ParticleType>
class GSE_NullPolicy
{
 public:
  /// Prepares this policy according to given time parameter.
  /// \param nPassedTimeInMS Passed time in milliseconds.
  inline void Prepare( unsigned int nPassedTimeInMS ){}
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(ParticleType &p) const {}
}; // end of GSE_NullPolicy
/////////////////////////////////////////////////////////////////
template <class ParticleType>
class GSE_EnergyInitializer
{
 public:
  /// Default constructor.
  GSE_EnergyInitializer(){}
  /// Prepares this policy according to given time parameter.
  /// \param nPassedTimeInMS Passed time in milliseconds.
  inline void Prepare( unsigned int nPassedTimeInMS ){}
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(ParticleType &p) const
  {
    p.m_fEnergy = 1.0f;
  } 
};
/////////////////////////////////////////////////////////////////
// A velocity initializer for particle system
template <class ParticleType>
class GSE_VelocityInitializer
{

 public:
  /// Velocity vector.
  GSE_Vector3 m_vVelocity ;
  /// Default constructor.
  GSE_VelocityInitializer() :m_vVelocity(0,0,0) {}
  /// Prepares this policy according to given time parameter.
  /// \param nPassedTimeInMS Passed time in milliseconds.
  inline void Prepare( unsigned int nPassedTimeInMS ){}
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(ParticleType &p) const
  {
    p.m_vVelocity = m_vVelocity;
  } 
};
/////////////////////////////////////////////////////////////////
/// A color initializer for particle system.
template <class ParticleType>
class GSE_ColorInitializer
{
 public:
  /// Color value.
  GSE_Color m_vColor;
  /// Default constructor.
  GSE_ColorInitializer() {}
  /// Modifies given particle.
  /// \param p particle which values are to be modified.
  inline void operator()(ParticleType &p) const
  {
    p.m_vColor = m_vColor;
  }
};
/////////////////////////////////////////////////////////////////
#endif
