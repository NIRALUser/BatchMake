PROJECT(BatchMakeDashboard)

SET(BatchMakeDashboard_SRCS
  HttpRequest.cxx
  HttpRequest.h
  SliceExtractor.cxx
  SliceExtractor.h
)

IF(WIN32)
SET(BatchMakeDashboard_LIBS
 ws2_32
)
ELSE(WIN32)
SET(Uploader_LIBS
 socket
 nsl
)
ENDIF(WIN32)


ADD_LIBRARY(BatchMakeDashboard ${BatchMakeDashboard_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeDashboard ${BatchMakeDashboard_LIBS})
