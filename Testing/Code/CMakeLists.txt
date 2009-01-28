#--------------------------------------------------------------------
# Source

SET(BATCHMAKE_TESTS ${CXX_TEST_PATH}/BatchMakeTests)

ADD_EXECUTABLE(BatchMakeMemoryTest BatchMakeMemoryTest.cxx)
TARGET_LINK_LIBRARIES(BatchMakeMemoryTest itksys)

IF(DASHBOARD_SUPPORT)

  SET(BatchMakeTest_SRCS
    bmDashboardSendTest.cxx
    bmSystemInfoTest.cxx
    bmFileActionsTest.cxx
    )
  SET (TEST_DATA ${BATCHMAKE_TESTS_SOURCE_DIR}/TestData)

  ADD_TEST(DashboardSendTest ${BATCHMAKE_TESTS} DashboardSendTest)
  ADD_TEST(SystemInfoTest ${BATCHMAKE_TESTS} SystemInfoTest)

  ADD_TEST(FileActionsTest ${BATCHMAKE_TESTS} FileActionsTest ${TEST_DATA}/FileActionsTest.bms)

  ADD_EXECUTABLE(BatchMakeTests BatchMakeTests.cxx ${BatchMakeTest_SRCS})
  TARGET_LINK_LIBRARIES(BatchMakeTests ${BatchMake_LIBRARIES})

  ADD_DEPENDENCIES(BatchMakeTests BatchMakeLib)

ENDIF(DASHBOARD_SUPPORT)

