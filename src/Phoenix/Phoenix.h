

// This fixes error on win32 platforms with CTimer
#undef GetCurrentTime
/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "PhoenixWindowsWrapper.h"
#endif
#include "PhoenixGuiSystem.h"
#include "PhoenixAVLTree.h"
#include "PhoenixCore.h"
#include "PhoenixGlobals.h"
#include "PhoenixIndexArray.h"
#include "PhoenixMath.h" 
#include "PhoenixMathUtils.h"
#include "PhoenixMatrix2x2.h"
#include "PhoenixMatrix3x3.h"
#include "PhoenixMatrix4x4.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixObjectCounter.h"
#include "PhoenixQuaternion.h"
#include "PhoenixSDLScreen.h"
#include "PhoenixSpatial.h"
#include "PhoenixTGAImage.h"
#include "PhoenixVector2.h"
#include "PhoenixVector3.h"
#include "PhoenixVector4.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixCollision.h"
#include "PhoenixMilkshapeLoader.h"
#include "PhoenixLight.h"
#include "PhoenixGraph.h"
#include "PhoenixOctree.h"
#include "PhoenixSkybox.h"
#include "PhoenixRenderQueue.h"
#include "PhoenixGameObject.h"
#include "PhoenixTransform.h"
#include "PhoenixTransformGraph.h"
#include "PhoenixParticleSystem.h"
#include "PhoenixObjectUpdater.h"
#include "PhoenixStateMachine.h"
#include "PhoenixModelHelper.h"
#include "PhoenixDDSImage.h"
#include "PhoenixMessageSystem.h"
#include "PhoenixModelLoader.h"
#include "PhoenixObjLoader.h"
#include <PhoenixALObjectArray.h>
#include <PhoenixALSampleTypes.h>
#include <PhoenixALSoundTypes.h>
#include <PhoenixAudioSystem.h>
#include <PhoenixListener.h>
#include <PhoenixOggSample.h>
#include <PhoenixOggStreamSample.h>
#include <PhoenixSoundBase.h>
#include <PhoenixSoundSampleBase.h>
