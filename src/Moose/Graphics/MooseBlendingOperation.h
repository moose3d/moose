/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
