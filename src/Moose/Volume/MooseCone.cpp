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

#include "MooseCone.h"
using namespace Moose::Math;
using namespace Moose::Volume;
/////////////////////////////////////////////////////////////////
void 
Moose::Volume::CCone::SetAngle( float fAngle )
{
  m_fAngle = fAngle;
  m_fSinAngle = sinf(m_fAngle);
  m_fCosAngle = cosf(m_fAngle);
  m_fSinReciprocal = (1.0f / m_fSinAngle);
  m_fCosAngleSqr = m_fCosAngle * m_fCosAngle;
  m_fSinAngleSqr = m_fSinAngle * m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
Moose::Volume::CCone::CCone() : CPositional(), m_vDirection(CVector3<float>(0.0f,0.0f,-1.0f)),m_fLength(1.0f)
{
  SetAngle(0.0f);
}
/////////////////////////////////////////////////////////////////
Moose::Volume::CCone::CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength ) : CPositional(vPos),m_vDirection(vDir)
{
  SetAngle(fAngle);
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
