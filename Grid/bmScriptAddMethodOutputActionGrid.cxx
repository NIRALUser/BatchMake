/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAddMethodOutputActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptAddMethodOutputAction.h"

namespace bm {

/** */
void ScriptAddMethodOutputAction::GenerateGrid()
{
 // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  MString appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_manager->GetApplicationWrapperList()->begin();
  while (itApp != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*itApp)->GetName().toChar(),appName.toChar()))
      {
      app = *(*itApp);
      appFound = true;
      break;
      }
    itApp++;
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
  const ScriptActionManager::DashboardMethod* meth = NULL;
  std::vector<ScriptActionManager::DashboardExperiment>::const_iterator it = dashboard->experiments.begin();
  while(it != dashboard->experiments.end())
    {
    std::vector<ScriptActionManager::DashboardMethod>::const_iterator itM = (*it).methods.begin();
    while(itM != (*it).methods.end())
      {
      if(!strcmp((*itM).variable.c_str(),m_parameters[1].toChar()))
        {
        exp = &(*it);
        meth = &(*itM);
        break;
        }
      itM++;
      }
    it++;
    }

   if(!meth)
    {
    std::cout << "BMDashboard: Cannot find method" << std::endl;
    return;
    }

  if(!exp)
    {
    std::cout << "BMDashboard: Cannot find experiment" << std::endl;
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
  app.SetParameterValue("createMethodParameter","","1");
  withslash = "\"";
  withslash += exp->name;
  withslash += "\"";
  app.SetParameterValue("createMethodParameter.experiment","",withslash);
  withslash = "\"";
  withslash += meth->name;
  withslash += "\"";
  app.SetParameterValue("createMethodParameter.method","",withslash);
  withslash = "\"";
  withslash += m_parameters[2].toChar();
  withslash += "\"";
  app.SetParameterValue("createMethodParameter.name","",withslash);
  app.SetParameterValue("createMethodParameter.type","","1");

  if(m_parameters.size() > 3)
    {
    app.SetParameterValue("createMethodParameter.paramtype","",m_parameters[3].toChar());
    }
  else
    {
    app.SetParameterValue("createMethodParameter.paramtype","","string");
    }
  m_GridModule->AddApplication(&app);
}

} // end namespace bm