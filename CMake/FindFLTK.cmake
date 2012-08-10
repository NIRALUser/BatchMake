#
# Find the native FLTK includes and library
#
# The following settings are defined
# FLTK_FLUID_EXECUTABLE, where to find the Fluid tool
# FLTK_WRAP_UI, This allows the FLTK_WRAP_UI command to work.
# FLTK_INCLUDE_DIR, where to find include files
# FLTK_LIBRARIES, list of fltk libraries
# FLTK_FOUND, Don't use FLTK if false.

# The following settings should not be used in general.
# FLTK_BASE_LIBRARY    = the full path to fltk.lib
# FLTK_GL_LIBRARY      = the full path to fltk_gl.lib
# FLTK_FORMS_LIBRARY   = the full path to fltk_forms.lib
# FLTK_IMAGES_LIBRARY  = the full path to fltk_images.lib

# Platform dependent libraries required by FLTK
if(WIN32)
  if(NOT CYGWIN)
    if(BORLAND)
      set( FLTK_PLATFORM_DEPENDENT_LIBS import32 )
    else(BORLAND)
      set( FLTK_PLATFORM_DEPENDENT_LIBS wsock32 comctl32 )
    endif(BORLAND)
  endif(NOT CYGWIN)
endif(WIN32)

if(UNIX)
  include(${CMAKE_ROOT}/Modules/FindX11.cmake)
  set( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} -lm)
endif(UNIX)

if(APPLE)
  set( FLTK_PLATFORM_DEPENDENT_LIBS  "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
endif(APPLE)

if(CYGWIN)
  set( FLTK_PLATFORM_DEPENDENT_LIBS ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
endif(CYGWIN)

# If FLTK_INCLUDE_DIR is already defined we assigne its value to FLTK_DIR
if(FLTK_INCLUDE_DIR)
  set(FLTK_DIR ${FLTK_INCLUDE_DIR})
endif(FLTK_INCLUDE_DIR)


# If FLTK has been built using CMake we try to find everything directly
set(FLTK_DIR_STRING "directory containing FLTKConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/fltk for an installation.")

# Search only if the location is not already known.
if(NOT FLTK_DIR)
  # Get the system search path as a list.
  if(UNIX)
    string(REGEX MATCHALL "[^:]+" FLTK_DIR_SEARCH1 "$ENV{PATH}")
  else(UNIX)
    string(REGEX REPLACE "\\\\" "/" FLTK_DIR_SEARCH1 "$ENV{PATH}")
  endif(UNIX)
  string(REGEX REPLACE "/;" ";" FLTK_DIR_SEARCH2 ${FLTK_DIR_SEARCH1})

  # Construct a set of paths relative to the system search path.
  set(FLTK_DIR_SEARCH "")
  foreach(dir ${FLTK_DIR_SEARCH2})
    set(FLTK_DIR_SEARCH ${FLTK_DIR_SEARCH} "${dir}/../lib/fltk")
  endforeach(dir)

  #
  # Look for an installation or build tree.
  #
  find_path(FLTK_DIR FLTKConfig.cmake
    # Look for an environment variable FLTK_DIR.
    $ENV{FLTK_DIR}

    # Look in places relative to the system executable search path.
    ${FLTK_DIR_SEARCH}

    # Look in standard UNIX install locations.
    /usr/local/lib/fltk
    /usr/lib/fltk
    /usr/local/include
    /usr/include
    /usr/local/fltk
    /usr/X11R6/include

    # Read from the CMakeSetup registry entries.  It is likely that
    # FLTK will have been recently built.
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild1]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild2]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild3]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild4]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild5]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild6]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild7]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild8]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild9]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild10]

    # Help the user find it if we cannot.
    DOC "The ${FLTK_DIR_STRING}"
    )
endif(NOT FLTK_DIR)

