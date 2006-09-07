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
bmLaunch.h
bmProgressManager.h
bmScriptAction.h
bmScriptActionManager.h
bmScriptError.h
bmScriptParser.h
bmEditor.h
bmScriptEditorGUIBase.h
bmScriptEditorGUIControls.h
bmProgressGUIBase.h
bmProgressGUIControls.h
bmProgressManagerGUI.h
bmScriptErrorGUI.h
bmSplashScreenControls.h
bmIcons.h
bmScriptEchoAction.h
bmScriptForEachAction.h
bmScriptGetParamAction.h
bmScriptIfAction.h
bmScriptRandomizeAction.h
bmScriptRunAction.h
bmScriptSequenceAction.h
bmScriptSetAction.h
bmScriptSetAppAction.h
bmScriptSetAppOptionAction.h
bmScriptListFileInDirAction.h
bmScriptListDirInDirAction.h
bmScriptIncAction.h
bmScriptIntAction.h
bmScriptWriteFileAction.h
bmScriptAppendFileAction.h
bmScriptExtractSliceAction.h
bmScriptExtractStringAction.h
bmScriptIncludeAction.h
bmScriptSinAction.h
bmScriptDeleteFileAction.h
bmScriptRegExAction.h
bmScriptMakeDirectoryAction.h
bmEditor.h
bmScriptEditorGUIControls.h
bmSplashScreenControls.h
bmScriptErrorGUI.h
bmProgressManagerGUI.h
bmProgressGUIControls.h
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
OPTION(HIDE_CONSOLE "Hide the console" OFF)
IF(HIDE_CONSOLE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
ENDIF(HIDE_CONSOLE)

ADD_EXECUTABLE(BatchMake ${BatchMake_SRCS})
TARGET_LINK_LIBRARIES(BatchMake ${BATCHMAKE_LIBS})

# Installation file
INSTALL(TARGETS BatchMake BatchMakeLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
INSTALL(FILES ${BatchMake_HDRS}  ${BatchMakeLib_HDRS}  DESTINATION include/BatchMake/Code)
INSTALL(FILES CMakeLists.txt DESTINATION source/Code)
INSTALL(FILES BatchMake.ico DESTINATION source/Code)
INSTALL(FILES ${BatchMake_SRCS} ${BatchMakeLib_SRCS} ${BatchMakeLib_GUI_SRCS} DESTINATION source/Code)
