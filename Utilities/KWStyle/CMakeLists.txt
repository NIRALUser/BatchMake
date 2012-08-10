find_program(KWSTYLE_EXECUTABLE
  NAMES KWStyle
  PATHS
  /usr/local/bin
  )
mark_as_advanced(KWSTYLE_EXECUTABLE)

option(KWSTYLE_USE_VIM_FORMAT
  "Set KWStyle to generate errors in a VIM-compatible format." OFF)
if(KWSTYLE_USE_VIM_FORMAT)
  set(KWSTYLE_ARGUMENTS -vim ${KWSTYLE_ARGUMENTS})
endif(KWSTYLE_USE_VIM_FORMAT)

option(KWSTYLE_USE_MSVC_FORMAT
  "Set KWStyle to generate errors in a VisualStudio-compatible format." OFF)
if(KWSTYLE_USE_MSVC_FORMAT)
  set(KWSTYLE_ARGUMENTS -msvc ${KWSTYLE_ARGUMENTS})
endif(KWSTYLE_USE_MSVC_FORMAT)

add_custom_command(
  OUTPUT ${BatchMake_BINARY_DIR}/KWStyleReport.txt
  COMMAND ${KWSTYLE_EXECUTABLE}
  ARGS    -xml ${BatchMake_BINARY_DIR}/BatchMake.kws.xml -o ${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMakeOverwrite.txt ${KWSTYLE_ARGUMENTS} -D ${BatchMake_BINARY_DIR}/BatchMakeFiles.txt
  COMMENT "Coding Style Checker"
  )
add_custom_target(
  StyleCheck DEPENDS ${BatchMake_BINARY_DIR}/KWStyleReport.txt)

configure_file(
  ${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMake.kws.xml.in
  ${BatchMake_BINARY_DIR}/BatchMake.kws.xml)

configure_file(
  ${BatchMake_SOURCE_DIR}/Utilities/KWStyle/BatchMakeFiles.txt.in
  ${BatchMake_BINARY_DIR}/BatchMakeFiles.txt)
