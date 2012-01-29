project(PixelCounter)

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${USE_ITK_FILE})
endif(ITK_FOUND)

add_executable(PixelCounter main.cxx)
target_link_libraries(PixelCounter BatchMakeUtilities ${ITK_LIBRARIES})

# Add application to generate spheres to test the Pixel Counter
add_executable(GenerateSpheres generateSpheres.cxx)
target_link_libraries(GenerateSpheres ${ITK_LIBRARIES})

find_program( PixelCounterExec PixelCounter
  NAMES PixelCounter PixelCounter.exe
  PATHS ${CMAKE_BINARY_DIR}/bin )
find_program( GenerateSpheresExec GenerateSpheres
  NAMES GenerateSpheres GenerateSpheres.exe
  PATHS ${CMAKE_BINARY_DIR}/bin )

configure_file(${PixelCounter_SOURCE_DIR}/PixelCounter.bmm.in
               ${BatchMake_APPLICATIONS_DIR}/PixelCounter.bmm )
configure_file(${PixelCounter_SOURCE_DIR}/GenerateSpheres.bmm.in
               ${BatchMake_APPLICATIONS_DIR}/GenerateSpheres.bmm)

# Installation file
install(TARGETS PixelCounter
        RUNTIME DESTINATION bin)
#install(FILES PixelCounter.bmm DESTINATION bin/Applications)
#install(FILES GenerateSpheres.bmm DESTINATION bin/Applications)

#install(FILES main.cxx DESTINATION source/Examples/PixelCounter)
#install(FILES CMakeLists.txt DESTINATION source/Examples/PixelCounter)
