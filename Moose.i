%module Moose
%include "std_string.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_vector.i"
%include "typemaps.i"
%{

#include <Phoenix.h>
 //#include "CClearBuffers.h"
  using namespace Phoenix::Graphics;
  using namespace Phoenix::Math;

inline Phoenix::Scene::CScene * 
GameObjToScene( Phoenix::Scene::CGameObject *pObj )
{
	return dynamic_cast<Phoenix::Scene::CScene *>(pObj);
}

inline Phoenix::Scene::CGameObject * 
SceneToGameObj( Phoenix::Scene::CScene *pObj )
{
	return dynamic_cast<Phoenix::Scene::CGameObject *>(pObj);
}

inline Phoenix::Scene::CApplication *
GameObjToApplication( Phoenix::Scene::CGameObject *pObj )
{
	return dynamic_cast<Phoenix::Scene::CApplication *>(pObj);

}

inline std::vector<Phoenix::Scene::CGameObject *> ListToVector( std::list<Phoenix::Scene::CGameObject *> & list )
{
	std::vector<Phoenix::Scene::CGameObject *> objVec;
	std::copy( list.begin(), list.end(), back_inserter(objVec)); 
	return objVec;
}	


inline float    ToReal( float * f )  { return *f;}
inline int      ToInt( float * i )   { return *i;}
inline double   ToReal( double * f ) { return *f;}


%}
// This helps in converting individual elements into values
%typemap(out) float & {
  Tcl_SetObjResult(interp, Tcl_NewDoubleObj(*$1));
}
%include "PhoenixNamed.h"
%include "PhoenixTimer.h"
%include "PhoenixFPSCounter.h"
%include "PhoenixCacheable.h"
%template (cacheable) Phoenix::Core::CCacheable<GLuint>;
%include "PhoenixTypeBase.h"
%template (vertdescTypeBase) Phoenix::Core::CTypeBase< Phoenix::Graphics::ELEMENT_TYPE >;
%include "PhoenixVertexDescriptor.h"
%include "PhoenixNullable.h"
%include "PhoenixResourceManager.h"

%include "PhoenixSingleton.h"

%template (nameCreator) Phoenix::Core::CSingleton<Phoenix::Core::CUniqueNameCreator>;
%template (gameObjHandled) Phoenix::Core::CHandled< Phoenix::Scene::CGameObject >;
%include "PhoenixCore.h"


%include "PhoenixGraphEdge.h"
%template (trEdge) Phoenix::Core::TGraphEdge<Phoenix::Scene::CTransformNode>;

%include "PhoenixGraphNode.h"
%template (trNode) Phoenix::Core::TGraphNode<Phoenix::Scene::CTransformNode>;

%include "PhoenixGraph.h"
%template (trGraph) Phoenix::Core::TGraph<Phoenix::Scene::CTransformNode>;
%include "PhoenixVector2.h"
%template (CVector2f) Phoenix::Math::CVector2<float>;
%template (CVector2i) Phoenix::Math::CVector2<int>;

%include "PhoenixVector3.h"
%template (CVector3f) Phoenix::Math::CVector3<float>;
%template (CVector3i) Phoenix::Math::CVector3<int>;
%include "PhoenixVector4.h"
%template (CVector4f) Phoenix::Math::CVector4<float>;
%template (CVector4i) Phoenix::Math::CVector4<int>;
%template (CVector4c) Phoenix::Math::CVector4<unsigned char>;

%include "PhoenixMatrix4x4.h"
%template (CMatrix4x4f) Phoenix::Math::CMatrix4x4<float>;
%template (CMatrix4x4i) Phoenix::Math::CMatrix4x4<int>;


