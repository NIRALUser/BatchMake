PROJECT(Zip)

INCLUDE_DIRECTORIES(
  ${Zip_SOURCE_DIR}
  ${Zip_BINARY_DIR}
)

SET(ZLib_HDRS
  adler32.h  
  gzio.h      
  inftrees.h 
  uncompr.h
  compress.h 
  infblock.h  
  infutil.h   
  zutil.h    
  infcodes.c  
  deflate.h  
  inffast.h  
  inflate.h   
  trees.h 
  zip.h
  zutils.h
  unszip.h
  zconf.h
  itk_zlib_mangle.h
)

SET(ZLib_SRCS
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

SET(Zip_SRCS
  zip.c
  zipcmd.cxx
)

SET(Unzip_SRCS
  unzip.c
  unzipcmd.cxx
)


CONFIGURE_FILE(${Zip_SOURCE_DIR}/zlibDllConfig.h.in
               ${Zip_BINARY_DIR}/zlibDllConfig.h)

ADD_LIBRARY(ZLib ${ZLib_SRCS})

ADD_EXECUTABLE(Zip ${Zip_SRCS})
TARGET_LINK_LIBRARIES(Zip
  ZLib
)

ADD_EXECUTABLE(Unzip ${Unzip_SRCS})
TARGET_LINK_LIBRARIES(Unzip
  ZLib
)


# Installation file
INSTALL(TARGETS ZLib Zip Unzip 
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${ZLib_SRCS} ${Zip_SRCS} ${Unzip_SRCS} DESTINATION source/External/Zip)
INSTALL(FILES ${ZLib_HDRS} DESTINATION include/BatchMake/External/Zip)
INSTALL(FILES CMakeLists.txt DESTINATION include/BatchMake/External/Zip)
