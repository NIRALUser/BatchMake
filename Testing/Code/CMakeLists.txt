Project(BatchMakeTest)
#--------------------------------------------------------------------
# Source

SET(BATCHMAKE_TESTS ${CXX_TEST_PATH}/BatchMakeTests)

ADD_EXECUTABLE(BatchMakeMemoryTest BatchMakeMemoryTest.cxx)
TARGET_LINK_LIBRARIES(BatchMakeMemoryTest itksys ${ITK_LIBRARIES})

IF(DASHBOARD_SUPPORT)

  SET(BatchMakeTest_SRCS
    bmDashboardSendTest.cxx
    bmSystemInfoTest.cxx
    bmActionsTest.cxx
    bmErrorActionsTest.cxx
    )
  SET (TEST_DATA ${BatchMakeTest_SOURCE_DIR}/TestData)

  ADD_TEST(DashboardSendTest ${BATCHMAKE_TESTS} DashboardSendTest)
  ADD_TEST(SystemInfoTest ${BATCHMAKE_TESTS} SystemInfoTest)

  ADD_TEST(FileActionsTest   ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/FileActionsTest.bms)
  ADD_TEST(StringActionsTest ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/StringActionsTest.bms)
  ADD_TEST(MathActionsTest   ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/MathActionsTest.bms)
  ADD_TEST(AppActionsTest    ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/AppActionsTest.bms ${BatchMake_APPLICATIONS_DIR})
  ADD_TEST(QuoteActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/QuoteActionsTest.bms)
  ADD_TEST(ExitActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitActionsTest.bms)
  ADD_TEST(AddErrorActionsTest  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/AddErrorActionsTest.bms 5)
  ADD_TEST(ExitOnErrorActionsTest1  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitOnErrorActionsTest1.bms)
  ADD_TEST(ExitOnErrorActionsTest2  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitOnErrorActionsTest2.bms)
  ADD_TEST(ExitOnErrorActionsTest3  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/ExitOnErrorActionsTest3.bms 2)
  ADD_TEST(ExitOnErrorActionsTest4  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/ExitOnErrorActionsTest4.bms 3)
  ADD_TEST(GetErrorCountActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/GetErrorCountActionsTest.bms)
  ADD_TEST(ClearErrorsActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ClearErrorsActionsTest.bms)

  ADD_EXECUTABLE(BatchMakeTests BatchMakeTests.cxx ${BatchMakeTest_SRCS})
  TARGET_LINK_LIBRARIES(BatchMakeTests ${BatchMake_LIBRARIES})

  ADD_DEPENDENCIES(BatchMakeTests BatchMakeLib)

ENDIF(DASHBOARD_SUPPORT)

