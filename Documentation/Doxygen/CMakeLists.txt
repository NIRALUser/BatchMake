configure_file(
  ${BatchMake_SOURCE_DIR}/Documentation/Doxygen/doxygen.config.in
  ${BatchMake_BINARY_DIR}/doxygen.config
  )

#file(GLOB Doxygen ${BatchMake_SOURCE_DIR}/Documentation/Doxygen/?*.*)
#install(FILES ${Doxygen} DESTINATION source/Documentation/Doxygen)
