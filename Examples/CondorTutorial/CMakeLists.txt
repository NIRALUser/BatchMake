project(BatchmakeCondorTutorial)


find_package(ITK)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(FATAL_ERROR "Please set ITK_DIR.")
endif(ITK_FOUND)


find_package(FLTK)
if(FLTK_FOUND)
  include_directories(${FLTK_INCLUDE_DIR})
endif(FLTK_FOUND)

# The fluid-generated fltk sources have many warnings.  This macro
# will disable warnings for the generated files on some compilers.
macro(ITK_DISABLE_FLTK_GENERATED_WARNINGS files)
  if(CMAKE_COMPILER_IS_GNUCXX)
    foreach(f ${files})
      string(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
      string(REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}")
      set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -w)
    endforeach(f)
  endif(CMAKE_COMPILER_IS_GNUCXX)
endmacro(ITK_DISABLE_FLTK_GENERATED_WARNINGS)

subdirs(Applications)
