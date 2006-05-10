PROJECT(BatchMakeGUI)

FIND_PACKAGE(FLTK)
IF(FLTK_FOUND)
   INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
ENDIF(FLTK_FOUND)

SUBDIRS(External/Flu)
SUBDIRS(CondorWatcher)

INCLUDE_DIRECTORIES(
  ${BatchMakeGUI_SOURCE_DIR}
  ${BatchMakeGUI_SOURCE_DIR}/ApplicationWrapper
  ${BatchMakeGUI_SOURCE_DIR}/External/Flu
  ${BatchMakeGUI_SOURCE_DIR}/ProgressGUI
  ${BatchMakeGUI_SOURCE_DIR}/ScriptEditor
  ${BatchMakeGUI_SOURCE_DIR}/CondorWatcher

  ${BatchMakeGUI_BINARY_DIR}/SplashScreen
  ${BatchMakeGUI_BINARY_DIR}/ApplicationWrapper
  ${BatchMakeGUI_BINARY_DIR}/ProgressGUI
  ${BatchMakeGUI_BINARY_DIR}/ScriptEditor
  ${BatchMakeGUI_BINARY_DIR}/CondorWatcher
)


SET(BatchMakeGUI_SRCS
  SplashScreen/bmSplashScreenControls.cxx
  ApplicationWrapper/bmApplicationListGUIControls.cxx
  ApplicationWrapper/bmApplicationWrapperGUIControls.cxx
  ProgressGUI/bmProgressGUIControls.cxx
  ProgressGUI/bmProgressManagerGUI.cxx
  ProgressGUI/bmScriptErrorGUI.cxx
  ScriptEditor/bmEditor.cxx
  ScriptEditor/bmEditor.h
  ScriptEditor/bmScriptEditorGUIControls.cxx
)


SET(BatchMakeGUI_GUI_SRCS
  SplashScreen/bmSplashScreen.fl
  ApplicationWrapper/bmApplicationListGUI.fl
  ApplicationWrapper/bmApplicationWrapperGUI.fl
  ProgressGUI/bmProgressGUI.fl
  ScriptEditor/bmScriptEditorGUI.fl
)

LINK_LIBRARIES(
 ${FLTK_LIBRARIES}
 FLU
)

SET(FLTK_WRAP_UI 1)
FLTK_WRAP_UI( BatchMakeGUI ${BatchMakeGUI_GUI_SRCS} )

ADD_LIBRARY(BatchMakeGUI ${BatchMakeGUI_SRCS} ${BatchMakeGUI_FLTK_UI_SRCS})
ADD_DEPENDENCIES(BatchMakeGUI FLU CondorWatcher)
TARGET_LINK_LIBRARIES(BatchMakeGUI CondorWatcher)

