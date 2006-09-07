PROJECT(PixelCounter)

ADD_EXECUTABLE(PixelCounter main.cxx)
TARGET_LINK_LIBRARIES(PixelCounter BatchMakeDashboard BatchMakeUtilities)

# Installation file
INSTALL(TARGETS PixelCounter
        RUNTIME DESTINATION bin)
INSTALL(FILES main.cxx DESTINATION include/BatchMake/External/PixelCounter)
INSTALL(FILES CMakeLists.txt DESTINATION include/BatchMake/External/PixelCounter)
