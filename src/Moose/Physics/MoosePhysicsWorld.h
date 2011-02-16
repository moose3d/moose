////////////////////////////////////////////////////////////////////////////////
#include <ode/ode.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Physics 
  {
    class CPhysicsWorld
    {
    protected:
    
        btDefaultCollisionConfiguration *      m_pCollisionConfiguration;
        btCollisionDispatcher *                m_pCollisionDispatcher;
        btDiscreteDynamicsWorld *              m_pDynamicsWorld;
        btSequentialImpulseConstraintSolver *  m_pSolver;
        btAxisSweep3 *                         m_pOverlappingPairCache;
        CVector3<float> m_WorldAABBMin;
        CVector3<float> m_WorldAABBMax;
        
        bool     m_bQuickStep; ///!< Is quickstep algorithm enabled.
        int m_iMaxProxies;

    public:
      /// Creates world.
      CPhysicsWorld();
      /// Destroys world.
      virtual ~CPhysicsWorld();
      /// Sets gravity for world.
      /// \param vGravity Gravity to be set.
      void SetGravity( const Moose::Math::CVector3<float> & vGravity );
      /// \returns Gravity for this world.
      const Moose::Math::CVector3<float> GetGravity();
        /*
      /// Sets error correction per step. (commonly 0.1- 0.8)
      /// \param fErp Error correctionm value.
      void SetERP( float fErp );
      /// \returns Error correction per step.
      float GetERP();
      
      ////////////////////
      /// Sets constraint for mixing value (10^-9 - 1)
      /// \param fCfm CFM mixing value to be set.
      void SetConstraintForceMixing( float fCfm );
      ////////////////////
      /// Returns Constraint force mixing value.
      float GetConstraintForceMixing();
      void SetAutoDisableFlag( bool bFlag );
      bool GetAutoDisableFlag();
      
      void SetAutoDisableLinearThreshold( float fValue);
      float GetAutoDisableLinearThreshold();

      void SetAutoDisableAngularThreshold( float fValue );
      float GetAutoDisableAngluarThreshold();
      
      void SetAutoDisableSteps( int iSteps );
      int GetAutoDisableSteps();

      void SetAutoDisableTime( float fTime );
      float GetAutoDisableTime();

      void ImpulseToForce( float fStepSize, float fX, float fY, float fZ, 
                           CVector3<float> & vVector );
      
      void FixedUpdate( float fStepSize );

      void UseQuickStep( bool bFlag );
      
      void SetQuickStepIterations( size_t nNumIterations );
      size_t GetQuickStepIterations();
      
      void SetQuickStepOverRelaxation( float fOverRelaxation );
      float GetQuickStepOverRelaxation( );
      
      void SetLinearDamping( float fScale );
      float GetLinearDamping();
      
      void SetAngularDamping( float fScale );
      float GetAngularDamping( );      
      
      void SetLinearDampingThreshold( float fThreshold );
      float GetLinearDampingThreshold( );

      void SetAngularDampingThreshold( float fThreshold );
      float GetAngularDampingThreshold( );

      void SetMaxAngularSpeed( float fMaxSpeed );
      float GetMaxAngularSpeed();

      void SetContactMaxCorrectingVelocity( float fVel );
      float GetContactMaxCorrectingVelocity();

      void SetContactSurfaceLayer( float fDepth );
      float GetContactSurfaceLayer();
*/


      
    };
  } // Physics
} // Moose
