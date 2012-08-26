/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptWorkingDirectoryAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptWorkingDirectoryAction_h_
#define __ScriptWorkingDirectoryAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "MString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptWorkingDirectoryAction : public ScriptAction
{
public:
  ScriptWorkingDirectoryAction();
  ~ScriptWorkingDirectoryAction();
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  MString Help();

};

} // end namespace bm

#endif
