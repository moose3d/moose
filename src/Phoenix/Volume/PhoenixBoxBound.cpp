#include "PhoenixBoxBound.h"
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CBoxBound::CBoxBound()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CBoxBound::~CBoxBound()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::COrientedBox &
Phoenix::Volume::CBoxBound::GetBoundingBox()
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Volume::COrientedBox &
Phoenix::Volume::CBoxBound::GetBoundingBox() const
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
