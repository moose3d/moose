#ifndef __MooseBlendingOperation_h__
#define __MooseBlendingOperation_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseOGLConsts.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for grouping blending operations. 
    class MOOSE_API CBlendingOperation : public Moose::Core::CEnableable
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
      /// Enables additive blending.
      void SetAdditiveBlending()
      {
          SetOperation( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE);
      }
      /// Enables default operation for transpareny effect.
      void SetDefaultTransparency()
      {
          SetOperation( BLEND_SRC_SRC_ALPHA, BLEND_DST_ONE_MINUS_SRC_ALPHA);
      }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
