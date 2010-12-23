////////////////////////////////////////////////////////////////////////////////
#include <btBulletDynamicsCommon.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Physics 
  {
    ////////////////////
    /// Rigid body type.
    enum RigidBodyType 
    { 
      DynamicRigidBody,  ///!< Positive mass, Moving
      StaticRigidBody,   ///!< Zero mass, no movement
      KinematicRigidBody ///!< Zero mass, no effect from dynamics.
    };
    ////////////////////
    /// Basic physics object.
    class CRigidBody : public Moose::Core::CTypeBase<Moose::Physics::RigidBodyType>
    {
    public:
     
    protected:
      dBodyID m_BodyID; ///!< ODE Body id.
    public:
      /// Creates body.
      CRigidBody();
      /// Destroys body.
      virtual ~CRigidBody();

      void SetPosition( const Moose::Math.:CVector3<float> & vPosition );
      const Moose::Math::CVector3<float> GetPosition();

      void SetRotation( const Moose::Math::CQuaternion & q );
      Moose::Math::CQuaternion GetRotation();
      
      void SetVelocity( const Moose::Math::CVector3<float> & vVelocity );
      Moose::Math::CVector3<float> GetVelocity();
      
      void SetAngularVelocity( const Moose::Math::CVector3<float> & vVelocity );
      Moose::Math::CVector3<float> GetAngularVelocity(); 

      void SetMass( float fMass );
      float GetMass();

      void AddForce( const Moose::Math::CVector3<float> & vForce );
      void AddTorque( const Moose::Math::CVector3<float> & vForce );
      void AddRelativeTorque( const Moose::Math::CVector3<float> & vForce );
      void AddRelativeForce( const Moose::Math::CVector3<float> & vForce );

      void AddForceAtPos( const Moose::Math::CVector3<float> & vForce, 
                          const Moose::Math::CVector3<float> & vPos );
      void AddForceAtRelPos( const Moose::Math::CVector3<float> & vForce, 
                             const Moose::Math::CVector3<float> & vPos );
      void AddRelForceAtPos( const Moose::Math::CVector3<float> & vForce, 
                          const Moose::Math::CVector3<float> & vPos );
      void AddRelForceAtRelPos( const Moose::Math::CVector3<float> & vForce, 
                                const Moose::Math::CVector3<float> & vPos );
      Moose::Math::CVector3<float> GetForce();
      Moose::Math::CVector3<float> GetTorque();

      void GetForce( const Moose::Math::CVector3<float> & vForce );
      void GetTorque( const Moose::Math::CVector3<float> & vTorque );


      
      void SetEnabled( bool bFlag );
      bool IsEnabled();

      void SetAutoDisable( bool bFlag );
      bool IsAutoDisable();

      /// Set dInfinity to disable.
      void SetAutoDisableLinearThreshold( float fThreshold );
      float GetAutoDisableLinearThreshold();

      /// Set dInfinity to disable.
      void SetAutoDisableAngularThreshold( float fThreshold );
      float GetAutoDisableAngularThreshold();
      
      /// Set to zero to disable.
      void SetAutoDisableSteps( size_t nSteps );
      size_t GetAutoDisableSteps();
      
      /// Set to zero to disable.
      void SetAutoDisableTime( float fTime );
      float GetAutoDisableTime();

      /// Sets autodisable parameters that are set on the world object.
      void SetAutoDisableDefaults();
      void SetLinearDamping( float fScale );
      void SetAngularDamping( float fScale );
      float GetLinearDamping();
      float GetAngularDamping();
      
      float GetLinearDampingThreshold();
      float GetAngularDampingThreshold();
      void SetLinearDampingThreshold( float fThreshold );
      void SetAngularDampingThreshold( float fThreshold );

      /// Sets world damping values to this object.
      void SetDampingDefaults();
      // By default, dInfinity
      void SetMaxAngularSpeed( float fMaxSpeed );
      float GetMaxAngularSpeed();

      void RegisterWithGameObject( Moose::Scene::CGameObject *pGameObject );
      // dBodySetMovedCallsback      
      // dBodySetData dBodyGetData
      
      void SetFiniteRotationMode( RotationMode kMode );
      RotationMode GetFiniteRotationMode();
      void SetFiniteRotationAxis( Moose::Math::CVector3<float> & vAxis );
      Moose::Math::CVector3<float> GetFiniteRotationAxis();
      size_t GetNumJoints();
      // dJointId GetJoint( size_t index );
      // dWorldId GetWorld();
      /// Does rigidbody use gravity or not.
      /// \param bFlag True to use gravity, false to not.
      void UseGravity( bool bFlag );
      /// \returns true if body uses world gravity, false otherwise. 
      bool UsesGravity();

      // dGeomId dBodyGetFirstGeom
      // dGeomId dBodyGetNextGeom gGeomId
      
      ////////////////////
      /// Called when body moves.
      inline static void OnMoveTrigger( dBodyId id );

      void UseSphere( Moose::Volume::CSphere & sphere );
      //      void Physics
    };
  } // UseBox
} // Moose


