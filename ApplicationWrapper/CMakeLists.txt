PROJECT(BatchMakeApplicationWrapper)

INCLUDE_DIRECTORIES(
  ${BatchMakeApplicationWrapper_SOURCE_DIR}
  ${BatchMakeApplicationWrapper_BINARY_DIR}
)


SET(BatchMakeApplicationWrapper_SRCS
  ApplicationWrapper.cxx
  ApplicationWrapperParam.cxx
  ApplicationWrapper.h
  ApplicationWrapperParam.h
)

LINK_LIBRARIES(
 BatchMakeUtilities
)


ADD_LIBRARY(BatchMakeApplicationWrapper ${BatchMakeApplicationWrapper_SRCS})