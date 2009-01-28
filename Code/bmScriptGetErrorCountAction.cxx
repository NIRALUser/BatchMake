/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetErrorCountAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGetErrorCountAction.h"
#include "BMString.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptGetErrorCountAction::ScriptGetErrorCountAction()
: ScriptAction()
{
}

ScriptGetErrorCountAction::~ScriptGetErrorCountAction()
{
}

bool ScriptGetErrorCountAction::TestParam(ScriptError* error,int linenumber)
{
  m_Manager->SetTestVariable(m_Parameters[0]);
  if (m_Parameters.size() > 1)
    {
    error->SetError( "Too many parameters for GetErrorCount", linenumber );
    return false;
    }
  if (m_Parameters.size() < 1)
    {
    error->SetError( "Not enough parameter for GetErrorCount", linenumber );
    return false;
    }
  return true;
}

MString ScriptGetErrorCountAction::Help()
{
  return "GetErrorCount( <variable> ) - Set the current number of error"
         " to the variable";
}


void ScriptGetErrorCountAction::Execute()
{
  int errorCount = static_cast<int>(m_ProgressManager->GetErrorCount());
  m_Manager->SetVariable( m_Parameters[0],
                          BMString(errorCount).toVariable().toChar() );
}

} // end namespace bm
