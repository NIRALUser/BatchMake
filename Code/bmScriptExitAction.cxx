/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExitAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExitAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptExitAction::ScriptExitAction()
: ScriptAction()
{
}

ScriptExitAction::~ScriptExitAction()
{
}

bool ScriptExitAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() > 0 && m_Parameters[0].length()!=0)
     {
     error->SetError(MString("Too many parameters for Exit"),linenumber);
     return false;
     }

   return true;
}

MString ScriptExitAction::Help()
{
  return "Exit() - exit the current script";
}


void ScriptExitAction::Execute()
{
  m_ProgressManager->AddOutput("Script has been stopped manually");
  m_ProgressManager->Stop();
}

} // end namespace bm
