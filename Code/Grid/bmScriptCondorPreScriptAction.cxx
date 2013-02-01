/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCondorPreScriptAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptCondorPreScriptAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"


namespace bm
{

/** */
ScriptCondorPreScriptAction::ScriptCondorPreScriptAction() : ScriptAction()
  {
  }

/** */
ScriptCondorPreScriptAction::~ScriptCondorPreScriptAction()
  {
  }

/** */
bool ScriptCondorPreScriptAction::TestParam(ScriptError* error,int linenumber)
  {
  if( m_Parameters.size() < 3 )
    {
    error->SetError(MString("Not enough parameters for CondorPreScript"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
  }

/** */
MString ScriptCondorPreScriptAction::Help()
  {
  return "CondorPreScript(<application> <script_filename> <Param1> <Param2> ... )";
  }

/** */
void ScriptCondorPreScriptAction::Execute()
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
      BMString("CondorPreScript: Cannot find application: ") +
      applicationName );
    return;
    }

  BMString m_value;
  // second param is the script name
  m_value += m_Manager->Convert(m_Parameters[1]).fromVariable().toChar();
  m_value += " ";

  // then add in the remaining params, adding a space in between
  for( unsigned int i = 2; i < m_Parameters.size(); ++i )
    {
    m_value += m_Manager->Convert(m_Parameters[i]).fromVariable().toChar();
    if(i < m_Parameters.size())
      {
      m_value += " ";
      }
    }

  // follow this with two variable names that Condor will populate
  m_value += " $JOB $JOBID";

  // set the value of the entire POST line in the Application
  app->SetCondorPreScript(m_value.toChar());
  return;
  }

} // end namespace bm