# If FLTK was found, load the configuration file to get the rest of the
# settings.
if(FLTK_DIR)

  # Check if FLTK was built using CMake
  if(EXISTS ${FLTK_DIR}/FLTKConfig.cmake)
    set(FLTK_BUILT_WITH_CMAKE 1)
  endif(EXISTS ${FLTK_DIR}/FLTKConfig.cmake)

  if(FLTK_BUILT_WITH_CMAKE)
    set(FLTK_FOUND 1)
    include(${FLTK_DIR}/FLTKConfig.cmake)

    # Fluid
    if(FLUID_COMMAND)
      set(FLTK_FLUID_EXECUTABLE ${FLUID_COMMAND} CACHE FILEPATH "Fluid executable")
    else(FLUID_COMMAND)
      find_program(FLTK_FLUID_EXECUTABLE fluid PATHS
        ${FLTK_EXECUTABLE_DIRS}
        ${FLTK_EXECUTABLE_DIRS}/RelWithDebInfo
        ${FLTK_EXECUTABLE_DIRS}/Debug
        ${FLTK_EXECUTABLE_DIRS}/Release
        NO_SYSTEM_PATH)
    endif(FLUID_COMMAND)
    # mark_as_advanced(FLTK_FLUID_EXECUTABLE)

    set(FLTK_INCLUDE_DIR ${FLTK_DIR})
    link_directories(${FLTK_LIBRARY_DIRS})

    set(FLTK_BASE_LIBRARY fltk)
    set(FLTK_GL_LIBRARY fltk_gl)
    set(FLTK_FORMS_LIBRARY fltk_forms)
    set(FLTK_IMAGES_LIBRARY fltk_images)

    # Add the extra libraries
    load_cache(${FLTK_DIR}
      READ_WITH_PREFIX
      FL FLTK_USE_SYSTEM_JPEG
      FL FLTK_USE_SYSTEM_PNG
      FL FLTK_USE_SYSTEM_ZLIB
      )

    set(FLTK_IMAGES_LIBS "")
    if(FLFLTK_USE_SYSTEM_JPEG)
      set(FLTK_IMAGES_LIBS ${FLTK_IMAGES_LIBS} fltk_jpeg)
    endif(FLFLTK_USE_SYSTEM_JPEG)
    if(FLFLTK_USE_SYSTEM_PNG)
      set(FLTK_IMAGES_LIBS ${FLTK_IMAGES_LIBS} fltk_png)
    endif(FLFLTK_USE_SYSTEM_PNG)
    if(FLFLTK_USE_SYSTEM_ZLIB)
      set(FLTK_IMAGES_LIBS ${FLTK_IMAGES_LIBS} fltk_zlib)
    endif(FLFLTK_USE_SYSTEM_ZLIB)
    set(FLTK_IMAGES_LIBS "${FLTK_IMAGES_LIBS}" CACHE INTERNAL
      "Extra libraries for fltk_images library.")

  else(FLTK_BUILT_WITH_CMAKE)

    # if FLTK was not built using CMake
    # Find fluid executable.
    find_program(FLTK_FLUID_EXECUTABLE fluid ${FLTK_INCLUDE_DIR}/fluid)

    # Use location of fluid to help find everything else.
    set(FLTK_INCLUDE_SEARCH_PATH "")
    set(FLTK_LIBRARY_SEARCH_PATH "")
    if(FLTK_FLUID_EXECUTABLE)
      get_filename_component(FLTK_BIN_DIR "${FLTK_FLUID_EXECUTABLE}" PATH)
      set(FLTK_INCLUDE_SEARCH_PATH ${FLTK_INCLUDE_SEARCH_PATH}
        ${FLTK_BIN_DIR}/../include ${FLTK_BIN_DIR}/..)
      set(FLTK_LIBRARY_SEARCH_PATH ${FLTK_LIBRARY_SEARCH_PATH}
        ${FLTK_BIN_DIR}/../lib)
      set(FLTK_WRAP_UI 1)
    endif(FLTK_FLUID_EXECUTABLE)

    set(FLTK_INCLUDE_SEARCH_PATH ${FLTK_INCLUDE_SEARCH_PATH}
      /usr/local/include
      /usr/include
      /usr/local/fltk
      /usr/X11R6/include
      )

    find_path(FLTK_INCLUDE_DIR FL/Fl.h ${FLTK_INCLUDE_SEARCH_PATH})

    set(FLTK_LIBRARY_SEARCH_PATH ${FLTK_LIBRARY_SEARCH_PATH}
      /usr/lib
      /usr/local/lib
      /usr/local/fltk/lib
      /usr/X11R6/lib
      ${FLTK_INCLUDE_DIR}/lib
      )

    find_library(FLTK_BASE_LIBRARY NAMES fltk fltkd
      PATHS ${FLTK_LIBRARY_SEARCH_PATH})
    find_library(FLTK_GL_LIBRARY NAMES fltkgl fltkgld fltk_gl
      PATHS ${FLTK_LIBRARY_SEARCH_PATH})
    find_library(FLTK_FORMS_LIBRARY NAMES fltkforms fltkformsd fltk_forms
      PATHS ${FLTK_LIBRARY_SEARCH_PATH})
    find_library(FLTK_IMAGES_LIBRARY NAMES fltkimages fltkimagesd fltk_images
      PATHS ${FLTK_LIBRARY_SEARCH_PATH})

    # Find the extra libraries needed for the fltk_images library.
    if(UNIX)
      find_program(FLTK_CONFIG_SCRIPT fltk-config PATHS ${FLTK_BIN_DIR})
      if(FLTK_CONFIG_SCRIPT)
        exec_program(${FLTK_CONFIG_SCRIPT} ARGS --use-images --ldflags
          OUTPUT_VARIABLE FLTK_IMAGES_LDFLAGS)
        set(FLTK_LIBS_EXTRACT_REGEX ".*-lfltk_images (.*) -lfltk.*")
        if("${FLTK_IMAGES_LDFLAGS}" MATCHES "${FLTK_LIBS_EXTRACT_REGEX}")
          string(REGEX REPLACE "${FLTK_LIBS_EXTRACT_REGEX}" "\\1"
            FLTK_IMAGES_LIBS "${FLTK_IMAGES_LDFLAGS}")
          string(REGEX REPLACE " +" ";" FLTK_IMAGES_LIBS "${FLTK_IMAGES_LIBS}")
          # The EXEC_PROGRAM will not be inherited into subdirectories from
          # the file that originally included this module.  Save the answer.
          set(FLTK_IMAGES_LIBS "${FLTK_IMAGES_LIBS}" CACHE INTERNAL
            "Extra libraries for fltk_images library.")
        endif("${FLTK_IMAGES_LDFLAGS}" MATCHES "${FLTK_LIBS_EXTRACT_REGEX}")
      endif(FLTK_CONFIG_SCRIPT)
    endif(UNIX)

  endif(FLTK_BUILT_WITH_CMAKE)
