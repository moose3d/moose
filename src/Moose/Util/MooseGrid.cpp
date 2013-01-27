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

#include "MooseGrid.h"
#include "MooseDefaultEntities.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseVector4.h"
#include "MooseMaterial.h"
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
  GetRenderState().GetMaterial().SetDiffuse( CColor4f(0.88,0.88,1.0,0.88) );
  GetRenderState().GetShaderHandle() = "moose_color_shader";
  GetRenderState().AddShaderAttrib("a_vertex", (*GetModelHandle())->GetVertexHandle());

  GetRenderState().AddShaderUniform("color", &GetRenderState().GetMaterial().GetDiffuse());
  bool bVal = GetRenderState().Prepare();
  assert( bVal );
  //g_Log << "RenderLayer is : " << GetRenderState().GetRenderLayer() << "\n";
}
////////////////////////////////////////////////////////////////////////////////
prefix::CGrid::~CGrid()
{
  
}
////////////////////////////////////////////////////////////////////////////////
