## Source directory
GENERIC_RELEASE=0:0:1
GENERIC_LIBRARY_VERSION=1:0:0
GENERIC_LIBRARY_NAME="Phoenix"

h_sources = PhoenixAVLTree.h PhoenixCamera.h PhoenixCollision.h PhoenixCore.h PhoenixDDSImage.h PhoenixDefaultEntities.h PhoenixFrameBuffer.h PhoenixFrustum.h PhoenixGameObject.h PhoenixGlobals.h PhoenixGraph.h PhoenixGuiSystem.h Phoenix.h PhoenixIndexArray.h PhoenixLight.h PhoenixMaterial.h PhoenixMathGeometry.h PhoenixMath.h PhoenixMathUtils.h PhoenixMatrix2x2.h PhoenixMatrix3x3.h PhoenixMatrix4x4.h PhoenixMilkshapeLoader.h PhoenixModel.h PhoenixModelHelper.h PhoenixObjectCounter.h PhoenixObjectUpdater.h PhoenixOctree.h PhoenixOGLRenderer.h PhoenixParticleSystem.h PhoenixQuaternion.h PhoenixRenderable.h PhoenixRenderQueue.h PhoenixResourceManager.h PhoenixSDLScreen.h PhoenixShader.h PhoenixSkybox.h PhoenixSpatial.h PhoenixStateMachine.h PhoenixTexture.h PhoenixTGAImage.h PhoenixTransformGraph.h PhoenixTransform.h PhoenixVector2.h PhoenixVector3.h PhoenixVector4.h PhoenixVertexDescriptor.h PhoenixVolume.h PhoenixEventHandler.h PhoenixMessageSystem.h

cc_sources = PhoenixCamera.cpp PhoenixCollision.cpp PhoenixDDSImage.cpp PhoenixFrameBuffer.cpp PhoenixFrustum.cpp PhoenixMath.cpp PhoenixMathGeometry.cpp PhoenixMilkshapeLoader.cpp PhoenixModel.cpp PhoenixModelHelper.cpp PhoenixOGLRenderer.cpp PhoenixQuaternion.cpp PhoenixSDLScreen.cpp PhoenixShader.cpp PhoenixSkybox.cpp PhoenixSpatial.cpp PhoenixTexture.cpp PhoenixTGAImage.cpp PhoenixTransform.cpp PhoenixVertexDescriptor.cpp PhoenixVolume.cpp PhoenixEventHandler.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES= libPhoenix.la
libPhoenix_la_SOURCES= $(h_sources) $(cc_sources)
#libPhoenix_0_99_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
