%module Moose
%include "std_string.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_vector.i"
%include "typemaps.i"
%{

#include <Moose.h>
#include <list>
 //#include "CClearBuffers.h"
  using namespace Moose::Graphics;
  using namespace Moose::Math;

inline Moose::Scene::CScene * 
GameObjToScene( Moose::Scene::CGameObject *pObj )
{
	return dynamic_cast<Moose::Scene::CScene *>(pObj);
}

inline Moose::Scene::CGameObject * 
SceneToGameObj( Moose::Scene::CScene *pObj )
{
	return dynamic_cast<Moose::Scene::CGameObject *>(pObj);
}

inline Moose::Scene::CApplication *
GameObjToApplication( Moose::Scene::CGameObject *pObj )
{
	return dynamic_cast<Moose::Scene::CApplication *>(pObj);
}

inline Moose::Scene::CGameObject *
TransformableAsGameObject( Moose::Scene::CTransformable *pObj )
{
  return dynamic_cast<Moose::Scene::CGameObject *>(pObj);
}

inline std::vector<Moose::Scene::CGameObject *> ListToVector( std::list<Moose::Scene::CGameObject *> & list )
{
	std::vector<Moose::Scene::CGameObject *> objVec;
	std::copy( list.begin(), list.end(), back_inserter(objVec)); 
	return objVec;
}	


inline float    ToReal( float * f )  { return *f;}
inline int      ToInt( float * i )   { return *i;}
inline double   ToReal( double * f ) { return *f;}

inline std::list<Moose::Scene::CGameObject *> GetNewList() { return std::list<Moose::Scene::CGameObject *>(); }
%}
// This helps in converting individual elements into values
%typemap(out) float & {
  Tcl_SetObjResult(interp, Tcl_NewDoubleObj(*$1));
}
%include "MooseNamed.h"
%include "MooseTimer.h"
%include "MooseFPSCounter.h"
%include "MooseCacheable.h"
%template (cacheable) Moose::Core::CCacheable<GLuint>;
%include "MooseTypeBase.h"
%template (vertdescTypeBase) Moose::Core::CTypeBase< Moose::Graphics::ELEMENT_TYPE >;
%include "MooseVertexDescriptor.h"
%include "MooseNullable.h"
%include "MooseResourceManager.h"

%include "MooseSingleton.h"

%template (nameCreator) Moose::Core::CSingleton<Moose::Core::CUniqueNameCreator>;
%template (gameObjHandled) Moose::Core::CHandled< Moose::Scene::CGameObject >;
%include "MooseCore.h"


%include "MooseGraphEdge.h"
%template (trEdge) Moose::Core::TGraphEdge<Moose::Scene::CTransformNode>;

%include "MooseGraphNode.h"
%template (trNode) Moose::Core::TGraphNode<Moose::Scene::CTransformNode>;

%include "MooseGraph.h"
%template (trGraph) Moose::Core::TGraph<Moose::Scene::CTransformNode>;
%include "MooseVector2.h"
%template (CVector2f) Moose::Math::CVector2<float>;
%template (CVector2i) Moose::Math::CVector2<int>;

%include "MooseVector3.h"
%template (CVector3f) Moose::Math::CVector3<float>;
%template (CVector3i) Moose::Math::CVector3<int>;
%include "MooseVector4.h"
%template (CVector4f) Moose::Math::CVector4<float>;
%template (CVector4i) Moose::Math::CVector4<int>;
%template (CVector4c) Moose::Math::CVector4<unsigned char>;

%include "MooseMatrix4x4.h"
%template (CMatrix4x4f) Moose::Math::CMatrix4x4<float>;
%template (CMatrix4x4i) Moose::Math::CMatrix4x4<int>;


