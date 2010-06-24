GENERIC_LIBRARY_NAME="Phoenix"
AM_CXXFLAGS = -O2
h_sources = PhoenixAlphaTestOperation.h  PhoenixRenderableProperty.h   PhoenixModelHelper.h     PhoenixRenderable.h  PhoenixRenderableModel.h PhoenixTexture.h PhoenixBlendingOperation.h   PhoenixGuiSystem.h        PhoenixModelLoader.h     PhoenixRenderQueue.h  PhoenixTGAImage.h PhoenixCamera.h              PhoenixIndexArray.h       PhoenixObjLoader.h       PhoenixRenderState.h  PhoenixVertexDescriptor.h PhoenixDDSImage.h            PhoenixLight.h            PhoenixOGLConsts.h       PhoenixSDLScreen.h PhoenixFrameBuffer.h         PhoenixMaterial.h         PhoenixOGLRenderer.h     PhoenixShader.h PhoenixFrustum.h             PhoenixMilkshapeLoader.h  PhoenixParticleSystem.h  PhoenixSkybox.h PhoenixAmbientLight.h PhoenixSpotLight.h PhoenixDirectionalLight.h PhoenixPointLight.h PhoenixModel.h PhoenixRenderableModelShared.h PhoenixFFMpeg.h PhoenixRenderableVideo.h PhoenixPolygonOffset.h PhoenixLineRenderable.h PhoenixSphereRenderable.h PhoenixBoxRenderable.h PhoenixClearBuffers.h

cc_sources = PhoenixCamera.cpp       PhoenixRenderableProperty.cpp   PhoenixModelLoader.cpp  PhoenixSDLScreen.cpp  PhoenixTGAImage.cpp PhoenixDDSImage.cpp     PhoenixGuiSystem.cpp        PhoenixObjLoader.cpp    PhoenixShader.cpp     PhoenixVertexDescriptor.cpp PhoenixFrameBuffer.cpp  PhoenixMilkshapeLoader.cpp  PhoenixOGLRenderer.cpp  PhoenixSkybox.cpp PhoenixFrustum.cpp      PhoenixModelHelper.cpp      PhoenixRenderable.cpp   PhoenixTexture.cpp PhoenixRenderableModel.cpp PhoenixAmbientLight.cpp PhoenixDirectionalLight.cpp PhoenixPointLight.cpp PhoenixSpotLight.cpp PhoenixModel.cpp PhoenixRenderableModelShared.cpp PhoenixRenderState.cpp PhoenixFFMpeg.cpp PhoenixRenderableVideo.cpp PhoenixPolygonOffset.cpp PhoenixLineRenderable.cpp PhoenixSphereRenderable.cpp PhoenixBoxRenderable.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math -I../Graphics
noinst_LTLIBRARIES = libPhoenixGraphics.la
libPhoenixGraphics_la_SOURCES = $(h_sources) $(cc_sources)
