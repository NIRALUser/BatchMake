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
  bmCondor.h
  bmCondor.cxx
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
Command/bmScriptDashboardExperimentAction.cxx
Command/bmScriptDashboardSendAction.cxx
Command/bmScriptSinAction.cxx
)

LINK_LIBRARIES(
 BatchMakeDashboard
)

ADD_LIBRARY(BatchMakeScriptEditor ${BatchMakeScriptEditor_SRCS})
