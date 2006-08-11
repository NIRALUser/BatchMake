/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAddMethodIdealOutputAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptAddMethodIdealOutputAction.h"
#include "HttpRequest.h"

namespace bm {

/** */
ScriptAddMethodIdealOutputAction::ScriptAddMethodIdealOutputAction()
: ScriptAction()
{
}

/** */
ScriptAddMethodIdealOutputAction::~ScriptAddMethodIdealOutputAction()
{
}

/** */
bool ScriptAddMethodIdealOutputAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() < 3)
    {
    error->SetError(MString("No enough parameter for AddMethodIdealOutput"),linenumber);
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
MString ScriptAddMethodIdealOutputAction::Help()
{
  return "AddMethodIdealOutput(variable MethodVariable 'description')";
}

/** */
void ScriptAddMethodIdealOutputAction::GenerateGrid()
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
  app.SetParameterValue("createMethodParameter.type","","2");

  m_GridModule->AddApplication(&app);
}

/** */
void ScriptAddMethodIdealOutputAction::Execute()
{
  m_manager->AddDashboardMethodParameter(m_parameters[0].toChar(),
                               m_parameters[1].toChar(),
                               m_parameters[2].toChar(),
                               true,true);
  
  if(m_GridModule)
    {
    this->GenerateGrid();
    return;
    }

  // Create the experiment on the dashboard
  std::string url = m_manager->GetDashboardURL();
  m_progressmanager->AddAction("BMDashboard: Creating Parameter");
  m_progressmanager->IsRunning();

  HttpRequest m_request;
  m_request.AddParam("user",m_manager->GetDashboardUser());

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
    m_progressmanager->AddError("BMDashboard: Cannot find method");
    return;
    }

  if(!exp)
    {
    m_progressmanager->AddError("BMDashboard: Cannot find experiment");
    return;
    }

  m_request.AddParam("project",exp->project);
  m_request.AddParam("method","CreateParameter");
  m_request.AddParam("name",m_parameters[2].toChar());
  m_request.AddParam("experiment",exp->name);

  m_request.AddParam("methodname",meth->name);
  m_request.AddParam("type","2"); //ideal output

  m_request.AddParam("hostname",m_request.GetHostName());
  m_request.AddParam("hostip",m_request.GetHostIp());

  url += "/dashboard.php";
  MString m_output = m_request.Send(url.c_str());

  if (m_output.length()>3)
    {
    m_progressmanager->AddError("Bad Host or connexion problem");
    }
  else
    {
    if (m_output.toInt() == 0)
      {
      m_progressmanager->FinishAction(MString("Data sent"));
      }
    else
      {
      m_progressmanager->FinishAction(MString("Dashboard problem when sending data"));
      switch(m_output.toInt())
        {
        case 1 :  m_progressmanager->AddError("Bad user name"); break;
        case 2 :  m_progressmanager->AddError("Bad project name"); break;
        case 3 :  m_progressmanager->AddError("User doesn't belong to project"); break;
        case 4 :  m_progressmanager->AddError("Over quota: please use DbClear function first"); break;
        case 5 :  m_progressmanager->AddError("Host Database error"); break;
        case -1 : m_progressmanager->AddError("Connexion problem"); break;
        }  
      }
    }

}

} // end namespace bm
