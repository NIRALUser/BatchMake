PROJECT(BatchMake)

FIND_PACKAGE(FLTK)
IF(FLTK_FOUND)
   INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
ENDIF(FLTK_FOUND)

#Compile Utilities
SUBDIRS(BatchMakeUtilities)

#Compile Application Wrapper
SUBDIRS(BatchMakeApplicationWrapper)

#Compile Script Editor
SUBDIRS(BatchMakeScriptEditor)

#Compile GUI
SUBDIRS(BatchMakeGUI)

# Add testing
MARK_AS_ADVANCED( DART_ROOT )
INCLUDE (${CMAKE_ROOT}/Modules/Dart.cmake)
IF(NOT DART_ROOT) 
  SET(DROP_LOCATION /DartClient)
ENDIF(NOT DART_ROOT)


SET (LIBRARY_OUTPUT_PATH ${BatchMake_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${BatchMake_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
SET(BatchMake_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(BatchMake_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})


INCLUDE_DIRECTORIES(
  ${BatchMake_SOURCE_DIR}
  ${BatchMake_SOURCE_DIR}/BatchMakeUtilities
  ${BatchMake_SOURCE_DIR}/BatchMakeApplicationWrapper
  ${BatchMake_SOURCE_DIR}/BatchMakeGUI
  ${BatchMake_SOURCE_DIR}/BatchMakeGUI/ScriptEditor
  ${BatchMake_SOURCE_DIR}/BatchMakeGUI/External/Flu
  ${BatchMake_SOURCE_DIR}/BatchMakeGUI/ProgressGUI
  ${BatchMake_SOURCE_DIR}/BatchMakeScriptEditor
  ${BatchMake_SOURCE_DIR}/BatchMakeScriptEditor/BatchMakeCommand
  ${BatchMake_SOURCE_DIR}/BatchMakeGUI/SplashScreen

  ${BatchMake_BINARY_DIR}
  ${BatchMake_BINARY_DIR}/BatchMakeGUI

)

SET(BatchMake_SRCS
  BatchMake.cxx
  CommandLineArgumentParser.cxx
)

IF(WIN32)
SET(BatchMake_SRCS
  ${BatchMake_SRCS}
  BatchMake.rc)
ENDIF(WIN32)

LINK_LIBRARIES(
 BatchMakeApplicationWrapper
 BatchMakeScriptEditor
 BatchMakeGUI
 BatchMakeUtilities
 ${FLTK_LIBRARIES}
 FLU
)

# Add an option to hide the console
OPTION("HIDE_CONSOLE",OFF)
IF(HIDE_CONSOLE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
ENDIF(HIDE_CONSOLE)

ADD_EXECUTABLE(BatchMake ${BatchMake_SRCS})

#Copy example directories
MAKE_DIRECTORY(${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/Applications/RViewAffineRegistration2.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration2.bmm COPYONLY IMMEDIATE)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/Applications/RViewAffineRegistration.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration.bmm COPYONLY IMMEDIATE)
