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
 ${FLTK_LIBRARIES}
)

SET(BATCHMAKE_LIBS
 BatchMakeApplicationWrapper
 BatchMakeScriptEditor
 BatchMakeGUI
 BatchMakeUtilities
 FLU
)
# Add an option to hide the console
OPTION("HIDE_CONSOLE",OFF)
IF(HIDE_CONSOLE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
ENDIF(HIDE_CONSOLE)

ADD_EXECUTABLE(BatchMake ${BatchMake_SRCS})
TARGET_LINK_LIBRARIES(BatchMake ${BATCHMAKE_LIBS})

