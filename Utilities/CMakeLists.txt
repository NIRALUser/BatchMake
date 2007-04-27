IF(USE_FLTK)
  SUBDIRS(Flu)
  IF(GRID_SUPPORT)
    SUBDIRS(CondorWatcher)
  ENDIF(GRID_SUPPORT)
ENDIF(USE_FLTK)

SUBDIRS(Zip)

IF( CMAKE_COMPILER_IS_GNUCXX )
  SET_SOURCE_FILES_PROPERTIES(Whetstone.cxx PROPERTIES COMPILE_FLAGS -O0)
ELSE( CMAKE_COMPILER_IS_GNUCXX )
  SET_SOURCE_FILES_PROPERTIES(Whetstone.cxx PROPERTIES COMPILE_FLAGS -Od)
ENDIF( CMAKE_COMPILER_IS_GNUCXX )

SET(BatchMakeUtilities_HDRS
  MString.h
  Timer.h
  XMLReader.h
  XMLWriter.h
  XMLIniIO.h
  TCPSocket.h
  XMLBufferReader.h
  SliceExtractor.h
  SystemInfo.h
  Whetstone.h
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
  SystemInfo.cxx
  Whetstone.cxx
)

ADD_LIBRARY(BatchMakeUtilities ${BatchMakeUtilities_SRCS})

# Installation file
INSTALL(TARGETS BatchMakeUtilities
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${BatchMakeUtilities_HDRS} DESTINATION include/BatchMake/Utilities)
INSTALL(FILES ${BatchMakeUtilities_SRCS} DESTINATION source/Utilities)
INSTALL(FILES CMakeLists.txt DESTINATION source/Utilities)
