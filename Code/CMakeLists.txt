set(BatchMakeLib_SRCS
  ${BatchMakeLib_SRCS}
  bmLaunch.cxx
  bmProgressManager.cxx
  bmScriptAction.cxx
  bmScriptActionManager.cxx
  bmScriptError.cxx
  bmScriptParser.cxx
  )

set(BatchMakeLib_HDRS
  bmLaunch.h
  bmProgressManager.h
  bmScriptAction.h
  bmScriptActionManager.h
  bmScriptError.h
  bmScriptParser.h
  bmIcons.h
  bmScriptEchoAction.h
  bmScriptForEachAction.h
  bmScriptForNFoldAction.h
  bmScriptGetParamAction.h
  bmScriptGetParamCountAction.h
  bmScriptIfAction.h
  bmScriptRandomizeAction.h
  bmScriptRunAction.h
  bmScriptSequenceAction.h
  bmScriptSetAction.h
  bmScriptSetAppAction.h
  bmScriptSetAppOptionAction.h
  bmScriptSetAppFlagAction.h
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
  bmScriptCopyFileAction.h
  bmScriptDeleteFileAction.h
  bmScriptRegExAction.h
  bmScriptMakeDirectoryAction.h
  ApplicationWrapper.h
  ApplicationWrapperParam.h
  ApplicationWrapperParamSub.h
  bmScriptGetTimeAction.h
  bmScriptGetCurrentDateTimeAction.h
  bmScriptGlobAction.h
  bmScriptGetFilenameAction.h
  bmScriptConvertImageAction.h
  bmScriptFileExistsAction.h
  bmScriptGetListSizeAction.h
  bmScriptExitAction.h
  bmScriptExitOnErrorAction.h
  bmScriptAddErrorAction.h
  bmScriptGetErrorCountAction.h
  bmScriptClearErrorsAction.h
  bmScriptMathAction.h
  )

include_directories(
    ${BatchMake_SOURCE_DIR}/Utilities/bmModuleDescriptionParser
    ${BatchMake_BINARY_DIR}/Utilities/bmModuleDescriptionParser
  )


if(BATCHMAKE_USE_XCEDE)
  set(BatchMakeLib_HDRS ${BatchMakeLib_HDRS}
  bmScriptGetXnatDataSetsAction.h
  bmScriptGetXnatFilenameAction.h
  bmScriptGetXcedeDataSetsAction.h
  bmScriptDownloadXcedeDataSetAction.h
  bmScriptGetXcedeFilenameAction.h
  )

  include_directories(
    ${BatchMake_SOURCE_DIR}/Utilities/bmcurl/curl
    ${ITK_DIR}/Utilities/itklibxml2/include
    ${ITK_DIR}/Utilities/itklibxml2/libxml
  )
endif(BATCHMAKE_USE_XCEDE)

#Add Commands
set(BatchMakeLib_SRCS
  ${BatchMakeLib_SRCS}
  bmScriptEchoAction.cxx
  bmScriptForEachAction.cxx
  bmScriptForNFoldAction.cxx
  bmScriptGetParamAction.cxx
  bmScriptGetParamCountAction.cxx
  bmScriptIfAction.cxx
  bmScriptRandomizeAction.cxx
  bmScriptRunAction.cxx
  bmScriptSequenceAction.cxx
  bmScriptSetAction.cxx
  bmScriptSetAppAction.cxx
  bmScriptSetAppOptionAction.cxx
  bmScriptSetAppFlagAction.cxx
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
  bmScriptCopyFileAction.cxx
  bmScriptDeleteFileAction.cxx
  bmScriptRegExAction.cxx
  bmScriptMakeDirectoryAction.cxx
  ApplicationWrapper.cxx
  ApplicationWrapperParam.cxx
  ApplicationWrapperParamSub.cxx
  bmScriptGetTimeAction.cxx
  bmScriptGetCurrentDateTimeAction.cxx
  bmScriptGlobAction.cxx
  bmScriptGetFilenameAction.cxx
  bmScriptConvertImageAction.cxx
  bmScriptFileExistsAction.cxx
  bmScriptGetListSizeAction.cxx
  bmScriptExitAction.cxx
  bmScriptExitOnErrorAction.cxx
  bmScriptAddErrorAction.cxx
  bmScriptGetErrorCountAction.cxx
  bmScriptClearErrorsAction.cxx
  bmScriptMathAction.cxx
  )