endif(FLTK_DIR)


set(FLTK_FOUND 1)
foreach(var FLTK_FLUID_EXECUTABLE FLTK_INCLUDE_DIR
    FLTK_BASE_LIBRARY FLTK_GL_LIBRARY
    FLTK_FORMS_LIBRARY FLTK_IMAGES_LIBRARY)
  if(NOT ${var})
    set(FLTK_FOUND 0)
  endif(NOT ${var})
endforeach(var)

if(FLTK_FOUND)
  set(FLTK_LIBRARIES ${FLTK_IMAGES_LIBRARY} ${FLTK_IMAGES_LIBS} ${FLTK_BASE_LIBRARY} ${FLTK_GL_LIBRARY}
    ${FLTK_FORMS_LIBRARY} )
  if(APPLE)
    set(FLTK_LIBRARIES ${FLTK_PLATFORM_DEPENDENT_LIBS} ${FLTK_LIBRARIES})
  else(APPLE)
    set(FLTK_LIBRARIES ${FLTK_LIBRARIES} ${FLTK_PLATFORM_DEPENDENT_LIBS})
  endif(APPLE)

  # The following deprecated settings are for compatibility with CMake 1.4
  set(HAS_FLTK ${FLTK_FOUND})
  set(FLTK_INCLUDE_PATH ${FLTK_INCLUDE_DIR})
  set(FLTK_FLUID_EXE ${FLTK_FLUID_EXECUTABLE})
  set(FLTK_LIBRARY ${FLTK_LIBRARIES})
endif(FLTK_FOUND)
