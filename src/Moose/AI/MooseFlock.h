#ifndef __MooseFlock_h__
#define __MooseFlock_h__
/////////////////////////////////////////////////////////////////
#include <vector>
#include <list>
#include "MooseVector3.h"
#include "MooseSphere.h"
#include "MooseAABB.h"
#include "MooseOneDirectional.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace AI
  {
    class CBoid;
    /////////////////////////////////////////////////////////////////
    typedef std::vector<CBoid *> BoidPtrVector;
    typedef std::list<CBoid *> BoidPtrList;
    /////////////////////////////////////////////////////////////////

    ////////////////////
    /// Flock (Herd, School, Swarm) of boids.
    class MOOSE_API CFlock 
    {
    private:
      /// List of boids in this flock ( each boid should belong at 
      /// most into one flock)
      BoidPtrVector			m_vecBoids;
      /// Target of this flock
      Moose::Math::CVector3<float>	m_vTarget;
      /// Target weight / importance
      float				m_fTargetWeight;
      /// Bounding sphere for this flock.
      Moose::Volume::CSphere		m_BoundingSphere;
      Moose::Volume::CAxisAlignedBox	m_BoundingBox;
    public:
      ////////////////////
      /// Constructor.
      CFlock();
      ////////////////////
      /// Destructor.
      ~CFlock();
      ////////////////////
      /// Adds new object into this flock.
      void AddToFlock(  CBoid *pBoid );
      ////////////////////
      /// Removes boid from this flock.
      void RemoveFromFlock(  CBoid * pBoid );
      ////////////////////
      /// Updates all boids.
      void Update();
      void SetTarget( const Moose::Math::CVector3<float> & vTargetPos );
      const Moose::Math::CVector3<float> & GetTarget() const;
      BoidPtrVector & GetBoids();
      Moose::Volume::CSphere & GetBoundingSphere();
    private:
      ////////////////////
      /// Determines which are the local neighbours for each boid
      void LocateNearestBoids( const CBoid *pBoid, BoidPtrList & lstClosest );
    };
    ////////////////////
    /// Autonomous flock member.
    class MOOSE_API CBoid : public Moose::Spatial::COneDirectional,
			      public Moose::Spatial::CPositional
    {
    private:
      /// Index in flock array
      int			m_iIndex;
      float			m_fSpeed;
      float			m_fMaxSpeed;
      Moose::Volume::CSphere			m_NeighborSphere; 
      Moose::Volume::CSphere			m_SeparationSphere;
    public:
      ////////////////////
      /// Constructor.
      CBoid();
      ////////////////////
      /// Destructor.
      virtual ~CBoid();
      ////////////////////
      /// Assigns boid location in the array.
      /// \param iIndex Location in flock array. Negative value 
      /// \par indicates that boid does not belong to a flock.
      void SetIndex( int iIndex );
      ////////////////////
      /// Returns boid location in flock array.
      /// \returns Positive int - Index of boid location in flock array. 
      /// \returns Negative int - Boid does not belong to a flock
      int GetIndex() const;
      ////////////////////
      /// Returns boolean indicating does this boid
      /// belong to a flock.
      /// \returns true if belongs to a flock
      /// \returns false if does not.
      bool BelongsToFlock() const;
      ////////////////////
      /// Steers to avoid crowding among closest boids.
      /// \param lstClosest List of currently closest boids.
      bool CalculateSeparation( const BoidPtrList & lstClosest, Moose::Math::CVector3<float> & vec );
      ////////////////////
      /// Steers to average heading of closest boids.
      /// \param lstClosest List of currently closest boids.
      void CalculateAlignment( const BoidPtrList & lstClosest, Moose::Math::CVector3<float> & vec);
      ////////////////////
      /// Steers to average position of closest boids.
      /// \param lstClosest List of currently closest boids.
      void CalculateCohesion( const BoidPtrList & lstClosest, Moose::Math::CVector3<float> & vec );

      void SetSpeed( float fSpeed );
      float GetSpeed();
      void SetMaxSpeed( float fSpeed);
      float GetMaxSpeed();
      
      const Moose::Volume::CSphere & GetNeighborSphere() const;
    };
  } // namespace AI
} // namespace Moose
/////////////////////////////////////////////////////////////////
#endif

