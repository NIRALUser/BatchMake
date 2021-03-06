cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(BatchMake)

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BatchMake_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all libraries.")
endif(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
mark_as_advanced(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BatchMake_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all libraries.")
endif(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
mark_as_advanced(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
set(BatchMake_LIBRARY_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(BatchMake_RUNTIME_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BatchMake_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all executables.")
endif(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
mark_as_advanced(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
set(BatchMake_APPLICATIONS_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Applications")

set(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(BatchMake_CMAKE_DIR "${BatchMake_SOURCE_DIR}/CMake" CACHE INTERNAL "")
subdirs(CMake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BatchMake_CMAKE_DIR}")

set(BatchMake_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES})

mark_as_advanced(CMAKE_BACKWARDS_COMPATIBILITY)

# On Mac we need the flag -fno-common
if(APPLE)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
endif(APPLE)


#-----------------------------------------------------------------------------
# BatchMake version number.  An even minor number corresponds to releases.

set(BatchMake_VERSION_MAJOR "1")
set(BatchMake_VERSION_MINOR "3")
set(BatchMake_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(BatchMake_VERSION_STRING
  "${BatchMake_VERSION_MAJOR}.${BatchMake_VERSION_MINOR}")
set(BatchMake_EXTENDED_VERSION_STRING
  "${BatchMake_VERSION_STRING}.${BatchMake_VERSION_PATCH}")

#-----------------------------------------------------------------------------
# Disable MSVC 8 warnings

if(WIN32)
  option(DISABLE_MSVC8_DEPRECATED_WARNINGS
    "Disable Visual Studio 8 deprecated warnings" ON)
  mark_as_advanced(FORCE DISABLE_MSVC8_DEPRECATED_WARNINGS)
  if(DISABLE_MSVC8_DEPRECATED_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  endif(DISABLE_MSVC8_DEPRECATED_WARNINGS)
endif(WIN32)

#-----------------------------------------------------------------------------
# We need ITK

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
endif(ITK_FOUND)

set(BUILD_SHARED_LIBS ${ITK_BUILD_SHARED})

set(BatchMake_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------
# Check for support FLTK

option(USE_FLTK "Add FLTK GUI support to BatchMake" OFF)
if(USE_FLTK)
  find_package(FLTK)
  if(FLTK_FOUND)
    include_directories(${FLTK_INCLUDE_DIR})
  endif(FLTK_FOUND)
  add_definitions(-DBM_USE_FLTK)
endif(USE_FLTK)

#-----------------------------------------------------------------------------
# Display the splashscreen or not
option(USE_SPLASHSCREEN "Display the splashscreen" ON)
if(USE_SPLASHSCREEN)
 add_definitions(-DBM_SPLASHSCREEN)
endif(USE_SPLASHSCREEN)

#-----------------------------------------------------------------------------
# Use XCEDE Functionalities
option(BATCHMAKE_USE_XCEDE "Use the XCEDE Commands" OFF)
if(BATCHMAKE_USE_XCEDE)
  if(NOT EXISTS ${ITK_DIR}/Utilities/itklibxml2/config.h)
    message("ITK_USE_LIBXML2 Should be set to ON in ITK in order to use XCEDE Functionality")
  endif(NOT EXISTS ${ITK_DIR}/Utilities/itklibxml2/config.h)

  add_definitions(-DBM_XCEDE)
endif(BATCHMAKE_USE_XCEDE)

#-----------------------------------------------------------------------------
# Setup Paths

if(BatchMake_INCLUDE_DIR)
  set(BatchMake_INCLUDE_PATH
    ${BatchMake_INCLUDE_DIR}/Code
    ${BatchMake_INCLUDE_DIR}/Utilities
    ${BatchMake_INCLUDE_DIR}/Utilities/Zip
    )
endif(BatchMake_INCLUDE_DIR)

set(BatchMake_INCLUDE_PATH
  ${BatchMake_INCLUDE_PATH}
  ${BatchMake_SOURCE_DIR}/Code
  ${BatchMake_SOURCE_DIR}/Utilities
  ${BatchMake_BINARY_DIR}
  ${BatchMake_BINARY_DIR}/Code
  ${BatchMake_SOURCE_DIR}/Utilities/bmModuleDescriptionParser
  ${BatchMake_BINARY_DIR}/Utilities/bmModuleDescriptionParser
  )

if(USE_FLTK)
  if(BatchMake_INCLUDE_DIR)
    set(BatchMake_INCLUDE_PATH
      ${BatchMake_INCLUDE_PATH}
      ${BatchMake_INCLUDE_DIR}/Utilities/Flu
      )
  endif(BatchMake_INCLUDE_DIR)
  set(BatchMake_INCLUDE_PATH
    ${BatchMake_INCLUDE_PATH}
    ${BatchMake_SOURCE_DIR}/Utilities/Flu
    )
endif(USE_FLTK)

include_directories(${BatchMake_INCLUDE_PATH})

#-----------------------------------------------------------------------------
# If this is the install version of BathMake we look for the include files

find_path(BatchMake_INCLUDE_DIR
  Code/ApplicationWrapper.h
  ${BatchMake_SOURCE_DIR}/../include/BatchMake)
mark_as_advanced(FORCE BatchMake_INCLUDE_DIR)


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#Compile the Utilities

subdirs( Utilities )

#-----------------------------------------------------------------------------
#Compile the main libraries

subdirs( Code )

set(BatchMake_LIBRARIES
  BatchMakeLib
  BatchMakeUtilities
  )

#-----------------------------------------------------------------------------
# Build BatchBoard Support

option(DASHBOARD_SUPPORT "Validation Dashboard Support" OFF)
if(DASHBOARD_SUPPORT)

  add_definitions(-DBM_DASHBOARD)

  if(BatchMake_INCLUDE_DIR)
    set(BatchMake_INCLUDE_PATH
      ${BatchMake_INCLUDE_PATH}
      ${BatchMake_INCLUDE_DIR}/Dashboard
      )
    include_directories(${BatchMake_INCLUDE_DIR}/Grid)
  endif(BatchMake_INCLUDE_DIR)
  set(BatchMake_INCLUDE_PATH
    ${BatchMake_INCLUDE_PATH}
    ${BatchMake_SOURCE_DIR}/Code/Dashboard
    )
  include_directories(${BatchMake_SOURCE_DIR}/Code/Dashboard)

endif(DASHBOARD_SUPPORT)

#-----------------------------------------------------------------------------
# Grid Support

option(GRID_SUPPORT "Grid Support" OFF)
if(GRID_SUPPORT)

  option(BATCHBOARD_SUPPORT "Support for BatchBoards" ON)
  if( BATCHBOARD_SUPPORT )
    add_definitions(-DBM_BATCHBOARD)
  endif( BATCHBOARD_SUPPORT )

  add_definitions(-DBM_GRID)

  if(BatchMake_INCLUDE_DIR)
    set(BatchMake_INCLUDE_PATH
      ${BatchMake_INCLUDE_PATH}
      ${BatchMake_INCLUDE_DIR}/Grid
      )
    include_directories(${BatchMake_INCLUDE_DIR}/Grid)
  endif(BatchMake_INCLUDE_DIR)
  set(BatchMake_INCLUDE_PATH
    ${BatchMake_INCLUDE_PATH}
    ${BatchMake_SOURCE_DIR}/Code/Grid
    )
  include_directories(${BatchMake_SOURCE_DIR}/Code/Grid)

  if(USE_FLTK)
    if(BatchMake_INCLUDE_DIR)
      set(BatchMake_INCLUDE_PATH
        ${BatchMake_INCLUDE_PATH}
        ${BatchMake_INCLUDE_DIR}/Utilities/CondorWatcher
        )
      include_directories(${BatchMake_INCLUDE_DIR}/Utilities/CondorWatcher)
    endif(BatchMake_INCLUDE_DIR)
    set(BatchMake_INCLUDE_PATH
      ${BatchMake_INCLUDE_PATH}
      ${BatchMake_SOURCE_DIR}/Utilities/CondorWatcher
      ${BatchMake_BINARY_DIR}/Utilities/CondorWatcher
      )
    include_directories(${BatchMake_SOURCE_DIR}/Utilities/CondorWatcher)
    include_directories(${BatchMake_BINARY_DIR}/Utilities/CondorWatcher)
  endif(USE_FLTK)

endif(GRID_SUPPORT)

#-----------------------------------------------------------------------------
# Build Testing

subdirs( Testing/Code )

mark_as_advanced(DART_ROOT)
include(Dart)
if(NOT DART_ROOT)
  set(DROP_LOCATION /DartClient/BatchMake)
endif(NOT DART_ROOT)

#-----------------------------------------------------------------------------
# Build the Applications

subdirs(Applications)

#-----------------------------------------------------------------------------
# Build the Examples

option(BUILD_EXAMPLES "BatchMake Examples" OFF)
if(BUILD_EXAMPLES)
  subdirs(Examples)
endif(BUILD_EXAMPLES)

#-----------------------------------------------------------------------------
# Doc
subdirs(Documentation/Doxygen)


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Use KWStyle for checking the coding style

option(BatchMake_USE_KWSTYLE
  "Run KWStyle in order to check for violations of the coding standard." OFF)
mark_as_advanced(BatchMake_USE_KWSTYLE)
if(BatchMake_USE_KWSTYLE)
  subdirs(Utilities/KWStyle)
endif(BatchMake_USE_KWSTYLE)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
configure_file(${BatchMake_SOURCE_DIR}/bmConfigure.h.in
               ${BatchMake_BINARY_DIR}/bmConfigure.h)

set(BATCHMAKE_PACKAGE_SOURCE OFF)

install(FILES ${BatchMake_BINARY_DIR}/bmConfigure.h
  DESTINATION include/BatchMake)

install(FILES ${BatchMake_BINARY_DIR}/BatchMakeConfig.cmake
  ${BatchMake_BINARY_DIR}/BatchMakeBuildSettings.cmake
  ${BatchMake_SOURCE_DIR}/CMake/ExportConfiguration/UseBatchMake.cmake
  DESTINATION lib/BatchMake)

if(BATCHMAKE_PACKAGE_SOURCE)
  install(FILES CMakeLists.txt Copyright.txt ReadMe.txt DartConfig.cmake
    DESTINATION source/BatchMake)

  file(GLOB IMAGES Images/?*.*)
  install(FILES ${IMAGES} DESTINATION source/BatchMake/Images)
endif(BATCHMAKE_PACKAGE_SOURCE)

#-----------------------------------------------------------------------------
# CPack options
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BatchMake")
set(CPACK_PACKAGE_VENDOR "Kitware Inc.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${BatchMake_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BatchMake_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${BatchMake_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY
  "BatchMake ${BatchMake_EXTENDED_VERSION_STRING}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_PACKAGE_ICON
    "${BatchMake_SOURCE_DIR}/Documentation/images\\\\InstallImage.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "relwithdebinfo\\\\BatchMake.exe")
  set(CPACK_NSIS_DISPLAY_NAME
    "${CPACK_PACKAGE_INSTALL_DIRECTORY} The Batch Processing Scripting Tool")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\public.kitware.com/BatchMake")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\public.kitware.com/BatchMake")
  set(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
  set(CPACK_STRIP_FILES "bin/BatchMake")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES "BatchMake" "BatchMake")
include(CPack)

#-----------------------------------------------------------------------------
# Export the configuration files
subdirs(CMake/ExportConfiguration)

