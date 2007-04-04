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
SET(BatchMake_VERSION_PATCH "3")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(BatchMake_VERSION_STRING "${BatchMake_VERSION_MAJOR}.${BatchMake_VERSION_MINOR}")
SET(BatchMake_EXTENDED_VERSION_STRING "${BatchMake_VERSION_MAJOR}.${BatchMake_VERSION_MINOR}.${BatchMake_VERSION_PATCH}")

# Disable MSVC 8 warnings
IF(WIN32)
  OPTION(DISABLE_MSVC8_DEPRECATED_WARNINGS "Disable Visual Studio 8 deprecated warnings" ON)
  MARK_AS_ADVANCED(FORCE DISABLE_MSVC8_DEPRECATED_WARNINGS)  
  IF(DISABLE_MSVC8_DEPRECATED_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
  ENDIF(DISABLE_MSVC8_DEPRECATED_WARNINGS)
ENDIF(WIN32)

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
   INCLUDE( ${USE_ITK_FILE} ) 
ENDIF(ITK_FOUND)
LINK_LIBRARIES(
${ITK_LIBRARIES}
)

OPTION(USE_FLTK "Add GUI support to BatchMake" ON) 
IF(USE_FLTK)
  FIND_PACKAGE(FLTK)
  IF(FLTK_FOUND)
    INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
    IF(CYGWIN)
      LINK_LIBRARIES( ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
    ENDIF(CYGWIN)
  ENDIF(FLTK_FOUND)
  ADD_DEFINITIONS(-DBM_USE_FLTK)
ENDIF(USE_FLTK)

# Add testing
MARK_AS_ADVANCED( DART_ROOT )
INCLUDE (${CMAKE_ROOT}/Modules/Dart.cmake)
IF(NOT DART_ROOT) 
  SET(DROP_LOCATION /DartClient/BatchMake)
ENDIF(NOT DART_ROOT)

# Display the splashscreen or not
OPTION(SPLASHSCREEN "Display the splashscreen" ON)
IF(SPLASHSCREEN)
 ADD_DEFINITIONS(-DBMSPLASHSCREEN)
ENDIF(SPLASHSCREEN)


SET (LIBRARY_OUTPUT_PATH ${BatchMake_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${BatchMake_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
SET(BatchMake_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(BatchMake_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

# If this is the install version of BathMake we look for the include files
FIND_PATH(BatchMake_INCLUDE_DIR Code/ApplicationWrapper.h ${BatchMake_SOURCE_DIR}/../include/BatchMake)
MARK_AS_ADVANCED(FORCE BatchMake_INCLUDE_DIR)

IF(BatchMake_INCLUDE_DIR)
  SET(BatchMake_INCLUDE_PATH
    ${BatchMake_INCLUDE_DIR}/Code
    ${BatchMake_INCLUDE_DIR}/Grid
    ${BatchMake_INCLUDE_DIR}/Utilities
    ${BatchMake_INCLUDE_DIR}/Utilities/Flu
    ${BatchMake_INCLUDE_DIR}/Utilities/Zip
    ${BatchMake_INCLUDE_DIR}/Utilities/CondorWatcher
    ${BatchMake_INCLUDE_DIR}/Dashboard
    ${BatchMake_INCLUDE_DIR}/Code)
ENDIF(BatchMake_INCLUDE_DIR)


SET(BatchMake_INCLUDE_PATH
  ${BatchMake_INCLUDE_PATH}
  ${BatchMake_SOURCE_DIR}/Code
  ${BatchMake_SOURCE_DIR}/Utilities
  ${BatchMake_SOURCE_DIR}/Utilities/Flu
  ${BatchMake_SOURCE_DIR}/Utilities/CondorWatcher
  ${BatchMake_BINARY_DIR}/Utilities/CondorWatcher
  ${BatchMake_SOURCE_DIR}/Dashboard
  ${BatchMake_BINARY_DIR}
  ${BatchMake_BINARY_DIR}/Code
)

INCLUDE_DIRECTORIES(${BatchMake_INCLUDE_PATH})

# Validation dashboard Support
OPTION(DASHBOARD_SUPPORT "Validation Dashboard Support" OFF)
IF(DASHBOARD_SUPPORT)
  ADD_DEFINITIONS(-DBM_DASHBOARD)
  INCLUDE_DIRECTORIES(
  ${BatchMake_SOURCE_DIR}/Dashboard
  )
  SET(BatchMake_LIBRARIES
  ${BatchMake_LIBRARIES}
  BatchMakeDashboard
  )
  SUBDIRS(Dashboard)
ENDIF(DASHBOARD_SUPPORT)

# Grid Support
OPTION(GRID_SUPPORT "Grid Support" OFF)
IF(GRID_SUPPORT)
  ADD_DEFINITIONS(-DBM_GRID)
  INCLUDE_DIRECTORIES(
  ${BatchMake_SOURCE_DIR}/Grid
  )
  SET(BatchMake_LIBRARIES
  ${BatchMake_LIBRARIES}
  BatchMakeGrid
  )
  SUBDIRS(Grid)
ENDIF(GRID_SUPPORT)

# Build the examples
OPTION(BUILD_EXAMPLES "BatchMake Examples" ON)
IF(BUILD_EXAMPLES)
  SUBDIRS(Examples)
ENDIF(BUILD_EXAMPLES)

#Compile the main libraries
SUBDIRS(
Utilities
Code
Testing/Code
)

SET(BatchMake_LIBRARIES
${BatchMake_LIBRARIES}
 BatchMakeLib
 BatchMakeUtilities
)

# Export the configuration files
SUBDIRS(
Utilities/ExportConfiguration
)


#-----------------------------------------------------------------------------
# BatchMake uses KWStyle for checking the coding style
OPTION(BatchMake_USE_KWSTYLE "Run KWStyle in order to check for violations of the coding standard." OFF)
MARK_AS_ADVANCED(BatchMake_USE_KWSTYLE)

IF(BatchMake_USE_KWSTYLE)
  FIND_PROGRAM(KWSTYLE_EXECUTABLE
  NAMES KWStyle 
  PATHS
  /usr/local/bin
  )
  
  OPTION(KWSTYLE_USE_VIM_FORMAT "Set KWStyle to generate errors with a VIM-compatible format." OFF)
  OPTION(KWSTYLE_USE_MSVC_FORMAT "Set KWStyle to generate errors with a VisualStudio-compatible format." OFF)

  IF(KWSTYLE_USE_VIM_FORMAT)
    SET(KWSTYLE_ARGUMENTS -vim ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_VIM_FORMAT)

  IF(KWSTYLE_USE_MSVC_FORMAT)
    SET(KWSTYLE_ARGUMENTS -msvc ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_MSVC_FORMAT)

  MARK_AS_ADVANCED(KWSTYLE_EXECUTABLE)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${BatchMake_BINARY_DIR}/KWStyleReport.txt
    COMMAND ${KWSTYLE_EXECUTABLE}
    ARGS    -xml ${BatchMake_BINARY_DIR}/BatchMake.kws.xml -o ${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMakeOverwrite.txt ${KWSTYLE_ARGUMENTS} -D ${BatchMake_BINARY_DIR}/BatchMakeFiles.txt  
    COMMENT "Coding Style Checker"
    )
  ADD_CUSTOM_TARGET(StyleCheck DEPENDS ${BatchMake_BINARY_DIR}/KWStyleReport.txt)

CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMake.kws.xml.in
               ${BatchMake_BINARY_DIR}/BatchMake.kws.xml)

CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMakeFiles.txt.in
               ${BatchMake_BINARY_DIR}/BatchMakeFiles.txt)

ENDIF(BatchMake_USE_KWSTYLE)


#Copy example directories
MAKE_DIRECTORY(${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/WrappedApplications/RViewAffineRegistration2.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration2.bmm COPYONLY IMMEDIATE)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/WrappedApplications/RViewAffineRegistration.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/RViewAffineRegistration.bmm COPYONLY IMMEDIATE)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Examples/WrappedApplications/PixelCounter.bmm ${BatchMake_BINARY_DIR}/bin/RelWithDebInfo/Applications/PixelCounter.bmm COPYONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Documentation/Doxygen/doxygen.config.in
               ${BatchMake_BINARY_DIR}/doxygen.config)
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Documentation/Doxygen/itkdoxygen.pl.in
               ${BatchMake_BINARY_DIR}/itkdoxygen.pl)

CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/bmConfigure.h.in 
               ${BatchMake_BINARY_DIR}/bmConfigure.h)

INSTALL(FILES ${BatchMake_BINARY_DIR}/bmConfigure.h DESTINATION source)
INSTALL(FILES CMakeLists.txt DESTINATION source)
INSTALL(FILES Copyright.txt DESTINATION source)
INSTALL(FILES ReadMe.txt DESTINATION source)
INSTALL(FILES DartConfig.cmake DESTINATION source)
INSTALL(FILES FindFLTK.cmake DESTINATION source)

FILE(GLOB IMAGES Images/*.*)
INSTALL(FILES ${IMAGES} DESTINATION source/Images)

FILE(GLOB Doxygen Documentation/Doxygen/*.*)
INSTALL(FILES ${Doxygen} DESTINATION source/Documentation/Doxygen)

# CPack options
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BatchMake")
SET(CPACK_PACKAGE_VENDOR "Kitware Inc.")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR ${BatchMake_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${BatchMake_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${BatchMake_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "BatchMake ${BatchMake_VERSION_MAJOR}.${BatchMake_VERSION_MINOR}.${BatchMake_VERSION_PATCH}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${BatchMake_SOURCE_DIR}/Documentation/images\\\\InstallImage.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "relwithdebinfo\\\\BatchMake.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The Batch Processing Scripting Tool")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\public.kitware.com/BatchMake")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\public.kitware.com/BatchMake")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/BatchMake")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "BatchMake" "BatchMake")
INCLUDE(CPack)




