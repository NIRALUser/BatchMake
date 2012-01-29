
#------------------------------------------------------------------------
# Create BatchMake application
include_directories(
    ${BatchMake_SOURCE_DIR}/Utilities/bmModuleDescriptionParser
    ${BatchMake_BINARY_DIR}/Utilities/bmModuleDescriptionParser
  )

set(BatchMake_SRCS BatchMake.cxx)
if(WIN32)
  if(NOT CYGWIN)
    if(NOT MINGW)
      set(BatchMake_SRCS ${BatchMake_SRCS} BatchMake.rc)
    endif(NOT MINGW)
  endif(NOT CYGWIN)

 option(HIDE_CONSOLE "Hide the console" OFF)
 if(HIDE_CONSOLE)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
 endif(HIDE_CONSOLE)
endif(WIN32)


add_executable(BatchMake BatchMake.cxx)

target_link_libraries(BatchMake ${BatchMake_LIBRARIES})

add_dependencies(BatchMake ${BatchMake_LIBRARIES})

#------------------------------------------------------------------------
# Create the dashboard send application

add_executable(bmGridSend bmGridSend.cxx)
target_link_libraries(bmGridSend ${BatchMake_LIBRARIES})
utility_source(BatchMake_GRID_SEND_EXE bmGridSend "" bmGridSend.cxx)
mark_as_advanced(BatchMake_GRID_SEND_EXE)

#------------------------------------------------------------------------
# Create the dashboard send application

add_executable(bmGridStore bmGridStore.cxx)
target_link_libraries(bmGridStore ${BatchMake_LIBRARIES})
utility_source(BatchMake_GRID_STORE_EXE bmGridStore "" bmGridStore.cxx)
mark_as_advanced(BatchMake_GRID_STORE_EXE)

#------------------------------------------------------------------------
# Create the dashboard send application

add_executable(bmSliceExtractor bmSliceExtractor.cxx)
target_link_libraries(bmSliceExtractor ${BatchMake_LIBRARIES})
utility_source(BatchMake_SLICE_EXTRACTOR_EXE bmSliceExtractor
               "" bmSliceExtractor.cxx)
mark_as_advanced(BatchMake_SLICE_EXTRACTOR_EXE)

#------------------------------------------------------------------------
#  Check Memory

add_executable(bmCheckMemory CheckMemory.cxx)
target_link_libraries(bmCheckMemory ${BatchMake_LIBRARIES})

#------------------------------------------------------------------------
#  Get CPU Info

add_executable(bmGetCPUInfo GetCPUInfo.cxx)
target_link_libraries(bmGetCPUInfo ${BatchMake_LIBRARIES})
utility_source(BatchMake_GET_CPU_INFO_EXE bmGetCPUInfo
               "" GetCPUInfo.cxx)
               mark_as_advanced(BatchMake_GET_CPU_INFO_EXE)

#------------------------------------------------------------------------
#  Get Memory Info

add_executable(bmGetMemoryInfo GetMemoryInfo.cxx)
target_link_libraries(bmGetMemoryInfo ${BatchMake_LIBRARIES})
utility_source(BatchMake_GET_MEMORY_INFO_EXE bmGetMemoryInfo
               "" GetMemoryInfo.cxx)
               mark_as_advanced(BatchMake_GET_MEMORY_INFO_EXE)

#------------------------------------------------------------------------
#  Get CPU Speed

add_executable(bmGetCPUSpeed GetCPUSpeed.cxx Whetstone.cxx)
target_link_libraries(bmGetCPUSpeed ${BatchMake_LIBRARIES})
if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(Whetstone.cxx PROPERTIES COMPILE_FLAGS -O0)
else(CMAKE_COMPILER_IS_GNUCXX)
  if(WIN32)
    set_source_files_properties(Whetstone.cxx PROPERTIES COMPILE_FLAGS -Od)
  endif(WIN32)
endif(CMAKE_COMPILER_IS_GNUCXX)
utility_source(BatchMake_GET_CPU_SPEED_EXE bmGetCPUSpeed
               "" GetCPUSpeed.cxx)
               mark_as_advanced(BatchMake_GET_CPU_SPEED_EXE)

#------------------------------------------------------------------------
# Move apps to where CMake can find them

macro(MOVE_EXECUTABLE_TOP_BINARY NAME CACHE_VAR)
  get_target_property(EXECUTABLE_PATH ${NAME} LOCATION)
  get_filename_component(EXECUTABLE_NAME ${EXECUTABLE_PATH} NAME)

  add_custom_command(
    TARGET ${NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
       -E copy ${EXECUTABLE_PATH} ${BatchMake_BINARY_DIR}/bin/${EXECUTABLE_NAME}    )
  set(${CACHE_VAR} ${BatchMake_BINARY_DIR}/bin/${EXECUTABLE_NAME}
    CACHE FILEPATH "" FORCE
    )
endmacro(MOVE_EXECUTABLE_TOP_BINARY NAME)

MOVE_EXECUTABLE_TOP_BINARY(bmGridSend BatchMake_GRID_SEND_EXE )
MOVE_EXECUTABLE_TOP_BINARY(bmGridStore BatchMake_GRID_STORE_EXE )
MOVE_EXECUTABLE_TOP_BINARY(bmSliceExtractor BatchMake_SLICE_EXTRACTOR_EXE )
MOVE_EXECUTABLE_TOP_BINARY(bmGetMemoryInfo BatchMake_GET_MEMORY_INFO_EXE )
MOVE_EXECUTABLE_TOP_BINARY(bmGetCPUSpeed BatchMake_GET_CPU_SPEED_EXE )
MOVE_EXECUTABLE_TOP_BINARY(bmGetCPUInfo BatchMake_GET_CPU_INFO_EXE )

#------------------------------------------------------------------------
# Installation file

install(TARGETS BatchMake bmGetCPUInfo bmGetMemoryInfo bmGetCPUSpeed
        bmSliceExtractor bmGridStore bmGridSend bmCheckMemory
        LIBRARY DESTINATION lib/BatchMake
        ARCHIVE DESTINATION lib/BatchMake
        RUNTIME DESTINATION bin
        )

if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES BatchMake.cxx BatchMake.ico BatchMake.rc Whetstone.h
          GetCPUInfo.cxx GetMemoryInfo.cxx GetCPUSpeed.cxx Whetstone.cxx
          bmGridSend.cxx bmGridStore.cxx bmSliceExtractor.cxx CheckMemory.cxx
          DESTINATION source/BatchMake/Applications
          )
  install(FILES CMakeLists.txt
          DESTINATION source/BatchMake/Applications
         )
endif(BATCHMAKE_INSTALL_SOURCE)
