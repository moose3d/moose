cmake_minimum_required( VERSION 2.6)
project(Moose)
# By default, build for IOS
#SET(IOS 1)

if(APPLE AND IOS)
# Platform Info
SET (CMAKE_SYSTEM_VERSION 1)
SET (CMAKE_SYSTEM_PROCESSOR arm)

# SDK Info
SET (SDKVER "4.3")
SET (DEVROOT "/Developer/Platforms/iPhoneOS.platform/Developer")
SET (SDKROOT "${DEVROOT}/SDKs/iPhoneOS${SDKVER}.sdk")
SET (CMAKE_OSX_SYSROOT "${SDKROOT}")
SET (CMAKE_OSX_ARCHITECTURES "armv7")
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

# C Compiler
SET (CMAKE_C_COMPILER "${DEVROOT}/usr/bin/llvm-gcc-4.2")
#SET (LINK_FLAGS "-arch armv6 -arch armv7")
#SET (CMAKE_C_LINK_EXECUTABLE "${DEVROOT}/usr/bin/g++-4.2")
SET (CMAKE_C_FLAGS "-x objective-c")
SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -ggdb")

# CXX Compiler
SET (CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/llvm-g++-4.2")
SET (CMAKE_CXX_FLAGS "-x objective-c++")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -ggdb")

# Definitions
#ADD_DEFINITIONS("-arch armv6")
ADD_DEFINITIONS("-arch armv7")
ADD_DEFINITIONS("-pipe")
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
#ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

# System Includes
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1/armv6-apple-darwin10")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/opt/iphone-${SDKVER}/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/local/iphone-${SDKVER}/include")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/usr/lib/gcc/arm-apple-darwin10/4.2.1/")
#LINK_DIRECTORIES("${SDKROOT}/opt/iphone-${SDKVER}/lib")
#LINK_DIRECTORIES("${SDKROOT}/usr/local/iphone-${SDKVER}/lib")

# Root Paths
SET (CMAKE_FIND_ROOT_PATH "${SDKROOT}" "/opt/iphone-${SDKVER}/" "/usr/local/iphone-${SDKVER}/")
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# CMake Parameters
SET (iPhone 1)
SET (iPhoneOS 1)
SET (iPhoneOS_VERSION ${SDKVER})
SET (CMAKE_CROSSCOMPILING 1)

#SET_TARGET_PROPERTIES(p3dm PROPERTIES LINK_FLAGS "-arch armv6 -arch armv7")

# HELPERS
#---------

# MACRO(ADD_FRAMEWORK appname fwname)
#     find_library(FRAMEWORK_${fwname}
#         NAMES ${fwname}
#         PATHS ${SDKROOT}/System/Library
#         PATH_SUFFIXES Frameworks
#         NO_DEFAULT_PATH)
#     if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
#         MESSAGE(ERROR ": Framework ${fwname} not found")
#     else()
#         TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
#         MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
#     endif()
# endmacro(ADD_FRAMEWORK)
endif(APPLE AND IOS)



# Setting prefix for Linux
if(UNIX AND (NOT IOS))

SET(MY_OWN_INSTALL_PREFIX "/usr" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

elseif(UNIX AND IOS)

SET(MY_OWN_INSTALL_PREFIX "~/Library/Moose-iOS" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

endif()

#- Add sources for a target
#
#  ADD_SOURCES(<target> <source1> [<source2> ...])
#
function(add_sources target)
  # define the <target>_SRCS properties if necessary
  get_property(prop_defined GLOBAL PROPERTY ${target}_SRCS DEFINED)
  if(NOT prop_defined)
    define_property(GLOBAL PROPERTY ${target}_SRCS
      BRIEF_DOCS "Sources for the ${target} target"
      FULL_DOCS "List of source files for the ${target} target")
  endif()
  # create list of sources (absolute paths)
  set(SRCS)
  foreach(src IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${src}")
      get_filename_component(src "${src}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${src}")
  endforeach()
  # append to global property
  set_property(GLOBAL APPEND PROPERTY "${target}_SRCS" "${SRCS}")
endfunction()


include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/AI)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Core)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Math)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Audio)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Scene)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Graphics)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Volume)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Util)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Spatial)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose/Physics)
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose)

