Project(ModuleDescriptionParserTesting)

set( PARSER1TEST_SOURCE Parser1Test )
add_executable( Parser1Test ${PARSER1TEST_SOURCE})
target_link_libraries( Parser1Test bmModuleDescriptionParser )

set( ModuleFactoryTest_SOURCE ModuleFactoryTest )
add_executable( ModuleFactoryTest ${ModuleFactoryTest_SOURCE})
target_link_libraries( ModuleFactoryTest bmModuleDescriptionParser )

include_directories(
  ${ModuleDescriptionParser_SOURCE_DIR}
  )

set(TEST_DATA ${ModuleDescriptionParserTesting_SOURCE_DIR}/TestData)
add_test(Parser1Test1 ${EXECUTABLE_OUTPUT_PATH}/Parser1Test ${TEST_DATA}/ParserTest1.xml)

add_test(Parser1Test2 ${EXECUTABLE_OUTPUT_PATH}/Parser1Test ${TEST_DATA}/ParserTest2.xml)

if(Slicer3_BINARY_DIR)
add_test(ModuleFactoryTest ${EXECUTABLE_OUTPUT_PATH}/ModuleFactoryTest ${Slicer3_BINARY_DIR}/${SLICER_INSTALL_LIBRARIES_DIR}/Plugins/)
endif(Slicer3_BINARY_DIR)

