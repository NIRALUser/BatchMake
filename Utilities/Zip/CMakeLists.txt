project(Zip)

include_directories(
  ${Zip_SOURCE_DIR}
  ${Zip_BINARY_DIR}
)

set(ZLib_HDRS
  inftrees.h
  infblock.h
  infutil.h
  zutil.h
  infcodes.h
  deflate.h
  inffast.h
  trees.h
  zip.h
  unzip.h
  zconf.h
  itk_zlib_mangle.h
  zlib.h
  inffixed.h
)

set(ZLib_SRCS
  adler32.c
  gzio.c
  inftrees.c
  uncompr.c
  compress.c
  infblock.c
  infutil.c
  zutil.c
  crc32.c
  infcodes.c
  deflate.c
  inffast.c
  inflate.c
  trees.c
)

set(Zip_SRCS
  zip.c
  zipcmd.cxx
)

set(Unzip_SRCS
  unzip.c
  unzipcmd.cxx
)


configure_file(${Zip_SOURCE_DIR}/zlibDllConfig.h.in
               ${Zip_BINARY_DIR}/zlibDllConfig.h)

add_library(BatchMakeZLib ${ZLib_SRCS} ${ZLib_HDRS})

add_executable(Zip ${Zip_SRCS})
target_link_libraries(Zip BatchMakeZLib)

add_executable(Unzip ${Unzip_SRCS})
target_link_libraries(Unzip BatchMakeZLib)

# Installation file
install(TARGETS BatchMakeZLib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/BatchMake
  ARCHIVE DESTINATION lib/BatchMake)
install(FILES ${ZLib_HDRS}
  DESTINATION include/BatchMake/Utilities/Zip)
if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES ${ZLib_SRCS} ${Zip_SRCS} ${Unzip_SRCS}
    DESTINATION source/BatchMake/Utilities/Zip)
  install(FILES CMakeLists.txt zlibDllConfig.h.in
    DESTINATION source/BatchMake/Utilities/Zip)
endif(BATCHMAKE_INSTALL_SOURCE)