# Make sure the linker can find the Moose library once it is built.
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/AI)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Core)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Math)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Audio)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Scene)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Graphics)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Volume)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Util)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Spatial)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Physics)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose)

link_directories( ${PROJECT_BINARY_DIR} )

add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/AI )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Audio )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Graphics )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Math )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Scene )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Spatial )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Util )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Volume )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Physics )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose )

get_property(Moose_SRCS GLOBAL PROPERTY Moose_SRCS)

#if(APPLE)
#add_library( Moose SHARED ${Moose_SRCS})
#set_target_properties( Moose PROPERTIES FRAMEWORK TRUE )
#else(APPLE)
# For linux, build regular static library
add_library( Moose SHARED ${Moose_SRCS})

FILE(GLOB audio_files 
  "${PROJECT_SOURCE_DIR}/src/Moose/Audio/*.h"
  "${PROJECT_SOURCE_DIR}/src/Moose/Audio/*.cpp"
)
# Set specific cxxflags for audio sources, samples tend to corrupt if any optimizations are used.
set_source_files_properties( ${Moose_SRCS} PROPERTIES COMPILE_FLAGS "-Wall -O2 -fPIC")
set_source_files_properties( ${audio_files} PROPERTIES COMPILE_FLAGS "-Wall -O0 -fPIC")

# ----------------------------------------------------------------------------
# iPhone-specific frameworks
# ----------------------------------------------------------------------------
if(APPLE AND IOS)

find_library( FOUNDATION_LIB Foundation)
find_library( OPENAL_LIB OpenAL)
find_library( OPENGLES_LIB OpenGLES)
find_library( AUDIOTOOLBOX_LIB AudioToolbox)
find_library( UIKIT_LIB UIKit)
find_library( QUARTZCORE_LIB QuartzCore)
find_library( COREGRAPHICS_LIB CoreGraphics)
MARK_AS_ADVANCED(FOUNDATION_LIB
OPENAL_LIB
OPENGLES_LIB
AUDIOTOOLBOX_LIB
UIKIT_LIB
QUARTZCORE_LIB
COREGRAPHICS_LIB
)
SET(EXTRA_LIBS ${FOUNDATION_LIB})
TARGET_LINK_LIBRARIES( Moose ${EXTRA_LIBS})
elseif(APPLE AND (NOT IOS))
# ----------------------------------------------------------------------------
# Apple-specific frameworks
# ---------------------------------------------------------------------------
find_package(Freetype)
find_package(SDL)
FIND_PACKAGE(Assimp)
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${SDL_INCLUDE_DIR})
include_directories("/usr/include")

find_library(VORBIS_LIB Vorbis)
MARK_AS_ADVANCED(
  VORBIS_LIB
)


