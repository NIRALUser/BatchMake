PROJECT(bmCondorTest)

SET(bmCondorTest_SRCS
  bmCondorTest.cxx
)

IF(WIN32)
LINK_LIBRARIES(
wsock32
)
ENDIF(WIN32)

ADD_EXECUTABLE(bmCondorTest ${bmCondorTest_SRCS})
