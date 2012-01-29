project(CondorWatcher)

include_directories(
  ${CondorWatcher_SOURCE_DIR}
  ${CondorWatcher_BINARY_DIR}
  )

set(CondorWatcher_HDRS
  CondorWatcher.h
  )

set(CondorWatcher_SRCS
  CondorWatcher.cxx
  )

set(CondorWatcherGUI_SRCS
  CondorWatcherGUI.fl
  )

link_libraries(
  ${FLTK_LIBRARIES}
  FLU
  )

set(FLTK_WRAP_UI 1)
fltk_wrap_ui( CondorWatcher  ${CondorWatcherGUI_SRCS} )

add_library( CondorWatcher ${CondorWatcher_SRCS} ${CondorWatcher_FLTK_UI_SRCS} ${CondorWatcher_HDRS})

add_executable( CondorWatcherTool main.cxx)
target_link_libraries(CondorWatcherTool CondorWatcher)
utility_source(BatchMake_CONDOR_WATCHER_TOOL_EXE CondorWatcherTool
               "" main.cxx)
mark_as_advanced(BatchMake_CONDOR_WATCHER_TOOL_EXE)

#MACRO
macro(MOVE_EXECUTABLE_TOP_BINARY NAME CACHE_VAR)
  get_target_property(EXECUTABLE_PATH ${NAME} LOCATION)
  get_filename_component(EXECUTABLE_NAME ${EXECUTABLE_PATH} NAME)

  add_custom_command(
    TARGET ${NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
       -E copy ${EXECUTABLE_PATH} ${BatchMake_BINARY_DIR}/bin/${EXECUTABLE_NAME}
    )
  set(${CACHE_VAR} ${BatchMake_BINARY_DIR}/bin/${EXECUTABLE_NAME}
      CACHE FILEPATH "" FORCE)
endmacro(MOVE_EXECUTABLE_TOP_BINARY NAME)

MOVE_EXECUTABLE_TOP_BINARY(CondorWatcherTool BatchMake_CONDOR_WATCHER_TOOL_EXE)

# Installation file
install(TARGETS CondorWatcherTool
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/BatchMake
  ARCHIVE DESTINATION lib/BatchMake)
  install(FILES ${CondorWatcher_HDRS}
          DESTINATION include/BatchMake/Utilities/CondorWatcher)
if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES ${CondorWatcher_SRCS} ${CondorWatcherGUI_SRCS} main.cxx
    DESTINATION source/Batchmake/Utilities/CondorWatcher)
  install(FILES CMakeLists.txt DESTINATION source/Batchmake/Utilities/CondorWatcher)
endif(BATCHMAKE_INSTALL_SOURCE)