%include "PhoenixQuaternion.h"
%include "PhoenixTransform.h"
%include "PhoenixTransformGraph.h"
%include "PhoenixSphereBound.h"
%include "PhoenixViewable.h"
%include "PhoenixEnableable.h"
%include "PhoenixBlendingOperation.h"
%include "PhoenixAlphaTestOperation.h"
%include "PhoenixMaterial.h"
%include "PhoenixRenderState.h"
%include "PhoenixPositional.h"
%include "PhoenixDimensional1D.h"
%include "PhoenixDimensional2D.h"
%include "PhoenixDimensional3D.h"
%include "PhoenixSphere.h"
%include "PhoenixOrientable.h"
%include "PhoenixAABB.h"
%include "PhoenixOBB.h"
%include "PhoenixOneDirectional.h"
%include "PhoenixLine.h"
%include "PhoenixRay.h"
%include "PhoenixOrientable.h"
%include "PhoenixCamera.h"
%template (CCameraMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Graphics::CCamera, Phoenix::Core::CHandle<Phoenix::Graphics::CCamera> > >;
%template (CCameraResMgr) Phoenix::Core::CResourceManager< Phoenix::Graphics::CCamera, Phoenix::Core::CHandle<Phoenix::Graphics::CCamera> >;
%include "PhoenixRenderableProperty.h"
%include "PhoenixTagged.h"
%include "PhoenixNamed.h"
%include "PhoenixUpdateable.h"
%include "PhoenixEnableable.h"
%include "PhoenixAIScript.h"
%include "PhoenixCollider.h"
%include "PhoenixSphereCollider.h"
%include "PhoenixCapsuleCollider.h"
%include "PhoenixBoxBound.h"
%include "PhoenixOBBCollider.h"
%include "PhoenixCompoundCollider.h"
// collider mgr template
%template (CColliderMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Collision::ICollider, Phoenix::Core::CHandle<Phoenix::Collision::ICollider> > >;
%template (CColliderResMgr) Phoenix::Core::CResourceManager< Phoenix::Collision::ICollider, Phoenix::Core::CHandle<Phoenix::Collision::ICollider> >;
%include "PhoenixOctree.h"
%template (gameObj_Octree) Phoenix::Spatial::COctree< Phoenix::Scene::CGameObject *>;


%include "PhoenixIndexArray.h"
%include "PhoenixTexture.h"
%template (TEXTURE_HANDLE) Phoenix::Core::CHandle< Phoenix::Graphics::COglTexture >;
%include "PhoenixDefaultEntities.h"
%rename (ostreamModel) operator<<(std::ostream &stream, const Phoenix::Graphics::CModel & model);
%include "PhoenixShader.h"
// shader mgr template
%template (CShaderMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Graphics::CShader, Phoenix::Core::CHandle<Phoenix::Graphics::CShader> > >;
%template (CShaderResMgr) Phoenix::Core::CResourceManager< Phoenix::Graphics::CShader, Phoenix::Core::CHandle<Phoenix::Graphics::CShader> >;
%include "PhoenixModel.h"
%template (CModelMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Graphics::CModel, Phoenix::Core::CHandle<Phoenix::Graphics::CModel> > >;
%template (CModelResMgr) Phoenix::Core::CResourceManager< Phoenix::Graphics::CModel, Phoenix::Core::CHandle<Phoenix::Graphics::CModel> >;
// This is one bugger; Compiler won't accept friend operator defintion for some reason?
//%rename (ostreamRenderable) operator<<(std::ostream &stream, const Phoenix::Graphics::CRenderable & r);
%include  "PhoenixRenderable.h"
%rename (ostreamRenderableModel) operator<<(std::ostream &stream, const Phoenix::Graphics::CRenderableModel & r);
%include "PhoenixClearBuffers.h"
%include "PhoenixRenderableModel.h"
%include "PhoenixGameObject.h"

// Game object mgr template 
%template (CGameObjectMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Scene::CGameObject, Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> > >;
%template (CGameObjectResMgr) Phoenix::Core::CResourceManager< Phoenix::Scene::CGameObject, Phoenix::Core::CHandle<Phoenix::Scene::CGameObject> >;
%include "PhoenixParticleSystem.h"
%include "PhoenixScene.h"
%rename (ostreamRenderableModelShared) operator<<(std::ostream &stream, const Phoenix::Graphics::CRenderableModelShared & r);
%include "PhoenixRenderableModelShared.h"
%include "PhoenixSkybox.h"
%include "PhoenixApplication.h"
%include "PhoenixLight.h"
%include "PhoenixDirectionalLight.h"
%include "PhoenixDirectionalLightObject.h"
%include "PhoenixRenderQueue.h"
 //%include "CClearBuffers.h"
%include "PhoenixCameraObject.h"
%include "PhoenixCollision.h"
%include "PhoenixMath.h"
// Audio system classes
%include "PhoenixListener.h"
%include "PhoenixALObjectArray.h"
%template (CALObjectArray1) Phoenix::Sound::CALObjectArray<1>; 
%template (CALObjectArray2) Phoenix::Sound::CALObjectArray<2>; 
%include "PhoenixSoundBase.h"
%include "PhoenixSoundSampleBase.h"
%include "PhoenixALSampleTypes.h"
%include "PhoenixALSoundTypes.h"
%include "PhoenixOggSample.h"
%include "PhoenixOggStreamSample.h"
%template (AudioSystem) Phoenix::Core::CSingleton< Phoenix::Sound::CAudioSystem >;
%include "PhoenixAudioSystem.h"
    
// sample mgr template
%template (SampleMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Sound::CALSample, Phoenix::Core::CHandle<Phoenix::Sound::CALSample> > >;
%template (SampleResMgr) Phoenix::Core::CResourceManager< Phoenix::Sound::CALSample, Phoenix::Core::CHandle<Phoenix::Sound::CALSample> >;

// stream mgr template
%template (StreamMgr) Phoenix::Core::CSingleton< Phoenix::Core::CResourceManager< Phoenix::Sound::CALStreamSample, Phoenix::Core::CHandle<Phoenix::Sound::CALStreamSample> > >;
%template (StreamResMgr) Phoenix::Core::CResourceManager< Phoenix::Sound::CALStreamSample, Phoenix::Core::CHandle<Phoenix::Sound::CALStreamSample> >;

%template (GameObjectVector) std::vector<Phoenix::Scene::CGameObject *>;
%include "PhoenixOGLRenderer.h"
 //%template (FireworksSingleton) Phoenix::Core::CSingleton< CFireWorks >;
%include "PhoenixPlane.h"
%template (VertexDescriptorHandle) Phoenix::Core::CHandle< Phoenix::Graphics::CVertexDescriptor >;
%template (IndexArrayHandle) Phoenix::Core::CHandle< Phoenix::Graphics::CIndexArray >;





Phoenix::Scene::CScene * GameObjToScene( Phoenix::Scene::CGameObject *pObj );
Phoenix::Scene::CGameObject *  SceneToGameObj( Phoenix::Scene::CScene *pObj );
Phoenix::Scene::CApplication * GameObjToApplication( Phoenix::Scene::CGameObject *pObj );
                             
std::vector<Phoenix::Scene::CGameObject *> ListToVector( std::list<Phoenix::Scene::CGameObject *> & list );



                             float    ToReal( float * f ); 
                             int      ToInt( float * i ); 
                             double   ToReal( double * f );
