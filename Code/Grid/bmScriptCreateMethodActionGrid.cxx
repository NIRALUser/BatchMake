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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {


/** */
void ScriptCreateMethodAction::GenerateGrid()
{
#ifdef BM_BATCHBOARD
  // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  std::string appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itapp = m_Manager->GetApplicationWrapperList()->begin();
  while (itapp != m_Manager->GetApplicationWrapperList()->end())
    {
    if( (*itapp)->GetName() == appName )
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
              << appName << std::endl;
    return;
    }

  app.ClearParameterValues();

  // Get the project name
  const ScriptActionManager::Dashboard * dashboard = m_Manager->GetDashboard();
  const ScriptActionManager::DashboardExperiment* exp = NULL;
  std::vector<ScriptActionManager::DashboardExperiment>::const_iterator it = dashboard->experiments.begin();
  while(it != dashboard->experiments.end())
    {
    if(!strcmp((*it).variable.c_str(),m_Parameters[1].toChar()))
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
  withslash += m_Manager->GetDashboardUser();
  withslash += "\"";
  app.SetParameterValue("hostname","",m_Manager->GetDashboardURL());
  app.SetParameterValue("user","",withslash);
  withslash = "\"";
  withslash += exp->project;
  withslash += "\"";
  app.SetParameterValue("project","",withslash);

  app.SetParameterValue("key","","1");
  withslash = "\"";
  withslash += m_Manager->GetDashboardKey();
  withslash += "\"";
  app.SetParameterValue("key","key",withslash);

  app.SetParameterValue("createMethod","","1");
  withslash = "\"";
  withslash += exp->name;
  withslash += "\"";
  app.SetParameterValue("createMethod","experimentName",withslash);
  withslash = "\"";
  withslash += m_Parameters[2].toChar();
  withslash += "\"";
  app.SetParameterValue("createMethod","name",withslash);
  if(m_Parameters.size()>3)
    {
    withslash = "\"";
    withslash += m_Parameters[3].toChar();
    withslash += "\"";
    app.SetParameterValue("createMethod","description",withslash);
    }
  m_GridModule->AddApplication(&app);
#endif
}

} // end namespace bm
