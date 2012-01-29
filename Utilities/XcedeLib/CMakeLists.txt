project(XcedeLib)

include_directories(
${BatchMake_SOURCE_DIR}/Utilities/bmcurl/curl
${ITK_DIR}/Utilities/itklibxml2/include
${ITK_DIR}/Utilities/itklibxml2/libxml
)

set(XCEDELIB_SRCS
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

add_library(XcedeLib ${XCEDELIB_SRCS})
target_link_libraries(XcedeLib itklibxml2 bmcurl ${ITK_LIBRARIES})
