PROJECT(CondorWatcher)

INCLUDE_DIRECTORIES(
  ${CondorWatcher_SOURCE_DIR}
  ${CondorWatcher_BINARY_DIR}
)


SET(CondorWatcher_SRCS
  main.cxx
  CondorWatcher.cxx
  CondorWatcher.h
)

SET(CondorWatcherGUI_SRCS
  CondorWatcherGUI.fl
)

LINK_LIBRARIES(
 ${FLTK_LIBRARIES}
 FLU
)

#ADD_LIBRARY( CondorWatcher ${CondorWatcher_SRCS})
ADD_EXECUTABLE( CondorWatcher ${CondorWatcher_SRCS})
SET(FLTK_WRAP_UI 1)
FLTK_WRAP_UI( CondorWatcher  ${CondorWatcherGUI_SRCS} )

