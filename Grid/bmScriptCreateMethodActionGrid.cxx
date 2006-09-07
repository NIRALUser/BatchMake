/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCreateMethodActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptCreateMethodAction.h"

namespace bm {


/** */
void ScriptCreateMethodAction::GenerateGrid()
{
  // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  MString appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itapp = m_manager->GetApplicationWrapperList()->begin();
  while (itapp != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*itapp)->GetName().toChar(),appName.toChar()))
      {
      app = *(*itapp);
      appFound = true;
      break;
      }
    itapp++;
    }

  if(!appFound)
    {
    std::cout << "ScriptDashboardSendAction::GenerateCondor : Cannot find bmGridSend " 
              << appName.toChar() << std::endl;
    return;
    }  

  // Get the project name
  const ScriptActionManager::Dashboard * dashboard = m_manager->GetDashboard();
  const ScriptActionManager::DashboardExperiment* exp = NULL;
  std::vector<ScriptActionManager::DashboardExperiment>::const_iterator it = dashboard->experiments.begin();
  while(it != dashboard->experiments.end())
    {
    if(!strcmp((*it).variable.c_str(),m_parameters[1].toChar()))
      {
      exp = &(*it);
      break;
      }
    it++;
    }

  if(!exp)
    {
    std::cout << "BMDashboard: Cannot find method" << std::endl;;
    return;
    }

  std::string withslash = "\"";
  withslash += m_manager->GetDashboardUser();
  withslash += "\"";
  app.SetParameterValue("hostname","",m_manager->GetDashboardURL());
  app.SetParameterValue("user","",withslash);
  withslash = "\"";
  withslash += exp->project;
  withslash += "\"";
  app.SetParameterValue("project","",withslash);
  app.SetParameterValue("createMethod","","1");
  withslash = "\"";
  withslash += exp->name;
  withslash += "\"";
  app.SetParameterValue("createMethod.experimentName","",withslash);
  withslash = "\"";
  withslash += m_parameters[2].toChar();
  withslash += "\"";
  app.SetParameterValue("createMethod.name","",withslash);
  if(m_parameters.size()>3)
    {
    withslash = "\"";
    withslash += m_parameters[3].toChar();
    withslash += "\"";
    app.SetParameterValue("createMethod.description","",withslash);
    }
  m_GridModule->AddApplication(&app);
}
  
} // end namespace bm
