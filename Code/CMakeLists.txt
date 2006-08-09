SET(BatchMake_SRCS
  BatchMake.cxx
)

IF(WIN32)
IF(NOT CYGWIN)
IF(NOT MINGW)
SET(BatchMake_SRCS
  ${BatchMake_SRCS}
  BatchMake.rc)
ENDIF(NOT MINGW)
ENDIF(NOT CYGWIN)
ENDIF(WIN32)

LINK_LIBRARIES(
 ${FLTK_LIBRARIES}
)

SET(BATCHMAKE_LIBS
 BatchMakeGUI
 BatchMakeApplicationWrapper
 BatchMakeScriptEditor
 BatchMakeUtilities
 CondorWatcher
 FLU
)
# Add an option to hide the console
OPTION("HIDE_CONSOLE",OFF)
IF(HIDE_CONSOLE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
ENDIF(HIDE_CONSOLE)

ADD_EXECUTABLE(BatchMake ${BatchMake_SRCS})
TARGET_LINK_LIBRARIES(BatchMake ${BATCHMAKE_LIBS})

