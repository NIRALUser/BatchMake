project(ImageRegTool)


find_package(ITK)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(FATAL_ERROR "Please set ITK_DIR.")
endif(ITK_FOUND)

include_directories(
  ${ITK_SOURCE_DIR}/Code/Common
  ${ITK_SOURCE_DIR}/Code/BasicFilters
  ${ITK_SOURCE_DIR}/Code/IO
  ${ITK_SOURCE_DIR}/Code/Numerics
  ${ITK_SOURCE_DIR}/Code/Numerics/vxl
  ${ITK_SOURCE_DIR}/Code/Numerics/Statistics
  ${ITK_SOURCE_DIR}/Utilities/MetaIO
  ${ITK_SOURCE_DIR}/Utilities/MetaIO/SaptialObject
  ${Applications_SOURCE_DIR}/ImageRegTool
  ${Applications_BINARY_DIR}/ImageRegTool
)

add_executable(ImageRegTool
    LandmarkRegistrator.cxx
    main.cxx
    )
install_targets(/bin ImageRegTool)

target_link_libraries(ImageRegTool
${ITK_LIBRARIES}
)

