add_sources(Moose 
  MooseExceptions.cpp
  MooseLogger.cpp
  MooseWindowsWrapper.cpp
)
if(APPLE)
  add_sources(Moose MooseAssetBundle.mm)
else(APPLE)
  add_sources(Moose MooseAssetBundle.cpp)
endif(APPLE)

