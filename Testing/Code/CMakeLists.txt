SET(BatchMakeTest_SRCS
bmDashboardSendTest.cxx
)

ADD_TEST(DashboardSendTest ${IO_TESTS} DashboardSendTest)

ADD_EXECUTABLE(BatchMakeTests BatchMakeTests.cxx ${BatchMakeTest_SRCS})
TARGET_LINK_LIBRARIES(BatchMakeTests BatchMakeDashboard)

ADD_DEPENDENCIES(BatchMakeTests BatchMakeDashboard)

