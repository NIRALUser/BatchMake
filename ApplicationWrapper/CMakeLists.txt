SET(BatchMakeApplicationWrapper_HDRS
  ApplicationWrapper.h
  ApplicationWrapperParam.h
  bmApplicationListGUIBase.h
  bmApplicationListGUIControls.h
  bmApplicationWrapperGUIBase.h
  bmApplicationWrapperGUIControls.h
)


SET(BatchMakeApplicationWrapper_SRCS
  ApplicationWrapper.cxx
  ApplicationWrapperParam.cxx
  bmApplicationListGUIControls.cxx
  bmApplicationWrapperGUIControls.cxx
)

LINK_LIBRARIES(
 BatchMakeUtilities
 ${FLTK_LIBRARIES}
)

SET(BatchMakeApplicationWrapper_GUI_SRCS
  bmApplicationListGUI.fl
  bmApplicationWrapperGUI.fl
)

SET(FLTK_WRAP_UI 1)
FLTK_WRAP_UI(BatchMakeApplicationWrapper ${BatchMakeApplicationWrapper_GUI_SRCS} )

ADD_LIBRARY(BatchMakeApplicationWrapper ${BatchMakeApplicationWrapper_SRCS}  ${BatchMakeApplicationWrapper_FLTK_UI_SRCS} )
ADD_DEPENDENCIES(BatchMakeApplicationWrapper FLU)

# Installation file
INSTALL(TARGETS BatchMakeApplicationWrapper
        ARCHIVE DESTINATION lib)
INSTALL(FILES ${BatchMakeApplicationWrapper_HDRS} DESTINATION include/BatchMake/ApplicationWrapper)
INSTALL(FILES ${BatchMakeApplicationWrapper_SRCS} ${BatchMakeApplicationWrapper_GUI_SRCS} DESTINATION source/ApplicationWrapper)
INSTALL(FILES CMakeLists.txt DESTINATION source/ApplicationWrapper)
