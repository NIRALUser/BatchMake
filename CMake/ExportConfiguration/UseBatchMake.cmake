#
# This module is provided as BatchMake_USE_FILE by BatchMakeConfig.cmake.
# It can be INCLUDEd in a project to load the needed compiler and linker
# settings to use BatchMake:
#   find_package(BatchMake REQUIRED)
#   include(${BatchMake_USE_FILE})
#

if(NOT BatchMake_USE_FILE_INCLUDED)
  set(BatchMake_USE_FILE_INCLUDED 1)

  # Load the compiler settings used for BatchMake.
  if(BatchMake_BUILD_SETTINGS_FILE)
    include(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
    CMAKE_IMPORT_BUILD_SETTINGS(${BatchMake_BUILD_SETTINGS_FILE})
  endif(BatchMake_BUILD_SETTINGS_FILE)

  # Add compiler flags needed to use BatchMake.
  set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} ${BatchMake_REQUIRED_C_FLAGS}")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${BatchMake_REQUIRED_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} ${BatchMake_REQUIRED_EXE_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} ${BatchMake_REQUIRED_SHARED_LINKER_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS
    "${CMAKE_MODULE_LINKER_FLAGS} ${BatchMake_REQUIRED_MODULE_LINKER_FLAGS}")

  # Add include directories needed to use BatchMake.
  include_directories(${BatchMake_INCLUDE_DIRS})

  # Add link directories needed to use BatchMake.
  link_directories(${BatchMake_LIBRARY_DIRS})

  # Add cmake module path.
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BatchMake_CMAKE_DIR}")

  # Use ITK.
  if(NOT ITK_DIR)
    set(ITK_DIR ${BatchMake_ITK_DIR})
    find_package(ITK)
    if(ITK_FOUND)
      include(${ITK_USE_FILE})
    else(ITK_FOUND)
      message("ITK not found in BatchMake_ITK_DIR=\"${BatchMake_ITK_DIR}\".")
    endif(ITK_FOUND)
  endif(NOT ITK_DIR)

  # Use FLTK.
  if(BatchMake_USE_FLTK AND NOT BatchMake_NO_USE_FLTK)
    if(NOT FLTK_DIR)
      set(FLTK_DIR ${BatchMake_FLTK_DIR})
      find_package(FLTK)
      if(FLTK_FOUND)
        if(FLTK11_USE_FILE)
          set(FLTK11_FOUND ${FLTK_FOUND})
          include(${FLTK11_USE_FILE})
        else(FLTK11_USE_FILE)
          message("FLTK11_USE_FILE not found.")
        endif(FLTK11_USE_FILE)
      else(FLTK_FOUND)
        message(
          "FLTK not found in BatchMake_FLTK_DIR=\"${BatchMake_FLTK_DIR}\".")
      endif(FLTK_FOUND)
    endif(NOT FLTK_DIR)
  endif(BatchMake_USE_FLTK AND NOT BatchMake_NO_USE_FLTK)

  # Same thing should probably be done for Qt and Phantom, but
  # I do not know enough about those packages at the moment

endif(NOT BatchMake_USE_FILE_INCLUDED)
