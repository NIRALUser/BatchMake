Project(BatchMakeTest)
#--------------------------------------------------------------------
# Source

set(BATCHMAKE_TESTS ${CXX_TEST_PATH}/BatchMakeTests)

add_executable(BatchMakeMemoryTest BatchMakeMemoryTest.cxx)
target_link_libraries(BatchMakeMemoryTest itksys ${ITK_LIBRARIES})

if(DASHBOARD_SUPPORT)

  set(BatchMakeTest_SRCS
    bmDashboardSendTest.cxx
    bmSystemInfoTest.cxx
    bmActionsTest.cxx
    bmErrorActionsTest.cxx
    )
  set(TEST_DATA ${BatchMakeTest_SOURCE_DIR}/TestData)

  add_test(DashboardSendTest ${BATCHMAKE_TESTS} DashboardSendTest)
  add_test(SystemInfoTest ${BATCHMAKE_TESTS} SystemInfoTest)

  add_test(FileActionsTest   ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/FileActionsTest.bms)
  add_test(StringActionsTest ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/StringActionsTest.bms)
  add_test(MathActionsTest   ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/MathActionsTest.bms)
  add_test(AppActionsTest    ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/AppActionsTest.bms ${BatchMake_APPLICATIONS_DIR})
  add_test(QuoteActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/QuoteActionsTest.bms)
  add_test(ExitActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitActionsTest.bms)
  add_test(AddErrorActionsTest  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/AddErrorActionsTest.bms 5)
  add_test(ExitOnErrorActionsTest1  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitOnErrorActionsTest1.bms)
  add_test(ExitOnErrorActionsTest2  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ExitOnErrorActionsTest2.bms)
  add_test(ExitOnErrorActionsTest3  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/ExitOnErrorActionsTest3.bms 2)
  add_test(ExitOnErrorActionsTest4  ${BATCHMAKE_TESTS} ErrorActionsTest ${TEST_DATA}/ExitOnErrorActionsTest4.bms 3)
  add_test(GetErrorCountActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/GetErrorCountActionsTest.bms)
  add_test(ClearErrorsActionsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/ClearErrorsActionsTest.bms)
  add_test(ParsingLongInputsTest  ${BATCHMAKE_TESTS} ActionsTest ${TEST_DATA}/MoreThan1000CharsParsingTest.bms)

  add_executable(BatchMakeTests BatchMakeTests.cxx ${BatchMakeTest_SRCS})
  target_link_libraries(BatchMakeTests ${BatchMake_LIBRARIES})

  add_dependencies(BatchMakeTests BatchMakeLib)

endif(DASHBOARD_SUPPORT)

