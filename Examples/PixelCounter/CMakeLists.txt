PROJECT(PixelCounter)

ADD_EXECUTABLE(PixelCounter main.cxx)
TARGET_LINK_LIBRARIES(PixelCounter BatchMakeUtilities ${ITK_LIBRARIES})

# Add application to generate spheres to test the Pixel Counter
ADD_EXECUTABLE(GenerateSpheres generateSpheres.cxx)
TARGET_LINK_LIBRARIES(GenerateSpheres ${ITK_LIBRARIES})

FIND_PROGRAM( PixelCounterExec PixelCounter 
  NAMES PixelCounter PixelCounter.exe
  PATHS ${CMAKE_BINARY_DIR}/bin )
FIND_PROGRAM( GenerateSpheresExec GenerateSpheres 
  NAMES GenerateSpheres GenerateSpheres.exe
  PATHS ${CMAKE_BINARY_DIR}/bin )

CONFIGURE_FILE(${PixelCounter_SOURCE_DIR}/PixelCounter.bmm.in
               ${BatchMake_APPLICATIONS_DIR}/PixelCounter.bmm )
CONFIGURE_FILE(${PixelCounter_SOURCE_DIR}/GenerateSpheres.bmm.in
               ${BatchMake_APPLICATIONS_DIR}/GenerateSpheres.bmm)

# Installation file
INSTALL(TARGETS PixelCounter
        RUNTIME DESTINATION bin)
#INSTALL(FILES PixelCounter.bmm DESTINATION bin/Applications)
#INSTALL(FILES GenerateSpheres.bmm DESTINATION bin/Applications)

#INSTALL(FILES main.cxx DESTINATION source/Examples/PixelCounter)
#INSTALL(FILES CMakeLists.txt DESTINATION source/Examples/PixelCounter)
