/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptForAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptForAction_h_
#define __ScriptForAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"

namespace bm {

class ScriptForAction : public ScriptAction
{
public:
  ScriptForAction();
  ~ScriptForAction();
  void AddAction(ScriptAction* action);
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  void Delete();

protected:
  std::vector<ScriptAction*> m_action;

};

} // end namespace bm

#endif
