/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptClearErrorsAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptClearErrorsAction.h"
#include "BMString.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptClearErrorsAction::ScriptClearErrorsAction()
: ScriptAction()
{
}

ScriptClearErrorsAction::~ScriptClearErrorsAction()
{
}

bool ScriptClearErrorsAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() > 0 && m_Parameters[0].length()!=0)
    {
    error->SetError( "Too many parameters for ClearErrors", linenumber );
    return false;
    }
  return true;
}

MString ScriptClearErrorsAction::Help()
{
  return "ClearErrors( ) - Set the number of errors to 0";
}


void ScriptClearErrorsAction::Execute()
{
  m_ProgressManager->ResetErrorCount();
}

} // end namespace bm
