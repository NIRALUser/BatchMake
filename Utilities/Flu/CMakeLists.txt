# Main CMakeLists.txt to build the FLU project using CMake (www.cmake.org)
# Written by Matthieu Jomier

project(FLU)

find_package(FLTK)
if(FLTK_FOUND)
   include_directories(${FLTK_INCLUDE_DIR})
endif(FLTK_FOUND)

set(FLU_SRCS
Flu_Button.cpp
Flu_Chat_Buffer.cpp
Flu_Choice_Group.cpp
Flu_Collapsable_Group.cpp
Flu_Combo_Box.cpp
Flu_Combo_List.cpp
Flu_Combo_Tree.cpp
Flu_DND.cpp
Flu_Dual_Progress_Meter.cpp
Flu_Dual_Slider.cpp
Flu_File_Chooser.cpp
Flu_Float_Input.cpp
Flu_Helpers.cpp
Flu_Int_Input.cpp
Flu_Label.cpp
Flu_Link_Button.cpp
Flu_Output.cpp
flu_file_chooser_pixmaps.cpp
flu_pixmaps.cpp
Flu_Progress.cpp
Flu_Progress_Meter.cpp
Flu_Return_Button.cpp
Flu_Separator.cpp
Flu_Simple_Group.cpp
FluSimpleString.cpp
Flu_Spinner.cpp
Flu_Toggle_Group.cpp
Flu_Tree_Browser.cpp
Flu_Wrap_Group.cpp
)


link_libraries(FLU
 ${FLTK_LIBRARIES}
)

add_library(FLU ${FLU_SRCS})

file(GLOB FLU_HDRS FLU/*.h)

# Installation file
install(TARGETS FLU
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/BatchMake
  ARCHIVE DESTINATION lib/BatchMake)
install(FILES ${FLU_HDRS}
  DESTINATION include/BatchMake/Utilities/Flu/FLU)
if(BATCHMAKE_INSTALL_SOURCE)
  install(FILES ${FLU_SRCS} ${CondorWatcherGUI_SRCS}
    DESTINATION source/BatchMake/Utilities/Flu)
  install(FILES CMakeLists.txt
    DESTINATION source/BatchMake/Utilities/Flu)
endif(BATCHMAKE_INSTALL_SOURCE)
