# Generate the ModuleDescriptionParserConfig.cmake file in the build tree 
# and configure one the installation tree.

# Settings specific to build trees
#
#
set(bmModuleDescriptionParser_INCLUDE_DIRS_CONFIG 
  ${bmModuleDescriptionParser_BINARY_DIR} 
  ${bmModuleDescriptionParser_SOURCE_DIR}
  )
set(bmModuleDescriptionParser_LIBRARY_DIRS_CONFIG 
  ${bmModuleDescriptionParser_BINARY_DIR}
  )
set(bmModuleDescriptionParser_USE_FILE_CONFIG 
  ${bmModuleDescriptionParser_BINARY_DIR}/UseModuleDescriptionParser.cmake
  )
set(ITK_DIR_CONFIG ${ITK_DIR})


# Configure ModuleDescriptionParserConfig.cmake for the build tree.
#
configure_file(
  ${bmModuleDescriptionParser_SOURCE_DIR}/bmModuleDescriptionParserConfig.cmake.in
  ${bmModuleDescriptionParser_BINARY_DIR}/bmModuleDescriptionParserConfig.cmake 
  @ONLY IMMEDIATE
  )

# Settings specific for installation trees
#
#

# Configure ModuleDescriptionParserConfig.cmake for the install tree.
#
configure_file(
  ${bmModuleDescriptionParser_SOURCE_DIR}/bmModuleDescriptionParserInstallConfig.cmake.in
  ${bmModuleDescriptionParser_BINARY_DIR}/install/bmModuleDescriptionParserConfig.cmake 
  @ONLY IMMEDIATE
  )
