PROJECT(Zip)

INCLUDE_DIRECTORIES(
  ${Zip_SOURCE_DIR}
  ${Zip_BINARY_DIR}
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

