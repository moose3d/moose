## Source directory
@SET_MAKE@
AM_CFLAGS = $(CFLAGS) $(CXXFLAGS)
GENERIC_RELEASE=0:0:1
GENERIC_LIBRARY_VERSION=1:0:0
GENERIC_LIBRARY_NAME="Phoenix"

SUBDIRS = Audio

h_sources = PhoenixAlphaTestOperation.h PhoenixAPI.h PhoenixAVLTree.h PhoenixBlendingOperation.h PhoenixCamera.h PhoenixCollision.h PhoenixConvert.h PhoenixCore.h PhoenixDDSImage.h PhoenixDefaultEntities.h PhoenixExceptions.h PhoenixFlock.h PhoenixFrameBuffer.h PhoenixFrustum.h PhoenixGameObject.h PhoenixGlobals.h PhoenixGraph.h PhoenixGuiSystem.h Phoenix.h PhoenixIndexArray.h PhoenixLight.h PhoenixListener.h PhoenixMaterial.h PhoenixMathGeometry.h PhoenixMath.h PhoenixMathUtils.h PhoenixMatrix2x2.h PhoenixMatrix3x3.h PhoenixMatrix4x4.h PhoenixMessageSystem.h PhoenixMilkshapeLoader.h PhoenixModelHelper.h PhoenixModelLoader.h PhoenixObjectCounter.h PhoenixObjectUpdater.h PhoenixObjLoader.h PhoenixOctree.h PhoenixOGLConsts.h PhoenixOGLRenderer.h PhoenixParticleSystem.h PhoenixQuaternion.h PhoenixRenderable.h PhoenixRenderQueue.h PhoenixRenderState.h PhoenixResourceManager.h PhoenixSDLScreen.h PhoenixShader.h PhoenixSkybox.h PhoenixSpatial.h PhoenixStateMachine.h PhoenixTexture.h PhoenixTGAImage.h PhoenixTransformGraph.h PhoenixTransform.h PhoenixVector2.h PhoenixVector3.h PhoenixVector4.h PhoenixVertexDescriptor.h PhoenixVolume.h 

cc_sources = PhoenixCamera.cpp PhoenixCollision.cpp PhoenixDDSImage.cpp PhoenixExceptions.cpp PhoenixFlock.cpp PhoenixFrameBuffer.cpp PhoenixFrustum.cpp PhoenixGameObject.cpp PhoenixGraph.cpp PhoenixGuiSystem.cpp PhoenixListener.cpp PhoenixMath.cpp PhoenixMathGeometry.cpp PhoenixMessageSystem.cpp PhoenixMilkshapeLoader.cpp PhoenixModelHelper.cpp PhoenixModelLoader.cpp PhoenixObjectUpdater.cpp PhoenixObjLoader.cpp PhoenixOctree.cpp PhoenixOGLRenderer.cpp PhoenixQuaternion.cpp PhoenixRenderable.cpp PhoenixSDLScreen.cpp PhoenixShader.cpp PhoenixSkybox.cpp PhoenixSpatial.cpp PhoenixStateMachine.cpp PhoenixTexture.cpp PhoenixTGAImage.cpp PhoenixTransform.cpp PhoenixVertexDescriptor.cpp PhoenixVolume.cpp 

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources) 

INCLUDES = -I$(top_srcdir) 

lib_LTLIBRARIES= libPhoenix.la
libPhoenix_la_SOURCES= $(h_sources) $(cc_sources)
libPhoenix_la_LIBADD = $(CEGUI_LIBS) $(LIBCONFIG_LIBS) Audio/libPhoenixAudio.la
#libPhoenix_0_99_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)




