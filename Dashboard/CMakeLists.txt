SET(BatchMakeDashboard_HDRS
  HttpRequest.h
  bmScriptDbSendValueAction.h
  bmScriptDbSendFileAction.h
  bmScriptDbClearAction.h
  bmScriptOpenTCPSocketAction.h
  bmScriptSendTCPAction.h
  bmScriptCloseTCPSocketAction.h
  bmScriptAddMethodInputAction.h
  bmScriptAddMethodIdealOutputAction.h
  bmScriptSetIdealOutputAction.h
  bmScriptDashboardHostAction.h
  bmScriptDashboardUserAction.h
  bmScriptDashboardSendAction.h
  bmScriptDashboardNotifyAction.h
  bmScriptCreateExperimentAction.h
  bmScriptCreateMethodAction.h   
  bmScriptAddMethodOutputAction.h  
  bmDashboardSend.h
)

SET(BatchMakeDashboard_SRCS
  HttpRequest.cxx
  bmScriptDbSendValueAction.cxx
  bmScriptDbSendFileAction.cxx
  bmScriptDbClearAction.cxx
  bmScriptOpenTCPSocketAction.cxx
  bmScriptSendTCPAction.cxx
  bmScriptCloseTCPSocketAction.cxx
  bmScriptAddMethodInputAction.cxx
  bmScriptAddMethodIdealOutputAction.cxx
  bmScriptSetIdealOutputAction.cxx
  bmScriptDashboardHostAction.cxx
  bmScriptDashboardUserAction.cxx
  bmScriptDashboardSendAction.cxx
  bmScriptDashboardNotifyAction.cxx
  bmScriptCreateExperimentAction.cxx
  bmScriptCreateMethodAction.cxx   
  bmScriptAddMethodOutputAction.cxx  
  bmScriptActionManagerDashboard.cxx
  bmDashboardSend.cxx
)

IF(WIN32)
SET(BatchMakeDashboard_LIBS
 ws2_32
)

ELSE(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
  CHECK_LIBRARY_EXISTS("c" gethostbyname "" NOT_NEED_LIBNSL)
  IF(NOT NOT_NEED_LIBNSL)
    CHECK_LIBRARY_EXISTS_CONCAT("nsl"    gethostbyname  HAVE_LIBNSL)
  ENDIF(NOT NOT_NEED_LIBNSL)
  IF(HAVE_LIBNSL)
    SET(BatchMakeDashboard_LIBS
      #socket
     nsl
    )
  ENDIF(HAVE_LIBNSL)
ENDIF(WIN32)

ADD_LIBRARY(BatchMakeDashboard ${BatchMakeDashboard_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeDashboard ${BatchMakeDashboard_LIBS})

IF(GRID_SUPPORT)
  TARGET_LINK_LIBRARIES(BatchMakeDashboard BatchMakeGrid)
ENDIF(GRID_SUPPORT)

# Create the dashboard send application
ADD_EXECUTABLE(bmGridSend bmGridSend.cxx)
TARGET_LINK_LIBRARIES(bmGridSend BatchMakeDashboard BatchMakeUtilities)

ADD_EXECUTABLE(bmGridStore bmGridStore.cxx)
TARGET_LINK_LIBRARIES(bmGridStore BatchMakeDashboard BatchMakeUtilities)

ADD_EXECUTABLE(bmSliceExtractor bmSliceExtractor.cxx)
TARGET_LINK_LIBRARIES(bmSliceExtractor BatchMakeDashboard BatchMakeUtilities)

# Installation file
INSTALL(TARGETS BatchMakeDashboard bmSliceExtractor bmGridStore bmGridSend
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${BatchMakeDashboard_HDRS} DESTINATION include/BatchMake/Dashboard)
INSTALL(FILES ${BatchMakeDashboard_SRCS} bmGridSend.cxx bmGridStore.cxx bmSliceExtractor.cxx DESTINATION source/Dashboard)
INSTALL(FILES CMakeLists.txt DESTINATION source/Dashboard)
