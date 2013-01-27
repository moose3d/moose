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

#include <UnitTest++/UnitTest++.h>
#include <MooseObjLoader.h>
#include <string>
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
using namespace Moose::Data;
using namespace Moose::Graphics;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( MooseObjLoader )
{
  CModelLoader *pLoader = new CObjLoader();

  CHECK_EQUAL( 0, pLoader->Load( "box.obj" ) ); 

  CVertexDescriptor *pVertices = pLoader->GetVertexArray();
  cerr << "# vertices loaded: " << pVertices->GetSize() << endl;
  CIndexArray *pIndices = pLoader->GetIndexArray();
  assert( pIndices );
  cerr << "# indices : " << pIndices->GetNumIndices() << endl;
  delete pIndices;
    
  pIndices = pLoader->GetIndexArray( "testgroup1");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup1: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup2");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup2: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup3");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup3: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  delete pVertices;   
    
  delete pLoader;

}
/////////////////////////////////////////////////////////////////
