PROJECT(BatchMake)


# If CMake is the version we're expecting, don't show the
# CMAKE_BACKWARDS_COMPATIBILITY option.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.6$")
  MARK_AS_ADVANCED(FORCE CMAKE_BACKWARDS_COMPATIBILITY)
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.6$")
  MARK_AS_ADVANCED(CLEAR CMAKE_BACKWARDS_COMPATIBILITY)
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.6$")


#-----------------------------------------------------------------------------
# BatchMake version number.  An even minor number corresponds to releases.
SET(BatchMake_VERSION_MAJOR "1")
SET(BatchMake_VERSION_MINOR "0")
SET(BatchMake_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(BatchMake_VERSION_STRING "${BatchMake_VERSION_MAJOR}.${BatchMake_VERSION_MINOR}")


FIND_PACKAGE(FLTK)
IF(FLTK_FOUND)
   INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
ENDIF(FLTK_FOUND)

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
  ${BatchMake_SOURCE_DIR}/Utilities
  ${BatchMake_SOURCE_DIR}/ApplicationWrapper
  ${BatchMake_SOURCE_DIR}/GUI
  ${BatchMake_SOURCE_DIR}/GUI/ScriptEditor
  ${BatchMake_SOURCE_DIR}/GUI/External/Flu
  ${BatchMake_SOURCE_DIR}/GUI/ProgressGUI
  ${BatchMake_SOURCE_DIR}/ScriptEditor
  ${BatchMake_SOURCE_DIR}/ScriptEditor/Command
  ${BatchMake_SOURCE_DIR}/GUI/SplashScreen

  ${BatchMake_BINARY_DIR}
  ${BatchMake_BINARY_DIR}/GUI
)

#Compile Utilities
SUBDIRS(Utilities)

#Compile Application Wrapper
SUBDIRS(ApplicationWrapper)

#Compile Script Editor
SUBDIRS(ScriptEditor)

#Compile GUI
SUBDIRS(GUI)

#Compile the BatchMake Application 
SUBDIRS(Code)

#Compile External 
applications
SUBDIRS(External)

#Copy example directories
MAKE_DIRECTORY(${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/Applications/RViewAffineRegistration2.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration2.bmm COPYONLY IMMEDIATE)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/Applications/RViewAffineRegistration.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration.bmm COPYONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Documentation/Doxygen/doxygen.config.in
               ${BatchMake_BINARY_DIR}/doxygen.config)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Documentation/Doxygen/itkdoxygen.pl.in
               ${BatchMake_BINARY_DIR}/itkdoxygen.pl)



