/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetIdealOutputAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptSetIdealOutputAction_h_
#define __ScriptSetIdealOutputAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "BMString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptSetIdealOutputAction : public ScriptAction
{
public:
  ScriptSetIdealOutputAction();
  ~ScriptSetIdealOutputAction();
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  MString Help();

  void GenerateGrid( const BMString& name, const BMString& value);

};

} // end namespace bm

#endif
