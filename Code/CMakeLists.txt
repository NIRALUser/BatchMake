SET(BatchMakeLib_SRCS
  ${BatchMakeLib_SRCS}
  bmLaunch.cxx
  bmProgressManager.cxx
  bmScriptAction.cxx
  bmScriptActionManager.cxx
  bmScriptError.cxx
  bmScriptParser.cxx
)

SET(BatchMakeLib_HDRS
bmEditor.h
bmScriptEditorGUIBase.h
bmScriptEditorGUIControls.h
bmProgressGUIBase.h
bmProgressGUIControls.h
bmProgressManagerGUI.h
bmScriptErrorGUI.h
bmSplashScreenControls.h
bmIcons.h
)

#Add Commands
SET(BatchMakeLib_SRCS
${BatchMakeLib_SRCS}
bmScriptEchoAction.cxx
bmScriptForEachAction.cxx
bmScriptGetParamAction.cxx
bmScriptIfAction.cxx
bmScriptRandomizeAction.cxx
bmScriptRunAction.cxx
bmScriptSequenceAction.cxx
bmScriptSetAction.cxx
bmScriptSetAppAction.cxx
bmScriptSetAppOptionAction.cxx
bmScriptListFileInDirAction.cxx
bmScriptListDirInDirAction.cxx
bmScriptIncAction.cxx
bmScriptIntAction.cxx
bmScriptWriteFileAction.cxx
bmScriptAppendFileAction.cxx
bmScriptExtractSliceAction.cxx
bmScriptExtractStringAction.cxx
bmScriptIncludeAction.cxx
bmScriptSinAction.cxx
bmScriptDeleteFileAction.cxx
bmScriptRegExAction.cxx
bmScriptMakeDirectoryAction.cxx
bmEditor.cxx
bmScriptEditorGUIControls.cxx
bmSplashScreenControls.cxx
bmScriptErrorGUI.cxx
bmProgressManagerGUI.cxx
bmProgressGUIControls.cxx
)

SET(BatchMakeLib_GUI_SRCS
bmProgressGUI.fl
bmScriptEditorGUI.fl
bmSplashScreen.fl
)

SET(FLTK_WRAP_UI 1)
FLTK_WRAP_UI( BatchMakeLib ${BatchMakeLib_GUI_SRCS} )

ADD_LIBRARY(BatchMakeLib ${BatchMakeLib_SRCS} ${BatchMakeLib_FLTK_UI_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeLib BatchMakeApplicationWrapper)
ADD_DEPENDENCIES(BatchMakeLib BatchMakeApplicationWrapper)

IF(GRID_SUPPORT)
  TARGET_LINK_LIBRARIES(BatchMakeLib CondorWatcher BatchMakeGrid)
  ADD_DEPENDENCIES(BatchMakeLib CondorWatcher)
ENDIF(GRID_SUPPORT)

IF(DASHBOARD_SUPPORT)
  TARGET_LINK_LIBRARIES(BatchMakeLib BatchMakeDashboard)
ENDIF(DASHBOARD_SUPPORT)

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
 BatchMakeLib
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

# Installation file
INSTALL(TARGETS BatchMake
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
INSTALL(FILES ${BatchMake_HDRS} DESTINATION include/BatchMake)
INSTALL(FILES CMakeLists.txt DESTINATION source/Code)
INSTALL(FILES ${BatchMake_SRCS} DESTINATION source/Code)
