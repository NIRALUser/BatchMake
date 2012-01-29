subdirs(
  PixelCounter
  )

if(GRID_SUPPORT)
  subdirs(
    CondorTest
    Calculator
    CondorTutorial
    )
endif(GRID_SUPPORT)

if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES CMakeLists.txt
          DESTINATION source/BatchMake/Examples)
endif(BATCHMAKE_INSTALL_SOURCE)

# Install the examples scripts
file(GLOB Scripts Scripts/?*.*)
install(FILES ${Scripts}
        DESTINATION examples/scripts)
