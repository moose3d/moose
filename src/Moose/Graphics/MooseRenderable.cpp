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

#include "MooseRenderable.h"
#include <assert.h>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Moose::Graphics;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderable::CRenderable() : m_pTransform(NULL), m_iId(-1)
{

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderable::~CRenderable()
{
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderState &
Moose::Graphics::CRenderable::GetRenderState()
{
  return m_RenderState;
}
/////////////////////////////////////////////////////////////////
Moose::Math::CTransform *
Moose::Graphics::CRenderable::GetTransform()
{
  return m_pTransform;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderable::SetTransform( Moose::Math::CTransform *pTransform )
{
  m_pTransform = pTransform;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderable::SetId( int id )
{
	m_iId = id;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CRenderable::GetId() const
{
	return m_iId;
}
/////////////////////////////////////////////////////////////////
std::ostream &
Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderable & renderable )
{
	stream << renderable.m_iId << " " << renderable.m_pTransform;
	return stream;
}
/////////////////////////////////////////////////////////////////
