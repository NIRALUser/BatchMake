project(ImageResample)

find_package(ITK)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(FATAL_ERROR "Please set ITK_DIR.")
endif(ITK_FOUND)

add_executable(ImageResample main.cxx)
target_link_libraries(ImageResample ${ITK_LIBRARIES})
