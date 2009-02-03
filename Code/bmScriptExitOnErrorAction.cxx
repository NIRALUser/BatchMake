/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExitOnErrorAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExitOnErrorAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptExitOnErrorAction::ScriptExitOnErrorAction()
: ScriptAction()
{
}

ScriptExitOnErrorAction::~ScriptExitOnErrorAction()
{
}

bool ScriptExitOnErrorAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() != 1)
     {
     error->SetError(
      MString("Wrong number of parameters for ExitOnError"), linenumber );
     return false;
     }

   return true;
}

MString ScriptExitOnErrorAction::Help()
{
  return "ExitOnError( '1' or '0' or 'true' or 'false' or 'TRUE' or ... )"
         " - Exit the current script if an error is found";
}


void ScriptExitOnErrorAction::Execute()
{
  bool exitOnError = false;
  BMString m_value = m_Parameters[0];
  if ( m_value.isVariable() )
    {
    m_value = m_value.fromVariable();
    }
  if( m_value.isInt() )
    {
    exitOnError = (m_value.toInt() == 1);
    }
  else
    {
    exitOnError = (m_value.toLowerCopy() == "true");
    }
  m_ProgressManager->SetStopOnError(exitOnError);
}

} // end namespace bm
