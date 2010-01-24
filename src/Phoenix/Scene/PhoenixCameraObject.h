#ifndef __PhoenixRTSCamera_h__
#define __PhoenixRTSCamera_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCamera.h>
#include <PhoenixCore.h>
#include <PhoenixVector3.h>
#include <PhoenixVector2.h>
#include <PhoenixGameObject.h>
#include <PhoenixDefaultEntities.h>
#include <PhoenixFrustum.h>
#include <PhoenixQuad.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Scene
	{
		// TODO make these as parameters to camera, accessible and alterable by script.
		struct CCameraSettings
		{
			float m_fStrafeSpeed;
			float m_fAccelSpeed;
			float m_fDeaccelSpeed;
			float m_fZoomDeaccelSpeed;
			float m_fMaxSpeed;
			float m_fMaxZoomOut;
			float m_fMaxZoomSpeed;
			bool  m_bUseParentRotation;
			bool  m_bFollowParent;
			CCameraSettings()
			{
				// Seemingly working default values.
				m_fStrafeSpeed 			= -1.1f;
				m_fAccelSpeed 			= 25.0f;
				m_fDeaccelSpeed 		= 50.0f;
				m_fZoomDeaccelSpeed = 50.0f;
				m_fMaxSpeed					= 50.0f;
				m_fMaxZoomOut				= 223.0f;
				m_fMaxZoomSpeed			= 150.0f;
				m_bUseParentRotation = false;
				m_bFollowParent      = false;
			}
		};
		//////////////////////////////////////////////////////////////////
		/// CameraObject with commonly used features, smooth strafing and movement,
		/// zooming, target tracking, rotating around point. It can be inserted as child of
		/// trackable object, and configured to use translation with or without rotation
		/// component to track objects.
		class CCameraObject : public Phoenix::Graphics::CCamera,
													public Phoenix::Scene::CGameObject
		{
		public:
			enum MoveDir { LEFT, RIGHT, IN, OUT, FORWARD, BACKWARD  };
		private:
			Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> m_hTarget; ///!< Target that is currently tracked.
			float		      											m_fSpeed;				///!< Present speed in moving and strafing.
			float		      											m_fZoomSpeed;   ///!< Present speed in zooming.
			Phoenix::Math::CVector3<float>     	m_vFocusPoint;  ///!< Where camera is focused ATM.
			MoveDir	      											m_MoveDir;      ///!< Which local direction camera is presently moving.
			bool		      											m_bMoving;      ///!< Is camera on move
			Phoenix::Math::CVector3<float>     	m_vVelocity;    ///!< Velocity of combined movement
			CCameraSettings 										m_Settings;     ///!< Some caps so camera won't overdo it.
		private:
			bool IsZoomAtMinimum();
			bool IsZoomAtMaximum();
		public:
			CCameraObject();

			void MoveLeft();
			void MoveRight();
			void MoveForward();
			void MoveBackward();
			void Stop();
			void Update( float fSeconds );
			void HandleMouseRotation( const Phoenix::Math::CVector2<int> & vOldPos, const Phoenix::Math::CVector2<int> & vNewPos );
			void SetZoom( float fZoomLevel );
			void Zoom( float fZoomStep  );
			void ZoomIn( float fZoomStep = 12.25f );
			void ZoomOut( float fZoomStep = 12.25f );

			//Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> & GetTargetHandle();
			void SetFollowParent( bool bFlag );
			MoveDir GetMoveDir();
			bool IsMoving();
			////////////////////
			/// Creates selection frustum from screen quad.
			/// \param quad Selection quad in opengl coordinates. Lower left corner at 0.0, width & height in pixels.
			/// \param selectionFrustum Storage for selection frustum.
			void CreateSelectionFrustum( const Phoenix::Math::CQuad & quad, Phoenix::Graphics::CFrustum & selectionFrustum );

		}; // CCameraObject
	} // Scene
} // Phoenix
/////////////////////////////////////////////////////////////////

#endif
