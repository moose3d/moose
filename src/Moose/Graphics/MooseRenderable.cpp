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
