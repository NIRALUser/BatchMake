/*=========================================================================

  Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   Module Description Parser
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Libs/ModuleDescriptionParser/ModuleParameterGroup.h $
  Date:      $Date$
  Version:   $Revision$

==========================================================================*/

#ifndef __bmModuleParameterGroup_h
#define __bmModuleParameterGroup_h

#include "bmModuleDescriptionParserWin32Header.h"
#include "bmModuleParameter.h"

#include <vector>
#include <string>

namespace bm
{

class ModuleDescriptionParser_EXPORT ModuleParameterGroup
{
public:
  ModuleParameterGroup() {};
  ModuleParameterGroup(const ModuleParameterGroup &parameters);

  void operator=(const ModuleParameterGroup &parameters);

  void SetLabel(const std::string &label) {
    this->Label = label;
  }

  const std::string& GetLabel() const {
    return this->Label;
  }

  void SetDescription(const std::string &description) {
    this->Description = description;
  }

  const std::string& GetDescription() const {
    return this->Description;
  }

  void SetAdvanced(const std::string &advanced) {
    this->Advanced = advanced;
  }

  const std::string& GetAdvanced() const {
    return this->Advanced;
  }

  void AddParameter(const ModuleParameter &parameter) {
    this->Parameters.push_back(parameter);
  }

  const std::vector<ModuleParameter>& GetParameters() const {
    return this->Parameters;
  }

  std::vector<ModuleParameter>& GetParameters() {
    return this->Parameters;
  }

private:
  std::string Label;
  std::string Description;
  std::string Advanced;
  std::vector<ModuleParameter> Parameters;
};

ModuleDescriptionParser_EXPORT std::ostream & operator<<(std::ostream &os, const ModuleParameterGroup &group);

} // end namespace

#endif