%include "MooseQuaternion.h"
%include "MooseTransform.h"
%include "MooseTransformGraph.h"
%include "MooseSphereBound.h"
%include "MooseViewable.h"
%include "MooseEnableable.h"
%include "MooseBlendingOperation.h"
%include "MooseAlphaTestOperation.h"
%include "MooseMaterial.h"
%include "MooseRenderState.h"
%include "MoosePositional.h"
%include "MooseDimensional1D.h"
%include "MooseDimensional2D.h"
%include "MooseDimensional3D.h"
%include "MooseSphere.h"
%include "MooseOrientable.h"
%include "MooseAABB.h"
%include "MooseOBB.h"
%include "MooseOneDirectional.h"
%include "MooseLine.h"
%include "MooseRay.h"
%include "MooseOrientable.h"
%include "MooseCamera.h"
%template (CCameraMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Graphics::CCamera, Moose::Core::CHandle<Moose::Graphics::CCamera> > >;
%template (CCameraResMgr) Moose::Core::CResourceManager< Moose::Graphics::CCamera, Moose::Core::CHandle<Moose::Graphics::CCamera> >;
%include "MooseRenderableProperty.h"
%include "MooseTagged.h"
%include "MooseNamed.h"
%include "MooseUpdateable.h"
%include "MooseEnableable.h"
%include "MooseAIScript.h"
%include "MooseCollider.h"
%include "MooseSphereCollider.h"
%include "MooseCapsuleCollider.h"
%include "MooseBoxBound.h"
%include "MooseOBBCollider.h"
%include "MooseCompoundCollider.h"
// collider mgr template
%template (CColliderMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Collision::ICollider, Moose::Core::CHandle<Moose::Collision::ICollider> > >;
%template (CColliderResMgr) Moose::Core::CResourceManager< Moose::Collision::ICollider, Moose::Core::CHandle<Moose::Collision::ICollider> >;
%include "MooseOctree.h"
%template (gameObj_Octree) Moose::Spatial::COctree< Moose::Scene::CGameObject *>;


%include "MooseIndexArray.h"
%include "MooseTexture.h"
%template (TEXTURE_HANDLE) Moose::Core::CHandle< Moose::Graphics::COglTexture >;

%template (CTextureMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Graphics::COglTexture, Moose::Core::CHandle<Moose::Graphics::COglTexture> > >;
%template (CTextureResMgr) Moose::Core::CResourceManager< Moose::Graphics::COglTexture, Moose::Core::CHandle<Moose::Graphics::COglTexture> >;
%include "MooseModel.h"
%include "MooseDefaultEntities.h"
%rename (ostreamModel) operator<<(std::ostream &stream, const Moose::Graphics::CModel & model);
%include "MooseShader.h"
// shader mgr template
%template (CShaderMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Graphics::CShader, Moose::Core::CHandle<Moose::Graphics::CShader> > >;
%template (CShaderResMgr) Moose::Core::CResourceManager< Moose::Graphics::CShader, Moose::Core::CHandle<Moose::Graphics::CShader> >;
%include "MooseModel.h"
%template (CModelMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Graphics::CModel, Moose::Core::CHandle<Moose::Graphics::CModel> > >;
%template (CModelResMgr) Moose::Core::CResourceManager< Moose::Graphics::CModel, Moose::Core::CHandle<Moose::Graphics::CModel> >;
// This is one bugger; Compiler won't accept friend operator defintion for some reason?
//%rename (ostreamRenderable) operator<<(std::ostream &stream, const Moose::Graphics::CRenderable & r);
%include  "MooseRenderable.h"
%rename (ostreamRenderableModel) operator<<(std::ostream &stream, const Moose::Graphics::CRenderableModel & r);
%include "MooseClearBuffers.h"
%include "MooseRenderableModel.h"
%include "MooseGameObject.h"
%template (GameObjectVector) std::vector<Moose::Scene::CGameObject *>;
// Game object mgr template 
%template (CGameObjectMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Scene::CGameObject, Moose::Core::CHandle<Moose::Scene::CGameObject> > >;
%template (CGameObjectResMgr) Moose::Core::CResourceManager< Moose::Scene::CGameObject, Moose::Core::CHandle<Moose::Scene::CGameObject> >;
%include "MooseParticleSystem.h"
%include "MooseSpatialGraph.h"
%template (CollectObjectsVec) Moose::Scene::CSpatialGraph::CollectObjects< std::vector< Moose::Scene::CGameObject *> >;
%include "MooseScene.h"
%rename (ostreamRenderableModelShared) operator<<(std::ostream &stream, const Moose::Graphics::CRenderableModelShared & r);
%include "MooseRenderableModelShared.h"
%include "MooseSkybox.h"
%include "MooseApplication.h"
%include "MooseLight.h"
%include "MooseDirectionalLight.h"
%include "MooseDirectionalLightObject.h"
%include "MooseRenderQueue.h"
 //%include "CClearBuffers.h"
%include "MooseCameraObject.h"
%include "MooseCollision.h"
%include "MooseMath.h"
%include "MooseMathUtils.h"
// Audio system classes
%include "MooseListener.h"
%include "MooseALObjectArray.h"
%template (CALObjectArray1) Moose::Sound::CALObjectArray<1>; 
%template (CALObjectArray2) Moose::Sound::CALObjectArray<2>; 
%include "MooseSoundBase.h"
%include "MooseSoundSampleBase.h"
%include "MooseALSampleTypes.h"
%include "MooseALSoundTypes.h"
%include "MooseOggSample.h"
%include "MooseOggStreamSample.h"
%template (AudioSystem) Moose::Core::CSingleton< Moose::Sound::CAudioSystem >;
%include "MooseAudioSystem.h"
    
// sample mgr template
%template (SampleMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Sound::CALSample, Moose::Core::CHandle<Moose::Sound::CALSample> > >;
%template (SampleResMgr) Moose::Core::CResourceManager< Moose::Sound::CALSample, Moose::Core::CHandle<Moose::Sound::CALSample> >;

// stream mgr template
%template (StreamMgr) Moose::Core::CSingleton< Moose::Core::CResourceManager< Moose::Sound::CALStreamSample, Moose::Core::CHandle<Moose::Sound::CALStreamSample> > >;
%template (StreamResMgr) Moose::Core::CResourceManager< Moose::Sound::CALStreamSample, Moose::Core::CHandle<Moose::Sound::CALStreamSample> >;

%include "MooseOGLRenderer.h"
 //%template (FireworksSingleton) Moose::Core::CSingleton< CFireWorks >;
%include "MoosePlane.h"
%template (VertexDescriptorHandle) Moose::Core::CHandle< Moose::Graphics::CVertexDescriptor >;
%template (IndexArrayHandle) Moose::Core::CHandle< Moose::Graphics::CIndexArray >;
%include "MooseGrid.h"
%include "MooseTransformIndicator.h"
%include "MooseUniqueNameCreator.h"
%include "MooseAxisObject.h"


Moose::Scene::CScene * GameObjToScene( Moose::Scene::CGameObject *pObj );
Moose::Scene::CGameObject *  SceneToGameObj( Moose::Scene::CScene *pObj );
Moose::Scene::CApplication * GameObjToApplication( Moose::Scene::CGameObject *pObj );
                             
std::vector<Moose::Scene::CGameObject *> ListToVector( std::list<Moose::Scene::CGameObject *> & list );



                             float    ToReal( float * f ); 
                             int      ToInt( float * i ); 
                             double   ToReal( double * f );