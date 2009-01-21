cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

PROJECT(bmModuleDescriptionParser)

CONFIGURE_FILE(
  ${bmModuleDescriptionParser_SOURCE_DIR}/bmModuleDescriptionParserConfigure.h.in 
  ${bmModuleDescriptionParser_BINARY_DIR}/bmModuleDescriptionParserConfigure.h
)

## ITK is required for expat.h
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

SET(bmModuleDescriptionParser_SRCS
  bmModuleParameter.cxx
  bmModuleParameterGroup.cxx
  bmModuleDescription.cxx
  bmModuleDescriptionParser.cxx
  bmModuleProcessInformation.cxx
  bmModuleLogo.cxx
  bmModuleFactory.cxx
  BatchMakeUtilities.cxx
)

IF (USE_BFD)
  IF (NOT WIN32)
    INCLUDE(CheckIncludeFile)
    CHECK_INCLUDE_FILE(bfd.h HAVE_BFD_HEADER)

    IF (HAVE_BFD_HEADER)
       # make sure we can build with libbfd
       MESSAGE(STATUS "Testing libbfd")
       TRY_COMPILE(HAVE_BFD
                   ${bmModuleDescriptionParser_BINARY_DIR}/CMake
                   ${bmModuleDescriptionParser_SOURCE_DIR}/CMake
                   TestBFD
                   CMAKE_FLAGS 
                      -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
                      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   OUTPUT_VARIABLE OUTPUT)
       MESSAGE(${OUTPUT})
       IF (HAVE_BFD)
         MESSAGE(STATUS "Testing libbfd - ok. ModuleFactory will look for global symbols in plugin executables.")
       ELSE (HAVE_BFD)
         MESSAGE(STATUS "Testing libbfd - error.  ModuleFactory will not look for global symbols in plugin executables.")
       ENDIF (HAVE_BFD)
    ENDIF (HAVE_BFD_HEADER)

    IF (HAVE_BFD)
      SET(bmModuleDescriptionParser_SRCS 
            ${bmModuleDescriptionParser_SRCS} 
            bmBinaryFileDescriptor.cxx)
    ENDIF (HAVE_BFD)
  ENDIF (NOT WIN32)
ENDIF (USE_BFD)


INCLUDE_DIRECTORIES(
  ${bmModuleDescriptionParser_BINARY_DIR}
  ${bmModuleDescriptionParser_SOURCE_DIR} 
)

IF(USE_PYTHON)
  FIND_PACKAGE(PythonLibs)
ENDIF(USE_PYTHON)

IF(USE_PYTHON)
# Python requires a pointer to the Slicer Application
INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_PATH}
)
ENDIF(USE_PYTHON)

ADD_LIBRARY(bmModuleDescriptionParser ${bmModuleDescriptionParser_SRCS})
IF(USE_PYTHON)
ADD_DEFINITIONS(-DUSE_PYTHON)
ENDIF(USE_PYTHON)

IF ( NOT WIN32 )
  IF(NOT APPLE)
    TARGET_LINK_LIBRARIES( bmModuleDescriptionParser util)
  ENDIF(NOT APPLE)
ENDIF ( NOT WIN32 )

TARGET_LINK_LIBRARIES(bmModuleDescriptionParser
  ITKEXPAT
  itksys
)
IF(USE_PYTHON)
   TARGET_LINK_LIBRARIES(bmModuleDescriptionParser
     ${PYTHON_LIBRARIES}
  )
ENDIF(USE_PYTHON)

IF (NOT WIN32)
  IF (HAVE_BFD)
    TARGET_LINK_LIBRARIES(bmModuleDescriptionParser bfd iberty)
  ENDIF(HAVE_BFD)
ENDIF (NOT WIN32)

IF(BUILD_TESTING)
  SUBDIRS(Testing)
ENDIF(BUILD_TESTING)


INCLUDE (GenerateModuleDescriptionParserConfig.cmake) 

CONFIGURE_FILE(
  ${bmModuleDescriptionParser_SOURCE_DIR}/UseModuleDescriptionParser.cmake.in
  ${bmModuleDescriptionParser_BINARY_DIR}/UseModuleDescriptionParser.cmake COPYONLY IMMEDIATE)

INSTALL(TARGETS bmModuleDescriptionParser 
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib/bmModuleDescriptionParser COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib/bmModuleDescriptionParser COMPONENT Development)

FILE(GLOB __files1 "${bmModuleDescriptionParser_SOURCE_DIR}/*.h")
INSTALL(FILES ${__files1} DESTINATION include/bmModuleDescriptionParser COMPONENT Development)
INSTALL(FILES ${bmModuleDescriptionParser_BINARY_DIR}/bmModuleDescriptionParserConfigure.h DESTINATION include/bmModuleDescriptionParser COMPONENT Development)

INSTALL(FILES 
  ${bmModuleDescriptionParser_BINARY_DIR}/UsebmModuleDescriptionParser.cmake 
  ${bmModuleDescriptionParser_BINARY_DIR}/install/bmModuleDescriptionParserConfig.cmake 
  DESTINATION lib/bmModuleDescriptionParser COMPONENT Development)