IF(ASSIMP_FOUND)
include_directories(${ASSIMP_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${ASSIMP_LIBRARIES}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${ASSIMP_LIBRARIES}")
ENDIF()


elseif(UNIX AND (NOT APPLE) AND (NOT IOS))

# ----------------------------------------------------------------------------
# Linux-specific libraries
# ----------------------------------------------------------------------------



FIND_PACKAGE(Vorbis)
IF(VORBIS_FOUND)
include_directories(${VORBIS_INCLUDE_DIR})
LIST(APPEND EXTERNAL_LIBS ${VORBIS_LIBRARY})
LIST(APPEND INTERNAL_LIBS ${VORBIS_LIBRARY})
ENDIF()

FIND_PACKAGE(Assimp)
IF(ASSIMP_FOUND)
include_directories(${ASSIMP_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${ASSIMP_LIBRARIES})
SET(INTERNAL_LIBS ${INTERNAL_LIBS} ${ASSIMP_LIBRARIES})
ENDIF()

endif()

find_package(GLEW)
FIND_PACKAGE(Bullet)
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenAL)
FIND_PACKAGE(TCL)
FIND_PACKAGE(SDL)

#FIND_PACKAGE(FTGL)

IF(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIR})
LIST(APPEND EXTERNAL_LIBS " ${GLEW_LIBRARY}")
LIST(APPEND INTERNAL_LIBS " ${GLEW_LIBRARY}")
ENDIF()

IF(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  LIST(APPEND EXTERNAL_LIBS " ${SDL_LIBRARY}")
  LIST(APPEND INTERNAL_LIBS " ${SDL_LIBRARY}")
ENDIF()

FIND_PACKAGE(Freetype)

IF(FREETYPE_FOUND)
  include_directories(${FREETYPE_INCLUDE_DIRS})
  LIST(APPEND EXTERNAL_LIBS " ${FREETYPE_LIBRARIES}")
  LIST(APPEND INTERNAL_LIBS " ${FREETYPE_LIBRARIES}")
  MESSAGE( STATUS "Found freetype: ${FREETYPE_INCLUDE_DIRS}")
ELSE()
  MESSAGE( ERROR "Freetype not found!")
ENDIF()


IF(BULLET_FOUND)
include_directories(${BULLET_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${BULLET_LIBRARIES}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${BULLET_LIBRARIES}")
ENDIF()

IF(OPENGL_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${OPENGL_LIBRARIES}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${OPENGL_LIBRARIES}")
ENDIF()

IF(OPENAL_FOUND)
include_directories(${OPENAL_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${OPENAL_LIBRARY}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${OPENAL_LIBRARY}")
ENDIF()

IF(TCL_FOUND)
include_directories(${TCL_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${TCL_LIBRARY}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${TCL_LIBRARY}")
ENDIF()


IF(FTGL_FOUND)
include_directories(${FTGL_INCLUDE_DIR})
SET(EXTERNAL_LIBS ${EXTERNAL_LIBS} " ${FTGL_LIBRARY}")
SET(INTERNAL_LIBS ${INTERNAL_LIBS} " ${FTGL_LIBRARY}")
ENDIF()


# libs settings
#SET( EXTERNAL_LIBS ${EXTERNAL_LIBS} "-lMoose")
#SET( INTERNAL_LIBS ${INTERNAL_LIBS} "libMoose.a")


SET( EXTERNAL_LIBS ${EXTERNAL_LIBS} "-lm -logg  -lavcodec -lavformat -lavutil -lswscale -lRocketCore")
SET( INTERNAL_LIBS ${INTERNAL_LIBS} "-lm -logg  -lavcodec -lavformat -lavutil -lswscale -lRocketCore")
# ------------------------------
# Replace semicolons with whitespace
string(REPLACE ";" " " TMP "${EXTERNAL_LIBS}")
SET( EXTERNAL_LIBS ${TMP})
string(REPLACE ";" "" TMP "${INTERNAL_LIBS}")
SET( INTERNAL_LIBS ${TMP})

install( TARGETS Moose 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

FILE(GLOB files 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Moose.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/MooseDefaultEntities.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Core/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Core/*.tpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/AI/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Audio/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Graphics/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Math/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Scene/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Spatial/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Util/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Volume/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Physics/*.h")

if(APPLE AND IOS)
install( FILES ${files} DESTINATION headers)
elseif(APPLE AND (NOT IOS))
install( FILES ${files} DESTINATION include/Moose)
elseif(UNIX)
install( FILES ${files} DESTINATION include/Moose)
endif()


#set (Moose_VERSION_MAJOR 1)
#set (Moose_VERSION_MINOR 0)
set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix "\${prefix}")
set (libdir "\${exec_prefix}/lib")
set (includedir "\${prefix}/include")
set (libraries "${EXTERNAL_LIBS}")
# Optional cmake config file
configure_file ( 
  "moose.pc.in"
  "moose.pc"
)
# Process shaders for separate platforms
FILE(GLOB shaderInFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Shaders/" "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Shaders/*.in")

foreach(shader ${shaderInFiles})
  string(REPLACE ".in" "" realShader ${shader})
  set (version "")
  IF(UNIX AND (NOT IOS))
    set (version "#version 150")
  endif()
  configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Shaders/${shader}" 
                  "${CMAKE_CURRENT_BINARY_DIR}/src/Moose/Shaders/${realShader}")
endforeach(shader)

FILE( GLOB shaderFiles 
  "${CMAKE_CURRENT_BINARY_DIR}/src/Moose/Shaders/*.vert"
  "${CMAKE_CURRENT_BINARY_DIR}/src/Moose/Shaders/*.frag"
)
if(APPLE AND IOS)
install( FILES ${shaderFiles} DESTINATION Assets/Shaders)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/moose.pc" DESTINATION .)
else()
install( FILES ${shaderFiles} DESTINATION share/Moose/Assets/Shaders)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/moose.pc" DESTINATION lib/pkgconfig)
endif()

 # BUILD Tcl module
if((NOT IOS))

SET (TCL_MODULE		MooseTCL)
SET (INTERFACE_FILES	Moose.i)
#SET (SRC_FILES		Moose_wrap.cxx )

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#Look for tcl

INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
#FIND_LIBRARY(TCL_LIBRARY NAMES tcl tcl84 tcl85 PATHS  /usr/lib /usr/local/lib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SWIG_FLAGS -c++ -DMOOSE_API="" -pkgversion 0.9)
SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES CMAKE_SWIG_FLAGS "-includeall ")
#SWIG_ADD_MODULE( ${TCL_MODULE} tcl ${INTERFACE_FILES} ${SRC_FILES})
SWIG_ADD_MODULE( ${TCL_MODULE} tcl ${INTERFACE_FILES} )
#ADD_DEPENDENCIES(${TCL_MODULE} Moose)
#SWIG_LINK_LIBRARIES(${TCL_MODULE} ${TCL_LIBRARIES} ${INTERNAL_LIBS} )
SWIG_LINK_LIBRARIES(${TCL_MODULE} libMoose.so ${VORBIS_LIBRARY} ${OPENAL_LIBRARY} ${GLEW_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES} ${FTGL_LIBRARY} tcl -L/opt/local/lib -lfreetype -logg -lvorbisfile -lswscale -lavcodec -lavutil -lavformat -lRocketCore)

SET_TARGET_PROPERTIES( ${TCL_MODULE} 
  PROPERTIES
  OUTPUT_NAME Moose
  PREFIX ""
)

IF(APPLE)
SET_TARGET_PROPERTIES( ${TCL_MODULE} 
  PROPERTIES
  SUFFIX ".dylib"
)
# Generate pkgIndex.tcl
ADD_CUSTOM_COMMAND(
   TARGET ${TCL_MODULE}
   POST_BUILD 
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating pkgIndex.tcl"
   COMMAND echo pkg_mkIndex  . Moose.dylib|tclsh
   )

ADD_CUSTOM_COMMAND(
   TARGET ${TCL_MODULE}
   POST_BUILD 
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Enabling auto-sourcing of SceneUtils"
   COMMAND echo "source \\$$dir/SceneUtils.tcl" >> pkgIndex.tcl
   )


SET( tclFiles 
  "${CMAKE_CURRENT_BINARY_DIR}/Moose.dylib"  
  "${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl"  
  "${CMAKE_CURRENT_SOURCE_DIR}/tcl/SceneUtils.tcl"
)
# Mac OS X has default TCL installation apparently in /opt/local? 
# At least in my system.
install( FILES ${tclFiles} DESTINATION /opt/local/lib/Moose )
ELSEIF(UNIX AND (NOT APPLE))
# Generate pkgIndex.tcl
ADD_CUSTOM_COMMAND(
   TARGET ${TCL_MODULE}
   POST_BUILD 
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating pkgIndex.tcl"
   COMMAND echo pkg_mkIndex  . Moose.so|tclsh
   )

ADD_CUSTOM_COMMAND(
   TARGET ${TCL_MODULE}
   POST_BUILD 
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Enabling auto-sourcing of SceneUtils"
   COMMAND echo "source \\$$dir/SceneUtils.tcl" >> pkgIndex.tcl
   )

SET( tclFiles 
  "${CMAKE_CURRENT_BINARY_DIR}/Moose.so"  
  "${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl"  
  "${CMAKE_CURRENT_SOURCE_DIR}/tcl/SceneUtils.tcl"
)
install( FILES ${tclFiles} DESTINATION lib/Moose )
ENDIF()

endif((NOT IOS))
