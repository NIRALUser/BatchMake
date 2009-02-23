/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRequirementsAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptRequirementsAction_h_
#define __ScriptRequirementsAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include "bmScriptAction.h"

namespace bm {

class ScriptRequirementsAction : public ScriptAction
{
public:
  ScriptRequirementsAction();
  ~ScriptRequirementsAction();
  void Execute();
  bool TestParam(ScriptError* error,int linenumber);
  MString Help();

};

} // end namespace bm

#endif
