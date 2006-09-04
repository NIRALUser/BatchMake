/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCreateMethodAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptCreateMethodAction_h_
#define __ScriptCreateMethodAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptCreateMethodAction : public ScriptAction
{
public:
  ScriptCreateMethodAction();
  ~ScriptCreateMethodAction();
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  MString Help();

#ifdef BM_GRID
  void GenerateGrid();
#endif

};

} // end namespace bm

#endif
