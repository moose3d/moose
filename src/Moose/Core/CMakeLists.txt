add_sources(Moose 
  MooseExceptions.cpp
  MooseLogger.cpp
  MooseWindowsWrapper.cpp
  MooseAPI.h
  MooseAVLTree.h
  MooseAssetBundle.h
  MooseCacheable.h
  MooseConvert.h
  MooseCore.h
  MooseEnableable.h
  MooseExceptions.h
MooseFPSCounter.h
MooseFocusable.h
MooseGlobals.h
MooseGraph.h
MooseGraphEdge.h
MooseGraphNode.h
MooseHashTable.h
MooseLog.h
MooseLogger.h
MooseNamed.h
MooseNullable.h
MooseObjectCounter.h
MooseRadixSort.h
MooseRadixSortOld.h
MooseResourceManager.h
MooseSelectable.h
MooseSingleton.h
MooseTagged.h
MooseTimeStamp.h
MooseTimer.h
MooseTypeBase.h
MooseUniqueNameCreator.h
MooseUpdateable.h
MooseViewable.h
MooseWindowsWrapper.h

)
if(APPLE AND IOS)
  add_sources(Moose MooseAssetBundle.mm)
elseif(APPLE OR UNIX)
  add_sources(Moose MooseAssetBundle.cpp)
endif()

