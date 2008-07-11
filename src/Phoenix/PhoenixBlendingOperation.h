#ifndef __PhoenixBlendingOperation_h__
#define __PhoenixBlendingOperation_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixOGLConsts.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for grouping blending operations. 
    class CBlendingOperation : public Phoenix::Core::CEnableable
    {
    private:
      BLEND_SRC_TYPE m_tBlendSrcType;
      BLEND_DST_TYPE m_tBlendDstType;
    public:
      ////////////////////
      /// Constructor. 
      CBlendingOperation() : m_tBlendSrcType(BLEND_SRC_ONE), m_tBlendDstType(BLEND_DST_ZERO) {}
      ////////////////////
      /// Assigns src operation for blending.
      /// \param tType Blending source operation;
      inline void SetSourceOperation( BLEND_SRC_TYPE tType)
      {
	m_tBlendSrcType = tType;
      }
      ////////////////////
      /// Assigns dest operation for blending.
      /// \param tType Blending dest operation;
      inline void SetDestinationOperation( BLEND_DST_TYPE tType)
      {
	m_tBlendDstType = tType;
      }
      ////////////////////
      /// Assigns operations for blending.
      /// \param tSource Source operation.
      /// \param tDestination Destination operation.
      inline void SetOperation( BLEND_SRC_TYPE tSource, BLEND_DST_TYPE tDestination )
      {
	m_tBlendSrcType = tSource;
	m_tBlendDstType = tDestination;
      }
      ////////////////////
      /// \returns blending source operation.
      BLEND_SRC_TYPE GetSourceOperation() const
      {
	return m_tBlendSrcType;
      }
      ////////////////////
      /// \returns blending destination operation.
      BLEND_DST_TYPE GetDestinationOperation() const 
      {
	return m_tBlendDstType;
      }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
