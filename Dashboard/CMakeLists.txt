PROJECT(BatchMakeDashboard)

SET(BatchMakeDashboard_SRCS
  HttpRequest.cxx
  HttpRequest.h
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

# Create the dashboard send application
ADD_EXECUTABLE(bmGridSend bmGridSend.cxx)
TARGET_LINK_LIBRARIES(bmGridSend BatchMakeDashboard BatchMakeUtilities)

ADD_EXECUTABLE(bmGridStore bmGridStore.cxx)
TARGET_LINK_LIBRARIES(bmGridStore BatchMakeDashboard BatchMakeUtilities)

ADD_EXECUTABLE(bmSliceExtractor bmSliceExtractor.cxx)
TARGET_LINK_LIBRARIES(bmSliceExtractor BatchMakeDashboard BatchMakeUtilities)
