

add_sources (Moose

  MooseAlphaTestOperation.h
  MooseAmbientLight.cpp  
  MooseAmbientLight.h  
  MooseBlendingOperation.h
  MooseBoxRenderable.cpp
  MooseBoxRenderable.h
  MooseCamera.cpp
  MooseCamera.h
  MooseCapsuleRenderable.cpp
  MooseCapsuleRenderable.h
  MooseDDSImage.cpp
  MooseDDSImage.h
  MooseDirectionalLight.cpp
  MooseDirectionalLight.h

  MooseFrameBuffer.cpp
  MooseFrameBuffer.h
  MooseFrustum.cpp
  MooseFrustum.h
  MooseLineRenderable.cpp
  MooseLineRenderable.h
  MooseMilkshapeLoader.cpp
  MooseMilkshapeLoader.h
  MooseModel.cpp
  MooseModel.h
  MooseModelHelper.cpp
  MooseModelHelper.h
  MooseModelLoader.cpp
  MooseModelLoader.h
  MooseOGLRenderer.cpp
  MooseOGLRenderer.h
  MooseObjLoader.cpp
  MooseObjLoader.h
  MooseOpenAssetImportLoader.h
  MooseOpenAssetImportLoader.cpp
  MooseParticleSystem.h
  MooseParticleSystemRenderable.h
  MoosePointLight.cpp
  MoosePointLight.h
  MoosePolygonOffset.cpp
  MoosePolygonOffset.h
  MooseRenderState.cpp
  MooseRenderState.h
  MooseRenderable.cpp
  MooseRenderable.h
  MooseRenderableModel.cpp
  MooseRenderableModel.h
  MooseRenderableModelShared.cpp
  MooseRenderableModelShared.h
  MooseRenderableProperty.cpp
  MooseRenderableProperty.h
  MooseScreen.h
  MooseSDLScreen.cpp
  MooseShader.cpp
  MooseShader.h
  MooseSkybox.cpp
  MooseSkybox.h
  MooseSphereRenderable.cpp
  MooseSphereRenderable.h
  MooseSpotLight.cpp
  MooseSpotLight.h
  MooseTGAImage.cpp
  MooseTGAImage.h
  MooseTexture.cpp
  MooseTexture.h
  MooseVertexDescriptor.cpp
  MooseVertexDescriptor.h
  MooseRocketRenderable.h
  MooseRocketRenderable.cpp
  MooseRocketRenderInterface.h
  MooseRocketRenderInterface.cpp
  )

if(UNIX AND (NOT APPLE))
  add_sources( Moose
    MooseFFMpeg.cpp
    MooseFFMpeg.h
    MooseRenderableVideo.cpp
    MooseRenderableVideo.h
  )
endif()
#set (graphics_sources  MooseVideoTexture.cpp)
#set (graphics_sources  MooseGuiSystem.cpp)
