project(bmCondorTest)

set(bmCondorTest_SRCS
  bmCondorTest.cxx
  )

add_executable(bmCondorTest ${bmCondorTest_SRCS})
target_link_libraries(bmCondorTest ${ITK_LIBRARIES})

if(WIN32)
  target_link_libraries(bmCondorTest
    wsock32
  )
endif(WIN32)


#----------------------------------------------------------------------
# Installation file

install(TARGETS bmCondorTest
        RUNTIME DESTINATION bin)
#install(FILES bmCondorTest.cxx DESTINATION source/Examples/CondorTest)
#install(FILES CMakeLists.txt DESTINATION source/Examples/CondorTest)

