
#---------------------------------------------------------------------
# Use FLTK = FLU and CondorWatcher

if(USE_FLTK)
  subdirs(Flu)
  if(GRID_SUPPORT)
    subdirs(CondorWatcher)
  endif(GRID_SUPPORT)
endif(USE_FLTK)

#---------------------------------------------------------------------
#  External Libraries

subdirs(Zip bmcurl bmModuleDescriptionParser)

if(BATCHMAKE_USE_XCEDE)
  subdirs(XcedeLib)
endif(BATCHMAKE_USE_XCEDE)



#---------------------------------------------------------------------
#  Source

set(BatchMakeUtilities_HDRS
  MString.h
  BMString.h
  Timer.h
  XMLReader.h
  XMLWriter.h
  XMLIniIO.h
  TCPSocket.h
  XMLBufferReader.h
  SliceExtractor.h
  HttpRequest.h
  )


set(BatchMakeUtilities_SRCS
  MString.cxx
  BMString.cxx
  Timer.cxx
  XMLReader.cxx
  XMLWriter.cxx
  XMLIniIO.cxx
  TCPSocket.cxx
  XMLBufferReader.cxx
  SliceExtractor.cxx
  HttpRequest.cxx
  )

#---------------------------------------------------------------------
#  Build BatchMakeUtilities Library

add_library(BatchMakeUtilities ${BatchMakeUtilities_SRCS} ${BatchMakeUtilities_HDRS})
target_link_libraries(BatchMakeUtilities ${ITK_LIBRARIES})

if(USE_FLTK)
  target_link_libraries(BatchMakeUtilities ${FLTK_LIBRARIES} FLU)
  add_dependencies(BatchMakeUtilities FLU)
  if(GRID_SUPPORT)
    # Bug with FLTK on Mac causes the CondorWatcher to load the window
    if(NOT APPLE)
    target_link_libraries(BatchMakeUtilities CondorWatcher)
    add_dependencies(BatchMakeUtilities CondorWatcher)
    endif(NOT APPLE)
  endif(GRID_SUPPORT)
endif(USE_FLTK)

#---------------------------------------------------------------------
# Installation file
install(TARGETS BatchMakeUtilities
  ARCHIVE DESTINATION lib/BatchMake
  LIBRARY DESTINATION lib/BatchMake
  RUNTIME DESTINATION bin)
install(FILES ${BatchMakeUtilities_HDRS}
  DESTINATION include/BatchMake/Utilities)
if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES ${BatchMakeUtilities_SRCS}
    DESTINATION source/BatchMake/Utilities)
  install(FILES CMakeLists.txt
    DESTINATION source/BatchMake/Utilities)
endif(BATCHMAKE_INSTALL_SOURCE)
