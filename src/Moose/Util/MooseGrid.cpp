#include "MooseGrid.h"
#include "MooseDefaultEntities.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseModel.h"
namespace prefix = Moose::Util;
using namespace Moose::Graphics;
////////////////////////////////////////////////////////////////////////////////
prefix::CGrid::CGrid() 
{
  if ( !g_Models("moose_grid_model"))
  {
    CModel *pModel = new CModel();
    // Square with ten segments inside it.
    // Width and length of square is two.
    // 
    // (-1,0,-1) +---+ (1,0,-1)
    //           |   |
    // (-1,0, 1) +---+ (1,0, 1)
    //
    CVertexDescriptor *pVertices = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, 40);
    for( int i=0;i<11;i++)
    {
      pVertices->GetPointer<float>(i)[0] = -1.0+((float)i)*0.2;
      pVertices->GetPointer<float>(i)[1] = 0.0f;
      pVertices->GetPointer<float>(i)[2] = -1.0f;
    }
    
    for( int i=11;i<22;i++)
    {
      pVertices->GetPointer<float>(i)[0] = -1.0+((float)i-11)*0.2;
      pVertices->GetPointer<float>(i)[1] = 0.0f;
      pVertices->GetPointer<float>(i)[2] = 1.0f;
    }

    for( int i=22;i<31;i++)
    {
      pVertices->GetPointer<float>(i)[0] = -1.0f;
      pVertices->GetPointer<float>(i)[1] = 0.0f;
      pVertices->GetPointer<float>(i)[2] = -1.0+((float)i-21)*0.2;
    }

    for( int i=31;i<40;i++)
    {
      pVertices->GetPointer<float>(i)[0] = 1.0f;
      pVertices->GetPointer<float>(i)[1] = 0.0f;
      pVertices->GetPointer<float>(i)[2] = -1.0+((float)i-30)*0.2;
    }
    g_Vertices.Create( pVertices, "moose_grid_vertices", pModel->GetVertexHandle());
    
    CIndexArray *pIndices = new CIndexArray( PRIMITIVE_LINE_LIST,44);
    unsigned short int array[44] = {
      // vertical
      0, 10,
      0, 11,
      1, 12,
      2, 13,
      3, 14,
      4, 15,
      5, 16,
      6, 17,
      7, 18,
      8, 19,
      9, 20,
      10, 21,
      11, 21,
      // horizontal
      22, 31,
      23, 32,
      24, 33,
      25, 34,
      26, 35,
      27, 36,
      28, 37,
      29, 38,
      30, 39
    };
    
    pIndices->Copy( array );
    g_Indices.Create( pIndices, "moose_grid_indices", pModel->GetIndices());
    g_Models.Create(pModel, "moose_grid_model", GetModelHandle());
    
  } else {
    GetModelHandle() = "moose_grid_model";
  }
  GetRenderState().GetBlendingOperation().SetDefaultTransparency();    
  GetRenderState().SetBaseColor( CColor4ub(225,225,255,225) );
}
////////////////////////////////////////////////////////////////////////////////
prefix::CGrid::~CGrid()
{
  
}
////////////////////////////////////////////////////////////////////////////////
