SUBDIRS(
  PixelCounter
  )

IF(GRID_SUPPORT)
  SUBDIRS(
    CondorTest
    Calculator
    CondorTutorial
    )
ENDIF(GRID_SUPPORT)

IF(BATCHMAKE_INSTALL_SOURCE)
  INSTALL(FILES CMakeLists.txt 
          DESTINATION source/BatchMake/Examples)
ENDIF(BATCHMAKE_INSTALL_SOURCE)

# Install the examples scripts
FILE(GLOB Scripts Scripts/?*.*)
INSTALL(FILES ${Scripts} 
        DESTINATION examples/scripts)
