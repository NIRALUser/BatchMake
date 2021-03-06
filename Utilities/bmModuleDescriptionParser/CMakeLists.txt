project(bmModuleDescriptionParser)

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

## ITK is required for expat.h

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if(bmModuleDescriptionParser_USE_PYTHON)
  find_package(PythonLibs)
endif(bmModuleDescriptionParser_USE_PYTHON)

if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build with shared libraries." ON)
endif(NOT DEFINED BUILD_SHARED_LIBS)

# --------------------------------------------------------------------------
# Include dirs

set(include_dirs
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

if(bmModuleDescriptionParser_USE_PYTHON)
  set(include_dirs ${include_dirs} ${PYTHON_INCLUDE_PATH})
endif(bmModuleDescriptionParser_USE_PYTHON)

include_directories(${include_dirs})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/bmModuleDescriptionParserConfigure.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/bmModuleDescriptionParserConfigure.h
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES
  ${headers}
  "${CMAKE_CURRENT_BINARY_DIR}/bmModuleDescriptionParserConfigure.h"
  DESTINATION include/${PROJECT_NAME} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Sources

set(bmModuleDescriptionParser_HDRS
  BatchMakeUtilities.h
  bmModuleDescription.h
  bmModuleDescriptionParser.h
  bmModuleFactory.h
  bmModuleLogo.h
  bmModuleParameter.h
  bmModuleParameterGroup.h
  bmModuleProcessInformation.h
  )

set(bmModuleDescriptionParser_SRCS
  bmModuleParameter.cxx
  bmModuleParameterGroup.cxx
  bmModuleDescription.cxx
  bmModuleDescriptionParser.cxx
  bmModuleProcessInformation.cxx
  bmModuleLogo.cxx
  bmModuleFactory.cxx
  BatchMakeUtilities.cxx
  )

if(USE_BFD)
  if(NOT WIN32)
    include(CheckIncludeFile)
    check_include_file(bfd.h HAVE_BFD_HEADER)

    if(HAVE_BFD_HEADER)
      # make sure we can build with libbfd
      message(STATUS "Testing libbfd")
      try_compile(HAVE_BFD
        ${CMAKE_CURRENT_BINARY_DIR}/CMake
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake
        TestBFD
        CMAKE_FLAGS
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        OUTPUT_VARIABLE OUTPUT)
      message(${OUTPUT})
      if(HAVE_BFD)
        message(STATUS "Testing libbfd - ok. ModuleFactory will look for global symbols in plugin executables.")
      else(HAVE_BFD)
        message(STATUS "Testing libbfd - error.  ModuleFactory will not look for global symbols in plugin executables.")
      endif(HAVE_BFD)
    endif(HAVE_BFD_HEADER)

    if(HAVE_BFD)
      set(bmModuleDescriptionParser_SRCS
        ${bmModuleDescriptionParser_SRCS}
        BinaryFileDescriptor.cxx)
    endif(HAVE_BFD)
  endif(NOT WIN32)
endif(USE_BFD)

# --------------------------------------------------------------------------
# Build and install the library

set(lib_name bmModuleDescriptionParser)
add_library(${lib_name}
  STATIC
  ${bmModuleDescriptionParser_SRCS}
  ${bmModuleDescriptionParser_HDRS}
)

set(link_libs
  ITKEXPAT
  itksys
  )
if(bmModuleDescriptionParser_USE_PYTHON)
  set(link_libs ${link_libs} ${PYTHON_LIBRARIES})
endif(bmModuleDescriptionParser_USE_PYTHON)
if(NOT WIN32 AND NOT APPLE AND NOT UNIX)
  set(link_libs ${link_libs} util pthread)
endif(NOT WIN32 AND NOT APPLE AND NOT UNIX)
if(UNIX)
  set(link_libs ${link_libs} pthread)
endif(UNIX)
if(NOT WIN32 AND HAVE_BFD)
  set(link_libs ${link_libs} bfd iberty)
endif(NOT WIN32 AND HAVE_BFD)

target_link_libraries(${lib_name} ${link_libs})

install(TARGETS ${lib_name}
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib/${PROJECT_NAME} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib/${PROJECT_NAME} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Testing

if(BUILD_TESTING)
  subdirs(Testing)
endif(BUILD_TESTING)

# --------------------------------------------------------------------------
# Install support files

include(GenerateModuleDescriptionParserConfig.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UseModuleDescriptionParser.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/UseModuleDescriptionParser.cmake COPYONLY IMMEDIATE)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/UseModuleDescriptionParser.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/install/bmModuleDescriptionParserConfig.cmake
  DESTINATION lib/${PROJECT_NAME} COMPONENT Development
  )
