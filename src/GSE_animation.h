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

#ifndef __GSE_animation_h__
#define __GSE_animation_h__

#include "GSE_structures.h"
#include "GSE_GraphNodes.h"
#include "GSE_math.h"
#include "GSE_IndexBuffer.h"
#include "GSE_animation.h"
#include "GSE_Configuration.h"
#include <stack>

using namespace Math;

// ----------------------------------------------------------------------
// definitions and global variables
// ----------------------------------------------------------------------
#define JOINT_NAME_LENGTH 32
// ---------------------------------------------------------------------
/// \brief The class for a keyframe in joint animation. 
/// It stores joint index, time in milliseconds and 
/// three floating-point values.
// ----------------------------------------------------------------------
class GSE_JointKeyframe 
{
 public:
  /// \brief To which joint this keyframe belongs to.
  unsigned int	m_iJointIndex;
  /// \brief The time when this keyframe occurs. 
  /// Milliseconds from the beginning of the animation
  int		m_iTime;
  /// \brief The values of the keyframe.
  GSE_Vector3	m_vValues;

};
// ----------------------------------------------------------------------
//
/// \brief The class for joints in skeletal animation.
///
/// The joint will be assigned a set of vertices of which 
/// position will be changed as the joint position changes.
///
// ----------------------------------------------------------------------
class GSE_Joint 
{
 public:
  /// boolean value for Is this Joint the root for other joints.
  char m_bRoot;
  /// Is animation for this node enabled, ie. will the this node 
  /// rotate and translate its children.
  char m_bAnimationEnabled;
  
  /// The starting time for current animation sequence in milliseconds
  int m_iStartTime;
  /// The end time of current animation sequence in milliseconds
  int m_iEndTime;
  
  /// The currently elapsed time relative to the animation sequence 
  /// after starting the animation in milliseconds
  int m_iCurrentTime;
  /// The time where the truly passed time is stored in milliseconds.
  int m_iPassingTime;
  /// Will this animation be looped after receiving the final keyframe.
  int m_bLoop;
  /// The last reached translation keyframe
  unsigned int m_nCurrentTranslationKeyframe;
  /// The last reached rotation keyframe
  unsigned int m_nCurrentRotationKeyframe;
  /// the original transformation of the joint
  GSE_Matrix4x4f m_mAbsolute;
  /// The transformation from parent to this joint
  GSE_Matrix4x4f m_mRelative;
  /// The final transformation, which is the combination of 
  /// all joint transformations prior to this one
  GSE_Matrix4x4f m_mFinal;

  /// The original translation of the joint in object space
  GSE_Vector3 m_vLocalTranslation;
  /// The original rotation of the joint 
  GSE_Vector3 m_vLocalRotation;
  
  /// The size of the m_pTranslationKeyframes array
  unsigned int m_nNumTranslationKeyframes;
  /// The size of the m_pRotationKeyframes array
  unsigned int m_nNumRotationKeyframes;
  /// The array of translation keyframes
  GSE_JointKeyframe *m_pTranslationKeyframes;
  /// The array of rotation keyframes
  GSE_JointKeyframe *m_pRotationKeyframes;
  
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  /// \param nNumTranslationKeyframes the number of translation keyframes in the joint.
  /// \param nNumRotationKeyframes the number of rotation keyframes in the joint.
  GSE_Joint( unsigned int nNumTranslationKeyframes, 
	     unsigned int nNumRotationKeyframes );
  /// The destructor
  ~GSE_Joint();
  /////////////////////////////////////////////////////////////////
  /// The copy constructor
  /// \param ref a reference to another joint where attributes are copied.
  GSE_Joint(const GSE_Joint &ref);
  /////////////////////////////////////////////////////////////////
  /// The assignment operator
  /// \param obj An instance of an object of which parameters are copied to this.
  GSE_Joint &operator=(GSE_Joint obj);
  /////////////////////////////////////////////////////////////////
  /// Returns true if the current animation in this node has finished.
  /// \returns boolean true - if finished, false otherwise. Animation with 
  /// looping enabld will return always false.
  inline int IsAnimationFinished() const 
  {
    if ( IsAnimationLooping() ) 
    {
      return 0;
    }
    return IsAnimationTimePassed();
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the currently set looping method for this joint.
  /// \returns boolean true - if looping is enabled, false otherwise.
  inline int IsAnimationLooping() const
  {
    return m_bLoop;
  }
  /////////////////////////////////////////////////////////////////
  /// \brief Returns boolean indicating has the time passed the animation interval.
  /// Used by animationrenderer to determine whether animation should be looped to the beginning.
  /// \returns boolean true - if passed time exceeds the animation interval, false otherwise.
  inline int IsAnimationTimePassed() const
  {
     return (m_iCurrentTime >= (m_iEndTime - m_iStartTime));
  }
 private:
  /// Initializes the pointers 
  void Init();
  /// Frees the memory reserved by the object.
  void Destroy();
  
};
// ----------------------------------------------------------------------
/// \brief The manager for GSE_Joints.
/// Stores every joint in the system.
class GSE_JointMgr : public GSE_Singleton<GSE_JointMgr>, 
		     public GSE_Container<GSE_Joint>
{
  friend class GSE_Singleton<GSE_JointMgr>;
 private:
  /// The constructor.
  GSE_JointMgr(){}
  /// The destructor.
  ~GSE_JointMgr(){}
 public:
  
};
// --------------------------------------------------------------------
/// \brief A class which modifies the vertex data as requested by animation.
/// It travels through the animation graph translating and 
/// rotating the vertices.
///
class GSE_AnimationRenderer
{

