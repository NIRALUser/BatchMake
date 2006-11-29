/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptForEachAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptForEachAction_h_
#define __ScriptForEachAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptForEachAction : public ScriptAction
{
public:
  ScriptForEachAction();
  ~ScriptForEachAction();
  void AddAction(ScriptAction* action);
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  void Delete();
  MString Help();

protected:

  void CreateLoop();

  std::vector<ScriptAction*> m_action;
  std::vector<MString> m_ForLoop;

};

} // end namespace bm

#endif