if(BATCHMAKE_USE_XCEDE)
  set(BatchMakeLib_SRCS ${BatchMakeLib_SRCS}
  bmScriptDownloadXnatDataSetAction.cxx
  bmScriptGetXnatDataSetsAction.cxx
  bmScriptGetXnatFilenameAction.cxx
  bmScriptGetXnatURIAction.cxx
  bmScriptGetXcedeDataSetsAction.cxx
  bmScriptDownloadXcedeDataSetAction.cxx
  bmScriptGetXcedeFilenameAction.cxx
  )
endif(BATCHMAKE_USE_XCEDE)

#
# Grid support
#
if(GRID_SUPPORT)
  set(BatchMakeLib_SRCS
    ${BatchMakeLib_SRCS}
    Grid/bmGrid.cxx
    Grid/bmScriptGridSingleNodeAction.cxx
    Grid/bmScriptGridDataHostAction.cxx
    Grid/bmScriptGridOutputHostAction.cxx
    Grid/bmScriptInputDirectoryAction.cxx
    Grid/bmScriptOutputDirectoryAction.cxx
    Grid/bmScriptWorkingDirectoryAction.cxx
    Grid/bmScriptExecutableDirectoryAction.cxx
    Grid/bmScriptExtractSliceActionGrid.cxx
    Grid/bmScriptRunActionGrid.cxx
    Grid/bmScriptSetActionGrid.cxx
    Grid/bmScriptGridSetGroupingAction.cxx
    Grid/bmScriptGridMaxNodesAction.cxx
    Grid/bmScriptGridTempDirectoryAction.cxx
    Grid/bmScriptGridTransferFileAction.cxx
    Grid/bmScriptGridRequirementsAction.cxx
    Grid/bmScriptGridOwnerAction.cxx
    Grid/bmScriptCondorPostScriptAction.cxx
    )

  if(DASHBOARD_SUPPORT)
    set(BatchMakeLib_SRCS
      ${BatchMakeLib_SRCS}
      Grid/bmScriptSetIdealOutputActionGrid.cxx
      Grid/bmScriptAddMethodIdealOutputActionGrid.cxx
      Grid/bmScriptCreateExperimentActionGrid.cxx
      Grid/bmScriptAddMethodOutputActionGrid.cxx
      Grid/bmScriptAddMethodInputActionGrid.cxx
      Grid/bmScriptDashboardSendActionGrid.cxx
      Grid/bmScriptCreateMethodActionGrid.cxx
      )
  endif(DASHBOARD_SUPPORT)

endif(GRID_SUPPORT)

#
# Dashboard support
#
if(DASHBOARD_SUPPORT)
  set(BatchMakeLib_SRCS
    ${BatchMakeLib_SRCS}
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
    Dashboard/bmScriptDashboardKeyAction.cxx
    Dashboard/bmScriptDashboardSendAction.cxx
    Dashboard/bmScriptDashboardNotifyAction.cxx
    Dashboard/bmScriptCreateExperimentAction.cxx
    Dashboard/bmScriptCreateMethodAction.cxx
    Dashboard/bmScriptCreateDashboardAction.cxx
    Dashboard/bmScriptCreateGraphAction.cxx
    Dashboard/bmScriptAddMethodOutputAction.cxx
    Dashboard/bmScriptActionManagerDashboard.cxx
    Dashboard/bmScriptAddDashboardLabelAction.cxx
    Dashboard/bmDashboardSend.cxx
    )
endif(DASHBOARD_SUPPORT)

