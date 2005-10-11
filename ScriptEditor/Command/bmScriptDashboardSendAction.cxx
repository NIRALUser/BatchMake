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
  if (m_parameters.size() <2)
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
  return "DashboardSend(methodid value1 value2 ...)";
}

/** */
void ScriptDashboardSendAction::Execute()
{
  std::string url = m_manager->GetDashboardURL();
  const char* experiment = m_manager->GetDashboardExperiment();
  m_progressmanager->AddAction("Send data to Dashboard");
  m_progressmanager->IsRunning();
  HttpRequest m_request;

  m_request.AddParam("experiment",experiment);
  m_request.AddParam("methodid",m_parameters[0].toChar());
  m_request.AddParam("hostname",m_request.GetHostName());
  m_request.AddParam("hostip",m_request.GetHostIp());

  // Send the value
  for(unsigned int i=1;i<m_parameters.size();i++)
    {
    std::string val = "value";
    char* num = new char[4];
    sprintf(num,"%d",i);
    val += num;

    MString m_value = m_manager->Convert(m_parameters[i]);
    m_value = m_value.removeChar('\'');
    m_request.AddParam(val.c_str(),m_value.toChar());
    }

  url += "/upload.php";
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
        case 1 :  m_progressmanager->AddError("Bad account name"); break;
        case 2 :  m_progressmanager->AddError("Bad password"); break;
        case 3 :  m_progressmanager->AddError("Bad project name"); break;
        case 4 :  m_progressmanager->AddError("Over quota: please use DbClear function first"); break;
        case 5 :  m_progressmanager->AddError("Host Database error"); break;
        case -1 : m_progressmanager->AddError("Connexion problem"); break;
        }  
      }
    }
}

} // end namespace bm
