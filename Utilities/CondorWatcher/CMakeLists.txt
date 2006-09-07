PROJECT(CondorWatcher)

INCLUDE_DIRECTORIES(
  ${CondorWatcher_SOURCE_DIR}
  ${CondorWatcher_BINARY_DIR}
)

SET(CondorWatcher_HDRS
  CondorWatcher.h
)

SET(CondorWatcher_SRCS
  CondorWatcher.cxx
)

SET(CondorWatcherGUI_SRCS
  CondorWatcherGUI.fl
)

LINK_LIBRARIES(
 ${FLTK_LIBRARIES}
 FLU
)

SET(FLTK_WRAP_UI 1)
FLTK_WRAP_UI( CondorWatcher  ${CondorWatcherGUI_SRCS} )
ADD_LIBRARY( CondorWatcher ${CondorWatcher_SRCS} ${CondorWatcher_FLTK_UI_SRCS})

ADD_EXECUTABLE( CondorWatcherTool main.cxx)
TARGET_LINK_LIBRARIES(CondorWatcherTool CondorWatcher)

# Installation file
INSTALL(TARGETS CondorWatcherTool CondorWatcher  
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${CondorWatcher_HDRS} DESTINATION include/BatchMake/Utilities/CondorWatcher)
INSTALL(FILES ${CondorWatcher_SRCS} ${CondorWatcherGUI_SRCS} main.cxx DESTINATION source/Utilities/CondorWatcher)
INSTALL(FILES CMakeLists.txt DESTINATION source/Utilities/CondorWatcher)
