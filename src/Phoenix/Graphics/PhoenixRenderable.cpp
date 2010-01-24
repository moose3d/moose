#include "PhoenixRenderable.h"
#include <assert.h>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderable::CRenderable() : m_pTransform(NULL), m_iId(-1)
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderable::~CRenderable()
{
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderState &
Phoenix::Graphics::CRenderable::GetRenderState()
{
  return m_RenderState;
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransform *
Phoenix::Graphics::CRenderable::GetTransform()
{
  return m_pTransform;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetTransform( Phoenix::Math::CTransform *pTransform )
{
  m_pTransform = pTransform;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetId( int id )
{
	m_iId = id;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CRenderable::GetId() const
{
	return m_iId;
}
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderable & renderable )
{
	stream << renderable.m_iId << " " << renderable.m_pTransform;
	return stream;
}
/////////////////////////////////////////////////////////////////
