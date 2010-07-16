GENERIC_LIBRARY_NAME="Moose"
AM_CXXFLAGS = -O2
h_sources = MooseAlphaTestOperation.h  MooseRenderableProperty.h   MooseModelHelper.h     MooseRenderable.h  MooseRenderableModel.h MooseTexture.h MooseBlendingOperation.h   MooseGuiSystem.h        MooseModelLoader.h     MooseRenderQueue.h  MooseTGAImage.h MooseCamera.h              MooseIndexArray.h       MooseObjLoader.h       MooseRenderState.h  MooseVertexDescriptor.h MooseDDSImage.h            MooseLight.h            MooseOGLConsts.h       MooseSDLScreen.h MooseFrameBuffer.h         MooseMaterial.h         MooseOGLRenderer.h     MooseShader.h MooseFrustum.h             MooseMilkshapeLoader.h  MooseParticleSystem.h  MooseSkybox.h MooseAmbientLight.h MooseSpotLight.h MooseDirectionalLight.h MoosePointLight.h MooseModel.h MooseRenderableModelShared.h MooseFFMpeg.h MooseRenderableVideo.h MoosePolygonOffset.h MooseLineRenderable.h MooseSphereRenderable.h MooseBoxRenderable.h MooseClearBuffers.h

cc_sources = MooseCamera.cpp       MooseRenderableProperty.cpp   MooseModelLoader.cpp  MooseSDLScreen.cpp  MooseTGAImage.cpp MooseDDSImage.cpp     MooseGuiSystem.cpp        MooseObjLoader.cpp    MooseShader.cpp     MooseVertexDescriptor.cpp MooseFrameBuffer.cpp  MooseMilkshapeLoader.cpp  MooseOGLRenderer.cpp  MooseSkybox.cpp MooseFrustum.cpp      MooseModelHelper.cpp      MooseRenderable.cpp   MooseTexture.cpp MooseRenderableModel.cpp MooseAmbientLight.cpp MooseDirectionalLight.cpp MoosePointLight.cpp MooseSpotLight.cpp MooseModel.cpp MooseRenderableModelShared.cpp MooseRenderState.cpp MooseFFMpeg.cpp MooseRenderableVideo.cpp MoosePolygonOffset.cpp MooseLineRenderable.cpp MooseSphereRenderable.cpp MooseBoxRenderable.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../Graphics
noinst_LTLIBRARIES = libMooseGraphics.la
libMooseGraphics_la_SOURCES = $(h_sources) $(cc_sources)
