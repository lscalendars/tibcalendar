cmake_minimum_required(VERSION 2.8)
project (tibastro)

# The version number.
set (tibastro_VERSION_MAJOR 0)
set (tibastro_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/libtastro")
add_subdirectory (libtastro)
add_subdirectory (cli)

