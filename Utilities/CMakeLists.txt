
#---------------------------------------------------------------------
# Use FLTK = FLU and CondorWatcher

IF(USE_FLTK)
  SUBDIRS(Flu)
  IF(GRID_SUPPORT)
    SUBDIRS(CondorWatcher)
  ENDIF(GRID_SUPPORT)
ENDIF(USE_FLTK)

#---------------------------------------------------------------------
#  External Libraries

SUBDIRS(Zip bmcurl bmModuleDescriptionParser)

IF(BATCHMAKE_USE_XCEDE)
  SUBDIRS(XcedeLib)
ENDIF(BATCHMAKE_USE_XCEDE)



#---------------------------------------------------------------------
#  Source

SET(BatchMakeUtilities_HDRS
  MString.h
  Timer.h
  XMLReader.h
  XMLWriter.h
  XMLIniIO.h
  TCPSocket.h
  XMLBufferReader.h
  SliceExtractor.h
  HttpRequest.h
  )


SET(BatchMakeUtilities_SRCS
  MString.cxx
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

ADD_LIBRARY(BatchMakeUtilities ${BatchMakeUtilities_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeUtilities ITKCommon ITKIO itksys)

IF(USE_FLTK)
  TARGET_LINK_LIBRARIES(BatchMakeUtilities ${FLTK_LIBRARIES} FLU)
  ADD_DEPENDENCIES(BatchMakeUtilities FLU)
  IF(GRID_SUPPORT)
    # Bug with FLTK on Mac causes the CondorWatcher to load the window
    IF(NOT APPLE)
    TARGET_LINK_LIBRARIES(BatchMakeUtilities CondorWatcher)
    ADD_DEPENDENCIES(BatchMakeUtilities CondorWatcher)
    ENDIF(NOT APPLE)
  ENDIF(GRID_SUPPORT)
ENDIF(USE_FLTK)

#---------------------------------------------------------------------
# Installation file
INSTALL(TARGETS BatchMakeUtilities 
  ARCHIVE DESTINATION lib/BatchMake
  LIBRARY DESTINATION lib/BatchMake
  RUNTIME DESTINATION bin)
INSTALL(FILES ${BatchMakeUtilities_HDRS} 
  DESTINATION include/BatchMake/Utilities)
IF(BATCHMAKE_INSTALL_SOURCE)
  INSTALL(FILES ${BatchMakeUtilities_SRCS} 
    DESTINATION source/BatchMake/Utilities)
  INSTALL(FILES CMakeLists.txt 
    DESTINATION source/BatchMake/Utilities)
ENDIF(BATCHMAKE_INSTALL_SOURCE)