 public:

  /// The different render passes for GSE_AnimationRenderer
  typedef enum {
    /// Sets up joint matrices by their initial position.
    RENDER_PASS_JOINT_SETUP = 0, 
    /// Modifies vertex positions according to rotations (animates).
    RENDER_PASS_UPDATE,
    /// Selects an animation sequence  (time interval)
    RENDER_PASS_SELECT_ANIMATION_TIME_INTERVAL,
    /// Prints the graph on stdout with ascii.
    RENDER_PASS_DEBUG_PRINT	 
  } GSE_AnimRenderPass_t;

  /// The default constructor.
  GSE_AnimationRenderer();
  /// The destructor.
  ~GSE_AnimationRenderer();
  /// When renderer encounters the node, it is passed to this method.
  int Enter( GSE_GraphNode<Animation::NodeType> *pNode);
  /// When renderer leaves the node (after processing children), it is passed to this method. 
  int Leave( GSE_GraphNode<Animation::NodeType> *pNode );
  /// Sets the render pass for following Render() call.
  void SetRenderPass( GSE_AnimRenderPass_t iRenderPass );
  /// Sets the passed time.
  /// \param iPassedTimeInMilliSeconds The passed time in milliseconds.
  void SetPassedTime( unsigned int iPassedTimeInMilliSeconds );
  /// Sets the mode where all child nodes will be updated despite their changes.
  /// Comes in handy when a parent joint moves/rotates and children must move also.
  void ForceUpdate();
  /// Sets the animation sequence selection interval in milliseconds for
  /// RENDER_PASS_SELECT_ANIMATION_INTERVAL.
  /// \param nStartTime The animation start time in milliseconds.
  /// \param nEndTime The animation end time in milliseconds.
  /// \param bLoop boolean value for looping the sequence.
  void SelectAnimationSequence( unsigned int nStartTime, 
				unsigned int nEndTime,
				bool bLoop);
 protected:

  /// Which render pass is active
  GSE_AnimRenderPass_t m_iRenderPassType;  
  /// The currently active final matrix for applying rotations and 
  /// translations 
  GSE_Matrix4x4f       m_mCurrentFinalMatrix;
  /// This stores the starting point in graph depth traversal 
  /// when all child nodes are forced to be updated.
  std::stack<char>     m_MatrixUpdateStack;
 private:
  /// The start time for animation selection in milliseconds.
  unsigned int m_nAnimSelectStartTime;
  /// The end time for animation selection in milliseconds.
  unsigned int m_nAnimSelectEndTime;
  /// The loop mode for animation selection
  bool	       m_nAnimSelectLoop;
  /// The time elapsed in milliseconds since last update.
  unsigned int m_iPassedTime;
  /// The total time passed in milliseconds since last update.
  unsigned int m_iTotalTimePassed;
  /// Variable controlling padding in debug printing
  std::string m_sPadding;  
  /// Variable which represents the arrow in debug printing
  std::string m_sArrow;

  /// Transforms the vertices by interpolating keyframe values;
  /// \param pNode Pointer to GSE_JointNode which is processed.
  int Handle_JointUpdate( GSE_JointNode *pNode );
  /// Transforms the vertices to the initial position. 
  /// Should be called only once before rendering the vertices.
  /// \param pNode Pointer to GSE_JointNode which is processed.
  int Handle_JointSetup( GSE_JointNode *pNode );
  /// Handles the particle system update; transforms particle display 
  /// coordinates etc.
  /// \param pNode Pointer to GSE_AnimParticleSystemNode which is processed.
  int Handle_ParticleSystemUpdate( GSE_AnimParticleSystemNode *pNode );

  /// For printing out animation graph in some stream in ASCII form.
  int Handle_DebugPrint_Enter( GSE_GraphNode<Animation::NodeType> *pNode ); 
  /// For printing out animation graph in some stream in ASCII form.
  int Handle_DebugPrint_Leave( GSE_GraphNode<Animation::NodeType> *pNode ); 
  
};

class GSE_AnimationConfig  : public GSE_Configuration
{
 public:
  GSE_AnimationConfig()
  {
    SetSectionString( "Animation" );
    SetSectionEndString( "}");
    SetWarningsEnabled(false);
  }
  
};

#endif
