/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_terrainTools_h__
#define __GSE_terrainTools_h__
/////////////////////////////////////////////////////////////////
#include "GSE_structures.h"
#include "GSE_GraphNodes.h"
#include <iostream>
#include <string>
/////////////////////////////////////////////////////////////////
using std::string;
/////////////////////////////////////////////////////////////////
/// A class for converting terrain data to renderable objects.
class GSE_TerrainConverter : public GSE_Singleton<GSE_TerrainConverter>
{
  friend class GSE_Singleton<GSE_TerrainConverter>;
 protected:
  ////////////////////
  /// The constructor.
  GSE_TerrainConverter(){}
  ////////////////////
  /// The destructor.
  ~GSE_TerrainConverter(){}

  ////////////////////
  /// Loads geometry from image.
  /// \param rsFilename path to file.
  /// \param ppGeometry pointer to a pointer where pointer to GSE_GeometryData object is stored.
  /// \param lstIndices list where triangle indices will be stored.
  /// \param fStepSize distance between two vertices.
  /// \param fMaxHeight maximum height for terrain.
  /// \param vPosition center point for this terrain object.
  /// \returns non-zero on error, zero otherwise.
  int LoadGeometry( const string &rsFilename, 
		    GSE_GeometryData **ppGeometry, 
		    std::list<unsigned int> &lstIndices,
		    float fStepSize,
		    float fMaxHeight,
		    const GSE_Vector3 &vPosition );
 public:
  ////////////////////
  /// Creates a terrain which is spatially partitioned with octree.
  /// \param rsFilename The filename of the grayscale image.
  /// \param rSceneGraph The scenegraph where new node is inserted.
  /// \param ppGeometryOctree The pointer to a pointer which will point to the geometryoctree.
  /// \param ppGeometryNode The pointer to a pointer where geometry node is stored.
  /// \param fStepSize The steps in z and x directions between vertices.
  /// \param fMaxHeight The maximum height value in y-coordinate which corresponds to value 255 in height map.
  /// \param nMaxTreeDepth maximum depth for octree (levels).
  /// \param nMinfacesInNode minimum number of faces in a octree cube which is not split further.
  /// \param vPosition center point for this terrain node.
  void CreateTerrainOctreeFromFile( const string &rsFilename,
				    GSE_SceneGraph &rSceneGraph,
				    GSE_GeometryOctree **ppGeometryOctree,
				    GSE_GeometryNode **ppGeometryNode,
				    float fStepSize,
				    float fMaxHeight,
				    unsigned int nMaxTreeDepth,
				    unsigned int nMinfacesInNode,
				    const GSE_Vector3 &vPosition);

  ////////////////////
  /// Creates a terrain which is one big chunk.
  /// \param rsFilename The filename of the grayscale image.
  /// \param rSceneGraph The scenegraph where new node is inserted.
  /// \param ppGeometryNode The pointer to a pointer where geometry node is stored.
  /// \param fStepSize The steps in z and x directions between vertices.
  /// \param fMaxHeight The maximum height value in y-coordinate which corresponds to value 255 in height map.
  /// \param vPosition center point for this terrain node.
  void CreateTerrainObjectFromFile(  const string &rsFilename,
				     GSE_SceneGraph &rSceneGraph,
				     GSE_GeometryNode **ppGeometryNode,
				     float fStepSize,float fMaxHeight,
				     const GSE_Vector3 &vPosition);
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_terrainTools.h,v $
// Revision 1.7  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.6  2007/05/31 13:33:41  entity
// more doxy comments.
//
// Revision 1.5  2007/04/02 12:54:46  entity
// added argument for terrain center position
//
// Revision 1.4  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.3  2007/03/29 13:11:09  entity
// more modular implementation
//
// Revision 1.2  2007/03/29 06:53:05  entity
// added nMaxTreeDepth and nMinfacesInNode arguments for CreateGeometryOctreeFromFile
//
// Revision 1.1  2007/03/28 21:07:32  entity
// first version
//
/////////////////////////////////////////////////////////////////
