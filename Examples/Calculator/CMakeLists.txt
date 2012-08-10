project(Calculator)

add_executable(Calculator main.cxx)

target_link_libraries(Calculator ${BatchMake_LIBRARIES})

# Installation file
install(TARGETS Calculator
        RUNTIME DESTINATION bin)
#install(FILES main.cxx DESTINATION source/Examples/Calculator)
#install(FILES CMakeLists.txt DESTINATION source/Examples/Calculator)
