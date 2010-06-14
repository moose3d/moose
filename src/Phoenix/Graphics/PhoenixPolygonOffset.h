#include "PhoenixEnableable.h"

namespace Phoenix
{
    namespace Graphics
    {
        ///////////////////////
        /// Polygon offset for solving z-fighting effects on overlaid objects.
        class CPolygonOffset : public Phoenix::Core::CEnableable
        {
        protected:
            float m_fFactor; ///!< Variable depth offset factor 
            float m_fUnits;  ///!< Constant depth offset factor
        public:
            CPolygonOffset() : m_fFactor(0.0), m_fUnits(0.0) { }
            virtual ~CPolygonOffset() {}
            void Set( float fFactor, float fUnits ) { m_fFactor = fFactor; m_fUnits = fUnits; }
            void SetFactor( float fValue ) { m_fFactor = fValue; }
            void SetUnits( float fValue ) { m_fUnits = fValue; }
            float & GetFactor() { return m_fFactor; }
            float & GetUnits()  { return m_fUnits; }
        };
    }
}

