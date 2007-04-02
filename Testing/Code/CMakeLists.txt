SET(BATCHMAKE_TESTS ${CXX_TEST_PATH}/BatchMakeTests)

SET(BatchMakeTest_SRCS
bmDashboardSendTest.cxx
)

ADD_TEST(DashboardSendTest ${BATCHMAKE_TESTS} DashboardSendTest)

ADD_EXECUTABLE(BatchMakeTests BatchMakeTests.cxx ${BatchMakeTest_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeTests BatchMakeDashboard)

ADD_DEPENDENCIES(BatchMakeTests BatchMakeDashboard)

