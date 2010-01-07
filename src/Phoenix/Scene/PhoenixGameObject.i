%module scene
%include "std_string.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_vector.i"
%{
#include "PhoenixTransformGraph.h"
#include "PhoenixGameObject.h"
#include "PhoenixGraph.h"
#include "PhoenixSingleton.h"
#include "PhoenixNullable.h"
#include "PhoenixResourceManager.h"
#include "PhoenixRenderableProperty.h"
#include "PhoenixViewable.h"
#include "PhoenixTagged.h"
#include "PhoenixNamed.h"
#include "PhoenixObjectUpdater.h"
#include "PhoenixEnableable.h"
#include "PhoenixAIScript.h"
#include "PhoenixCollider.h"
#include "PhoenixSphereCollider.h"
#include "PhoenixCameraObject.h"
#include "PhoenixScene.h"
#include "PhoenixApplication.h"
#include "PhoenixSpatialGraph.h"
#include "PhoenixOctree.h"

%}
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


%include "PhoenixTransformGraph.h"
%include "PhoenixSphereBound.h"
%include "PhoenixViewable.h"
%include "PhoenixRenderableProperty.h"
%include "PhoenixTagged.h"
%include "PhoenixNamed.h"
%include "PhoenixObjectUpdater.h"
%include "PhoenixEnableable.h"
%include "PhoenixAIScript.h"
%include "PhoenixCollider.h"
%include "PhoenixSphereCollider.h"


%include "PhoenixOctree.h"
%template (gameObj_Octree) Phoenix::Spatial::COctree< Phoenix::Scene::CGameObject *>;
%include "PhoenixGameObject.h"


%include "PhoenixSpatialGraph.h"
%include "PhoenixScene.h"
%include "PhoenixApplication.h"
