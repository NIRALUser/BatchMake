/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDashboardSendAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDashboardSendAction.h"
#include "HttpRequest.h"

namespace bm {

/** */
ScriptDashboardSendAction::ScriptDashboardSendAction()
: ScriptAction()
{
}

/** */
ScriptDashboardSendAction::~ScriptDashboardSendAction()
{
}

/** */
bool ScriptDashboardSendAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() < 1)
    {
    error->SetError(MString("No enough parameter for DashboardSend"),linenumber);
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
MString ScriptDashboardSendAction::Help()
{
  return "DashboardSend(method)";
}

/** */
void ScriptDashboardSendAction::Execute()
{
  typedef ScriptActionManager::Dashboard DashboardType;
  typedef ScriptActionManager::DashboardExperiment DashboardExperimentType;
  typedef ScriptActionManager::DashboardMethod DashboardMethodType;
  typedef ScriptActionManager::DashboardMethodParameter DashboardMethodParameterType;
  
  // Create the experiment on the dashboard
  std::string url = m_manager->GetDashboardURL();
  m_progressmanager->AddAction("BMDashboard: Creating Parameter");
  m_progressmanager->IsRunning();

  HttpRequest m_request;
  m_request.AddParam("user",m_manager->GetDashboardUser());

  // Get the project name
  const DashboardType * dashboard = m_manager->GetDashboard();
  const DashboardExperimentType* exp = NULL;
  const DashboardMethodType* meth = NULL;
  std::vector<DashboardExperimentType>::const_iterator it = dashboard->experiments.begin();
  while(it != dashboard->experiments.end())
    {
    std::vector<ScriptActionManager::DashboardMethod>::const_iterator itM = (*it).methods.begin();
    while(itM != (*it).methods.end())
      {
      if(!strcmp((*itM).variable.c_str(),m_parameters[0].toChar()))
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
  m_request.AddParam("method","AddData");
  m_request.AddParam("methodname",meth->name);
  m_request.AddParam("experiment",exp->name);

  m_request.AddParam("hostname",m_request.GetHostName());
  m_request.AddParam("hostip",m_request.GetHostIp());

  url += "/dashboard.php";

  // Check that the experiment exist
  std::vector<DashboardExperimentType>::const_iterator itE 
                                              = dashboard->experiments.begin();
  while(itE != dashboard->experiments.end())
    {
    std::vector<DashboardMethodType>::const_iterator itMeth = (*itE).methods.begin();
    while(itMeth != (*itE).methods.end())
      {
      if((!strcmp((*itMeth).variable.c_str(),m_parameters[0].toChar())))
        {
        std::vector<DashboardMethodParameterType>::const_iterator itParam 
                                                    = (*itMeth).parameters.begin();
        bool found = false;
        while(itParam != (*itMeth).parameters.end())
          {
          MString param = "${";
          param += (*itParam).variable.c_str();
          param += "}";
          m_request.AddParam((*itParam).name.c_str(),m_manager->Convert(param).toChar());
          itParam++;
          }
        }
      itMeth++;
      }
    itE++;
    }
  

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
