/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDashboardHostAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDashboardHostAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptDashboardHostAction::ScriptDashboardHostAction()
: ScriptAction()
{
}

/** */
ScriptDashboardHostAction::~ScriptDashboardHostAction()
{
}

/** */
bool ScriptDashboardHostAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for DashboardHost"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptDashboardHostAction::Help()
{
  return "DashboardHost(http://mydashboard.com)";
}

/** */
void ScriptDashboardHostAction::Execute()
{
  m_Manager->SetDashboardURL(m_Parameters[0].toChar());
  return;
}

} // end namespace bm
