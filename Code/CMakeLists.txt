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

