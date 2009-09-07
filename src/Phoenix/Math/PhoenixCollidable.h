#ifndef __PhoenixCollidable_h__
#define __PhoenixCollidable_h__
////////////////////////////////////////////////////////////////////////////////
#include "PhoenixCollider.h"
#include "PhoenixSphereCollider.h"
////////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Collision
  {
    ////////////////////
    /// Class for basic sphere collision + additional one
    class CCollidable : public Phoenix::Collision::CSphereCollider
    {
    protected:

      ICollider *  m_pCollider; ///!< Alternative collider pointer, released when object is destroyed.

    public:

      CCollidable();
      virtual ~CCollidable();

      void				SetCollider( Phoenix::Collision::ICollider *pCollider );
      Phoenix::Collision::ICollider *	GetCollider();
      
      bool Intersects( const Phoenix::Volume::CSphere & sphere   )     const;
      bool Intersects( const Phoenix::Graphics::CFrustum & frustum )   const;
      bool Intersects( const Phoenix::Volume::COrientedBox & box )     const;
      bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;

    }; // CCollidable
  } // Collision
} // Phoenix
////////////////////////////////////////////////////////////////////////////////
#endif
