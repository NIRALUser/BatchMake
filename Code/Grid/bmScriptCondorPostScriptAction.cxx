/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCondorPostScriptAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptCondorPostScriptAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"


namespace bm
{

/** */
ScriptCondorPostScriptAction::ScriptCondorPostScriptAction() : ScriptAction()
  {
  }

/** */
ScriptCondorPostScriptAction::~ScriptCondorPostScriptAction()
  {
  }

/** */
bool ScriptCondorPostScriptAction::TestParam(ScriptError* error,int linenumber)
  {
  if( m_Parameters.size() < 3 )
    {
    error->SetError(MString("Not enough parameters for CondorPostScript"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
  }

/** */
MString ScriptCondorPostScriptAction::Help()
  {
  return "CondorPostScript(<application> <script_filename> <Param1> <Param2> ... )";
  }

/** */
void ScriptCondorPostScriptAction::Execute()
  {
  // First we search for the application
  std::vector<BMString> appOptions = m_Parameters[0].tokenize(".");
  BMString application = appOptions.size() >= 1 ? appOptions[0] : "";

  // find the application name
  BMString applicationName =
    m_Manager->GetVariable(application)[0].fromVariable();

  // get the ApplicationWrapper instance for this execution of the application
  ApplicationWrapper * app =
    m_Manager->GetApplicationWrapper( applicationName );
  if( !app )
    {
    m_ProgressManager->AddError(
      BMString("CondorPostScript: Cannot find application: ") +
      applicationName );
    return;
    }

  BMString m_value;
  // second param is the script name
  m_value += m_Manager->Convert(m_Parameters[1]).fromVariable().toChar();

  // follow this with three variable names that Condor will populate
  m_value += " $JOB $JOBID $RETURN ";

  // then add in the remaining params, adding a space in between
  for( unsigned int i = 2; i < m_Parameters.size(); ++i )
    {
    m_value += m_Manager->Convert(m_Parameters[i]).fromVariable().toChar();
    if(i < m_Parameters.size())
      {
      m_value += " ";
      }
    }
  // set the value of the entire POST line in the Application
  app->SetCondorPostScript(m_value.toChar());
  return;
  }

} // end namespace bm
