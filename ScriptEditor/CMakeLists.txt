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
Command/bmScriptListFileInDirAction.cxx
Command/bmScriptListDirInDirAction.cxx
)

ADD_LIBRARY(BatchMakeScriptEditor ${BatchMakeScriptEditor_SRCS})
