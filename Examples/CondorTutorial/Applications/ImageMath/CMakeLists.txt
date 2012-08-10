project(ImageMath)


include_directories(
  ${ITK_SOURCE_DIR}/Utilities/zlib
  ${ITK_DIR}/Utilities/zlib
)

link_directories(
  ${ITK_LIBRARY_PATH}
)

link_libraries(
  ${ITK_LIBRARIES}
)

set( MAIN_SRCS
itkNJetImageFunction.h
itkCVTImageFilter.h
itkNJetImageFunction.txx
itkCVTImageFilter.txx
main.cxx
)
add_executable(ImageMath ${MAIN_SRCS})
