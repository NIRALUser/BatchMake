/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDashboardExperimentAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDashboardExperimentAction.h"

namespace bm {

/** */
ScriptDashboardExperimentAction::ScriptDashboardExperimentAction()
: ScriptAction()
{
}

/** */
ScriptDashboardExperimentAction::~ScriptDashboardExperimentAction()
{
}

/** */
bool ScriptDashboardExperimentAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for DashboardExperiment"),linenumber);
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
MString ScriptDashboardExperimentAction::Help()
{
  return "DashboardExperiment(encryptedExperimentId)";
}

/** */
void ScriptDashboardExperimentAction::Execute()
{
  m_manager->SetDashboardExperiment(m_parameters[0].toChar());
}

} // end namespace bm
