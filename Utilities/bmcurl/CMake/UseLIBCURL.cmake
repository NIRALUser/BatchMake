#
# This module is provided as LIBCURL_USE_FILE by LIBCURLConfig.cmake.
# It can be INCLUDEd in a project to load the needed compiler and linker
# settings to use LIBCURL:
#   find_package(LIBCURL REQUIRED)
#   include(${LIBCURL_USE_FILE})
#

if(NOT LIBCURL_USE_FILE_INCLUDED)
  set(LIBCURL_USE_FILE_INCLUDED 1)

  # Load the compiler settings used for LIBCURL.
  #if(LIBCURL_BUILD_SETTINGS_FILE)
    #include(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
    #CMAKE_IMPORT_BUILD_SETTINGS(${LIBCURL_BUILD_SETTINGS_FILE})
  #endif(LIBCURL_BUILD_SETTINGS_FILE)

  # Add compiler flags needed to use LIBCURL.
  #set(CMAKE_C_FLAGS
    #"${CMAKE_C_FLAGS} ${LIBCURL_REQUIRED_C_FLAGS}")
  #set(CMAKE_CXX_FLAGS
    #"${CMAKE_CXX_FLAGS} ${LIBCURL_REQUIRED_CXX_FLAGS}")
  #set(CMAKE_EXE_LINKER_FLAGS
    #"${CMAKE_EXE_LINKER_FLAGS} ${LIBCURL_REQUIRED_EXE_LINKER_FLAGS}")
  #set(CMAKE_SHARED_LINKER_FLAGS
    #"${CMAKE_SHARED_LINKER_FLAGS} ${LIBCURL_REQUIRED_SHARED_LINKER_FLAGS}")
  #set(CMAKE_MODULE_LINKER_FLAGS
    #"${CMAKE_MODULE_LINKER_FLAGS} ${LIBCURL_REQUIRED_MODULE_LINKER_FLAGS}")

  # Add include directories needed to use LIBCURL.
  include_directories(${LIBCURL_INCLUDE_DIRS})

  # Add link directories needed to use LIBCURL.
  link_directories(${LIBCURL_LIBRARY_DIRS})

  #add_definitions(-DHAVE_CONFIG_H)

  # Add cmake module path.
  #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LIBCURL_CMAKE_DIR}")

endif(NOT LIBCURL_USE_FILE_INCLUDED)
