/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCreateMethodAction.cxx
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
#include "HttpRequest.h"

namespace bm {

/** */
ScriptCreateMethodAction::ScriptCreateMethodAction()
: ScriptAction()
{
}

/** */
ScriptCreateMethodAction::~ScriptCreateMethodAction()
{
}

/** */
bool ScriptCreateMethodAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() < 3)
    {
    error->SetError(MString("No enough parameter for CreateMethod"),linenumber);
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
MString ScriptCreateMethodAction::Help()
{
  return "CreateMethod(methodVariable ExperimentVariable 'MethodName' [method description])";
}
  
/** */
void ScriptCreateMethodAction::Execute()
{
  m_manager->AddDashboardMethod(m_parameters[0].toChar(),
                                m_parameters[1].toChar(),
                                m_parameters[2].toChar());
  
#ifdef BM_GRID
  if(m_GridModule)
    {
    this->GenerateGrid();
    return;
    }
#endif

  // Create the experiment on the dashboard
  std::string url = m_manager->GetDashboardURL();
  m_ProgressManager->AddAction("BMDashboard: Creating Method");
  m_ProgressManager->IsRunning();

  HttpRequest m_request;
  m_request.AddParam("user",m_manager->GetDashboardUser());

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
    m_ProgressManager->AddError("BMDashboard: Cannot find method");
    return;
    }

  m_request.AddParam("project",exp->project.c_str());
  m_request.AddParam("method","CreateMethod");
  m_request.AddParam("name",m_parameters[2].toChar());
  m_request.AddParam("experiment",exp->name.c_str());

  if(m_parameters.size()>3)
    {
    m_request.AddParam("description",m_parameters[3].toChar());
    }
  m_request.AddParam("hostname",m_request.GetHostName().c_str());
  m_request.AddParam("hostip",m_request.GetHostIp().c_str());

  url += "/dashboard.php";
  MString m_output = m_request.Send(url.c_str());
 
  if (m_output.length()>3)
    {
    m_ProgressManager->AddError("Bad Host or connexion problem");
    }
  else
    {
    if (m_output.toInt() == 0)
      {
      m_ProgressManager->FinishAction(MString("Data sent"));
      }
    else
      {
      m_ProgressManager->FinishAction(MString("Dashboard problem when sending data"));
      switch(m_output.toInt())
        {
        case 1 :  m_ProgressManager->AddError("Bad user name"); break;
        case 2 :  m_ProgressManager->AddError("Bad project name"); break;
        case 3 :  m_ProgressManager->AddError("User doesn't belong to project"); break;
        case 4 :  m_ProgressManager->AddError("Over quota: please use DbClear function first"); break;
        case 5 :  m_ProgressManager->AddError("Host Database error"); break;
        case -1 : m_ProgressManager->AddError("Connexion problem"); break;
        }  
      }
    }
}

} // end namespace bm
