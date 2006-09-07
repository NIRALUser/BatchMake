/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDashboardUserAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDashboardUserAction.h"

namespace bm {

/** */
ScriptDashboardUserAction::ScriptDashboardUserAction()
: ScriptAction()
{
}

/** */
ScriptDashboardUserAction::~ScriptDashboardUserAction()
{
}

/** */
bool ScriptDashboardUserAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for DashboardUser"),linenumber);
    return false;
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptDashboardUserAction::Help()
{
  return "DashboardUser('FirstName LastName')";
}

/** */
void ScriptDashboardUserAction::Execute()
{
  m_manager->SetDashboardUser(m_parameters[0].toChar());
}

} // end namespace bm
