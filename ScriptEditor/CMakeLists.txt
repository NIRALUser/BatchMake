PROJECT(BatchMakeScriptEditor)

INCLUDE_DIRECTORIES(
  ${BatchMakeScriptEditor_SOURCE_DIR}
  ${BatchMakeScriptEditor_BINARY_DIR}
  ${BatchMakeScriptEditor_SOURCE_DIR}/Command
  ${BatchMakeScriptEditor_BINARY_DIR}/Command
)

IF(DASHBOARD_SUPPORT)
  INCLUDE_DIRECTORIES(
  ${BatchMakeScriptEditor_SOURCE_DIR}/Dashboard
  ${BatchMakeScriptEditor_BINARY_DIR}/Dashboard
  )
  SET(BatchMakeScriptEditor_SRCS
    ${BatchMakeScriptEditor_SRCS}
    Dashboard/bmScriptDbSendValueAction.cxx
    Dashboard/bmScriptDbSendFileAction.cxx
    Dashboard/bmScriptDbClearAction.cxx
    Dashboard/bmScriptOpenTCPSocketAction.cxx
    Dashboard/bmScriptSendTCPAction.cxx
    Dashboard/bmScriptCloseTCPSocketAction.cxx
    Dashboard/bmScriptAddMethodInputAction.cxx
    Dashboard/bmScriptAddMethodIdealOutputAction.cxx
    Dashboard/bmScriptSetIdealOutputAction.cxx
    Dashboard/bmScriptDashboardHostAction.cxx
    Dashboard/bmScriptDashboardUserAction.cxx
    Dashboard/bmScriptDashboardSendAction.cxx
    Dashboard/bmScriptDashboardNotifyAction.cxx
    Dashboard/bmScriptCreateExperimentAction.cxx
    Dashboard/bmScriptCreateMethodAction.cxx   
    Dashboard/bmScriptAddMethodOutputAction.cxx  
    Dashboard/bmScriptActionManagerDashboard.cxx  
    )

  IF(GRID_SUPPORT)
    SET(BatchMakeScriptEditor_SRCS
    ${BatchMakeScriptEditor_SRCS}
    Grid/bmScriptSetIdealOutputActionGrid.cxx
    Grid/bmScriptAddMethodIdealOutputActionGrid.cxx
    Grid/bmScriptCreateExperimentActionGrid.cxx
    Grid/bmScriptAddMethodOutputActionGrid.cxx
    Grid/bmScriptAddMethodInputActionGrid.cxx
    Grid/bmScriptDashboardSendActionGrid.cxx
    Grid/bmScriptCreateMethodActionGrid.cxx
    )
  ENDIF(GRID_SUPPORT)

ENDIF(DASHBOARD_SUPPORT)


IF(GRID_SUPPORT)
  INCLUDE_DIRECTORIES(
  ${BatchMakeScriptEditor_SOURCE_DIR}/Grid
  ${BatchMakeScriptEditor_BINARY_DIR}/Grid
  )
  SET(BatchMakeScriptEditor_SRCS
    ${BatchMakeScriptEditor_SRCS}
    Grid/bmGrid.h
    Grid/bmGrid.cxx
    Grid/bmScriptGridSingleNodeAction.cxx
    Grid/bmScriptGridDataHostAction.cxx
    Grid/bmScriptGridOutputHostAction.cxx
    Grid/bmScriptOutputDirectoryAction.cxx
    Grid/bmScriptDataDirectoryAction.cxx
    Grid/bmScriptExtractSliceActionGrid.cxx
    Grid/bmScriptRunActionGrid.cxx
    Grid/bmScriptSetActionGrid.cxx
   )
ENDIF(GRID_SUPPORT)


SET(BatchMakeScriptEditor_SRCS
  ${BatchMakeScriptEditor_SRCS}
  bmLaunch.cxx
  bmProgressManager.cxx
  bmScriptAction.cxx
  bmScriptActionManager.cxx
  bmScriptError.cxx
  bmScriptParser.cxx
)

#Add Commands
SET(BatchMakeScriptEditor_SRCS
${BatchMakeScriptEditor_SRCS}
Command/bmScriptEchoAction.cxx
Command/bmScriptForEachAction.cxx
Command/bmScriptGetParamAction.cxx
Command/bmScriptIfAction.cxx
Command/bmScriptRandomizeAction.cxx
Command/bmScriptRunAction.cxx
Command/bmScriptSequenceAction.cxx
Command/bmScriptSetAction.cxx
Command/bmScriptSetAppAction.cxx
Command/bmScriptSetAppOptionAction.cxx
Command/bmScriptListFileInDirAction.cxx
Command/bmScriptListDirInDirAction.cxx
Command/bmScriptIncAction.cxx
Command/bmScriptIntAction.cxx
Command/bmScriptWriteFileAction.cxx
Command/bmScriptAppendFileAction.cxx
Command/bmScriptExtractSliceAction.cxx
Command/bmScriptExtractStringAction.cxx
Command/bmScriptIncludeAction.cxx
Command/bmScriptSinAction.cxx
Command/bmScriptDeleteFileAction.cxx
Command/bmScriptRegExAction.cxx
Command/bmScriptMakeDirectoryAction.cxx
)

IF(SUPPORT_DASHBOARD)
  LINK_LIBRARIES(
    BatchMakeDashboard
  )
ENDIF(SUPPORT_DASHBOARD)

ADD_LIBRARY(BatchMakeScriptEditor ${BatchMakeScriptEditor_SRCS})
