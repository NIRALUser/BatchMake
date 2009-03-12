PROJECT(PixelCounter)

ADD_EXECUTABLE(PixelCounter main.cxx)
TARGET_LINK_LIBRARIES(PixelCounter BatchMakeUtilities ${ITK_LIBRARIES})

# Add application to generate spheres to test the Pixel Counter
ADD_EXECUTABLE(GenerateSpheres generateSpheres.cxx)
TARGET_LINK_LIBRARIES(GenerateSpheres ${ITK_LIBRARIES})


CONFIGURE_FILE(${PixelCounter_SOURCE_DIR}/PixelCounter.bmm
               ${BatchMake_APPLICATIONS_DIR}/PixelCounter.bmm
               COPYONLY)

# Installation file
INSTALL(TARGETS PixelCounter
        RUNTIME DESTINATION bin)
INSTALL(FILES PixelCounter.bmm DESTINATION bin/Applications)
#INSTALL(FILES main.cxx DESTINATION source/Examples/PixelCounter)
#INSTALL(FILES CMakeLists.txt DESTINATION source/Examples/PixelCounter)
