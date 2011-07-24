add_sources(Moose 
  MooseAxisObject.cpp
  MooseGrid.cpp
  MooseTransformIndicator.cpp
  MooseAxisObject.h
  MooseGrid.h
  MooseTransformIndicator.h
  MooseTextureData.h
  MooseTextureData.cpp
  MooseDDSData.h
  MooseDDSData.cpp  
  MooseTGAData.h
  MooseTGAData.cpp
)
if( APPLE AND IOS )
add_sources(   MoosePNGData.h  MoosePNGData.mm)
endif()

