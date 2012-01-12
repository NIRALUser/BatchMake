PROJECT(XcedeLib)

INCLUDE_DIRECTORIES(
${BatchMake_SOURCE_DIR}/Utilities/bmcurl/curl
${ITK_DIR}/Utilities/itklibxml2/include
${ITK_DIR}/Utilities/itklibxml2/libxml
)

SET(XCEDELIB_SRCS
   Project.cxx
   Resource.cxx
   Subject.cxx
   Xcede.cxx
   XcedeCatalog.cxx
   XnatCatalog.cxx 
   Project.h
   Resource.h
   Subject.h
   Xcede.h
   XcedeCatalog.h
   XnatCatalog.h)

ADD_LIBRARY(XcedeLib ${XCEDELIB_SRCS})
TARGET_LINK_LIBRARIES(XcedeLib itklibxml2 bmcurl ${ITK_LIBRARIES})