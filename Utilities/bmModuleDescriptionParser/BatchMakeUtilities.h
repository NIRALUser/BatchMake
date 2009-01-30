/*=========================================================================

  Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   Module Description Parser
  Module:    $HeadURL: http://www.na-mic.org/svn/Slicer3/trunk/Libs/ModuleDescriptionParser/ModuleDescription.h $
  Date:      $Date$
  Version:   $Revision$

==========================================================================*/

#ifndef __bmBatchMakeUtilities_h
#define __bmBatchMakeUtilities_h

#include "bmModuleDescriptionParserWin32Header.h"

#include "bmModuleDescription.h"

namespace bm
{

ModuleDescriptionParser_EXPORT std::string GenerateBatchMakeWrapper(const ModuleDescription &module);

}

#endif
