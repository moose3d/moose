GENERIC_LIBRARY_NAME="Phoenix"
#AM_CXXFLAGS = -O0 
h_sources = PhoenixAlphaTestOperation.h  PhoenixGameRenderable.h   PhoenixModelHelper.h     PhoenixRenderable.h   PhoenixTexture.h PhoenixBlendingOperation.h   PhoenixGuiSystem.h        PhoenixModelLoader.h     PhoenixRenderQueue.h  PhoenixTGAImage.h PhoenixCamera.h              PhoenixIndexArray.h       PhoenixObjLoader.h       PhoenixRenderState.h  PhoenixVertexDescriptor.h PhoenixDDSImage.h            PhoenixLight.h            PhoenixOGLConsts.h       PhoenixSDLScreen.h PhoenixFrameBuffer.h         PhoenixMaterial.h         PhoenixOGLRenderer.h     PhoenixShader.h PhoenixFrustum.h             PhoenixMilkshapeLoader.h  PhoenixParticleSystem.h  PhoenixSkybox.h

cc_sources = PhoenixCamera.cpp       PhoenixGameRenderable.cpp   PhoenixModelLoader.cpp  PhoenixSDLScreen.cpp  PhoenixTGAImage.cpp PhoenixDDSImage.cpp     PhoenixGuiSystem.cpp        PhoenixObjLoader.cpp    PhoenixShader.cpp     PhoenixVertexDescriptor.cpp PhoenixFrameBuffer.cpp  PhoenixMilkshapeLoader.cpp  PhoenixOGLRenderer.cpp  PhoenixSkybox.cpp PhoenixFrustum.cpp      PhoenixModelHelper.cpp      PhoenixRenderable.cpp   PhoenixTexture.cpp

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

INCLUDES = -I@top_srcdir@/ -I../Core -I../Volume  -I../Graphics -I../Spatial -I../Math
noinst_LTLIBRARIES = libPhoenixGraphics.la
libPhoenixGraphics_la_SOURCES = $(h_sources) $(cc_sources)
