IF(GRID_SUPPORT)
  SUBDIRS(CondorWatcher)
ENDIF(GRID_SUPPORT)

SUBDIRS(Flu Zip)

SET(BatchMakeUtilities_HDRS
  MString.h
  Timer.h
  XMLReader.h
  XMLWriter.h
  XMLIniIO.h
  TCPSocket.h
  XMLBufferReader.h
  SliceExtractor.h
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
)

ADD_LIBRARY(BatchMakeUtilities ${BatchMakeUtilities_SRCS})

# Installation file
INSTALL(TARGETS BatchMakeUtilities
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${BatchMakeUtilities_HDRS} DESTINATION include/BatchMake/Utilities)
INSTALL(FILES ${BatchMakeUtilities_SRCS} DESTINATION source/Utilities)
INSTALL(FILES CMakeLists.txt DESTINATION source/Utilities)
