cmake_minimum_required( VERSION 2.6)
project(Moose)
#set (Moose_VERSION_MAJOR 1)
#set (Moose_VERSION_MINOR 0)
add_definitions("-Wall -O2")
# Optional cmake config file
#configure_file ( 
#  "src/Moose/Core/MooseAPI.h.in"
#  "src/Moose/Core/MooseAPI.h"
#)
#set_property (GLOBAL PROPERTY SRCS "")


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


add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/AI )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Audio )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Graphics )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Math )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Scene )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Spatial )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Util )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/Moose/Volume )

get_property(Moose_SRCS GLOBAL PROPERTY Moose_SRCS)
add_library( Moose STATIC ${Moose_SRCS})

find_package(Freetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(SDL)
include_directories(${SDL_INCLUDE_DIR})