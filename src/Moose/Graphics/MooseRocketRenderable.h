#ifndef __MooseRocketRenderable_h__
#define __MooseRocketRenderable_h__
#include <MooseRenderable.h>
////////////////////////////////////////////////////////////////////////////////
namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        class COglRenderer;
        ////////////////////
        /// Renders GUI components using librocket. 
        class MOOSE_API CRocketRenderable : public Moose::Graphics::CRenderable
        {
        protected:
            /// Rocket context. Not freed when renderable is destroyed.
            Rocket::Core::Context * m_pContext; 
        public:
            ////////////////////
            /// Initializes context to this renderable.
            CRocketRenderable( Rocket::Core::Context *pContext );
            ////////////////////
            /// Called by Moose when object is rendered.
            void Render( Moose::Graphics::COglRenderer & r );

        }; // CRocketRenderable
    } // Graphics 
} // Moose
////////////////////////////////////////////////////////////////////////////////
#endif
