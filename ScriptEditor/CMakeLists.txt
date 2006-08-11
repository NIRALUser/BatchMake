PROJECT(BatchMakeScriptEditor)

INCLUDE_DIRECTORIES(
  ${BatchMakeScriptEditor_SOURCE_DIR}
  ${BatchMakeScriptEditor_BINARY_DIR}
  ${BatchMakeScriptEditor_SOURCE_DIR}/Command
  ${BatchMakeScriptEditor_BINARY_DIR}/Command
)


SET(BatchMakeScriptEditor_SRCS
  bmLaunch.cxx
  bmProgressManager.cxx
  bmScriptAction.cxx
  bmScriptActionManager.cxx
  bmScriptError.cxx
  bmScriptParser.cxx
  bmGrid.h
  bmGrid.cxx
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
Command/bmScriptDbSendValueAction.cxx
Command/bmScriptDbSendFileAction.cxx
Command/bmScriptDbClearAction.cxx
Command/bmScriptExtractSliceAction.cxx
Command/bmScriptExtractStringAction.cxx
Command/bmScriptIncludeAction.cxx
Command/bmScriptDashboardHostAction.cxx
Command/bmScriptDashboardUserAction.cxx
Command/bmScriptDashboardSendAction.cxx
Command/bmScriptDashboardNotifyAction.cxx
Command/bmScriptCreateExperimentAction.cxx
Command/bmScriptCreateMethodAction.cxx
Command/bmScriptSinAction.cxx
Command/bmScriptDeleteFileAction.cxx
Command/bmScriptOpenTCPSocketAction.cxx
Command/bmScriptSendTCPAction.cxx
Command/bmScriptCloseTCPSocketAction.cxx
Command/bmScriptAddMethodInputAction.cxx
Command/bmScriptAddMethodOutputAction.cxx
Command/bmScriptGridDataHostAction.cxx
Command/bmScriptGridOutputHostAction.cxx
Command/bmScriptOutputDirectoryAction.cxx
Command/bmScriptDataDirectoryAction.cxx
Command/bmScriptGridSingleNodeAction.cxx
Command/bmScriptAddMethodIdealOutputAction.cxx
Command/bmScriptSetIdealOutputAction.cxx
)

LINK_LIBRARIES(
 BatchMakeDashboard
)

ADD_LIBRARY(BatchMakeScriptEditor ${BatchMakeScriptEditor_SRCS})