if(USE_FLTK)

  set(BatchMakeLib_HDRS
    ${BatchMakeLib_HDRS}
    bmEditor.h
    bmScriptEditorGUIBase.h
    bmProgressGUIBase.h
    bmProgressGUIControls.h
    bmProgressManagerGUI.h
    bmScriptErrorGUI.h
    bmScriptEditorGUIControls.h
    bmSplashScreenControls.h
    bmApplicationListGUIBase.h
    bmApplicationListGUIControls.h
    bmApplicationWrapperGUIBase.h
    bmApplicationWrapperGUIControls.h
    )

  set(BatchMakeLib_SRCS
    ${BatchMakeLib_SRCS}
    bmScriptEditorGUIControls.cxx
    bmSplashScreenControls.cxx
    bmScriptErrorGUI.cxx
    bmProgressManagerGUI.cxx
    bmProgressGUIControls.cxx
    bmApplicationListGUIControls.cxx
    bmApplicationWrapperGUIControls.cxx
    bmEditor.cxx
    )

  set(BatchMakeLib_GUI_SRCS
    bmProgressGUI.fl
    bmScriptEditorGUI.fl
    bmSplashScreen.fl
    bmApplicationListGUI.fl
    bmApplicationWrapperGUI.fl
    )

  set(FLTK_WRAP_UI 1)
  fltk_wrap_ui( BatchMakeLib ${BatchMakeLib_GUI_SRCS} )

  add_library(BatchMakeLib ${BatchMakeLib_SRCS} ${BatchMakeLib_FLTK_UI_SRCS} ${BatchMakeLib_HDRS} ${BatchMakeLib_GUI_SRCS})

  # Following is needed to ensure the existence of CondorWatcherGUI
  #  generated by fluid when BatchMakeUtilities is built
  add_dependencies(BatchMakeLib BatchMakeUtilities)

  if(FLTK_FOUND)
    if(CYGWIN)
      target_link_libraries(BatchMakeLib
        ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
    endif(CYGWIN)
  endif(FLTK_FOUND)

else(USE_FLTK)

  add_library(BatchMakeLib ${BatchMakeLib_SRCS} ${BatchMakeLib_HDRS})

endif(USE_FLTK)

target_link_libraries(BatchMakeLib BatchMakeUtilities bmModuleDescriptionParser)

if(DASHBOARD_SUPPORT)

  if(WIN32)

    set(BatchMakeDashboard_LIBS ws2_32)

  else(WIN32)

    include(${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
    CHECK_LIBRARY_EXISTS("c" gethostbyname "" NOT_NEED_LIBNSL)
    if(NOT NOT_NEED_LIBNSL)
      CHECK_LIBRARY_EXISTS("nsl" gethostbyname "" HAVE_LIBNSL)
    endif(NOT NOT_NEED_LIBNSL)
    if(HAVE_LIBNSL)
      set(BatchMakeDashboard_LIBS
          #socket
          nsl
          )
    endif(HAVE_LIBNSL)

  endif(WIN32)

  target_link_libraries(BatchMakeLib ${BatchMakeDashboard_LIBS})

endif(DASHBOARD_SUPPORT)

# If we use XCEDE
if(BATCHMAKE_USE_XCEDE)
  target_link_libraries(BatchMakeLib XcedeLib bmcurl)
endif(BATCHMAKE_USE_XCEDE)

# Installation file
install(TARGETS BatchMakeLib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/BatchMake
  ARCHIVE DESTINATION lib/BatchMake)
install(FILES ${BatchMake_HDRS}  ${BatchMakeLib_HDRS}
  DESTINATION include/BatchMake)
if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES CMakeLists.txt DESTINATION source/Code)
  install(FILES ${BatchMakeLib_SRCS} DESTINATION source/Code)
  if(USE_FLTK)
    install(FILES ${BatchMakeLib_GUI_SRCS} DESTINATION source/Code)
  endif(USE_FLTK)
endif(BATCHMAKE_INSTALL_SOURCE)
