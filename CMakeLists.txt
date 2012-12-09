cmake_minimum_required(VERSION 2.8)
project (tibastro)

# adding some modules, coming from Wesnoth
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# gettext executables are not required when NLS is deactivated
find_package(Gettext)


# this is to have a lot of warnings
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb3 -O0 -Wall -Wextra -std=c99 -Wno-long-long -pedantic")
endif()


# The version number.
set (tibastro_VERSION_MAJOR 0)
set (tibastro_VERSION_MINOR 1)


include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/libtastro")
add_subdirectory (libtastro)
add_subdirectory (cli)

# translation options
if(NOT MSVC)
# Pot updates don't work at Windows so no reason to even try
option(ENABLE_POT_UPDATE_TARGET "Enables the tools to update the pot files and manuals. This target has extra dependencies." OFF)
endif(NOT MSVC)
option(ENABLE_NLS "Enable building of translations" 1)

find_package( Gettext )

#if (GETTEXT_FOUND AND ENABLE_NLS)
# 	add_subdirectory(po)
#endif (GETTEXT_FOUND AND ENABLE_NLS)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
