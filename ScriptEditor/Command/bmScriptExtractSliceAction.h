/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExtractSliceAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptExtractSliceAction_h_
#define __ScriptExtractSliceAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptExtractSliceAction : public ScriptAction
{
public:
  ScriptExtractSliceAction();
  ~ScriptExtractSliceAction();
  void Execute();
  MString Help();
  bool TestParam(ScriptError* error,int linenumber);
  void GenerateGrid();
};

} // end namespace bm

#endif
