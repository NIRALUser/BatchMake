/*=========================================================================

  Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   Module Description Parser
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Libs/ModuleDescriptionParser/ModuleDescriptionParser.h $
  Date:      $Date$
  Version:   $Revision$

==========================================================================*/
#ifndef __bmModuleDescriptionParser_h
#define __bmModuleDescriptionParser_h

#include <string>

#include "bmModuleDescriptionParserWin32Header.h"

namespace bm
{

class ModuleDescription;
class ModuleParameterGroup;

class ModuleDescriptionParser_EXPORT ModuleDescriptionParser
{
public:
  ModuleDescriptionParser() {};
  ~ModuleDescriptionParser() {};

  int Parse( const std::string& xml, ModuleDescription& description);
};

} // end namespace bm

#endif
