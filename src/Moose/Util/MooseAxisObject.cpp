#include "MooseAxisObject.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseModel.h"
#include "MooseBoxRenderable.h"
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Scene;
using namespace Moose::Volume;
////////////////////////////////////////////////////////////////////////////////
Moose::Util::CAxisObject::CAxisObject()
{
  InitializeRenderables(1);
  CBoxRenderable *pBox = new CBoxRenderable(); // this will be dealt later when object it destroyed.
  COrientedBox b;
  b.SetWidth(0.1);
  b.SetHeight(0.1);
  b.SetLength(0.1);
  b.SetPosition(0,0,0);
  pBox->UseBox(b);
  
  AddRenderable( pBox, 0, &GetWorldTransform() );
  
}
////////////////////////////////////////////////////////////////////////////////
