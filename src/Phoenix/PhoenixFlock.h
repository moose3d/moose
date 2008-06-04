#ifndef __PhoenixFlock_h__
#define __PhoenixFlock_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    class CBoid;
    ////////////////////
    /// Flock (Herd, School, Swarm) of boids.
    template <class BOID_TYPE>
    class CFlock 
    {
    private:
      /// List of boids in this flock ( each boid should belong at 
      /// most into one flock)
      std::vector<BOID_TYPE *>	m_vecBoids;
      /// Average center of this flock
      CVector3<float>		m_vCenter;
      /// Target of this flock
      CVector3<float>		m_vTarget;
      /// Target weight / importance
      float			m_fTargetWeight;
      /// Bounding sphere for this flock.
      Phoenix::Volume::CSphere  m_BoundingSphere;
    public:
      ////////////////////
      /// Constructor.
      CFlock();
      ////////////////////
      /// Destructor.
      ~CFlock();
      ////////////////////
      /// Adds new object into this flock.
      void AddToFlock( const BOID_TYPE *pBoid );
      ////////////////////
      /// Removes boid from this flock.
      void RemoveFromFlock( const BOID_TYPE * pBoid );
      ////////////////////
      /// Updates all boids.
      void Update();

    private:
      ////////////////////
      /// Determines which are the local neighbours for each boid
      void LocateNearestBoids( const BOID_TYPE *pBoid, std::list & lstClosest );
    };
    ////////////////////
    /// Autonomous flock member.
    class CBoid : public Phoenix::Spatial::CPositional,
		  public Phoenix::Spatial::COneDirectional
    {
    private:
      /// Object bounding sphere
      Phoenix::Volume::CSphere	m_BoundingSphere;
      /// Index in flock array
      size_t			m_nIndex;
    public:
      ////////////////////
      /// Constructor.
      CBoid();
      ////////////////////
      /// Assigns boid location in the array.
      /// \param iIndex Location in flock array. Negative value 
      /// \par indicates that boid does not belong to a flock.
      void SetIndex( int iIndex );
      ////////////////////
      /// Returns boid location in flock array.
      /// \returns Positive int - Index of boid location in flock array. 
      /// \returns Negative int - Boid does not belong to a flock
      const int GetIndex() const;
      ////////////////////
      /// Returns boolean indicating does this boid
      /// belong to a flock.
      /// \returns true if belongs to a flock
      /// \returns false if does not.
      const bool BelongsToFlock() const;
      ////////////////////
      /// Steers to avoid crowding among closest boids.
      /// \param lstClosest List of currently closest boids.
      void CalculateSeparation( const std::list<CBoid *pBoid> & lstClosest );
      ////////////////////
      /// Steers to average heading of closest boids.
      /// \param lstClosest List of currently closest boids.
      void CalculateAlignment( const std::list<CBoid *pBoid> & lstClosest);
      ////////////////////
      /// Steers to average position of closest boids.
      /// \param lstClosest List of currently closest boids.
      void CalculateCohesion( const std::list<CBoid *pBoid> & lstClosest);

    };
  } // namespace AI
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

