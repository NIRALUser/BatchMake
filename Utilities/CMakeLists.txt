PROJECT(BatchMakeUtilities)

INCLUDE_DIRECTORIES(
  ${BatchMakeUtilities_SOURCE_DIR}
  ${BatchMakeUtilitiesr_BINARY_DIR}
)


SET(BatchMakeUtilities_SRCS
  MString.cxx
  Timer.cxx
  XMLReader.cxx
  XMLWriter.cxx
  XMLIniIO.cxx
  TCPSocket.cxx
)

ADD_LIBRARY(BatchMakeUtilities ${BatchMakeUtilities_SRCS})
