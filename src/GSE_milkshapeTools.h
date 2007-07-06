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
#ifndef __GSE_milkshapeTools_h__
#define __GSE_milkshapeTools_h__
/////////////////////////////////////////////////////////////////
#include "GSE_Milkshape.h"
#include "GSE_structures.h"
#include "GSE_GraphNodes.h"
#include <string>
/////////////////////////////////////////////////////////////////
using std::string;
using Geometry::GSE_OrientedBox;
using Geometry::GSE_Sphere;
/////////////////////////////////////////////////////////////////
/// \brief A milkshape 3d model converter.
class GSE_MS3DObjectConverter : public GSE_Singleton<GSE_MS3DObjectConverter>
{
  friend class GSE_Singleton<GSE_MS3DObjectConverter>;
 private:
  /// \brief Texture path prefix.
  string m_strTexturePath;
  /////////////////////////////////////////////////////////////////
  /// \brief the constructor.
  GSE_MS3DObjectConverter(){}
 public:
  /////////////////////////////////////////////////////////////////
  /// \brief Sets the texture path.
  void SetTexturePath( const string &strPath );
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the texture path
  const string & GetTexturePath() const;
  /////////////////////////////////////////////////////////////////
  /// \brief Creates rendernodes and optionally animation nodes from given model data.
  /// \param rModel the Milkshape model data
  /// \param pRenderGraph the rendergraph where nodes are created. Must be non-NULL.
  /// \param pAnimGraph the animation graph where jointnodes are created. If NULL, animation is 
  ///  not taken into account and model is considered to be static.
  /// \param ppGeometryNode A pointer to the pointer where root of the model rendergraph is stored.
  /// \param ppJointNode A pointer to the pointer where root of the model animation joints is stored.
  /// \param iQuality The texture quality flag, default value is QUALITY.
  /// \param sRootName name of the root node in joints.
  /// \returns non-zero on error, 0 on success.
  int Convert( GSE_MS3DModel &rModel, 
	       GSE_RenderGraph *pRenderGraph,
	       GSE_AnimationGraph *pAnimGraph,
	       GSE_GeometryNode **ppGeometryNode,
	       GSE_JointNode **ppJointNode,
	       std::string sRootName,
	       TextureQuality::Type iQuality = TextureQuality::QUALITY);

  /////////////////////////////////////////////////////////////////
  /// \brief Constructs GSE_SceneLimbNodes from the joints attached
  /// to pJointroot and their child joints. The nodes will be attached
  /// to the pSceneNode.
  /// \param pSceneNode The scenenode where scenelimbs are attached to.
  /// \param pJointRoot The JointNode whose children are taken into account.
  void CreateSceneLimbsFromJoints(  GSE_SceneNode *pSceneNode,
				    GSE_JointNode *pJointRoot );
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the bounding sphere for specific joint in the model (in object coordinates)
  /// \param rModel the model data
  /// \param sJoint the name of the joint for which the bounding volume is calculated.
  /// \returns The bounding sphere.
  GSE_Sphere GetBoundingSphereForJoint ( const GSE_MS3DModel &rModel, const std::string & sJoint );
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the oriented bounding box for specific joint in the model (in object coordinates)
  /// \param rModel the model data.
  /// \param sJoint the name of the joint for which the bounding volume is calculated.
  /// \returns The oriented bounding box.
  GSE_OrientedBox GetOBBForJoint( const GSE_MS3DModel &rModel, const std::string & sJoint );
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the bounding sphere for vertices which are not animated in the model (in object coordinates)
  /// \param rModel the model data.
  /// \returns The bounding sphere.
  GSE_Sphere GetBoundingSphereForUnanimated(const GSE_MS3DModel &rModel);
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the oriented bounding box for vertices which are not animated in the model (in object coordinates)
  /// \param rModel the model data.
  /// \returns The oriented bounding box.
  GSE_OrientedBox GetOBBForUnanimated(const GSE_MS3DModel &rModel);
  
};
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_milkshapeTools.h,v $
// Revision 1.11  2007/06/24 17:28:46  entity
// ms3d fix
//
// Revision 1.10  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.9  2007/05/21 18:33:36  entity
// texture path added
//
// Revision 1.8  2007/04/03 12:51:43  entity
// texture quality added
//
// Revision 1.7  2007/03/25 15:36:21  entity
// comments added
//
// Revision 1.6  2007/03/23 12:50:01  entity
// added GetBoundingSphereForJoint,GetOBBForJoint,
// GetBoundingSphereForUnanimated,GetOBBForUnanimated
//
// Revision 1.5  2007/03/23 11:52:41  entity
// unneeded function param removed
//
// Revision 1.4  2007/03/23 11:51:52  entity
// CreateSceneLimbsFromJoints added
//
// Revision 1.3  2007/03/23 07:32:09  entity
// fixed compile errors
//
// Revision 1.2  2007/03/23 07:26:35  entity
// log added
//
/////////////////////////////////////////////////////////////////
