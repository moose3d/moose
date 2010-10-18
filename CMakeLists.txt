cmake_minimum_required( VERSION 2.6)
project(Moose)


add_definitions("-Wall -O2 -fPIC")

SET(MY_OWN_INSTALL_PREFIX "/usr" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)




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
include_directories( ${PROJECT_SOURCE_DIR}/src/Moose)

# Make sure the linker can find the Hello library once it is built.
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/AI)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Core)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Math)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Audio)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Scene)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Graphics)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Volume)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Util)
link_directories( ${PROJECT_BINARY_DIR}/src/Moose/Spatial)
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
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose )

get_property(Moose_SRCS GLOBAL PROPERTY Moose_SRCS)
add_library( Moose STATIC ${Moose_SRCS})

find_package(Freetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(SDL)
include_directories(${SDL_INCLUDE_DIR})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIR})

install( TARGETS Moose 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
FILE(GLOB files 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Moose.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/MooseDefaultEntities.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Core/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/AI/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Audio/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Graphics/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Math/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Scene/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Spatial/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Util/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Volume/*.h")

install( FILES ${files} DESTINATION include/Moose)

FILE( GLOB shaderFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/Moose/Shaders/*")
install( FILES ${shaderFiles} DESTINATION share/Moose/Assets/Shaders)

SET( EXTERNAL_LIBS "-lMoose ${GLEW_LIBRARY} -lGL -lGLU -lfreetype  -lvorbisfile -lvorbis -lm -logg -lopenal -ltcl -lavcodec -lavformat -lavutil -lswscale -lSDL ")
SET( INTERNAL_LIBS libMoose.a ${GLEW_LIBRARY} -lGL -lGLU -lfreetype  -lvorbisfile -lvorbis -lm -logg -lopenal -ltcl -lavcodec -lavformat -lavutil -lswscale -lSDL)

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


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/moose.pc" DESTINATION lib/pkgconfig)
SET (TCL_MODULE		MooseTCL)
SET (INTERFACE_FILES	Moose.i)
#SET (SRC_FILES		Moose_wrap.cxx )
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#Look for tcl

INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
FIND_LIBRARY(TCL_LIBRARY NAMES tcl tcl84 tcl85 PATHS  /usr/lib /usr/local/lib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SWIG_FLAGS -c++ -DMOOSE_API="" -pkgversion 0.9)
SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${INTERFACE_FILES} PROPERTIES CMAKE_SWIG_FLAGS "-includeall ")
#SWIG_ADD_MODULE( ${TCL_MODULE} tcl ${INTERFACE_FILES} ${SRC_FILES})
SWIG_ADD_MODULE( ${TCL_MODULE} tcl ${INTERFACE_FILES} )
ADD_DEPENDENCIES(${TCL_MODULE} Moose)
SWIG_LINK_LIBRARIES(${TCL_MODULE} ${TCL_LIBRARIES} ${INTERNAL_LIBS})
SET_TARGET_PROPERTIES( ${TCL_MODULE} 
  PROPERTIES
  OUTPUT_NAME Moose
  PREFIX ""
  )
FILE(GLOB tclFiles 
  "${CMAKE_CURRENT_BINARY_DIR}/Moose.so"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl"
  "${CMAKE_CURRENT_SOURCE_DIR}/tcl/SceneUtils.tcl"
)

# Generate pkgIndex.tcl
ADD_CUSTOM_COMMAND(
   TARGET ${TCL_MODULE}
   POST_BUILD 
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating pkgIndex.tcl"
   COMMAND echo pkg_mkIndex  . Moose.so|tclsh
 )

install( FILES ${tclFiles} DESTINATION lib/Moose )
